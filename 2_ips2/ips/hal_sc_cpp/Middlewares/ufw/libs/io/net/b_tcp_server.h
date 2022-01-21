/*
 * tcp_server.h - a part of ufw++ object envirinment for
 * microcontrollers.
 *
 *   Created on: 22 янв. 2021 г.
 *       Author: Dmitry Donskih
 *
 * Copyright (c) 2019-2020 by Dmitry Donskih
 * All rights reserved.
 * 
 * Attention Software modification allowed only with
 * the permission of the author.
 * 
 */

#ifndef UFW_LIBS_IO_NET_B_TCP_SERVER_H_
#define UFW_LIBS_IO_NET_B_TCP_SERVER_H_

#define UFW_DEFAULT_IOBUFFER_SIZE (2*1024)
#define UFW_STD_IP_MTU (1550)

#include <os/os_thread.h>
#include <containers/ufw_containers.h>
#include <system/System.h>
#include <io/io_endpoint.h>
#include <protocols/message_handler.h>
#include <lib_macros.hpp>
#include <common/i_eraser_gc.h>
#include "socket.h"

namespace ufw
{
  template<class Tp>
  class tcpServerWorker;
  

  /**
   *
   */
  class b_tcpServer : public rtosThread
  {
      //using handler_type = Tp;
      using handler_ptr = void*;
    public:
      b_tcpServer (const char* name,uint16_t port,uint8_t max_clients);
      virtual ~b_tcpServer ();

      /**
       *
       * @param id
       */
      void freeWorker(uint32_t id);

      ioEndpoint* getClientOutput(uint8_t id);
    protected:
      ufw::vector<handler_ptr> m_workers;
      uint32_t* m_ip_table=nullptr;
      uint8_t  m_free_workers;
      uint16_t m_tcp_port;



      void threadFunction () final;
      int findFree();
      virtual void create_connection(int sock,uint32_t addr,uint32_t id)=0;
//      virtual MessageHandler* createHandler();
  };

  /**
   *
   * @tparam HandlClass
   */
  template<class HandlClass>
  class tcpServer :public b_tcpServer
  {
      using handler_type = HandlClass;
      using handler_ptr = HandlClass*;
    public:
      tcpServer (uint16_t port,uint8_t max_clients,const char* name)
      :b_tcpServer(name,port,max_clients)
      {
      }

      /**
       *
       * @param sock
       * @param addr
       * @param id
       */
      void create_connection(int sock,uint32_t addr,uint32_t id) override
      {
	m_workers[id] = new ufw::tcpServerWorker<HandlClass>(sock, addr, id, this);
      }



      virtual ~tcpServer ()
      {
	for(size_t i=0;i<m_workers.size();i++)
	  {
	    if(m_workers[i])
	      {
		close(reinterpret_cast<tcpServerWorker<handler_type>*>(m_workers[i])->getSocket());
		delete reinterpret_cast<tcpServerWorker<handler_type>*>(m_workers[i]);
	      }
	  }

      }


  };

} /* namespace ufw */


namespace ufw
{
  /**
   *
   * @tparam Tp
   */
  template<class Tp>
  class tcpServerWorker : public ioEndpoint, public rtosThread
  {
    using handler_type = Tp;
    using handler_ptr = Tp*;

    public:
    /**
     *
     * @param active_socket
     * @param ipDestinationAddr
     * @param id
     * @param parrent
     */
      tcpServerWorker (int active_socket,uint32_t ipDestinationAddr,uint32_t id,b_tcpServer *parrent)
    :rtosThread(TYPE_NAME(handler_type),512,osPriorityLow),
     m_sock(active_socket),
     client_ip(ipDestinationAddr),
     m_id(id),
     m_parrent(parrent)
    {
	m_handler=new handler_type;
	m_handler->setWritebackIF(this);
	io_buffer = new uint8_t[UFW_DEFAULT_IOBUFFER_SIZE];

    }

      /**
       *
       * @return
       */
      int getSocket()
      {
	return m_sock;
      }

      /**
       *
       * @return
       */
      uint32_t getIp()
      {
	return client_ip;
      }
      /**
       *
       * @param data
       * @param size
       * @return
       */
      size_t write_to(uint8_t *data,size_t size) override
      {
	return write(m_sock,data,size);
      }

      /**
       *
       */
      virtual ~tcpServerWorker ()
      {
	ufw::System::instance().stddebug()<<__PRETTY_FUNCTION__<<"\n"<<"Server Deleted!\n";
	close(m_sock);
	m_sock=-1;
	m_parrent->freeWorker(m_id);

	Stop();
	delete m_handler;
	delete [] io_buffer;
	ufw::System::instance().stddebug()<<__PRETTY_FUNCTION__<<"\n";
      }

    protected:
      b_tcpServer *m_parrent=nullptr;
      handler_ptr m_handler=nullptr;
      uint8_t* io_buffer=nullptr;
      uint32_t client_ip;
      uint32_t m_id=0;
      int m_sock=-1;

      void onCloseConnection()
      {

	m_parrent->freeWorker(m_id);
	SheuldeErase(this);
	while(1)
	  {
	    ufw::System::instance().stddebug()<<"Still Running"<<"\n";
	    ufw::System::instance().stddebug()<<__PRETTY_FUNCTION__<<"\n";
	    osDelay(100);
	  }

      }

      void noDataHandler()
      {
	onCloseConnection();
      };

      void errnoHandler(int i_errno)
      {
	ufw::System::instance().stddebug()<<"Catch -1 on read(). Errno="<<i_errno<<"\n";
	onCloseConnection();
      }

      void threadFunction () override
      {
	int rx_count=0;
	while(1)
	  {
	    rx_count=read(m_sock,io_buffer,1550);
	    if(rx_count>0)
	      {
		m_handler->handleMessage(new ByteArray(io_buffer,rx_count));
	      }
	    else if(rx_count==0)
	      {
		noDataHandler();
		continue;
	      }
	    else
	      {
		errnoHandler(errno);
	      }
	  }
      }
  };

} /* namespace ufw */


#endif /* UFW_LIBS_IO_NET_B_TCP_SERVER_H_ */

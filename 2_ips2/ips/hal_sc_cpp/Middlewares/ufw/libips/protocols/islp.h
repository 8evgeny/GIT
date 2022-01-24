/*
 * islp.h - a part of ufw++ object envirinment for
 * microcontrollers.
 *
 *   Created on: 21 ноя. 2020 г.
 *       Author: Dmitry Donskih
 *
 * Copyright (c) 2019-2020 by Dmitry Donskih
 * All rights reserved.
 * 
 * Attention Software modification allowed only with
 * the permission of the author.
 * 
 */

#ifndef UFW_LIBIPS_PROTOCOLS_ISLP_H_
#define UFW_LIBIPS_PROTOCOLS_ISLP_H_


#include <protocols/islp_details.hpp>

//#include <ipcThread.h>
#include <os/os_thread.h>
#include <io/net/multicast.h>
//#include <system/configs/device_config.h>
//#include <protocols/yellmsg.hpp>
//#include <protocols/setstationmessage.hpp>
//#include <protocols/stationresponse.hpp>



namespace ISLP
{

  class IslpHandler :public ufw::rtosThread
    {
      public:
	static IslpHandler& Instance();
	IslpHandler* pointer();

      private:
	multicastSocketIn m_in_sock;
	multicastSocketOut m_out_sock;

	intercomStation &m_self = ufw::DeviceConfig::Instance(); //! Pointer to self intercomStation structure.
	ufw::arcCrypt *m_key = nullptr; //! Pointer to Crypt object (automaticly created in case of nullptr(default)).
	ISLP::State m_state = ISLP::State::None; //! Current state (after last operation).



	ISLP::State getState();
	ufw::arcCrypt* getKey();


	ISLP::State operator()(basicMessage &message);
	ISLP::State proceed(basicMessage &message);
	ISLP::State yellHandler(yellMsg &message);
	ISLP::State setIpHandler(setStationMessage &message);
	ISLP::State checkYellValid(yellMsg &message);
	void setKey(uint32_t key);
	int responce();

	void threadFunction() override;
////////////////////////////////////////////////////////////////////////////
/*
 *
 *
 *
 */
/////////////////////////////////////////////////////////////////////////////

	IslpHandler ();
	~IslpHandler ();
	IslpHandler (const IslpHandler &other) = delete;
	IslpHandler (IslpHandler &&other) = delete;
	IslpHandler& operator= (const IslpHandler &other) = delete;
	IslpHandler& operator= (IslpHandler &&other) = delete;

    };

} /* namespace ips */

#endif /* UFW_LIBIPS_PROTOCOLS_ISLP_H_ */

/*
 * call_controller.h - a part of ufw++ object envirinment for
 * microcontrollers.
 *
 *   Created on: 24 дек. 2020 г.
 *       Author: Dmitry Donskih
 *
 * Copyright (c) 2019-2020 by Dmitry Donskih
 * All rights reserved.
 * 
 * Attention Software modification allowed only with
 * the permission of the author.
 * 
 */

#ifndef UFW_LIBS_SYSTEM_PROXY_CALL_CONTROLLER_H_
#define UFW_LIBS_SYSTEM_PROXY_CALL_CONTROLLER_H_



#include <audio.hpp>
#include <os/os_thread.h>
#include <timers.h>
#include <ipsUdp.h>
#include "ctrlmessage.h"
#include "ctl_types.h"

namespace ufw
{
//  struct CtlMessage
//  {
//      uint32_t id;
//      uint32_t function;
//      uint32_t destination;
//      uint32_t prio;
//      uint32_t parameters;
//      uint32_t source;
//  };
//
//  class Call :public rtosThread
//  {
//    public:
//      Call(CtlMessage msg);
//
//  };

  class CallController :public rtosThread
    {
      public:
	static CallController& Instance();
	CallController* pointer();

	void KbdRaise(int key);
	void KbdFall(int key);
	void OnTimeout();

    protected:
      void threadFunction () override;

      private:
	CallController ();

	UDPAudioIn *Ain=nullptr;
	UDPAudioOut *Aout=nullptr;
//	uint8_t rq_buffer[128];
//	uint8_t rq_buffer2[64];

	UDPout* Sender=nullptr;
	CtlMessage* tmp_message=nullptr;

	void onCall(CtlMessage& msg);
	void onAck(CtlMessage &msg);
	void onCancell(CtlMessage& msg);

	void Simplex(CtlMessage msg);
	void Duplex(CtlMessage msg);
	void Group(CtlMessage msg);
	void Activate(CtlMessage& msg);
	void Close(CtlMessage msg);
	void Cancell(CtlMessage msg);
	void Acknowlege(CtlMessage msg);

	void Request(CtlMessage msg);
	void IncomeHandler(CtlMessage msg);
	CtlMessage* m_current_call=nullptr;

	int m_self_cid=0;
	int m_callindicator=0;
//	int m_currentprio=-1;
//	int m_currentcid=0;
//	int m_currentcallid=0;
	void DropState();


//	xTimerHandle RcvTimeout;
	osTimerId_t RcvTimeout;

	uint8_t *ibuffer=nullptr;
	uint8_t *obuffer=nullptr;

	inline bool isIdle()
	{
	  return (m_current_call==nullptr);
	}

	inline bool isICallee()
	{
	  return (m_current_call->destination==m_self_cid);
	}

	bool selfcallCheck();


	CallController (const CallController &other) = delete;
	CallController (CallController &&other) = delete;
	CallController& operator= (const CallController &other) = delete;
	CallController& operator= (CallController &&other) = delete;

    };



} /* namespace ufw */
void Loop(void* par);


#endif /* UFW_LIBS_SYSTEM_PROXY_CALL_CONTROLLER_H_ */

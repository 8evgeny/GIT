/*
 * call_controller.cpp - a part of ufw++ object envirinment for
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

#include <system/proxy/call_controller.h>

#include "FreeRTOS.h"
#include "cmsis_os2.h"
#include "task.h"
#include "queue.h"
#include "audio.hpp"
#include "codec.h"
#include <system/System.h>
#include <map>

#define _GLOBAL_MTU 1556

extern uint8_t *g_ufw_ip_settings;

std::map<uint32_t, uint32_t> plan =
  {
    { 1000, 0xac110a46 }, //70
	{ 1001, 0xac110a47 }, //71
	{ 1002, 0xac110a48 }, //72
	{ 1003, 0xac110a49 }, //73
	{ 1010, 0x460a11ac },
	{ 1999, 0xc0a80032 },
	{ 1020, 0xac110a50 } }; // 0xac110a78 } };//{ 1020, 0xac110a50 } }; //80

#define UFW_CC_LASTKEY	14

uint32_t UFW_IPS_SELF_CID;
uint32_t cid2peer (uint32_t cid)
{
  return plan.at (cid);
}

uint32_t fallback_groups[] =
  { 1, 3, 1010, 1002, 1003, 2, 4, 1010, 1002, 1003 };

ufw::CtlMessage keys[] =
  {
    { 0, ufw::VoiceControl::GR, 1, 10, 0, 0 },
    { 1, ufw::VoiceControl::DX, 1001, 10, 0, 0 },
    { 2, ufw::VoiceControl::DX, 1002, 10, 0, 0 },
    { 3, ufw::VoiceControl::DX, 1003, 10, 0, 0 },
    { 4, ufw::VoiceControl::DX, 1020, 10, 0, 0 },
    { 5, ufw::ControlLines::TOG, 1001, 10, 0, 0 },
    { 6, ufw::ControlLines::TOG, 1002, 10, 0, 0 },
    { 7, ufw::ControlLines::TOG, 1003, 10, 0, 0 },
    { 8, ufw::ControlLines::TOG, 1020, 10, 0, 0 },
    { 9, ufw::VoiceControl::PA, 1001, 10, 0, 0 },
    { 10, ufw::VoiceControl::PA, 1002, 10, 1, 0 },
    { 11, ufw::VoiceControl::PA, 1003, 10, 1, 0 },
    { 12, ufw::VoiceControl::PA, 1003, 10, 1, 0 },
    { 13, ufw::Sys::VOLUP, 1020, 1, 0, 0 },
    { 14, ufw::Sys::VOLDOWN, 1020, 1, 0, 0 } };

static TaskHandle_t loopback_handle = nullptr;
static ufw::System &Self = ufw::System::instance ();

namespace ufw
{
  static void TimeoutFunction (void *arg)
  {
    Self.stddebug () << "Timeout evt\n";
    CallController::Instance ().OnTimeout ();
  }

  /**********************************************************************
   *
   * @return
   */
  CallController& CallController::Instance ()
  {
    static CallController s_ctl;
    return s_ctl;
  }

  CallController* CallController::pointer ()
  {
    return this;
  }

  /**********************************************************************
   * Constructs an controller
   */
  CallController::CallController ()
  {
    ibuffer = new uint8_t[2 * 1024];
    obuffer = new uint8_t[2 * 1024];
    Sender = new UDPout (0, 15000);
    Self.stddebug () << "CallController::CallController () - starts!\n";
    RcvTimeout = osTimerNew (&TimeoutFunction, osTimerOnce, 0, 0);
    for (auto [x, y] : plan)
      {
	if (y == *(uint32_t*) g_ufw_ip_settings)
	  {
	    m_self_cid = x;
	    UFW_IPS_SELF_CID = x;
	  }
      }
  }

  /**********************************************************************
   * @Group Caller actions
   */
  /**
   * Channel control: Public address
   * @param msg
   */
  void CallController::Simplex (CtlMessage msg)
  {
    if (isIdle ())
      {
	m_current_call = new CtlMessage (msg);
	m_current_call->source = m_self_cid;
	if (selfcallCheck ()) return;
	Aout = new UDPAudioOut (cid2peer (msg.destination), 16000, obuffer);
	Ain = new UDPAudioIn (cid2peer (msg.destination), 16000, ibuffer);
	Request (msg);
	Self.stddebug () << "Channel requested\n";
	osTimerStart (RcvTimeout, 3000);
      } else
      {
	Self.stddebug () << "Busy\n";
      }
  }

  /**
   *
   * @param msg
   */
  void CallController::Duplex (CtlMessage msg)
  {
    if (isIdle ())
      {
	m_current_call = new CtlMessage (msg);
	if (selfcallCheck ()) return;
	Aout = new UDPAudioOut (cid2peer (msg.destination), 16000, obuffer);
	Aout->setPipe (audioDevice::getInstance ().ADCPointer ());
	Request (msg);
	Self.stddebug () << "Channel opened\n";
	audioDevice::getInstance ().Play ();
      } else
      {
	Self.stddebug () << "Busy\n";
      }

  }

  /**
   *
   * @param msg
   */
  void CallController::Group (CtlMessage msg)
  {
  }

  /**
   * Control lines control
   * @param msg
   */
  void CallController::Activate (CtlMessage &msg)
  {
    Request (msg);
  }

  /**
   *
   * @param msg
   */
  void CallController::Cancell (CtlMessage msg)
  {
    Self.stddebug () << "Cancell send\n";
    if (!isIdle ())
      {
	Self.stddebug () << "Cancell\n";
	msg.function = Responce::Cancel;
	Request (msg);
      } else
      {
	Self.stddebug () << "Failed!\n";
      }
  }

  /**
   *
   * @param msg
   */
  void CallController::Acknowlege (CtlMessage msg)
  {
    Self.stddebug () << "ACK send\n";
    msg.destination = msg.source;
    msg.function = Responce::ACK;
    Request (msg);
  }

  /**
   * Channel control close
   * @param msg
   */
  void CallController::Close (CtlMessage msg)
  {
    audioDevice::getInstance ().Stop ();
    if (Ain) Ain->Remove ();
    Ain = nullptr;
    if (Aout) Aout->Remove ();
    Aout = nullptr;
    delete m_current_call;
    m_current_call = nullptr;
    Self.servicesOS ().outputs->resetDO (m_callindicator);
    m_callindicator = 0;
    Self.stddebug () << "Channel closed\n";
  }

  /**
   * Adds self as source of message and sends it
   * @param msg
   */
  void CallController::Request (CtlMessage msg)
  {
    msg.source = UFW_IPS_SELF_CID;
    //Checks the dest address
    if (msg.destination == m_self_cid)
      {
	if (isIdle ())
	  {
	    Self.stddebug () << "Error trying to send selfie message\n";
	    return;
	  }
	if (isICallee ())
	  {
	    msg.destination = m_current_call->source;
	  } else
	  {
	    msg.destination = m_current_call->destination;
	  }
      }
//    uint8_t *t_buffer = new uint8_t[CtlMessage::cm_size ()];
    msg.Serialize ((uint8_t*) Sender->writeBuffer ());
//    Sender->write_to (cid2peer (msg.destination), t_buffer, CtlMessage::cm_size ());
    Sender->write_to (cid2peer (msg.destination), Sender->writeBuffer (), CtlMessage::cm_size ());
//    delete[] t_buffer;
  }
  
  /**
   * Reset state as idle
   */
  void CallController::DropState ()
  {
    delete m_current_call;
    m_current_call = nullptr;
  }

  /*****************************************************************
   * @Group ActonHandlers
   */

  /**
   *
   * @param msg
   */
  void CallController::onCall (CtlMessage &msg)
  {
    Self.stddebug () << "(void*) m_current_call\n";
    if (isIdle ())
      {
	m_current_call = new CtlMessage (msg);

	if (msg.function == VoiceControl::PA)
	  {
	    Ain = new UDPAudioIn (cid2peer (msg.source), 16000, ibuffer);
	    Aout = new UDPAudioOut (cid2peer (msg.source), 16000, obuffer); // TODO: Test Remove in release
	    Ain->setPipe (audioDevice::getInstance ().DACPointer ());
	    Aout->setPipe (audioDevice::getInstance ().ADCPointer ());
	    Acknowlege (msg);
	    audioDevice::getInstance ().Play ();
	    Self.stddebug () << "VoiceControl::PA Play\n";
	    return;
	  }

	if (msg.function == VoiceControl::DX)
	  {
	    Ain = new UDPAudioIn (cid2peer (msg.source), 16000, ibuffer);
	    //Aout = new UDPAudioOut (cid2peer (msg.source), 16000, obuffer); // TODO: Test Remove in release
	    Ain->setPipe (audioDevice::getInstance ().DACPointer ());
	    //Aout->setPipe (audioDevice::getInstance ().ADCPointer ());
	    audioDevice::getInstance ().Play ();
	    //msg.destination = msg.source;
	    //msg.function = Responce::ACK;
//	Request (msg);
	    return;
	  }
      }
  }

  /**
   *
   * @param msg
   */
  void CallController::onAck (CtlMessage &msg)
  {
    if (!isIdle ())
      {
	osTimerStop (RcvTimeout);
	Self.stddebug () << "Channel acknowleged\n";
	if (Aout) Aout->setPipe (audioDevice::getInstance ().ADCPointer ());
	if (Ain) Ain->setPipe (audioDevice::getInstance ().DACPointer ());
	audioDevice::getInstance ().Play ();
      } else
      {
	Self.stddebug () << "Nothing to ACK IDLE\n";
      }
  }

  /**
   *
   * @param msg
   */
  void CallController::onCancell (CtlMessage &msg)
  {
    Self.stddebug () << "onCancell\n";
    if (m_current_call) Close (*m_current_call);
  }

  /**
   *
   */
  void CallController::OnTimeout ()
  {
    //Cancell (*m_current_call);
    Close (*m_current_call);
  }

  bool CallController::selfcallCheck ()
  {
    if (m_current_call->destination == m_current_call->source)
      {
	Self.stddebug () << "Trying selfcall\n";
	DropState ();
	return true;
      }
    return false;
  }

  /********************************************************************************
   * @Group EventHandlers
   */

  /**
   * On the key rise action
   * @param key
   */
  void CallController::KbdRaise (int key)
  {
    if (key <= UFW_CC_LASTKEY)
      {
	if (keys[key].function == VoiceControl::DX)
	  {
	    Duplex (keys[key]);
	  }
      }
    if (key == 15)
      {
	Self.stddebug () << __PRETTY_FUNCTION__ << "\n";
	if (loopback_handle)
	  {
	    audioDevice::getInstance ().Stop ();
	    vTaskDelete (loopback_handle);
	    loopback_handle = nullptr;
	  } else
	  {
	    xTaskCreate (Loop, "Audio Loopback", 1024, (void*) this, osPriorityNormal,
			 &loopback_handle);
	  }
      }
    if (key == 16)
      {
	Close (*m_current_call);
      }
  }

  /**
   * On the key fall action
   * @param key
   */
  void CallController::KbdFall (int key)
  {
//    Self.servicesOS ().outputs->resetDO(key);
    if (key > UFW_CC_LASTKEY) return;
//    if (m_current_call)
//      {
//	if (keys[key] == *m_current_call)
//	  {
//	    Cancell (*m_current_call);
//	    Close (*m_current_call);
//	    return;
//	  }
////	if(m_current_call->function==VoiceControl::DX)
//
//      }

    switch (keys[key].function)
      {
      case VoiceControl::PA:
	if (isIdle ())
	  {
	    Self.stddebug () << "VoiceControl::PA\n";
	    Self.servicesOS ().outputs->setDO (key);
	    m_callindicator = key; //TODO: Correct output indication
	    Simplex (keys[key]);
	  } else
	  {
	    Cancell (*m_current_call);
	    Close (*m_current_call);
	  }
	break;
      case VoiceControl::DX:
	Self.stddebug () << "VoiceControl::DX\n";
	if (m_current_call)
	  {
	    if (m_current_call->id == keys[key].id)
	      {
		Cancell (*m_current_call);
		Close (*m_current_call);
	      }
	  }
	break;
      case VoiceControl::GR:
	Self.stddebug () << "VoiceControl::GR\n";
	break;
      case ControlLines::ON:
	Self.stddebug () << "ControlLines::ON\n";
	Activate (keys[key]);
	break;
      case ControlLines::OFF:
	Self.stddebug () << "ControlLines::OFF\n";
	Activate (keys[key]);
	break;
      case ControlLines::TOG:
	Self.stddebug () << "ControlLines::TOG\n";
	Activate (keys[key]);
	break;
      case Sys::Mute:
	Self.stddebug () << "Sys::Mute\n";
	MicMuteToggle ();
	break;
      case Sys::VOLUP:
	Self.stddebug () << "Sys::VOLUP\n";
	break;
      case Sys::VOLDOWN:
	Self.stddebug () << "Sys::VOLDOWN\n";
	VolMinus ();
	break;
      }
  }

  /**
   *
   * @param msg
   */
  void CallController::IncomeHandler (CtlMessage msg)
  {
    Self.stddebug () << "Control:ID=" << msg.id << " FN=" << msg.function << " DEST="
	<< msg.destination << " SRC=" << msg.source << "\n";
    if (msg.destination == UFW_IPS_SELF_CID)
      {
	switch (msg.function)
	  {
	  case ControlLines::ON:
	    Self.stddebug () << "ControlLines::ON\n";
	    Self.servicesOS ().outputs->setDO (msg.parameters);
	    break;
	  case ControlLines::OFF:
	    Self.stddebug () << "ControlLines::OFF\n";
	    Self.servicesOS ().outputs->resetDO (msg.parameters);
	    break;
	  case ControlLines::TOG:
	    Self.stddebug () << "ControlLines::TOG\n";
	    Self.servicesOS ().outputs->toggleDO (msg.parameters);
	    break;
	  case VoiceControl::DX:
	    if (m_current_call)
	      {
//		Cancell (*m_current_call);
		Close (*m_current_call);
	      }
	    onCall (msg);
	    break;
	  case VoiceControl::PA:
	    if (m_current_call)
	      {
//		Cancell (*m_current_call);
		Close (*m_current_call);
	      }
	    onCall (msg);
	    break;
	  case Responce::ACK:
	    onAck (msg);
	    break;
	  case Responce::Cancel:
	    onCancell (msg);
	    break;
	  }

      }
  }

  /************************************************************************
   * Thread that listen to incoming ctl messages
   */
  void CallController::threadFunction ()
  {

    uint8_t *rcv_buffer = new uint8_t[_GLOBAL_MTU];
    struct sockaddr_in self;
    struct sockaddr *ps;

    self.sin_family = AF_INET;
    self.sin_addr.s_addr = INADDR_ANY; //IP_ADDR_ANY;
    self.sin_port = htons(15000);

    ps = (sockaddr*) &self;
    int listener = -1000;

    if ((listener = socket (AF_INET, SOCK_DGRAM, 0)) < 0) LIB_ERROR_HANDL;
    if (bind (listener, ps, sizeof(self)) < 0) LIB_ERROR_HANDL;

    while (1)
      {
	int size = 0;
	size = recv (listener, rcv_buffer, _GLOBAL_MTU, 0);
	if (size > 0)
	  {
	    if (size != sizeof(CtlMessage))
	      {
		Self.stddebug () << "Control: size mismatch";
	      } else
	      {

		IncomeHandler (tmp_message->Deserialize (rcv_buffer));
	      }
	  }
      }

  }

}

///////////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////////

/**
 * ADC-DAC loop test
 * @param par
 */
void Loop (void *par)
{
//  audioDevice &loop = audioDevice::getInstance ();
  uint8_t *buffer = new uint8_t[audioDevice::getInstance ().ADCInstance ().getSize ()];
  audioDevice::getInstance ().Play ();

  while (1)
    {
      //inpipe -> outpipe
      audioDevice::getInstance ().ADCInstance ().pop (buffer);
      audioDevice::getInstance ().DACInstance ().push (buffer);
    }
}


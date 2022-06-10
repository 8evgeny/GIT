#ifndef CONFERENCECALL_H
#define CONFERENCECALL_H

#include "callcontrol.h"

/*!
 \brief Concrete State that implements and also provides a backreference to the CallControl object, associated
    with the State. This backreference can be used by States to transition the
    Context to another State.

 \class CallWaiting conferencecall.h "conferencecall.h"
*/
class alignas (4) ConferenceCall : public State
{

private:

    /*!
     \brief This method is used to end a conference call

    \fn endCircularCall
            */
    void endConferenceCall();

    /*!
     \brief This method is used to disconnect from the conference call

    \fn hungUp
            */
    void hungUp();
    /*!
     \brief The function is used to turn off all LEDs

    \fn switchOffLeds
     \param v is used to send LED dynamic storage to the function
            */
    void switchOffLeds(std::vector <uint16_t> &v);
    /*!
     \brief  The function sends request to the remote subscribers by UDP

    \fn sendRequest
     \param distId is used to send an ID remote subscriber to the function
     \param reqType is used to send a call type to the function
     \param timeout is used to send a timeout value to the function
            */
    void sendRequest(uint16_t distId, CallControl::Request reqType, uint16_t timeout);

    /*!
     \brief This method is used to handle acknowledgment requests

    \fn handleAck
            */
    void handleAck();

    /*!
     \brief This method is used to send an hang up request to disconnect from the conference call

     \fn confHungUpReq
    */
    void confHungUpReq();

public:
    /*!
     \brief The overridden method to handle a button press

     \fn handleButton
    */
    void handleButton() override;
    /*!
     \brief The overridden method to handle a received Json message

     \fn handleJsonMessage
    */
    void handleJsonMessage() override;
    /*!
     \brief The overridden method to handle a connection loss

    \fn handleLostLink
            */
    void handleLostLink() override;
    /*!
     \brief The overridden method is used to check a response to the request and send a repeated request

    \fn handleCheck
            */
    void handleRepeatedRequestCallBack() override;
    /*!
     \brief The overridden method is used to handle an unknown incoming call data when the auto answer callback is called

    \fn handleUnknownIncomingCallBack
            */
    void handleUnknownIncomingCallBack() override;

    constexpr static uint16_t TIMEOUT {300};
};

#endif // CONFERENCECALL_H

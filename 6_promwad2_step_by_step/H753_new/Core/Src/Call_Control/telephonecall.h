/*!
 \file telephonecall.h

*/

#ifndef TELEPHONECALL_H
#define TELEPHONECALL_H

#include "callcontrol.h"

/*!
 \brief Concrete State that implements and also provides a backreference to the CallControl object, associated
    with the State. This backreference can be used by States to transition the
    Context to another State.

 \class CallWaiting telephonecall.h "telephonecall.h"
*/
class TelephoneCall : public State
{
private:
    /*!
     \brief This method is used to handle acknowledgment requests

    \fn handleAck
            */
    void handleAck();
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

    constexpr static uint16_t DIALING_TIMEOUT {3000};
};

#endif // TELEPHONECALL_H

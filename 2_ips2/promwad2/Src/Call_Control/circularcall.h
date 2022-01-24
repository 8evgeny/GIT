/*!
 \file circularcall.h

*/

#ifndef CIRCULARCALL_H
#define CIRCULARCALL_H

#include "callcontrol.h"

/*!
 \brief Concrete State that implements and also provides a backreference to the CallControl object, associated
    with the State. This backreference can be used by States to transition the
    Context to another State.

 \class CallWaiting circularcall.h "circularcall.h"
*/
class CircularCall : public State
{
private:
    /*!
     \brief This method is used to end a circular call

     \fn endCircularCall
    */
    void endCircularCall();
    /*!
     \brief This method is used to disconnect from the circular call

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

#endif // CIRCULARCALL_H

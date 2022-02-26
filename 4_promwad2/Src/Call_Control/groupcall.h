#ifndef GROUPCALL_H
#define GROUPCALL_H

#include "callcontrol.h"

struct PriorStruct {
    uint8_t distId = 0,
            distPriority = 0;
};

/*!
 \brief Concrete State that implements and also provides a backreference to the CallControl object, associated
    with the State. This backreference can be used by States to transition the
    Context to another State.

 \class CallWaiting grooupcall.h "groupcall.h"
*/
class GroupCall : public State
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

//    void handleSendRequest(uint8_t) override;

//    constexpr static uint16_t TIMEOUT {200};

    uint16_t subscriberNum = 0;
//    PriorStruct priorArray[20];
//    uint8_t priorIter = 0;
};

#endif // GROUPCALL_H

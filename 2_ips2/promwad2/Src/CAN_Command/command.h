/**
 * @file command.h
 *
 */

#ifndef COMMAND_H
#define COMMAND_H

#ifdef STM32F777xx
#include "can_stm32f7xx.h"
#else
#include "can_stm32f1xx.h"
#include <vector>
#endif

/*!
 \brief Implementation of the command pattern.
        Create a class that encapsulates an object and a member function.

 \class Command command.h "command.h"
*/
class Command
{
public:
    /**
     * \brief Base destructor.
     *
     * \fn ~Command
     */
    virtual ~Command() {}

    virtual bool execute() = 0;

protected:
    /**
     * \brief Initialization method
     *
     * \fn Command
     * \param p pointer to CAN class
     */
    Command(CAN *p)
    {
        pCan = p;
    }
    CAN *pCan;  /**< The pointer to CAN class */
};

///**
// * \brief The class member to create a message
// *
// * \class CreateCanMessage command.h "command.h"
// */
//class CreateCanMessage : public Command
//{
//public:
//    /**
//     * \brief The message creating constructor
//     * \param p pointer to CAN class
//     */
//    CreateCanMessage(CAN* p) : Command(p) {}

//    /**
//     * \brief The message creating method
//     *
//     * \fn execute
//     */
//    bool execute()
//    {
//        pCan->createMessage();
//        return true;
//    }
//};

/**
 * \brief  The class member to send a message
 *
 * \class SendCanMessage command.h "command.h"
 */
class SendCanMessage : public Command
{
public:
    /**
     * \brief The message sending constructor
     *
     * \fn SendCanMessage
     * \param p pointer to CAN class
     */
    SendCanMessage(CAN *p) : Command(p) {}
    /**
     * \brief The message sending method
     *
     * \fn execute
     */
    bool execute()
    {
        pCan->sendMessage();
        return true;
    }
};

///**
// * \brief The message verification class member
// *
// * \class CheckCanMessage command.h "command.h"
// */
//class CheckCanMessage : public Command
//{
//public:
//    /**
//     * \brief The message verification constructor
//     *
//     * \fn CheckCanMessage
//     * \param p pointer to CAN class
//     */
//    CheckCanMessage(CAN *p) : Command(p) {}
//    /**
//     * \brief The message verification method
//     *
//     * \fn execute
//     */
//    bool execute()
//    {
//        return pCan->crcCheck();
//    }
//};

/**
 * \brief The function creates CAN message and sends it to the CAN bus
 *
 * \fn sendToCan
 */
void sendToCan();

#endif // COMMAND_H

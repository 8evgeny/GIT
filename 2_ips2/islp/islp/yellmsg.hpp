#ifndef YELLMSG_H
#define YELLMSG_H

#include "basicmessage.hpp"
#include "arcstylecrypt.hpp"

namespace ISLP {
/**
 * @class yellMsg
 * @brief The yellMsg class
 * Creates new session key and generate yell.
 * Yell() message must been sent to broadcast address, every client who undestands signature must send
 * StationInfo() packet to protocol multicast address/port.
 */

class yellMsg : public basicMessage
{
public:
    /**
     * @brief yellMsg - generate message and regenerate Key.
     * @param crypt - pointer to encryption object(!Key renewed when function calls!)
     * Notes:
     * - __!Function regenerate `crypt` key(you must save it to reuse (if need) `arcCrypt::takeKey():const *uint8_t`!__
     * - If pointer `crypt=null`, object will be created with generated key(!don't forget to delete it!).
     */
    yellMsg(arcCrypt *crypt);
    /**
     * @brief operator () - returns pointer to a message.
     * @return pointer to a message.
     *
     * Notes:
     * - __!Size of message not a constant. You must take it with `yellMsg::size():size_t` function!(see example)__
     *
     * Usage example:
     *
     * Send message to BSD-socket
     * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.c}
     * arcCrypt *key=nullptr;
     * yellMsg yell(key);
     * write(socket,yell(),yell.size());
     * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     */
    uint8_t* operator()();

private:
    void generatePayload();
    void setCryptKey();

#ifdef IPS_TESTING
public:
    static bool validateYell(void* data,size_t size);
    static bool validateYell(basicMessage *msg);
#endif
};

}
#endif // YELLMSG_H

/*
 * ITMacrocell.hpp
 *
 *  Created on: 21 сент. 2020 г.
 *      Author: Dmitry Donskih
 */

#ifndef USER_INC_ITMACROCELL_HPP_
#define USER_INC_ITMACROCELL_HPP_

#include <devices/stream_device.h>

void itmPutc (char c);
void itmPuts (const char *str); /* Pointer to the string */

namespace ufw
{



  class ioITM : public ttY
  {
    public:

      static ioITM& Instance ();
      ioITM* pointer ();

      void write_t (const char *buf, size_t size) override;
//      void printfl (const char *fmt, ...) override;
    protected:

    private:
      ioITM ();
      virtual ~ioITM ();
      ioITM& operator= (ioITM&);
  };

}
using coreOut=ufw::ioITM;
#endif /* USER_INC_ITMACROCELL_HPP_ */

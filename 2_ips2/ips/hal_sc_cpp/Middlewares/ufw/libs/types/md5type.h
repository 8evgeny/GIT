/*
 * md5type.h
 *
 *  Created on: 18 дек. 2020 г.
 *      Author: vader
 */

#ifndef UFW_TYPES_MD5TYPE_H_
#define UFW_TYPES_MD5TYPE_H_

namespace ufw
{
  
  class md5extends
  {
    public:
      md5extends ();
      ~md5extends ();
      md5extends (const md5extends &other) = delete;
      md5extends (md5extends &&other) = delete;
      md5extends& operator= (const md5extends &other) = delete;
      md5extends& operator= (md5extends &&other) = delete;
  };

} /* namespace ufw */

#endif /* UFW_TYPES_MD5TYPE_H_ */

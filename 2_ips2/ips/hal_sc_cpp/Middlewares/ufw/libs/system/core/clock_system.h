/*
 * clock_system.h
 *
 *  Created on: 18 дек. 2020 г.
 *      Author: vader
 */

#ifndef UFW_LIBS_SYSTEM_CORE_CLOCK_SYSTEM_H_
#define UFW_LIBS_SYSTEM_CORE_CLOCK_SYSTEM_H_

namespace ufw
{
  class ClockSystem
    {
      public:
	static ClockSystem& Instance();

	void Init();
      private:
	ClockSystem ();
	~ClockSystem ();
	ClockSystem (const ClockSystem &other) = delete;
	ClockSystem (ClockSystem &&other) = delete;
	ClockSystem& operator= (const ClockSystem &other) = delete;
	ClockSystem& operator= (ClockSystem &&other) = delete;
    };

} /* namespace ufw */

#endif /* UFW_LIBS_SYSTEM_CORE_CLOCK_SYSTEM_H_ */

#ifndef LIBS_HPP
#define LIBS_HPP
#include "libs/containers/ufw_array.h"
#include "libs/containers/ufw_string.h"
#include "libs/containers/ufw_vector.h"


/*
 *  TODO: run perf tests
 *  sudo sh -c 'echo 1 > /proc/sys/kernel/perf_event_paranoid'
 *  or add kernel.perf_event_paranoid=1 to /etc/sysctl.conf
 */

void CheckInt(int i=100);

#endif // LIBS_HPP

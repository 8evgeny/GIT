/*
 * apps.hpp
 *
 *  Created on: 6 авг. 2020 г.
 *      Author: Dmitry Donskih
 */

#ifndef INC_APPS_HPP_
#define INC_APPS_HPP_

#include <ipc_pipe.hpp>

void sawGenerator (void *pipe_in);
void pipePrinter (void *pipe_out);

#endif /* INC_APPS_HPP_ */

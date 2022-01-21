#ifndef LIB_MACROS_HPP
#define LIB_MACROS_HPP

#include <error/error_handler.h>
//#include <system/System.h>



#ifdef PLATFORM_TEST
#define UFW_DEBUG_INC			<iostream>
#define UFW_DEBUG_OUTPUT    		std::cout
#define UFW_DEBUG_PLACEHOLDER(x) 	UFW_DEBUG_OUTPUT<<__PRETTY_FUNCTION__<<": " x <<"/n"; //TODO: Rewrite macro. Add variable arguments print
#else
#define UFW_DEBUG_INC			<system/system.h>
#define UFW_DEBUG_OUTPUT
#define UFW_DEBUG_PLACEHOLDER(x)
//#define __UFWDebug			ufw::System::stddebug()<<__PRETTY_FUNCTION__<<"\nDEBUG:"
//#define __UFWService			ufw::System::stddebug()<<"SerVice:"
#endif

#ifdef UFW_CONTAINERS_DEBUG
#define CONTAINER_DBGMSG    		UFW_DEBUG_OUTPUT
#else
#define CONTAINER_DBGMSG    		UFW_DEBUG_OUTPUT
#endif

#ifdef UFW_ALLOCATIONS_DEBUG
#define ALLOCATOR_DBGMSG    		UFW_DEBUG_OUTPUT
#else
#define ALLOCATOR_DBGMSG    //
#endif

#define STRINGIFY(a)  #a
#define TYPE_NAME(Type)	STRINGIFY(Type)

#define UFW_COUNT_ALLOCATIONS

#endif // LIB_MACROS_HPP

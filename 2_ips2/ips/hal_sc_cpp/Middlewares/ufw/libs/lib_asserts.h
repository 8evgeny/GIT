#pragma once


#ifdef PLATFORM_TEST
#include <exception>
#define LIB_ERROR_HANDL throw
#define PLATFORM_MEM_EXCEPTION  throw std::bad_alloc()
#else
#include <error/error_handler.h>
#define LIB_ERROR_HANDL ufw::errors::Handle(ufw::errors::type::Critical, ufw::errors::place::Logical,__PRETTY_FUNCTION__)
#define PLATFORM_MEM_EXCEPTION  ufw::errors::Handle(ufw::errors::type::Critical,ufw::errors::place::Memory,__PRETTY_FUNCTION__)
#define LIB_ASSERT_HANDLE ufw::errors::Handle(ufw::errors::type::Assert, ufw::errors::place::Assertation,__PRETTY_FUNCTION__)
#define LIB_HALERROR_HANDLE ufw::errors::Handle(ufw::errors::type::Critical, ufw::errors::place::HAL,__PRETTY_FUNCTION__)
#define LIB_MEMERROR_HANDLE ufw::errors::Handle(ufw::errors::type::Critical, ufw::errors::place::Memory,__PRETTY_FUNCTION__)
#define LIB_CPUERROR_HANDLE ufw::errors::Handle(ufw::errors::type::Critical, ufw::errors::place::CPU,__PRETTY_FUNCTION__)

#endif

#define LIB_ASSERT(x)	if(!(x)){LIB_ASSERT_HANDLE;}

#define UASSERT	//TODO: create standart assert;

#ifdef DEBUG
#ifdef PLATFORM_TEST
#define LIB_DBG std::cout
#else
#define LIB_DBG //
#endif
#else
#define LIB_DBG //
#endif

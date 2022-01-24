/*
 * sysRNG.hpp - a part of CMppCore -an object engine for Cortex-M
 * microcontrollers.
 *
 *  Created on: 24 сент. 2020 г.
 *      Author: Dmitry Donskih
 *
 * Copyright (c) 2019-2020 by Dmitry Donskih
 * All rights reserved.
 * 
 * Attention Software modification allowed only with 
 * the permission of the author.
 */
#ifndef SYSRNG_HPP_
#define SYSRNG_HPP_



#include <cstdint>
#include <cstddef>

class sysRNG
{
  public:
    static sysRNG& getInstance();
    sysRNG* getInstancePtr();
    void number(uint32_t* num_ptr);
    void numbers(void* data_ptr,size_t size);
    int number();
    template <typename T>
    T get()
    {
      T temp;
      numbers(&temp,sizeof(T));
      return temp;
    }
#ifdef TESTS
    friend void test_rng(void *args);

#endif


  private:
    sysRNG ();
    ~sysRNG ();
    sysRNG (const sysRNG &other) = delete;
    sysRNG (sysRNG &&other) = delete;
    sysRNG& operator= (const sysRNG &other) = delete;
    sysRNG& operator= (sysRNG &&other) = delete;
};

#ifdef TESTS
void test_rng(void *args);

#endif

#endif /* SYSRNG_HPP_ */

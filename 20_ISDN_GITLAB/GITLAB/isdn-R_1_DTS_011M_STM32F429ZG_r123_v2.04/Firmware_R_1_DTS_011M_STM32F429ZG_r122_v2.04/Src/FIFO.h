/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   FIFO.h
 * Author: roman.ganaza
 *
 * Created on November 11, 2020, 11:24 AM
 */

#ifndef FIFO_H
#define FIFO_H

#define __COUNTERS_TYPE     unsigned short
#define __ELEMENTS_TYPE     unsigned char
//размер должен быть степенью двойки: 4,8,16,32...128
// если размер нужен 256 или более, необходимо поменять тип tail и head на unsigned short
#define FIFO( size ) struct {\
    __COUNTERS_TYPE tail;\
    __COUNTERS_TYPE head;\
    __ELEMENTS_TYPE buf[size];\
  }
#define FIFO_int16( size ) struct {\
    __COUNTERS_TYPE tail;\
    __COUNTERS_TYPE head;\
    signed short buf[size];\
  }

//количество элементов в очереди
#define FIFO_COUNT(fifo)     ((__COUNTERS_TYPE)((fifo.head-fifo.tail) & (FIFO_SIZE(fifo)-1))) // обязательно приведение типа к беззнаковому

//размер fifo
#define FIFO_SIZE(fifo)      ( sizeof(fifo.buf)/sizeof(fifo.buf[0]) )

//fifo заполнено?
#define FIFO_IS_FULL(fifo)   (FIFO_COUNT(fifo)==FIFO_SIZE(fifo))

//fifo пусто?
#define FIFO_IS_EMPTY(fifo)  (fifo.tail==fifo.head)

//количество свободного места в fifo
#define FIFO_SPACE(fifo)     (FIFO_SIZE(fifo)-FIFO_COUNT(fifo))

//поместить элемент в fifo
#define FIFO_PUSH(fifo, byte) \
  do\
  {\
    fifo.buf[fifo.head & (FIFO_SIZE(fifo)-1)]=byte;\
    fifo.head++;\
  } while(0)

//взять первый элемент из fifo
#define FIFO_FRONT(fifo) (fifo.buf[fifo.tail & (FIFO_SIZE(fifo)-1)])

//взять первый элемент из FIFO и уменьшить количество элементов в очереди
#define FIFO_POP(fifo)   (fifo.buf[fifo.tail++ & (FIFO_SIZE(fifo)-1)])

//очистить fifo
#define FIFO_FLUSH(fifo)   \
  {\
    fifo.tail=0;\
    fifo.head=0;\
  } 

#endif /* FIFO_H */


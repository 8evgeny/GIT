/*
 * sysflash.hpp
 *
 *  Created on: Aug 29, 2020
 *      Author: Dmitry Dmitry Donskih
 */

#ifndef USER_INC_SYSFLASH_HPP_
#define USER_INC_SYSFLASH_HPP_
#include <cstdint>
#include <cstddef>

#define SYS_IC_FLASHWORD_SIZE	(256/8)

#define TESTS

typedef union
    {
    int i[SYS_IC_FLASHWORD_SIZE / sizeof(int)];
    char c[SYS_IC_FLASHWORD_SIZE / sizeof(char)];
    long int l[SYS_IC_FLASHWORD_SIZE / sizeof(long int)];
    uint8_t b[SYS_IC_FLASHWORD_SIZE];
    uint16_t hw[SYS_IC_FLASHWORD_SIZE / sizeof(uint16_t)];
    uint32_t w[SYS_IC_FLASHWORD_SIZE / sizeof(uint32_t)];
    } flashword_t;

class systemFlash
    { //TODO: write tests for every type of IO ops
protected:
    systemFlash();
    systemFlash(const systemFlash&);
    systemFlash& operator=(systemFlash&);
    void read_block(void *adress);
    int write_block(void *flash_dest, void *mem_src);
    int write_bytes(void *flash_dest, void *mem_src, size_t size);
    int erase_sector(uint32_t sector=0);

    flashword_t *blk_buffer=nullptr;
    flashword_t *i_buffer=nullptr;
    flashword_t *o_buffer=nullptr;
    uint32_t errno = 0;

public:
    static systemFlash& getInstance();
    int write(int32_t *dest, int value);
    int write(void *dest, void *src, size_t size);
    int fill(void *dest, int value, size_t size);
    flashword_t readFlashword(void *address);

#ifdef TESTS
    friend void testFlashFs(void *args);
    friend void formatIntFlash(void *args);
#endif

    };

#ifdef TESTS

void testFlashFs(void *args);
void formatIntFlash(void *args);

#endif

#endif /* USER_INC_SYSFLASH_HPP_ */

/*
 * =====================================================================================
 *
 *       Filename:  disasm.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/06/2019 10:14:54 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  pluto (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef _TEST_H_
#define _TEST_H_

#include <udis86.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

extern ud_t ud_obj;

void custom_init_ud_t(void* buffer, int buffer_size);
int print_asm(void* buffer, int buffer_size, void* mem_addr);

#endif

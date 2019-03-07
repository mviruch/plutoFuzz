/*
 * =====================================================================================
 *
 *       Filename:  msg.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/06/2019 10:41:54 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  pluto (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdarg.h>

#include <sys/types.h>

void msg(pid_t pid, const char* format, ...);

/*
 * LIBOIL - Library of Optimized Inner Loops
 * Copyright (c) 2003,2004 David A. Schleef <ds@schleef.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _LIBOIL_DEBUG_H_
#define _LIBOIL_DEBUG_H_

enum {
  OIL_DEBUG_NONE = 0,
  OIL_DEBUG_ERROR,
  OIL_DEBUG_WARNING,
  OIL_DEBUG_INFO,
  OIL_DEBUG_DEBUG,
  OIL_DEBUG_LOG
};

#define OIL_ERROR(...) OIL_DEBUG_PRINT(OIL_DEBUG_ERROR, __VA_ARGS__)
#define OIL_WARNING(...) OIL_DEBUG_PRINT(OIL_DEBUG_WARNING, __VA_ARGS__)
#define OIL_INFO(...) OIL_DEBUG_PRINT(OIL_DEBUG_INFO, __VA_ARGS__)
#define OIL_DEBUG(...) OIL_DEBUG_PRINT(OIL_DEBUG_DEBUG, __VA_ARGS__)
#define OIL_LOG(...) OIL_DEBUG_PRINT(OIL_DEBUG_LOG, __VA_ARGS__)

#ifndef __PRETTY_FUNCTION__
#define __PRETTY_FUNCTION__ ""
#endif

#define OIL_DEBUG_PRINT(level, ...) do { \
  if(_oil_debug_enabled) { \
    oil_debug_print((level), __FILE__, __PRETTY_FUNCTION__, __LINE__, \
	__VA_ARGS__); \
  } \
}while(0)

extern int _oil_debug_enabled;
extern int _oil_debug_level;

void _oil_debug_init (void);

void oil_debug_print (int level, const char *file, const char *func,
    int line, const char *format, ...);

#endif


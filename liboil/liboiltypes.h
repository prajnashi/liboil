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

#ifndef _LIBOIL_TYPES_H_
#define _LIBOIL_TYPES_H_

#include <stdint.h>

#ifndef NULL
#define NULL ((void *)0)
#endif

typedef struct _OilFunctionClass OilFunctionClass;
typedef struct _OilFunctionImpl OilFunctionImpl;
typedef struct _OilParameter OilParameter;
typedef struct _OilTest OilTest;

typedef void (*OilTestFunction) (OilTest *test);

#define type_s8 int8_t
#define type_u8 uint8_t
#define type_s16 int16_t
#define type_u16 uint16_t
#define type_s32 int32_t
#define type_u32 uint32_t
#define type_f32 float
#define type_f64 double

#define type_min_s8 (-128)
#define type_min_u8 (0)
#define type_min_s16 (-32768)
#define type_min_u16 (0)
#define type_min_s32 (-2147483647 - 1)
#define type_min_u32 (0)

#define type_max_s8 (127)
#define type_max_u8 (255)
#define type_max_s16 (32767)
#define type_max_u16 (65535)
#define type_max_s32 (2147483647)
#define type_max_u32 (4294967295U)


#endif


/* liboil - Library of Optimized Inner Loops
 * Copyright (C) 2003  David A. Schleef <ds@schleef.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of version 2.1 of the GNU Lesser General
 * Public License as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307 USA.
 */

#ifndef _LIBOIL_TYPES_H_
#define _LIBOIL_TYPES_H_

#include <stdint.h>

#ifndef NULL
#define NULL ((void *)0)
#endif

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


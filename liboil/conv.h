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

#ifndef _LIBOIL_CONV_H_
#define _LIBOIL_CONV_H_

#include <liboil/liboilfunction.h>

OIL_DECLARE_CLASS(conv_f64_s8);
OIL_DECLARE_CLASS(conv_f64_s16);
OIL_DECLARE_CLASS(conv_f64_s32);
OIL_DECLARE_CLASS(conv_f64_u8);
OIL_DECLARE_CLASS(conv_f64_u16);
OIL_DECLARE_CLASS(conv_f64_u32);
OIL_DECLARE_CLASS(conv_f64_f32);

OIL_DECLARE_CLASS(conv_f32_s8);
OIL_DECLARE_CLASS(conv_f32_s16);
OIL_DECLARE_CLASS(conv_f32_u8);
OIL_DECLARE_CLASS(conv_f32_u16);

OIL_DECLARE_CLASS(conv_s32_s8);
OIL_DECLARE_CLASS(conv_s32_s16);
OIL_DECLARE_CLASS(conv_s32_u8);
OIL_DECLARE_CLASS(conv_s32_u16);

OIL_DECLARE_CLASS(conv_u32_s8);
OIL_DECLARE_CLASS(conv_u32_s16);
OIL_DECLARE_CLASS(conv_u32_u8);
OIL_DECLARE_CLASS(conv_u32_u16);

OIL_DECLARE_CLASS(conv_s16_s8);
OIL_DECLARE_CLASS(conv_s16_u8);

OIL_DECLARE_CLASS(conv_u16_s8);
OIL_DECLARE_CLASS(conv_u16_u8);

OIL_DECLARE_CLASS(clipconv_s8_u8);
OIL_DECLARE_CLASS(clipconv_s8_s16);
OIL_DECLARE_CLASS(clipconv_s8_u16);
OIL_DECLARE_CLASS(clipconv_s8_s32);
OIL_DECLARE_CLASS(clipconv_s8_u32);
OIL_DECLARE_CLASS(clipconv_s8_f32);
OIL_DECLARE_CLASS(clipconv_s8_f64);

OIL_DECLARE_CLASS(clipconv_u8_s8);
OIL_DECLARE_CLASS(clipconv_u8_s16);
OIL_DECLARE_CLASS(clipconv_u8_u16);
OIL_DECLARE_CLASS(clipconv_u8_s32);
OIL_DECLARE_CLASS(clipconv_u8_u32);
OIL_DECLARE_CLASS(clipconv_u8_f32);
OIL_DECLARE_CLASS(clipconv_u8_f64);

OIL_DECLARE_CLASS(clipconv_s16_u16);
OIL_DECLARE_CLASS(clipconv_s16_s32);
OIL_DECLARE_CLASS(clipconv_s16_u32);
OIL_DECLARE_CLASS(clipconv_s16_f32);
OIL_DECLARE_CLASS(clipconv_s16_f64);

OIL_DECLARE_CLASS(clipconv_u16_s16);
OIL_DECLARE_CLASS(clipconv_u16_s32);
OIL_DECLARE_CLASS(clipconv_u16_u32);
OIL_DECLARE_CLASS(clipconv_u16_f32);
OIL_DECLARE_CLASS(clipconv_u16_f64);

OIL_DECLARE_CLASS(clipconv_s32_u32);
OIL_DECLARE_CLASS(clipconv_s32_f32);
OIL_DECLARE_CLASS(clipconv_s32_f64);

OIL_DECLARE_CLASS(clipconv_u32_s32);
OIL_DECLARE_CLASS(clipconv_u32_f32);
OIL_DECLARE_CLASS(clipconv_u32_f64);

OIL_DECLARE_CLASS(conv_f32_s32);
OIL_DECLARE_CLASS(conv_f32_u32);
OIL_DECLARE_CLASS(conv_f32_f64);




OIL_DECLARE_CLASS(conv_s8_u8);
OIL_DECLARE_CLASS(conv_s8_s16);
OIL_DECLARE_CLASS(conv_s8_u16);
OIL_DECLARE_CLASS(conv_s8_s32);
OIL_DECLARE_CLASS(conv_s8_u32);
OIL_DECLARE_CLASS(conv_s8_f32);
OIL_DECLARE_CLASS(conv_s8_f64);

OIL_DECLARE_CLASS(conv_u8_s8);
OIL_DECLARE_CLASS(conv_u8_s16);
OIL_DECLARE_CLASS(conv_u8_u16);
OIL_DECLARE_CLASS(conv_u8_s32);
OIL_DECLARE_CLASS(conv_u8_u32);
OIL_DECLARE_CLASS(conv_u8_f32);
OIL_DECLARE_CLASS(conv_u8_f64);

OIL_DECLARE_CLASS(conv_s16_u16);
OIL_DECLARE_CLASS(conv_s16_s32);
OIL_DECLARE_CLASS(conv_s16_u32);
OIL_DECLARE_CLASS(conv_s16_f32);
OIL_DECLARE_CLASS(conv_s16_f64);

OIL_DECLARE_CLASS(conv_u16_s16);
OIL_DECLARE_CLASS(conv_u16_s32);
OIL_DECLARE_CLASS(conv_u16_u32);
OIL_DECLARE_CLASS(conv_u16_f32);
OIL_DECLARE_CLASS(conv_u16_f64);

OIL_DECLARE_CLASS(conv_s32_u32);
OIL_DECLARE_CLASS(conv_s32_f32);
OIL_DECLARE_CLASS(conv_s32_f64);

OIL_DECLARE_CLASS(conv_u32_s32);
OIL_DECLARE_CLASS(conv_u32_f32);
OIL_DECLARE_CLASS(conv_u32_f64);

#endif


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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <liboil/liboilfunction.h>


OIL_DEFINE_CLASS(abs_u8_s8, NULL);
OIL_DEFINE_CLASS(abs_u16_s16, NULL);
OIL_DEFINE_CLASS(abs_u32_s32, NULL);
OIL_DEFINE_CLASS(abs_f32_f32, NULL);
OIL_DEFINE_CLASS(abs_f64_f64, NULL);

OIL_DEFINE_CLASS(average2_u8, NULL);

#if 0
OIL_DEFINE_CLASS(clip_s8, NULL);
OIL_DEFINE_CLASS(clip_u8, NULL);
OIL_DEFINE_CLASS(clip_s16, NULL);
OIL_DEFINE_CLASS(clip_u16, NULL);
OIL_DEFINE_CLASS(clip_s32, NULL);
OIL_DEFINE_CLASS(clip_u32, NULL);
OIL_DEFINE_CLASS(clip_f32, NULL);
OIL_DEFINE_CLASS(clip_f64, NULL);
#endif

OIL_DEFINE_CLASS(dct12_f32, NULL);

OIL_DEFINE_CLASS(dct36_f32, NULL);

OIL_DEFINE_CLASS(diffsquaresum_f64, NULL);

OIL_DEFINE_CLASS(downsample1x_f64, NULL);

OIL_DEFINE_CLASS(fdct8_f64, NULL);

OIL_DEFINE_CLASS(fdct8x8_f64, NULL);

OIL_DEFINE_CLASS(fdct8x8s_s16, NULL);

OIL_DEFINE_CLASS(get8x8_f64, NULL);

//OIL_DEFINE_CLASS(idct8_f64, NULL);

//OIL_DEFINE_CLASS(idct8x8_s16, NULL);
OIL_DEFINE_CLASS(idct8x8s_s16, NULL);
OIL_DEFINE_CLASS(imdct32_f32, NULL);
OIL_DEFINE_CLASS(mix_u8, NULL);
OIL_DEFINE_CLASS(mult8x8_s16, NULL);

OIL_DEFINE_CLASS(multsum_f32, NULL);
OIL_DEFINE_CLASS(multsum_f64, NULL);

OIL_DEFINE_CLASS(permute_s8, NULL);
OIL_DEFINE_CLASS(permute_u8, NULL);
OIL_DEFINE_CLASS(permute_s16, NULL);
OIL_DEFINE_CLASS(permute_u16, NULL);
OIL_DEFINE_CLASS(permute_s32, NULL);
OIL_DEFINE_CLASS(permute_u32, NULL);
OIL_DEFINE_CLASS(permute_f32, NULL);
OIL_DEFINE_CLASS(permute_f64, NULL);

OIL_DEFINE_CLASS(sad8x8_f64, NULL);
OIL_DEFINE_CLASS(sad8x8_s16, NULL);

OIL_DEFINE_CLASS(scalaradd_s8, NULL);
OIL_DEFINE_CLASS(scalaradd_u8, NULL);
OIL_DEFINE_CLASS(scalaradd_s16, NULL);
OIL_DEFINE_CLASS(scalaradd_u16, NULL);
OIL_DEFINE_CLASS(scalaradd_s32, NULL);
OIL_DEFINE_CLASS(scalaradd_u32, NULL);
OIL_DEFINE_CLASS(scalaradd_f32, NULL);
OIL_DEFINE_CLASS(scalaradd_f64, NULL);

OIL_DEFINE_CLASS(scalarmult_s8, NULL);
OIL_DEFINE_CLASS(scalarmult_u8, NULL);
OIL_DEFINE_CLASS(scalarmult_s16, NULL);
OIL_DEFINE_CLASS(scalarmult_u16, NULL);
OIL_DEFINE_CLASS(scalarmult_s32, NULL);
OIL_DEFINE_CLASS(scalarmult_u32, NULL);
OIL_DEFINE_CLASS(scalarmult_f32, NULL);
OIL_DEFINE_CLASS(scalarmult_f64, NULL);

OIL_DEFINE_CLASS(sincos_f64, NULL);
OIL_DEFINE_CLASS(squaresum_f64, NULL);
OIL_DEFINE_CLASS(sum_f64, NULL);

OIL_DEFINE_CLASS(trans8x8_s8, NULL);
OIL_DEFINE_CLASS(trans8x8_u8, NULL);
OIL_DEFINE_CLASS(trans8x8_s16, NULL);
OIL_DEFINE_CLASS(trans8x8_u16, NULL);
OIL_DEFINE_CLASS(trans8x8_s32, NULL);
OIL_DEFINE_CLASS(trans8x8_u32, NULL);
OIL_DEFINE_CLASS(trans8x8_f32, NULL);
OIL_DEFINE_CLASS(trans8x8_f64, NULL);

OIL_DEFINE_CLASS(vectoradd_s8, NULL);
OIL_DEFINE_CLASS(vectoradd_u8, NULL);
OIL_DEFINE_CLASS(vectoradd_s16, NULL);
OIL_DEFINE_CLASS(vectoradd_u16, NULL);
OIL_DEFINE_CLASS(vectoradd_s32, NULL);
OIL_DEFINE_CLASS(vectoradd_u32, NULL);
OIL_DEFINE_CLASS(vectoradd_f32, NULL);
OIL_DEFINE_CLASS(vectoradd_f64, NULL);

OIL_DEFINE_CLASS(zigzag8x8_s16, NULL);


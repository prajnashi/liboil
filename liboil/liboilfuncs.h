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

#ifndef _LIBOIL_FUNCS_H_
#define _LIBOIL_FUNCS_H_

extern OilFunctionClass *oil_function_conv_s8_u8_class_ptr;
#define conv_s8_u8 ((void (*)(int8_t *dest, int dstr, uint8_t *src, int sstr, int n)) \
	oil_function_conv_s8_u8_class_ptr->func)
extern OilFunctionClass *oil_function_conv_s8_s16_class_ptr;
#define conv_s8_s16 ((void (*)(int8_t *dest, int dstr, int16_t *src, int sstr, int n)) \
	oil_function_conv_s8_s16_class_ptr->func)
extern OilFunctionClass *oil_function_conv_s8_u16_class_ptr;
#define conv_s8_u16 ((void (*)(int8_t *dest, int dstr, uint16_t *src, int sstr, int n)) \
	oil_function_conv_s8_u16_class_ptr->func)
extern OilFunctionClass *oil_function_conv_s8_s32_class_ptr;
#define conv_s8_s32 ((void (*)(int8_t *dest, int dstr, int32_t *src, int sstr, int n)) \
	oil_function_conv_s8_s32_class_ptr->func)
extern OilFunctionClass *oil_function_conv_s8_u32_class_ptr;
#define conv_s8_u32 ((void (*)(int8_t *dest, int dstr, uint32_t *src, int sstr, int n)) \
	oil_function_conv_s8_u32_class_ptr->func)
extern OilFunctionClass *oil_function_conv_s8_f32_class_ptr;
#define conv_s8_f32 ((void (*)(int8_t *dest, int dstr, float *src, int sstr, int n)) \
	oil_function_conv_s8_f32_class_ptr->func)
extern OilFunctionClass *oil_function_conv_s8_f64_class_ptr;
#define conv_s8_f64 ((void (*)(int8_t *dest, int dstr, double *src, int sstr, int n)) \
	oil_function_conv_s8_f64_class_ptr->func)
extern OilFunctionClass *oil_function_conv_u8_s8_class_ptr;
#define conv_u8_s8 ((void (*)(uint8_t *dest, int dstr, int8_t *src, int sstr, int n)) \
	oil_function_conv_u8_s8_class_ptr->func)
extern OilFunctionClass *oil_function_conv_u8_s16_class_ptr;
#define conv_u8_s16 ((void (*)(uint8_t *dest, int dstr, int16_t *src, int sstr, int n)) \
	oil_function_conv_u8_s16_class_ptr->func)
extern OilFunctionClass *oil_function_conv_u8_u16_class_ptr;
#define conv_u8_u16 ((void (*)(uint8_t *dest, int dstr, uint16_t *src, int sstr, int n)) \
	oil_function_conv_u8_u16_class_ptr->func)
extern OilFunctionClass *oil_function_conv_u8_s32_class_ptr;
#define conv_u8_s32 ((void (*)(uint8_t *dest, int dstr, int32_t *src, int sstr, int n)) \
	oil_function_conv_u8_s32_class_ptr->func)
extern OilFunctionClass *oil_function_conv_u8_u32_class_ptr;
#define conv_u8_u32 ((void (*)(uint8_t *dest, int dstr, uint32_t *src, int sstr, int n)) \
	oil_function_conv_u8_u32_class_ptr->func)
extern OilFunctionClass *oil_function_conv_u8_f32_class_ptr;
#define conv_u8_f32 ((void (*)(uint8_t *dest, int dstr, float *src, int sstr, int n)) \
	oil_function_conv_u8_f32_class_ptr->func)
extern OilFunctionClass *oil_function_conv_u8_f64_class_ptr;
#define conv_u8_f64 ((void (*)(uint8_t *dest, int dstr, double *src, int sstr, int n)) \
	oil_function_conv_u8_f64_class_ptr->func)
extern OilFunctionClass *oil_function_conv_s16_s8_class_ptr;
#define conv_s16_s8 ((void (*)(int16_t *dest, int dstr, int8_t *src, int sstr, int n)) \
	oil_function_conv_s16_s8_class_ptr->func)
extern OilFunctionClass *oil_function_conv_s16_u8_class_ptr;
#define conv_s16_u8 ((void (*)(int16_t *dest, int dstr, uint8_t *src, int sstr, int n)) \
	oil_function_conv_s16_u8_class_ptr->func)
extern OilFunctionClass *oil_function_conv_s16_u16_class_ptr;
#define conv_s16_u16 ((void (*)(int16_t *dest, int dstr, uint16_t *src, int sstr, int n)) \
	oil_function_conv_s16_u16_class_ptr->func)
extern OilFunctionClass *oil_function_conv_s16_s32_class_ptr;
#define conv_s16_s32 ((void (*)(int16_t *dest, int dstr, int32_t *src, int sstr, int n)) \
	oil_function_conv_s16_s32_class_ptr->func)
extern OilFunctionClass *oil_function_conv_s16_u32_class_ptr;
#define conv_s16_u32 ((void (*)(int16_t *dest, int dstr, uint32_t *src, int sstr, int n)) \
	oil_function_conv_s16_u32_class_ptr->func)
extern OilFunctionClass *oil_function_conv_s16_f32_class_ptr;
#define conv_s16_f32 ((void (*)(int16_t *dest, int dstr, float *src, int sstr, int n)) \
	oil_function_conv_s16_f32_class_ptr->func)
extern OilFunctionClass *oil_function_conv_s16_f64_class_ptr;
#define conv_s16_f64 ((void (*)(int16_t *dest, int dstr, double *src, int sstr, int n)) \
	oil_function_conv_s16_f64_class_ptr->func)
extern OilFunctionClass *oil_function_conv_u16_s8_class_ptr;
#define conv_u16_s8 ((void (*)(uint16_t *dest, int dstr, int8_t *src, int sstr, int n)) \
	oil_function_conv_u16_s8_class_ptr->func)
extern OilFunctionClass *oil_function_conv_u16_u8_class_ptr;
#define conv_u16_u8 ((void (*)(uint16_t *dest, int dstr, uint8_t *src, int sstr, int n)) \
	oil_function_conv_u16_u8_class_ptr->func)
extern OilFunctionClass *oil_function_conv_u16_s16_class_ptr;
#define conv_u16_s16 ((void (*)(uint16_t *dest, int dstr, int16_t *src, int sstr, int n)) \
	oil_function_conv_u16_s16_class_ptr->func)
extern OilFunctionClass *oil_function_conv_u16_s32_class_ptr;
#define conv_u16_s32 ((void (*)(uint16_t *dest, int dstr, int32_t *src, int sstr, int n)) \
	oil_function_conv_u16_s32_class_ptr->func)
extern OilFunctionClass *oil_function_conv_u16_u32_class_ptr;
#define conv_u16_u32 ((void (*)(uint16_t *dest, int dstr, uint32_t *src, int sstr, int n)) \
	oil_function_conv_u16_u32_class_ptr->func)
extern OilFunctionClass *oil_function_conv_u16_f32_class_ptr;
#define conv_u16_f32 ((void (*)(uint16_t *dest, int dstr, float *src, int sstr, int n)) \
	oil_function_conv_u16_f32_class_ptr->func)
extern OilFunctionClass *oil_function_conv_u16_f64_class_ptr;
#define conv_u16_f64 ((void (*)(uint16_t *dest, int dstr, double *src, int sstr, int n)) \
	oil_function_conv_u16_f64_class_ptr->func)
extern OilFunctionClass *oil_function_conv_s32_s8_class_ptr;
#define conv_s32_s8 ((void (*)(int32_t *dest, int dstr, int8_t *src, int sstr, int n)) \
	oil_function_conv_s32_s8_class_ptr->func)
extern OilFunctionClass *oil_function_conv_s32_s16_class_ptr;
#define conv_s32_s16 ((void (*)(int32_t *dest, int dstr, int16_t *src, int sstr, int n)) \
	oil_function_conv_s32_s16_class_ptr->func)
extern OilFunctionClass *oil_function_conv_s32_u8_class_ptr;
#define conv_s32_u8 ((void (*)(int32_t *dest, int dstr, uint8_t *src, int sstr, int n)) \
	oil_function_conv_s32_u8_class_ptr->func)
extern OilFunctionClass *oil_function_conv_s32_u16_class_ptr;
#define conv_s32_u16 ((void (*)(int32_t *dest, int dstr, uint16_t *src, int sstr, int n)) \
	oil_function_conv_s32_u16_class_ptr->func)
extern OilFunctionClass *oil_function_conv_s32_u32_class_ptr;
#define conv_s32_u32 ((void (*)(int32_t *dest, int dstr, uint32_t *src, int sstr, int n)) \
	oil_function_conv_s32_u32_class_ptr->func)
extern OilFunctionClass *oil_function_conv_s32_f32_class_ptr;
#define conv_s32_f32 ((void (*)(int32_t *dest, int dstr, float *src, int sstr, int n)) \
	oil_function_conv_s32_f32_class_ptr->func)
extern OilFunctionClass *oil_function_conv_s32_f64_class_ptr;
#define conv_s32_f64 ((void (*)(int32_t *dest, int dstr, double *src, int sstr, int n)) \
	oil_function_conv_s32_f64_class_ptr->func)
extern OilFunctionClass *oil_function_conv_u32_s8_class_ptr;
#define conv_u32_s8 ((void (*)(uint32_t *dest, int dstr, int8_t *src, int sstr, int n)) \
	oil_function_conv_u32_s8_class_ptr->func)
extern OilFunctionClass *oil_function_conv_u32_s16_class_ptr;
#define conv_u32_s16 ((void (*)(uint32_t *dest, int dstr, int16_t *src, int sstr, int n)) \
	oil_function_conv_u32_s16_class_ptr->func)
extern OilFunctionClass *oil_function_conv_u32_u8_class_ptr;
#define conv_u32_u8 ((void (*)(uint32_t *dest, int dstr, uint8_t *src, int sstr, int n)) \
	oil_function_conv_u32_u8_class_ptr->func)
extern OilFunctionClass *oil_function_conv_u32_u16_class_ptr;
#define conv_u32_u16 ((void (*)(uint32_t *dest, int dstr, uint16_t *src, int sstr, int n)) \
	oil_function_conv_u32_u16_class_ptr->func)
extern OilFunctionClass *oil_function_conv_u32_s32_class_ptr;
#define conv_u32_s32 ((void (*)(uint32_t *dest, int dstr, int32_t *src, int sstr, int n)) \
	oil_function_conv_u32_s32_class_ptr->func)
extern OilFunctionClass *oil_function_conv_u32_f32_class_ptr;
#define conv_u32_f32 ((void (*)(uint32_t *dest, int dstr, float *src, int sstr, int n)) \
	oil_function_conv_u32_f32_class_ptr->func)
extern OilFunctionClass *oil_function_conv_u32_f64_class_ptr;
#define conv_u32_f64 ((void (*)(uint32_t *dest, int dstr, double *src, int sstr, int n)) \
	oil_function_conv_u32_f64_class_ptr->func)
extern OilFunctionClass *oil_function_conv_f32_s8_class_ptr;
#define conv_f32_s8 ((void (*)(float *dest, int dstr, int8_t *src, int sstr, int n)) \
	oil_function_conv_f32_s8_class_ptr->func)
extern OilFunctionClass *oil_function_conv_f32_s16_class_ptr;
#define conv_f32_s16 ((void (*)(float *dest, int dstr, int16_t *src, int sstr, int n)) \
	oil_function_conv_f32_s16_class_ptr->func)
extern OilFunctionClass *oil_function_conv_f32_u8_class_ptr;
#define conv_f32_u8 ((void (*)(float *dest, int dstr, uint8_t *src, int sstr, int n)) \
	oil_function_conv_f32_u8_class_ptr->func)
extern OilFunctionClass *oil_function_conv_f32_u16_class_ptr;
#define conv_f32_u16 ((void (*)(float *dest, int dstr, uint16_t *src, int sstr, int n)) \
	oil_function_conv_f32_u16_class_ptr->func)
extern OilFunctionClass *oil_function_conv_f32_s32_class_ptr;
#define conv_f32_s32 ((void (*)(float *dest, int dstr, int32_t *src, int sstr, int n)) \
	oil_function_conv_f32_s32_class_ptr->func)
extern OilFunctionClass *oil_function_conv_f32_u32_class_ptr;
#define conv_f32_u32 ((void (*)(float *dest, int dstr, uint32_t *src, int sstr, int n)) \
	oil_function_conv_f32_u32_class_ptr->func)
extern OilFunctionClass *oil_function_conv_f32_f64_class_ptr;
#define conv_f32_f64 ((void (*)(float *dest, int dstr, double *src, int sstr, int n)) \
	oil_function_conv_f32_f64_class_ptr->func)
extern OilFunctionClass *oil_function_conv_f64_s8_class_ptr;
#define conv_f64_s8 ((void (*)(double *dest, int dstr, int8_t *src, int sstr, int n)) \
	oil_function_conv_f64_s8_class_ptr->func)
extern OilFunctionClass *oil_function_conv_f64_u8_class_ptr;
#define conv_f64_u8 ((void (*)(double *dest, int dstr, uint8_t *src, int sstr, int n)) \
	oil_function_conv_f64_u8_class_ptr->func)
extern OilFunctionClass *oil_function_conv_f64_s16_class_ptr;
#define conv_f64_s16 ((void (*)(double *dest, int dstr, int16_t *src, int sstr, int n)) \
	oil_function_conv_f64_s16_class_ptr->func)
extern OilFunctionClass *oil_function_conv_f64_u16_class_ptr;
#define conv_f64_u16 ((void (*)(double *dest, int dstr, uint16_t *src, int sstr, int n)) \
	oil_function_conv_f64_u16_class_ptr->func)
extern OilFunctionClass *oil_function_conv_f64_s32_class_ptr;
#define conv_f64_s32 ((void (*)(double *dest, int dstr, int32_t *src, int sstr, int n)) \
	oil_function_conv_f64_s32_class_ptr->func)
extern OilFunctionClass *oil_function_conv_f64_u32_class_ptr;
#define conv_f64_u32 ((void (*)(double *dest, int dstr, uint32_t *src, int sstr, int n)) \
	oil_function_conv_f64_u32_class_ptr->func)
extern OilFunctionClass *oil_function_conv_f64_f32_class_ptr;
#define conv_f64_f32 ((void (*)(double *dest, int dstr, float *src, int sstr, int n)) \
	oil_function_conv_f64_f32_class_ptr->func)
extern OilFunctionClass *oil_function_clipconv_s8_u8_class_ptr;
#define clipconv_s8_u8 ((void (*)(int8_t *dest, int dstr, uint8_t *src, int sstr, int n)) \
	oil_function_clipconv_s8_u8_class_ptr->func)
extern OilFunctionClass *oil_function_clipconv_s8_u16_class_ptr;
#define clipconv_s8_u16 ((void (*)(int8_t *dest, int dstr, uint16_t *src, int sstr, int n)) \
	oil_function_clipconv_s8_u16_class_ptr->func)
extern OilFunctionClass *oil_function_clipconv_s8_u32_class_ptr;
#define clipconv_s8_u32 ((void (*)(int8_t *dest, int dstr, uint32_t *src, int sstr, int n)) \
	oil_function_clipconv_s8_u32_class_ptr->func)
extern OilFunctionClass *oil_function_clipconv_u8_u32_class_ptr;
#define clipconv_u8_u32 ((void (*)(uint8_t *dest, int dstr, uint32_t *src, int sstr, int n)) \
	oil_function_clipconv_u8_u32_class_ptr->func)
extern OilFunctionClass *oil_function_clipconv_u8_u16_class_ptr;
#define clipconv_u8_u16 ((void (*)(uint8_t *dest, int dstr, uint16_t *src, int sstr, int n)) \
	oil_function_clipconv_u8_u16_class_ptr->func)
extern OilFunctionClass *oil_function_clipconv_s16_u16_class_ptr;
#define clipconv_s16_u16 ((void (*)(int16_t *dest, int dstr, uint16_t *src, int sstr, int n)) \
	oil_function_clipconv_s16_u16_class_ptr->func)
extern OilFunctionClass *oil_function_clipconv_s16_u32_class_ptr;
#define clipconv_s16_u32 ((void (*)(int16_t *dest, int dstr, uint32_t *src, int sstr, int n)) \
	oil_function_clipconv_s16_u32_class_ptr->func)
extern OilFunctionClass *oil_function_clipconv_s32_u32_class_ptr;
#define clipconv_s32_u32 ((void (*)(int32_t *dest, int dstr, uint32_t *src, int sstr, int n)) \
	oil_function_clipconv_s32_u32_class_ptr->func)
extern OilFunctionClass *oil_function_clipconv_u16_u32_class_ptr;
#define clipconv_u16_u32 ((void (*)(uint16_t *dest, int dstr, uint32_t *src, int sstr, int n)) \
	oil_function_clipconv_u16_u32_class_ptr->func)
extern OilFunctionClass *oil_function_clipconv_s8_s16_class_ptr;
#define clipconv_s8_s16 ((void (*)(int8_t *dest, int dstr, int16_t *src, int sstr, int n)) \
	oil_function_clipconv_s8_s16_class_ptr->func)
extern OilFunctionClass *oil_function_clipconv_s8_s32_class_ptr;
#define clipconv_s8_s32 ((void (*)(int8_t *dest, int dstr, int32_t *src, int sstr, int n)) \
	oil_function_clipconv_s8_s32_class_ptr->func)
extern OilFunctionClass *oil_function_clipconv_u8_s16_class_ptr;
#define clipconv_u8_s16 ((void (*)(uint8_t *dest, int dstr, int16_t *src, int sstr, int n)) \
	oil_function_clipconv_u8_s16_class_ptr->func)
extern OilFunctionClass *oil_function_clipconv_u8_s32_class_ptr;
#define clipconv_u8_s32 ((void (*)(uint8_t *dest, int dstr, int32_t *src, int sstr, int n)) \
	oil_function_clipconv_u8_s32_class_ptr->func)
extern OilFunctionClass *oil_function_clipconv_s16_s32_class_ptr;
#define clipconv_s16_s32 ((void (*)(int16_t *dest, int dstr, int32_t *src, int sstr, int n)) \
	oil_function_clipconv_s16_s32_class_ptr->func)
extern OilFunctionClass *oil_function_clipconv_u16_s32_class_ptr;
#define clipconv_u16_s32 ((void (*)(uint16_t *dest, int dstr, int32_t *src, int sstr, int n)) \
	oil_function_clipconv_u16_s32_class_ptr->func)
extern OilFunctionClass *oil_function_clipconv_u8_s8_class_ptr;
#define clipconv_u8_s8 ((void (*)(uint8_t *dest, int dstr, int8_t *src, int sstr, int n)) \
	oil_function_clipconv_u8_s8_class_ptr->func)
extern OilFunctionClass *oil_function_clipconv_u16_s16_class_ptr;
#define clipconv_u16_s16 ((void (*)(uint16_t *dest, int dstr, int16_t *src, int sstr, int n)) \
	oil_function_clipconv_u16_s16_class_ptr->func)
extern OilFunctionClass *oil_function_clipconv_u32_s32_class_ptr;
#define clipconv_u32_s32 ((void (*)(uint32_t *dest, int dstr, int32_t *src, int sstr, int n)) \
	oil_function_clipconv_u32_s32_class_ptr->func)
extern OilFunctionClass *oil_function_clipconv_s8_f32_class_ptr;
#define clipconv_s8_f32 ((void (*)(int8_t *dest, int dstr, float *src, int sstr, int n)) \
	oil_function_clipconv_s8_f32_class_ptr->func)
extern OilFunctionClass *oil_function_clipconv_s8_f64_class_ptr;
#define clipconv_s8_f64 ((void (*)(int8_t *dest, int dstr, double *src, int sstr, int n)) \
	oil_function_clipconv_s8_f64_class_ptr->func)
extern OilFunctionClass *oil_function_clipconv_u8_f32_class_ptr;
#define clipconv_u8_f32 ((void (*)(uint8_t *dest, int dstr, float *src, int sstr, int n)) \
	oil_function_clipconv_u8_f32_class_ptr->func)
extern OilFunctionClass *oil_function_clipconv_u8_f64_class_ptr;
#define clipconv_u8_f64 ((void (*)(uint8_t *dest, int dstr, double *src, int sstr, int n)) \
	oil_function_clipconv_u8_f64_class_ptr->func)
extern OilFunctionClass *oil_function_clipconv_s16_f32_class_ptr;
#define clipconv_s16_f32 ((void (*)(int16_t *dest, int dstr, float *src, int sstr, int n)) \
	oil_function_clipconv_s16_f32_class_ptr->func)
extern OilFunctionClass *oil_function_clipconv_s16_f64_class_ptr;
#define clipconv_s16_f64 ((void (*)(int16_t *dest, int dstr, double *src, int sstr, int n)) \
	oil_function_clipconv_s16_f64_class_ptr->func)
extern OilFunctionClass *oil_function_clipconv_u16_f32_class_ptr;
#define clipconv_u16_f32 ((void (*)(uint16_t *dest, int dstr, float *src, int sstr, int n)) \
	oil_function_clipconv_u16_f32_class_ptr->func)
extern OilFunctionClass *oil_function_clipconv_u16_f64_class_ptr;
#define clipconv_u16_f64 ((void (*)(uint16_t *dest, int dstr, double *src, int sstr, int n)) \
	oil_function_clipconv_u16_f64_class_ptr->func)
extern OilFunctionClass *oil_function_clipconv_s32_f32_class_ptr;
#define clipconv_s32_f32 ((void (*)(int32_t *dest, int dstr, float *src, int sstr, int n)) \
	oil_function_clipconv_s32_f32_class_ptr->func)
extern OilFunctionClass *oil_function_clipconv_s32_f64_class_ptr;
#define clipconv_s32_f64 ((void (*)(int32_t *dest, int dstr, double *src, int sstr, int n)) \
	oil_function_clipconv_s32_f64_class_ptr->func)
extern OilFunctionClass *oil_function_clipconv_u32_f32_class_ptr;
#define clipconv_u32_f32 ((void (*)(uint32_t *dest, int dstr, float *src, int sstr, int n)) \
	oil_function_clipconv_u32_f32_class_ptr->func)
extern OilFunctionClass *oil_function_clipconv_u32_f64_class_ptr;
#define clipconv_u32_f64 ((void (*)(uint32_t *dest, int dstr, double *src, int sstr, int n)) \
	oil_function_clipconv_u32_f64_class_ptr->func)
extern OilFunctionClass *oil_function_permute_s8_class_ptr;
#define permute_s8 ((void (*)(int8_t *dest, int dstr, int8_t *src, int sstr, int32_t *perm, int pstr, int n)) \
	oil_function_permute_s8_class_ptr->func)
extern OilFunctionClass *oil_function_permute_u8_class_ptr;
#define permute_u8 ((void (*)(uint8_t *dest, int dstr, uint8_t *src, int sstr, int32_t *perm, int pstr, int n)) \
	oil_function_permute_u8_class_ptr->func)
extern OilFunctionClass *oil_function_permute_s16_class_ptr;
#define permute_s16 ((void (*)(int16_t *dest, int dstr, int16_t *src, int sstr, int32_t *perm, int pstr, int n)) \
	oil_function_permute_s16_class_ptr->func)
extern OilFunctionClass *oil_function_permute_u16_class_ptr;
#define permute_u16 ((void (*)(uint16_t *dest, int dstr, uint16_t *src, int sstr, int32_t *perm, int pstr, int n)) \
	oil_function_permute_u16_class_ptr->func)
extern OilFunctionClass *oil_function_permute_s32_class_ptr;
#define permute_s32 ((void (*)(int32_t *dest, int dstr, int32_t *src, int sstr, int32_t *perm, int pstr, int n)) \
	oil_function_permute_s32_class_ptr->func)
extern OilFunctionClass *oil_function_permute_u32_class_ptr;
#define permute_u32 ((void (*)(uint32_t *dest, int dstr, uint32_t *src, int sstr, int32_t *perm, int pstr, int n)) \
	oil_function_permute_u32_class_ptr->func)
extern OilFunctionClass *oil_function_permute_f32_class_ptr;
#define permute_f32 ((void (*)(float *dest, int dstr, float *src, int sstr, int32_t *perm, int pstr, int n)) \
	oil_function_permute_f32_class_ptr->func)
extern OilFunctionClass *oil_function_permute_f64_class_ptr;
#define permute_f64 ((void (*)(double *dest, int dstr, double *src, int sstr, int32_t *perm, int pstr, int n)) \
	oil_function_permute_f64_class_ptr->func)
extern OilFunctionClass *oil_function_splat_u8_class_ptr;
#define splat_u8 ((void (*)(uint8_t *dest, int dstr, uint8_t value, int n)) \
	oil_function_splat_u8_class_ptr->func)
extern OilFunctionClass *oil_function_splat_u32_class_ptr;
#define splat_u32 ((void (*)(uint32_t *dest, int dstr, uint32_t *src, int n)) \
	oil_function_splat_u32_class_ptr->func)
extern OilFunctionClass *oil_function_tablelookup_u8_class_ptr;
#define tablelookup_u8 ((void (*)(uint8_t *dest, int dstr, uint8_t *src, int sstr, uint8_t *table, int tablestride, int n)) \
	oil_function_tablelookup_u8_class_ptr->func)
extern OilFunctionClass *oil_function_trans8x8_u8_class_ptr;
#define trans8x8_u8 ((void (*)(uint8_t *dest, int dstr, uint8_t *src, int sstr)) \
	oil_function_trans8x8_u8_class_ptr->func)
extern OilFunctionClass *oil_function_trans8x8_u16_class_ptr;
#define trans8x8_u16 ((void (*)(uint16_t *dest, int dstr, uint16_t *src, int sstr)) \
	oil_function_trans8x8_u16_class_ptr->func)
extern OilFunctionClass *oil_function_trans8x8_u32_class_ptr;
#define trans8x8_u32 ((void (*)(uint32_t *dest, int dstr, uint32_t *src, int sstr)) \
	oil_function_trans8x8_u32_class_ptr->func)
extern OilFunctionClass *oil_function_trans8x8_f64_class_ptr;
#define trans8x8_f64 ((void (*)(double *dest, int dstr, double *src, int sstr)) \
	oil_function_trans8x8_f64_class_ptr->func)
extern OilFunctionClass *oil_function_dct12_f32_class_ptr;
#define dct12_f32 ((void (*)(float *dest, int dstr, float *src, int sstr)) \
	oil_function_dct12_f32_class_ptr->func)
extern OilFunctionClass *oil_function_dct36_f32_class_ptr;
#define dct36_f32 ((void (*)(float *dest, int dstr, float *src, int sstr, int n)) \
	oil_function_dct36_f32_class_ptr->func)
extern OilFunctionClass *oil_function_fdct8_f64_class_ptr;
#define fdct8_f64 ((void (*)(double *dest, double *src, int dstr, int sstr)) \
	oil_function_fdct8_f64_class_ptr->func)
extern OilFunctionClass *oil_function_idct8_f64_class_ptr;
#define idct8_f64 ((void (*)(double *dest, int dstr, double *src, int sstr)) \
	oil_function_idct8_f64_class_ptr->func)
extern OilFunctionClass *oil_function_idct8x8_f64_class_ptr;
#define idct8x8_f64 ((void (*)(double *dest, int dstr, double *src, int sstr)) \
	oil_function_idct8x8_f64_class_ptr->func)
extern OilFunctionClass *oil_function_idct8x8_s16_class_ptr;
#define idct8x8_s16 ((void (*)(int16_t *dest, int dstr, int16_t *src, int sstr)) \
	oil_function_idct8x8_s16_class_ptr->func)
extern OilFunctionClass *oil_function_imdct32_f32_class_ptr;
#define imdct32_f32 ((void (*)(float *dest, float *src)) \
	oil_function_imdct32_f32_class_ptr->func)
extern OilFunctionClass *oil_function_conv8x8_s16_f64_class_ptr;
#define conv8x8_s16_f64 ((void (*)(int16_t *dest, int dstr, double *src, int sstr)) \
	oil_function_conv8x8_s16_f64_class_ptr->func)
extern OilFunctionClass *oil_function_conv8x8_f64_s16_class_ptr;
#define conv8x8_f64_s16 ((void (*)(double *dest, int dstr, int16_t *src, int sstr)) \
	oil_function_conv8x8_f64_s16_class_ptr->func)
extern OilFunctionClass *oil_function_clipconv8x8_u8_s16_class_ptr;
#define clipconv8x8_u8_s16 ((void (*)(uint8_t *dest, int dstr, int16_t *src, int sstr)) \
	oil_function_clipconv8x8_u8_s16_class_ptr->func)
extern OilFunctionClass *oil_function_scanlinescale2_u8_class_ptr;
#define scanlinescale2_u8 ((void (*)(unsigned char,  *dest, unsigned char,  *src, int len)) \
	oil_function_scanlinescale2_u8_class_ptr->func)
extern OilFunctionClass *oil_function_dequantize8x8_s16_class_ptr;
#define dequantize8x8_s16 ((void (*)(int16_t *dest, int dstr, int16_t *src, int sstr, int16_t *quant, int qstr)) \
	oil_function_dequantize8x8_s16_class_ptr->func)
extern OilFunctionClass *oil_function_yuv2rgbx_u8_class_ptr;
#define yuv2rgbx_u8 ((void (*)(uint8_t *rgbp, uint8_t *yp, uint8_t *up, uint8_t *vp, int n)) \
	oil_function_yuv2rgbx_u8_class_ptr->func)
extern OilFunctionClass *oil_function_yuv2rgbx_sub2_u8_class_ptr;
#define yuv2rgbx_sub2_u8 ((void (*)(uint8_t *rgbp, uint8_t *yp, uint8_t *up, uint8_t *vp, int n)) \
	oil_function_yuv2rgbx_sub2_u8_class_ptr->func)
extern OilFunctionClass *oil_function_yuv2rgbx_sub4_u8_class_ptr;
#define yuv2rgbx_sub4_u8 ((void (*)(uint8_t *rgbp, uint8_t *yp, uint8_t *up, uint8_t *vp, int n)) \
	oil_function_yuv2rgbx_sub4_u8_class_ptr->func)
extern OilFunctionClass *oil_function_unzigzag8x8_s16_class_ptr;
#define unzigzag8x8_s16 ((void (*)(int16_t *dest, int dstr, int16_t *src, int sstr)) \
	oil_function_unzigzag8x8_s16_class_ptr->func)
extern OilFunctionClass *oil_function_abs_u8_s8_class_ptr;
#define abs_u8_s8 ((void (*)(uint8_t *dest, int dstr, int8_t *src, int sstr, int n)) \
	oil_function_abs_u8_s8_class_ptr->func)
extern OilFunctionClass *oil_function_abs_u16_s16_class_ptr;
#define abs_u16_s16 ((void (*)(uint16_t *dest, int dstr, int16_t *src, int sstr, int n)) \
	oil_function_abs_u16_s16_class_ptr->func)
extern OilFunctionClass *oil_function_abs_u32_s32_class_ptr;
#define abs_u32_s32 ((void (*)(uint32_t *dest, int dstr, int32_t *src, int sstr, int n)) \
	oil_function_abs_u32_s32_class_ptr->func)
extern OilFunctionClass *oil_function_abs_f32_f32_class_ptr;
#define abs_f32_f32 ((void (*)(float *dest, int dstr, float *src, int sstr, int n)) \
	oil_function_abs_f32_f32_class_ptr->func)
extern OilFunctionClass *oil_function_abs_f64_f64_class_ptr;
#define abs_f64_f64 ((void (*)(double *dest, int dstr, double *src, int sstr, int n)) \
	oil_function_abs_f64_f64_class_ptr->func)
extern OilFunctionClass *oil_function_average2_u8_class_ptr;
#define average2_u8 ((void (*)(uint8_t *dest, int dstr, uint8_t *src1, int sstr1, uint8_t *src2, int sstr2, int n)) \
	oil_function_average2_u8_class_ptr->func)
extern OilFunctionClass *oil_function_clip_s8_class_ptr;
#define clip_s8 ((void (*)(int8_t *dest, int dstr, int8_t *src, int sstr, int n, type_s8min , type_s8max )) \
	oil_function_clip_s8_class_ptr->func)
extern OilFunctionClass *oil_function_clip_u8_class_ptr;
#define clip_u8 ((void (*)(uint8_t *dest, int dstr, uint8_t *src, int sstr, int n, type_u8min , type_u8max )) \
	oil_function_clip_u8_class_ptr->func)
extern OilFunctionClass *oil_function_clip_s16_class_ptr;
#define clip_s16 ((void (*)(int16_t *dest, int dstr, int16_t *src, int sstr, int n, type_s16min , type_s16max )) \
	oil_function_clip_s16_class_ptr->func)
extern OilFunctionClass *oil_function_clip_u16_class_ptr;
#define clip_u16 ((void (*)(uint16_t *dest, int dstr, uint16_t *src, int sstr, int n, type_u16min , type_u16max )) \
	oil_function_clip_u16_class_ptr->func)
extern OilFunctionClass *oil_function_clip_s32_class_ptr;
#define clip_s32 ((void (*)(int32_t *dest, int dstr, int32_t *src, int sstr, int n, type_s32min , type_s32max )) \
	oil_function_clip_s32_class_ptr->func)
extern OilFunctionClass *oil_function_clip_u32_class_ptr;
#define clip_u32 ((void (*)(uint32_t *dest, int dstr, uint32_t *src, int sstr, int n, type_u32min , type_u32max )) \
	oil_function_clip_u32_class_ptr->func)
extern OilFunctionClass *oil_function_clip_f32_class_ptr;
#define clip_f32 ((void (*)(float *dest, int dstr, float *src, int sstr, int n, type_f32min , type_f32max )) \
	oil_function_clip_f32_class_ptr->func)
extern OilFunctionClass *oil_function_clip_f64_class_ptr;
#define clip_f64 ((void (*)(double *dest, int dstr, double *src, int sstr, int n, type_f64min , type_f64max )) \
	oil_function_clip_f64_class_ptr->func)
extern OilFunctionClass *oil_function_diffsquaresum_f64_class_ptr;
#define diffsquaresum_f64 ((void (*)(double *dest, double *src1, int sstr1, double *src2, int sstr2, int n)) \
	oil_function_diffsquaresum_f64_class_ptr->func)
extern OilFunctionClass *oil_function_downsample1x_f64_class_ptr;
#define downsample1x_f64 ((void (*)(double *dest, int dstr, double *src, int sstr, int ndest, int nsrc, double ratio, double invratio)) \
	oil_function_downsample1x_f64_class_ptr->func)
extern OilFunctionClass *oil_function_get8x8_f64_class_ptr;
#define get8x8_f64 ((void (*)(double *dest, int dstr, double *src, int sstr, int x, int y)) \
	oil_function_get8x8_f64_class_ptr->func)
extern OilFunctionClass *oil_function_mix_u8_class_ptr;
#define mix_u8 ((void (*)(uint8_t *dest, uint8_t *src1, uint8_t *src2, uint8_t *scale, int n)) \
	oil_function_mix_u8_class_ptr->func)
extern OilFunctionClass *oil_function_mult8x8_s16_class_ptr;
#define mult8x8_s16 ((void (*)(int16_t *dest, int16_t *src1, int16_t *src2, int dstr, int sstr1, int sstr2)) \
	oil_function_mult8x8_s16_class_ptr->func)
extern OilFunctionClass *oil_function_multsum_f32_class_ptr;
#define multsum_f32 ((void (*)(float *dest, float *src1, int sstr1, float *src2, int sstr2, int n)) \
	oil_function_multsum_f32_class_ptr->func)
extern OilFunctionClass *oil_function_multsum_f64_class_ptr;
#define multsum_f64 ((void (*)(double *dest, double *src1, int sstr1, double *src2, int sstr2, int n)) \
	oil_function_multsum_f64_class_ptr->func)
extern OilFunctionClass *oil_function_sad8x8_s16_class_ptr;
#define sad8x8_s16 ((void (*)(uint32_t *dest, int dstr, int16_t *src1, int sstr1, int16_t *src2, int sstr2)) \
	oil_function_sad8x8_s16_class_ptr->func)
extern OilFunctionClass *oil_function_sad8x8_f64_class_ptr;
#define sad8x8_f64 ((void (*)(double *dest, int dstr, double *src1, int sstr1, double *src2, int sstr2)) \
	oil_function_sad8x8_f64_class_ptr->func)
extern OilFunctionClass *oil_function_scalaradd_s8_class_ptr;
#define scalaradd_s8 ((void (*)(int8_t *dest, int dstr, int8_t *src, int sstr, int8_t val, int n)) \
	oil_function_scalaradd_s8_class_ptr->func)
extern OilFunctionClass *oil_function_scalaradd_u8_class_ptr;
#define scalaradd_u8 ((void (*)(uint8_t *dest, int dstr, uint8_t *src, int sstr, uint8_t val, int n)) \
	oil_function_scalaradd_u8_class_ptr->func)
extern OilFunctionClass *oil_function_scalaradd_s16_class_ptr;
#define scalaradd_s16 ((void (*)(int16_t *dest, int dstr, int16_t *src, int sstr, int16_t val, int n)) \
	oil_function_scalaradd_s16_class_ptr->func)
extern OilFunctionClass *oil_function_scalaradd_u16_class_ptr;
#define scalaradd_u16 ((void (*)(uint16_t *dest, int dstr, uint16_t *src, int sstr, uint16_t val, int n)) \
	oil_function_scalaradd_u16_class_ptr->func)
extern OilFunctionClass *oil_function_scalaradd_s32_class_ptr;
#define scalaradd_s32 ((void (*)(int32_t *dest, int dstr, int32_t *src, int sstr, int32_t val, int n)) \
	oil_function_scalaradd_s32_class_ptr->func)
extern OilFunctionClass *oil_function_scalaradd_u32_class_ptr;
#define scalaradd_u32 ((void (*)(uint32_t *dest, int dstr, uint32_t *src, int sstr, uint32_t val, int n)) \
	oil_function_scalaradd_u32_class_ptr->func)
extern OilFunctionClass *oil_function_scalaradd_f32_class_ptr;
#define scalaradd_f32 ((void (*)(float *dest, int dstr, float *src, int sstr, float val, int n)) \
	oil_function_scalaradd_f32_class_ptr->func)
extern OilFunctionClass *oil_function_scalaradd_f64_class_ptr;
#define scalaradd_f64 ((void (*)(double *dest, int dstr, double *src, int sstr, double val, int n)) \
	oil_function_scalaradd_f64_class_ptr->func)
extern OilFunctionClass *oil_function_scalarmult_s8_class_ptr;
#define scalarmult_s8 ((void (*)(int8_t *dest, int dstr, int8_t *src, int sstr, int8_t value, int n)) \
	oil_function_scalarmult_s8_class_ptr->func)
extern OilFunctionClass *oil_function_scalarmult_u8_class_ptr;
#define scalarmult_u8 ((void (*)(uint8_t *dest, int dstr, uint8_t *src, int sstr, uint8_t value, int n)) \
	oil_function_scalarmult_u8_class_ptr->func)
extern OilFunctionClass *oil_function_scalarmult_s16_class_ptr;
#define scalarmult_s16 ((void (*)(int16_t *dest, int dstr, int16_t *src, int sstr, int16_t value, int n)) \
	oil_function_scalarmult_s16_class_ptr->func)
extern OilFunctionClass *oil_function_scalarmult_u16_class_ptr;
#define scalarmult_u16 ((void (*)(uint16_t *dest, int dstr, uint16_t *src, int sstr, uint16_t value, int n)) \
	oil_function_scalarmult_u16_class_ptr->func)
extern OilFunctionClass *oil_function_scalarmult_s32_class_ptr;
#define scalarmult_s32 ((void (*)(int32_t *dest, int dstr, int32_t *src, int sstr, int32_t value, int n)) \
	oil_function_scalarmult_s32_class_ptr->func)
extern OilFunctionClass *oil_function_scalarmult_u32_class_ptr;
#define scalarmult_u32 ((void (*)(uint32_t *dest, int dstr, uint32_t *src, int sstr, uint32_t value, int n)) \
	oil_function_scalarmult_u32_class_ptr->func)
extern OilFunctionClass *oil_function_scalarmult_f32_class_ptr;
#define scalarmult_f32 ((void (*)(float *dest, int dstr, float *src, int sstr, float value, int n)) \
	oil_function_scalarmult_f32_class_ptr->func)
extern OilFunctionClass *oil_function_scalarmult_f64_class_ptr;
#define scalarmult_f64 ((void (*)(double *dest, int dstr, double *src, int sstr, double value, int n)) \
	oil_function_scalarmult_f64_class_ptr->func)
extern OilFunctionClass *oil_function_sincos_f64_class_ptr;
#define sincos_f64 ((void (*)(double *dest_sin, double *dest_cos, int n, double offset, double interval)) \
	oil_function_sincos_f64_class_ptr->func)
extern OilFunctionClass *oil_function_squaresum_f64_class_ptr;
#define squaresum_f64 ((void (*)(double *dest, double *src, int n)) \
	oil_function_squaresum_f64_class_ptr->func)
extern OilFunctionClass *oil_function_sum_f64_class_ptr;
#define sum_f64 ((void (*)(double *dest, double *src, int sstr, int n)) \
	oil_function_sum_f64_class_ptr->func)
extern OilFunctionClass *oil_function_vectoradd_s8_class_ptr;
#define vectoradd_s8 ((void (*)(int8_t *dest, int dstr, int8_t *src1, int sstr1, int8_t *src2, int sstr2, int n, int8_t v1, int8_t v2)) \
	oil_function_vectoradd_s8_class_ptr->func)
extern OilFunctionClass *oil_function_vectoradd_u8_class_ptr;
#define vectoradd_u8 ((void (*)(uint8_t *dest, int dstr, uint8_t *src1, int sstr1, uint8_t *src2, int sstr2, int n, uint8_t v1, uint8_t v2)) \
	oil_function_vectoradd_u8_class_ptr->func)
extern OilFunctionClass *oil_function_vectoradd_s16_class_ptr;
#define vectoradd_s16 ((void (*)(int16_t *dest, int dstr, int16_t *src1, int sstr1, int16_t *src2, int sstr2, int n, int16_t v1, int16_t v2)) \
	oil_function_vectoradd_s16_class_ptr->func)
extern OilFunctionClass *oil_function_vectoradd_u16_class_ptr;
#define vectoradd_u16 ((void (*)(uint16_t *dest, int dstr, uint16_t *src1, int sstr1, uint16_t *src2, int sstr2, int n, uint16_t v1, uint16_t v2)) \
	oil_function_vectoradd_u16_class_ptr->func)
extern OilFunctionClass *oil_function_vectoradd_s32_class_ptr;
#define vectoradd_s32 ((void (*)(int32_t *dest, int dstr, int32_t *src1, int sstr1, int32_t *src2, int sstr2, int n, int32_t v1, int32_t v2)) \
	oil_function_vectoradd_s32_class_ptr->func)
extern OilFunctionClass *oil_function_vectoradd_u32_class_ptr;
#define vectoradd_u32 ((void (*)(uint32_t *dest, int dstr, uint32_t *src1, int sstr1, uint32_t *src2, int sstr2, int n, uint32_t v1, uint32_t v2)) \
	oil_function_vectoradd_u32_class_ptr->func)
extern OilFunctionClass *oil_function_vectoradd_f32_class_ptr;
#define vectoradd_f32 ((void (*)(float *dest, int dstr, float *src1, int sstr1, float *src2, int sstr2, int n, float v1, float v2)) \
	oil_function_vectoradd_f32_class_ptr->func)
extern OilFunctionClass *oil_function_vectoradd_f64_class_ptr;
#define vectoradd_f64 ((void (*)(double *dest, int dstr, double *src1, int sstr1, double *src2, int sstr2, int n, double v1, double v2)) \
	oil_function_vectoradd_f64_class_ptr->func)

#endif


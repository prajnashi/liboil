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

extern OilFunctionClass _oil_function_conv_s8_u8_class;
#define conv_s8_u8 ((void (*)(int8_t *dest, int dstr, uint8_t *src, int sstr, int n)) \
	_oil_function_conv_s8_u8_class.func)
extern OilFunctionClass _oil_function_conv_s8_s16_class;
#define conv_s8_s16 ((void (*)(int8_t *dest, int dstr, int16_t *src, int sstr, int n)) \
	_oil_function_conv_s8_s16_class.func)
extern OilFunctionClass _oil_function_conv_s8_u16_class;
#define conv_s8_u16 ((void (*)(int8_t *dest, int dstr, uint16_t *src, int sstr, int n)) \
	_oil_function_conv_s8_u16_class.func)
extern OilFunctionClass _oil_function_conv_s8_s32_class;
#define conv_s8_s32 ((void (*)(int8_t *dest, int dstr, int32_t *src, int sstr, int n)) \
	_oil_function_conv_s8_s32_class.func)
extern OilFunctionClass _oil_function_conv_s8_u32_class;
#define conv_s8_u32 ((void (*)(int8_t *dest, int dstr, uint32_t *src, int sstr, int n)) \
	_oil_function_conv_s8_u32_class.func)
extern OilFunctionClass _oil_function_conv_s8_f32_class;
#define conv_s8_f32 ((void (*)(int8_t *dest, int dstr, float *src, int sstr, int n)) \
	_oil_function_conv_s8_f32_class.func)
extern OilFunctionClass _oil_function_conv_s8_f64_class;
#define conv_s8_f64 ((void (*)(int8_t *dest, int dstr, double *src, int sstr, int n)) \
	_oil_function_conv_s8_f64_class.func)
extern OilFunctionClass _oil_function_conv_u8_s8_class;
#define conv_u8_s8 ((void (*)(uint8_t *dest, int dstr, int8_t *src, int sstr, int n)) \
	_oil_function_conv_u8_s8_class.func)
extern OilFunctionClass _oil_function_conv_u8_s16_class;
#define conv_u8_s16 ((void (*)(uint8_t *dest, int dstr, int16_t *src, int sstr, int n)) \
	_oil_function_conv_u8_s16_class.func)
extern OilFunctionClass _oil_function_conv_u8_u16_class;
#define conv_u8_u16 ((void (*)(uint8_t *dest, int dstr, uint16_t *src, int sstr, int n)) \
	_oil_function_conv_u8_u16_class.func)
extern OilFunctionClass _oil_function_conv_u8_s32_class;
#define conv_u8_s32 ((void (*)(uint8_t *dest, int dstr, int32_t *src, int sstr, int n)) \
	_oil_function_conv_u8_s32_class.func)
extern OilFunctionClass _oil_function_conv_u8_u32_class;
#define conv_u8_u32 ((void (*)(uint8_t *dest, int dstr, uint32_t *src, int sstr, int n)) \
	_oil_function_conv_u8_u32_class.func)
extern OilFunctionClass _oil_function_conv_u8_f32_class;
#define conv_u8_f32 ((void (*)(uint8_t *dest, int dstr, float *src, int sstr, int n)) \
	_oil_function_conv_u8_f32_class.func)
extern OilFunctionClass _oil_function_conv_u8_f64_class;
#define conv_u8_f64 ((void (*)(uint8_t *dest, int dstr, double *src, int sstr, int n)) \
	_oil_function_conv_u8_f64_class.func)
extern OilFunctionClass _oil_function_conv_s16_s8_class;
#define conv_s16_s8 ((void (*)(int16_t *dest, int dstr, int8_t *src, int sstr, int n)) \
	_oil_function_conv_s16_s8_class.func)
extern OilFunctionClass _oil_function_conv_s16_u8_class;
#define conv_s16_u8 ((void (*)(int16_t *dest, int dstr, uint8_t *src, int sstr, int n)) \
	_oil_function_conv_s16_u8_class.func)
extern OilFunctionClass _oil_function_conv_s16_u16_class;
#define conv_s16_u16 ((void (*)(int16_t *dest, int dstr, uint16_t *src, int sstr, int n)) \
	_oil_function_conv_s16_u16_class.func)
extern OilFunctionClass _oil_function_conv_s16_s32_class;
#define conv_s16_s32 ((void (*)(int16_t *dest, int dstr, int32_t *src, int sstr, int n)) \
	_oil_function_conv_s16_s32_class.func)
extern OilFunctionClass _oil_function_conv_s16_u32_class;
#define conv_s16_u32 ((void (*)(int16_t *dest, int dstr, uint32_t *src, int sstr, int n)) \
	_oil_function_conv_s16_u32_class.func)
extern OilFunctionClass _oil_function_conv_s16_f32_class;
#define conv_s16_f32 ((void (*)(int16_t *dest, int dstr, float *src, int sstr, int n)) \
	_oil_function_conv_s16_f32_class.func)
extern OilFunctionClass _oil_function_conv_s16_f64_class;
#define conv_s16_f64 ((void (*)(int16_t *dest, int dstr, double *src, int sstr, int n)) \
	_oil_function_conv_s16_f64_class.func)
extern OilFunctionClass _oil_function_conv_u16_s8_class;
#define conv_u16_s8 ((void (*)(uint16_t *dest, int dstr, int8_t *src, int sstr, int n)) \
	_oil_function_conv_u16_s8_class.func)
extern OilFunctionClass _oil_function_conv_u16_u8_class;
#define conv_u16_u8 ((void (*)(uint16_t *dest, int dstr, uint8_t *src, int sstr, int n)) \
	_oil_function_conv_u16_u8_class.func)
extern OilFunctionClass _oil_function_conv_u16_s16_class;
#define conv_u16_s16 ((void (*)(uint16_t *dest, int dstr, int16_t *src, int sstr, int n)) \
	_oil_function_conv_u16_s16_class.func)
extern OilFunctionClass _oil_function_conv_u16_s32_class;
#define conv_u16_s32 ((void (*)(uint16_t *dest, int dstr, int32_t *src, int sstr, int n)) \
	_oil_function_conv_u16_s32_class.func)
extern OilFunctionClass _oil_function_conv_u16_u32_class;
#define conv_u16_u32 ((void (*)(uint16_t *dest, int dstr, uint32_t *src, int sstr, int n)) \
	_oil_function_conv_u16_u32_class.func)
extern OilFunctionClass _oil_function_conv_u16_f32_class;
#define conv_u16_f32 ((void (*)(uint16_t *dest, int dstr, float *src, int sstr, int n)) \
	_oil_function_conv_u16_f32_class.func)
extern OilFunctionClass _oil_function_conv_u16_f64_class;
#define conv_u16_f64 ((void (*)(uint16_t *dest, int dstr, double *src, int sstr, int n)) \
	_oil_function_conv_u16_f64_class.func)
extern OilFunctionClass _oil_function_conv_s32_s8_class;
#define conv_s32_s8 ((void (*)(int32_t *dest, int dstr, int8_t *src, int sstr, int n)) \
	_oil_function_conv_s32_s8_class.func)
extern OilFunctionClass _oil_function_conv_s32_s16_class;
#define conv_s32_s16 ((void (*)(int32_t *dest, int dstr, int16_t *src, int sstr, int n)) \
	_oil_function_conv_s32_s16_class.func)
extern OilFunctionClass _oil_function_conv_s32_u8_class;
#define conv_s32_u8 ((void (*)(int32_t *dest, int dstr, uint8_t *src, int sstr, int n)) \
	_oil_function_conv_s32_u8_class.func)
extern OilFunctionClass _oil_function_conv_s32_u16_class;
#define conv_s32_u16 ((void (*)(int32_t *dest, int dstr, uint16_t *src, int sstr, int n)) \
	_oil_function_conv_s32_u16_class.func)
extern OilFunctionClass _oil_function_conv_s32_u32_class;
#define conv_s32_u32 ((void (*)(int32_t *dest, int dstr, uint32_t *src, int sstr, int n)) \
	_oil_function_conv_s32_u32_class.func)
extern OilFunctionClass _oil_function_conv_s32_f32_class;
#define conv_s32_f32 ((void (*)(int32_t *dest, int dstr, float *src, int sstr, int n)) \
	_oil_function_conv_s32_f32_class.func)
extern OilFunctionClass _oil_function_conv_s32_f64_class;
#define conv_s32_f64 ((void (*)(int32_t *dest, int dstr, double *src, int sstr, int n)) \
	_oil_function_conv_s32_f64_class.func)
extern OilFunctionClass _oil_function_conv_u32_s8_class;
#define conv_u32_s8 ((void (*)(uint32_t *dest, int dstr, int8_t *src, int sstr, int n)) \
	_oil_function_conv_u32_s8_class.func)
extern OilFunctionClass _oil_function_conv_u32_s16_class;
#define conv_u32_s16 ((void (*)(uint32_t *dest, int dstr, int16_t *src, int sstr, int n)) \
	_oil_function_conv_u32_s16_class.func)
extern OilFunctionClass _oil_function_conv_u32_u8_class;
#define conv_u32_u8 ((void (*)(uint32_t *dest, int dstr, uint8_t *src, int sstr, int n)) \
	_oil_function_conv_u32_u8_class.func)
extern OilFunctionClass _oil_function_conv_u32_u16_class;
#define conv_u32_u16 ((void (*)(uint32_t *dest, int dstr, uint16_t *src, int sstr, int n)) \
	_oil_function_conv_u32_u16_class.func)
extern OilFunctionClass _oil_function_conv_u32_s32_class;
#define conv_u32_s32 ((void (*)(uint32_t *dest, int dstr, int32_t *src, int sstr, int n)) \
	_oil_function_conv_u32_s32_class.func)
extern OilFunctionClass _oil_function_conv_u32_f32_class;
#define conv_u32_f32 ((void (*)(uint32_t *dest, int dstr, float *src, int sstr, int n)) \
	_oil_function_conv_u32_f32_class.func)
extern OilFunctionClass _oil_function_conv_u32_f64_class;
#define conv_u32_f64 ((void (*)(uint32_t *dest, int dstr, double *src, int sstr, int n)) \
	_oil_function_conv_u32_f64_class.func)
extern OilFunctionClass _oil_function_conv_f32_s8_class;
#define conv_f32_s8 ((void (*)(float *dest, int dstr, int8_t *src, int sstr, int n)) \
	_oil_function_conv_f32_s8_class.func)
extern OilFunctionClass _oil_function_conv_f32_s16_class;
#define conv_f32_s16 ((void (*)(float *dest, int dstr, int16_t *src, int sstr, int n)) \
	_oil_function_conv_f32_s16_class.func)
extern OilFunctionClass _oil_function_conv_f32_u8_class;
#define conv_f32_u8 ((void (*)(float *dest, int dstr, uint8_t *src, int sstr, int n)) \
	_oil_function_conv_f32_u8_class.func)
extern OilFunctionClass _oil_function_conv_f32_u16_class;
#define conv_f32_u16 ((void (*)(float *dest, int dstr, uint16_t *src, int sstr, int n)) \
	_oil_function_conv_f32_u16_class.func)
extern OilFunctionClass _oil_function_conv_f32_s32_class;
#define conv_f32_s32 ((void (*)(float *dest, int dstr, int32_t *src, int sstr, int n)) \
	_oil_function_conv_f32_s32_class.func)
extern OilFunctionClass _oil_function_conv_f32_u32_class;
#define conv_f32_u32 ((void (*)(float *dest, int dstr, uint32_t *src, int sstr, int n)) \
	_oil_function_conv_f32_u32_class.func)
extern OilFunctionClass _oil_function_conv_f32_f64_class;
#define conv_f32_f64 ((void (*)(float *dest, int dstr, double *src, int sstr, int n)) \
	_oil_function_conv_f32_f64_class.func)
extern OilFunctionClass _oil_function_conv_f64_s8_class;
#define conv_f64_s8 ((void (*)(double *dest, int dstr, int8_t *src, int sstr, int n)) \
	_oil_function_conv_f64_s8_class.func)
extern OilFunctionClass _oil_function_conv_f64_u8_class;
#define conv_f64_u8 ((void (*)(double *dest, int dstr, uint8_t *src, int sstr, int n)) \
	_oil_function_conv_f64_u8_class.func)
extern OilFunctionClass _oil_function_conv_f64_s16_class;
#define conv_f64_s16 ((void (*)(double *dest, int dstr, int16_t *src, int sstr, int n)) \
	_oil_function_conv_f64_s16_class.func)
extern OilFunctionClass _oil_function_conv_f64_u16_class;
#define conv_f64_u16 ((void (*)(double *dest, int dstr, uint16_t *src, int sstr, int n)) \
	_oil_function_conv_f64_u16_class.func)
extern OilFunctionClass _oil_function_conv_f64_s32_class;
#define conv_f64_s32 ((void (*)(double *dest, int dstr, int32_t *src, int sstr, int n)) \
	_oil_function_conv_f64_s32_class.func)
extern OilFunctionClass _oil_function_conv_f64_u32_class;
#define conv_f64_u32 ((void (*)(double *dest, int dstr, uint32_t *src, int sstr, int n)) \
	_oil_function_conv_f64_u32_class.func)
extern OilFunctionClass _oil_function_conv_f64_f32_class;
#define conv_f64_f32 ((void (*)(double *dest, int dstr, float *src, int sstr, int n)) \
	_oil_function_conv_f64_f32_class.func)
extern OilFunctionClass _oil_function_clipconv_s8_u8_class;
#define clipconv_s8_u8 ((void (*)(int8_t *dest, int dstr, uint8_t *src, int sstr, int n)) \
	_oil_function_clipconv_s8_u8_class.func)
extern OilFunctionClass _oil_function_clipconv_s8_u16_class;
#define clipconv_s8_u16 ((void (*)(int8_t *dest, int dstr, uint16_t *src, int sstr, int n)) \
	_oil_function_clipconv_s8_u16_class.func)
extern OilFunctionClass _oil_function_clipconv_s8_u32_class;
#define clipconv_s8_u32 ((void (*)(int8_t *dest, int dstr, uint32_t *src, int sstr, int n)) \
	_oil_function_clipconv_s8_u32_class.func)
extern OilFunctionClass _oil_function_clipconv_u8_u32_class;
#define clipconv_u8_u32 ((void (*)(uint8_t *dest, int dstr, uint32_t *src, int sstr, int n)) \
	_oil_function_clipconv_u8_u32_class.func)
extern OilFunctionClass _oil_function_clipconv_u8_u16_class;
#define clipconv_u8_u16 ((void (*)(uint8_t *dest, int dstr, uint16_t *src, int sstr, int n)) \
	_oil_function_clipconv_u8_u16_class.func)
extern OilFunctionClass _oil_function_clipconv_s16_u16_class;
#define clipconv_s16_u16 ((void (*)(int16_t *dest, int dstr, uint16_t *src, int sstr, int n)) \
	_oil_function_clipconv_s16_u16_class.func)
extern OilFunctionClass _oil_function_clipconv_s16_u32_class;
#define clipconv_s16_u32 ((void (*)(int16_t *dest, int dstr, uint32_t *src, int sstr, int n)) \
	_oil_function_clipconv_s16_u32_class.func)
extern OilFunctionClass _oil_function_clipconv_s32_u32_class;
#define clipconv_s32_u32 ((void (*)(int32_t *dest, int dstr, uint32_t *src, int sstr, int n)) \
	_oil_function_clipconv_s32_u32_class.func)
extern OilFunctionClass _oil_function_clipconv_u16_u32_class;
#define clipconv_u16_u32 ((void (*)(uint16_t *dest, int dstr, uint32_t *src, int sstr, int n)) \
	_oil_function_clipconv_u16_u32_class.func)
extern OilFunctionClass _oil_function_clipconv_s8_s16_class;
#define clipconv_s8_s16 ((void (*)(int8_t *dest, int dstr, int16_t *src, int sstr, int n)) \
	_oil_function_clipconv_s8_s16_class.func)
extern OilFunctionClass _oil_function_clipconv_s8_s32_class;
#define clipconv_s8_s32 ((void (*)(int8_t *dest, int dstr, int32_t *src, int sstr, int n)) \
	_oil_function_clipconv_s8_s32_class.func)
extern OilFunctionClass _oil_function_clipconv_u8_s16_class;
#define clipconv_u8_s16 ((void (*)(uint8_t *dest, int dstr, int16_t *src, int sstr, int n)) \
	_oil_function_clipconv_u8_s16_class.func)
extern OilFunctionClass _oil_function_clipconv_u8_s32_class;
#define clipconv_u8_s32 ((void (*)(uint8_t *dest, int dstr, int32_t *src, int sstr, int n)) \
	_oil_function_clipconv_u8_s32_class.func)
extern OilFunctionClass _oil_function_clipconv_s16_s32_class;
#define clipconv_s16_s32 ((void (*)(int16_t *dest, int dstr, int32_t *src, int sstr, int n)) \
	_oil_function_clipconv_s16_s32_class.func)
extern OilFunctionClass _oil_function_clipconv_u16_s32_class;
#define clipconv_u16_s32 ((void (*)(uint16_t *dest, int dstr, int32_t *src, int sstr, int n)) \
	_oil_function_clipconv_u16_s32_class.func)
extern OilFunctionClass _oil_function_clipconv_u8_s8_class;
#define clipconv_u8_s8 ((void (*)(uint8_t *dest, int dstr, int8_t *src, int sstr, int n)) \
	_oil_function_clipconv_u8_s8_class.func)
extern OilFunctionClass _oil_function_clipconv_u16_s16_class;
#define clipconv_u16_s16 ((void (*)(uint16_t *dest, int dstr, int16_t *src, int sstr, int n)) \
	_oil_function_clipconv_u16_s16_class.func)
extern OilFunctionClass _oil_function_clipconv_u32_s32_class;
#define clipconv_u32_s32 ((void (*)(uint32_t *dest, int dstr, int32_t *src, int sstr, int n)) \
	_oil_function_clipconv_u32_s32_class.func)
extern OilFunctionClass _oil_function_clipconv_s8_f32_class;
#define clipconv_s8_f32 ((void (*)(int8_t *dest, int dstr, float *src, int sstr, int n)) \
	_oil_function_clipconv_s8_f32_class.func)
extern OilFunctionClass _oil_function_clipconv_s8_f64_class;
#define clipconv_s8_f64 ((void (*)(int8_t *dest, int dstr, double *src, int sstr, int n)) \
	_oil_function_clipconv_s8_f64_class.func)
extern OilFunctionClass _oil_function_clipconv_u8_f32_class;
#define clipconv_u8_f32 ((void (*)(uint8_t *dest, int dstr, float *src, int sstr, int n)) \
	_oil_function_clipconv_u8_f32_class.func)
extern OilFunctionClass _oil_function_clipconv_u8_f64_class;
#define clipconv_u8_f64 ((void (*)(uint8_t *dest, int dstr, double *src, int sstr, int n)) \
	_oil_function_clipconv_u8_f64_class.func)
extern OilFunctionClass _oil_function_clipconv_s16_f32_class;
#define clipconv_s16_f32 ((void (*)(int16_t *dest, int dstr, float *src, int sstr, int n)) \
	_oil_function_clipconv_s16_f32_class.func)
extern OilFunctionClass _oil_function_clipconv_s16_f64_class;
#define clipconv_s16_f64 ((void (*)(int16_t *dest, int dstr, double *src, int sstr, int n)) \
	_oil_function_clipconv_s16_f64_class.func)
extern OilFunctionClass _oil_function_clipconv_u16_f32_class;
#define clipconv_u16_f32 ((void (*)(uint16_t *dest, int dstr, float *src, int sstr, int n)) \
	_oil_function_clipconv_u16_f32_class.func)
extern OilFunctionClass _oil_function_clipconv_u16_f64_class;
#define clipconv_u16_f64 ((void (*)(uint16_t *dest, int dstr, double *src, int sstr, int n)) \
	_oil_function_clipconv_u16_f64_class.func)
extern OilFunctionClass _oil_function_clipconv_s32_f32_class;
#define clipconv_s32_f32 ((void (*)(int32_t *dest, int dstr, float *src, int sstr, int n)) \
	_oil_function_clipconv_s32_f32_class.func)
extern OilFunctionClass _oil_function_clipconv_s32_f64_class;
#define clipconv_s32_f64 ((void (*)(int32_t *dest, int dstr, double *src, int sstr, int n)) \
	_oil_function_clipconv_s32_f64_class.func)
extern OilFunctionClass _oil_function_clipconv_u32_f32_class;
#define clipconv_u32_f32 ((void (*)(uint32_t *dest, int dstr, float *src, int sstr, int n)) \
	_oil_function_clipconv_u32_f32_class.func)
extern OilFunctionClass _oil_function_clipconv_u32_f64_class;
#define clipconv_u32_f64 ((void (*)(uint32_t *dest, int dstr, double *src, int sstr, int n)) \
	_oil_function_clipconv_u32_f64_class.func)
extern OilFunctionClass _oil_function_splat_u8_class;
#define splat_u8 ((void (*)(uint8_t *dest, int dstr, uint8_t value, int n)) \
	_oil_function_splat_u8_class.func)
extern OilFunctionClass _oil_function_splat_u32_class;
#define splat_u32 ((void (*)(uint32_t *dest, int dstr, uint32_t *src, int n)) \
	_oil_function_splat_u32_class.func)
extern OilFunctionClass _oil_function_tablelookup_u8_class;
#define tablelookup_u8 ((void (*)(uint8_t *dest, int dstr, uint8_t *src, int sstr, uint8_t *table, int tablestride, int n)) \
	_oil_function_tablelookup_u8_class.func)
extern OilFunctionClass _oil_function_scalarmult_s8_class;
#define scalarmult_s8 ((void (*)(int8_t *dest, int dstr, int8_t *src, int sstr, int8_t value, int n)) \
	_oil_function_scalarmult_s8_class.func)
extern OilFunctionClass _oil_function_scalarmult_u8_class;
#define scalarmult_u8 ((void (*)(uint8_t *dest, int dstr, uint8_t *src, int sstr, uint8_t value, int n)) \
	_oil_function_scalarmult_u8_class.func)
extern OilFunctionClass _oil_function_scalarmult_s16_class;
#define scalarmult_s16 ((void (*)(int16_t *dest, int dstr, int16_t *src, int sstr, int16_t value, int n)) \
	_oil_function_scalarmult_s16_class.func)
extern OilFunctionClass _oil_function_scalarmult_u16_class;
#define scalarmult_u16 ((void (*)(uint16_t *dest, int dstr, uint16_t *src, int sstr, uint16_t value, int n)) \
	_oil_function_scalarmult_u16_class.func)
extern OilFunctionClass _oil_function_scalarmult_s32_class;
#define scalarmult_s32 ((void (*)(int32_t *dest, int dstr, int32_t *src, int sstr, int32_t value, int n)) \
	_oil_function_scalarmult_s32_class.func)
extern OilFunctionClass _oil_function_scalarmult_u32_class;
#define scalarmult_u32 ((void (*)(uint32_t *dest, int dstr, uint32_t *src, int sstr, uint32_t value, int n)) \
	_oil_function_scalarmult_u32_class.func)
extern OilFunctionClass _oil_function_scalarmult_f32_class;
#define scalarmult_f32 ((void (*)(float *dest, int dstr, float *src, int sstr, float value, int n)) \
	_oil_function_scalarmult_f32_class.func)
extern OilFunctionClass _oil_function_scalarmult_f64_class;
#define scalarmult_f64 ((void (*)(double *dest, int dstr, double *src, int sstr, double value, int n)) \
	_oil_function_scalarmult_f64_class.func)

#endif


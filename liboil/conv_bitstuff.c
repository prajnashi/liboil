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
#include <liboil.h>

#ifdef HAVE_IEEE754_H

#include <ieee754.h>

static void conv_f32_u8_bitstuff(float *dst, int dest_stride, uint8_t *src,
	int src_stride, int n)
{
	const float offset = -65536;
	union ieee754_float id;
	int i;

	id.f = 0x1ffff;
	for(i=0;i<n;i++){
		id.ieee.mantissa = (*src<<7);
		*dst = id.f + offset;
		OIL_INCREMENT(dst, dest_stride);
		OIL_INCREMENT(src, src_stride);
	}
}
OIL_DEFINE_IMPL(conv_f32_u8_bitstuff, conv_f32_u8_class);

static void conv_f32_s8_bitstuff(float *dst, int dest_stride, int8_t *src,
	int src_stride, int n)
{
	const float offset = -384;
	union ieee754_float id;
	int i;

	id.f = 0x1ff;
	for(i=0;i<n;i++){
		id.ieee.mantissa = ((*src^0x80)<<15);
		*dst = id.f + offset;
		OIL_INCREMENT(dst, dest_stride);
		OIL_INCREMENT(src, src_stride);
	}
}
OIL_DEFINE_IMPL(conv_f32_s8_bitstuff, conv_f32_s8_class);

static void conv_f32_u16_bitstuff(float *dst, int dest_stride, uint16_t *src,
	int src_stride, int n)
{
	const float offset = -65536;
	union ieee754_float id;
	int i;

	id.f = 0x1ffff;
	for(i=0;i<n;i++){
		id.ieee.mantissa = (*src<<7);
		*dst = id.f + offset;
		OIL_INCREMENT(dst, dest_stride);
		OIL_INCREMENT(src, src_stride);
	}
}
OIL_DEFINE_IMPL(conv_f32_u16_bitstuff, conv_f32_u16_class);

static void conv_f32_s16_bitstuff(float *dst, int dest_stride, int16_t *src,
	int src_stride, int n)
{
	const float offset = -98304;
	union ieee754_float id;
	int i;

	id.f = 0x1ffff;
	for(i=0;i<n;i++){
		id.ieee.mantissa = ((*src^0x8000)<<7);
		*dst = id.f + offset;
		OIL_INCREMENT(dst, dest_stride);
		OIL_INCREMENT(src, src_stride);
	}
}
OIL_DEFINE_IMPL(conv_f32_s16_bitstuff, conv_f32_s16_class);


#define signbit_S32(x) (((uint32_t)(x))>>31)

/* This implementation is slightly inaccurate */
static void conv_s16_f32_bitstuff(int16_t *dst, int dest_stride, float *src,
	int src_stride, int n)
{
	const float offset = 98304.5;
	union ieee754_double id;
	int i;
	int16_t d;

	for(i=0;i<n;i++){
		id.d = offset + *src;
		d = 0x8000 | (id.ieee.mantissa0 >> 4);
		d += (-32768-d)*signbit_S32(id.ieee.exponent-1039);
		d += (32767-d)*signbit_S32(1039-id.ieee.exponent);
		*dst = d;
		OIL_INCREMENT(dst, dest_stride);
		OIL_INCREMENT(src, src_stride);
	}
}
OIL_DEFINE_IMPL(conv_s16_f32_bitstuff, conv_s16_f32_class);


#if 0
static void conv_f64_u8_bitstuff(float *dst, int dest_stride, uint8_t *src,
	int src_stride, int n)
{
	const float offset = -65536;
	union ieee754_float id;
	int i;

	id.f = 0x1ffff;
	for(i=0;i<n;i++){
		id.ieee.mantissa = (*src<<7);
		*dst = id.f + offset;
		OIL_INCREMENT(dst, dest_stride);
		OIL_INCREMENT(src, src_stride);
	}
}
OIL_DEFINE_IMPL(conv_f64_u8_bitstuff, conv_f64_u8_class);

static void conv_f64_s8_bitstuff(float *dst, int dest_stride, int8_t *src,
	int src_stride, int n)
{
	const float offset = -384;
	union ieee754_float id;
	int i;

	id.f = 0x1ff;
	for(i=0;i<n;i++){
		id.ieee.mantissa = ((*src^0x80)<<15);
		*dst = id.f + offset;
		OIL_INCREMENT(dst, dest_stride);
		OIL_INCREMENT(src, src_stride);
	}
}
OIL_DEFINE_IMPL(conv_f64_s8_bitstuff, conv_f64_s8_class);

static void conv_f64_u16_bitstuff(float *dst, int dest_stride, uint16_t *src,
	int src_stride, int n)
{
	const float offset = -65536;
	union ieee754_float id;
	int i;

	id.f = 0x1ffff;
	for(i=0;i<n;i++){
		id.ieee.mantissa = (*src<<7);
		*dst = id.f + offset;
		OIL_INCREMENT(dst, dest_stride);
		OIL_INCREMENT(src, src_stride);
	}
}
OIL_DEFINE_IMPL(conv_f64_u16_bitstuff, conv_f64_u16_class);

static void conv_f64_s16_bitstuff(float *dst, int dest_stride, int16_t *src,
	int src_stride, int n)
{
	const float offset = -98304;
	union ieee754_float id;
	int i;

	id.f = 0x1ffff;
	for(i=0;i<n;i++){
		id.ieee.mantissa = ((*src^0x8000)<<7);
		*dst = id.f + offset;
		OIL_INCREMENT(dst, dest_stride);
		OIL_INCREMENT(src, src_stride);
	}
}
OIL_DEFINE_IMPL(conv_f64_s16_bitstuff, conv_f64_s16_class);
#endif

/* This implementation is slightly inaccurate */
static void conv_s16_f64_bitstuff(int16_t *dst, int dest_stride, float *src,
	int src_stride, int n)
{
	const float offset = 98304.5;
	union ieee754_double id;
	int i;
	int16_t d;

	for(i=0;i<n;i++){
		id.d = offset + *src;
		d = 0x8000 ^ (id.ieee.mantissa0 >> 4);
		d += (-32768-d)*signbit_S32(id.ieee.exponent-1039);
		d += (32767-d)*signbit_S32(1039-id.ieee.exponent);
#if 0
/* for clipping */
		if (id.ieee.exponent < 1039) {
		  d = -32768;
		}
		if (id.ieee.exponent > 1039) {
		  d = 32767;
		}
#endif
		*dst = d;
		OIL_INCREMENT (dst, dest_stride);
		OIL_INCREMENT (src, src_stride);
	}
}
OIL_DEFINE_IMPL(conv_s16_f64_bitstuff, conv_s16_f64_class);

#endif


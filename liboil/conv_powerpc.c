/* liboil - Library of Optimized Inner Loops
 * Copyright (C) 2002,2003  David A. Schleef <ds@schleef.org>
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
#include <liboil/conv.h>


#if defined(SL_COMPILE_ARCH_POWERPC)

#define __SL_DEFINE_CLIPCONVERT__POWERPC(dsttag,dsttype,srctag,srctype,minval,maxval,insn1,insn2)					\
SL_FUNC void _sl_clipconvert_##dsttag##_##srctag##__powerpc(dsttype *dst,	\
	int dst_stride, srctype *src, int src_stride, int n)		\
{									\
	double min = (minval);						\
	double max = (maxval);						\
	double ftmp;							\
									\
	asm volatile(							\
		"	sub %1,%1,%6		\n"			\
		"	sub %0,%0,%7		\n"			\
		"	mtctr %2		\n"			\
		insn1							\
		"	fsub 1,0,%3		\n"			\
		"	fsel 0,1,0,%3		\n"			\
		"	fsub 1,0,%4		\n"			\
		"	fsel 0,1,%4,0		\n"			\
		"	fctiw 1,0		\n"			\
		"	stfd 1,0(%5)		\n"			\
		insn2							\
		"	bdnz 1b			\n"			\
	: "+b" (dst), "+b" (src)					\
	: "b" (n), "f" (min), "f" (max), "b" (&ftmp), "r" (src_stride),	\
	  "r" (dst_stride)						\
	: "32", "33", "11", "ctr" );					\
}

#define LFDUX	"1:	lfdux 0,%6,%1		\n"
#define LFSUX	"1:	lfsux 0,%6,%1		\n"

#define LBZ_STBUX \
		"	lbz 11,7(%5)		\n" \
		"	stbux 11,%7,%0		\n"
#define LHZ_STHUX \
		"	lhz 11,7(%5)		\n" \
		"	sthux 11,%7,%0		\n"
#define LWZ_STWUX \
		"	lwz 11,7(%5)		\n" \
		"	stwux 11,%7,%0		\n"

__SL_DEFINE_CLIPCONVERT__POWERPC(S8,int8_t,F32,float, -128.0, 127.0, LFSUX, LBZ_STBUX)
__SL_DEFINE_CLIPCONVERT__POWERPC(U8,uint8_t,F32,float, 0.0, 255.0, LFSUX, LBZ_STBUX)
__SL_DEFINE_CLIPCONVERT__POWERPC(S16,int16_t,F32,float, -32768.0, 32767.0, LFSUX, LHZ_STHUX)
__SL_DEFINE_CLIPCONVERT__POWERPC(U16,uint16_t,F32,float, 0.0, 65535.0, LFSUX, LHZ_STHUX)
__SL_DEFINE_CLIPCONVERT__POWERPC(S32,int32_t,F32,float, -2147483648.0, 2147483647.0, LFSUX, LWZ_STWUX)
__SL_DEFINE_CLIPCONVERT__POWERPC(U32,uint32_t,F32,float, 0.0, 4294967295.0, LFSUX, LWZ_STWUX)

__SL_DEFINE_CLIPCONVERT__POWERPC(S8,int8_t,F64,double, -128.0, 127.0, LFDUX, LBZ_STBUX)
__SL_DEFINE_CLIPCONVERT__POWERPC(U8,uint8_t,F64,double, 0.0, 255.0, LFDUX, LBZ_STBUX)
__SL_DEFINE_CLIPCONVERT__POWERPC(S16,int16_t,F64,double, -32768.0, 32767.0, LFDUX, LHZ_STHUX)
__SL_DEFINE_CLIPCONVERT__POWERPC(U16,uint16_t,F64,double, 0.0, 65535.0, LFDUX, LHZ_STHUX)
__SL_DEFINE_CLIPCONVERT__POWERPC(S32,int32_t,F64,double, -2147483648.0, 2147483647.0, LFDUX, LWZ_STWUX)
__SL_DEFINE_CLIPCONVERT__POWERPC(U32,uint32_t,F64,double, 0.0, 4294967295.0, LFDUX, LWZ_STWUX)


#if defined(SL_COMPILE_FEATURE_POWERPC_ALTIVEC)
SL_FUNC void _sl_clipconvert_S8_F32__powerpc_altivec(
        int8_t *dst, int dst_stride, float *src, int src_stride, int n)
{
	static uint8_t vec[16] __attribute__ ((__aligned__ (16)));
	int i;
	
	if(n&3){
		double x;
		for(i=0;i<(n&3);i++){
			x = rint(*src);
			if(x>127.0)x=127.0;
			if(x<-128.0)x=-128.0;
			*dst = x;
			(void *)src += src_stride;
			(void *)dst += dst_stride;
		}
	}
	n>>=2;
	for(i=0;i<n;i++){
		((float *)vec)[0] = *src;
		(void *)src += src_stride;
		((float *)vec)[1] = *src;
		(void *)src += src_stride;
		((float *)vec)[2] = *src;
		(void *)src += src_stride;
		((float *)vec)[3] = *src;

		asm volatile(
			"	lvx %%v0,0,%0		\n"
			"	vrfin %%v0,%%v0		\n"
			"	vctsxs %%v0,%%v0,24	\n"
			"	stvx %%v0,0,%0		\n"
		:
		: "b" (&(vec[0]))
		: "memory"
		);

		*dst = ((int8_t *)vec)[0];
		(void *)dst += dst_stride;
		*dst = ((int8_t *)vec)[4];
		(void *)dst += dst_stride;
		*dst = ((int8_t *)vec)[8];
		(void *)dst += dst_stride;
		*dst = ((int8_t *)vec)[12];
		(void *)dst += dst_stride;
	}
}

SL_FUNC void _sl_clipconvert_S16_F32__powerpc_altivec(
        int16_t *dst, int dst_stride, float *src, int src_stride, int n)
{
	static uint8_t vec[16] __attribute__ ((__aligned__ (16)));
	int i;
	
	if(n&3){
		double x;
		for(i=0;i<(n&3);i++){
			x = rint(*src);
			if(x>32767.0)x=32767.0;
			if(x<-32768.0)x=-32768.0;
			*dst = x;
			(void *)src += src_stride;
			(void *)dst += dst_stride;
		}
	}
	n>>=2;
	for(i=0;i<n;i++){
		((float *)vec)[0] = *src;
		(void *)src += src_stride;
		((float *)vec)[1] = *src;
		(void *)src += src_stride;
		((float *)vec)[2] = *src;
		(void *)src += src_stride;
		((float *)vec)[3] = *src;

		asm volatile(
			"	lvx %%v0,0,%0		\n"
			"	vrfin %%v0,%%v0		\n"
			"	vctsxs %%v0,%%v0,16	\n"
			"	stvx %%v0,0,%0		\n"
		:
		: "b" (&(vec[0]))
		: "memory"
		);

		*dst = ((int16_t *)vec)[0];
		(void *)dst += dst_stride;
		*dst = ((int16_t *)vec)[2];
		(void *)dst += dst_stride;
		*dst = ((int16_t *)vec)[4];
		(void *)dst += dst_stride;
		*dst = ((int16_t *)vec)[6];
		(void *)dst += dst_stride;
	}
}

SL_FUNC void _sl_clipconvert_S32_F32__powerpc_altivec(
        int32_t *dst, int dst_stride, float *src, int src_stride, int n)
{
	static uint8_t vec[16] __attribute__ ((__aligned__ (16)));
	int i;
	
	if(n&3){
		double x;
		for(i=0;i<(n&3);i++){
			x = rintf(*src);
			if(x>2147483647.0)x=2147483647.0;
			if(x<-2147483648.0)x=-2147483648.0;
			*dst = x;
			(void *)src += src_stride;
			(void *)dst += dst_stride;
		}
	}
	n>>=2;
	for(i=0;i<n;i++){
		((float *)vec)[0] = *src;
		(void *)src += src_stride;
		((float *)vec)[1] = *src;
		(void *)src += src_stride;
		((float *)vec)[2] = *src;
		(void *)src += src_stride;
		((float *)vec)[3] = *src;

		asm volatile(
			"	lvx %%v0,0,%0		\n"
			"	vrfin %%v0,%%v0		\n"
			"	vctsxs %%v0,%%v0,0	\n"
			"	stvx %%v0,0,%0		\n"
		:
		: "b" (&(vec[0]))
		: "memory"
		);

		*dst = ((int32_t *)vec)[0];
		(void *)dst += dst_stride;
		*dst = ((int32_t *)vec)[1];
		(void *)dst += dst_stride;
		*dst = ((int32_t *)vec)[2];
		(void *)dst += dst_stride;
		*dst = ((int32_t *)vec)[3];
		(void *)dst += dst_stride;
	}
}
#endif


 SL_FUNC void _sl_convert_S16_F64__powerpc(
        int16_t *dst, int dst_stride, double *src, int src_stride, int n)
{
	double ftmp;
	
	asm volatile(
		"	sub %1,%1,%4		\n"
		"	sub %0,%0,%5		\n"
		"	mtctr %2		\n"
		"1:	lfdux 0,%4,%1		\n"
		"	fctiw 1,0		\n"
		"	stfd 1,0(%3)		\n"
		"	lhz 11,6(%3)		\n"
		"	sthux 11,%5,(%0)	\n"
		"	bdnz 1b			\n"
	: "+b" (dst), "+b" (src)
	: "b" (n), "b" (&ftmp), "r" (src_stride), "r" (dst_stride)
	: "32", "33", "11", "ctr" );
}

 SL_FUNC void _sl_convert_S16_F32__powerpc(
        int16_t *dst, int dst_stride, float *src, int src_stride, int n)
{
	double ftmp;
	
	asm volatile(
		"	sub %1,%1,%4		\n"
		"	sub %0,%0,%5		\n"
		"	mtctr %2		\n"
		"1:	lfdux 0,%4,%1		\n"
		"	fctiw 1,0		\n"
		"	stfd 1,0(%3)		\n"
		"	lhz 11,6(%3)		\n"
		"	sthux 11,%5,(%0)	\n"
		"	bdnz 1b			\n"
	: "+b" (dst), "+b" (src)
	: "b" (n), "b" (&ftmp), "r" (src_stride), "r" (dst_stride)
	: "32", "33", "11", "ctr" );
}

#endif


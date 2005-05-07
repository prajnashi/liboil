/*
 * LIBOIL - Library of Optimized Inner Loops
 * Copyright (c) 2001,2003,2004 David A. Schleef <ds@schleef.org>
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <liboil/liboilfunction.h>

#include <math.h>
#include <string.h>
#include <stdlib.h>



#define CONV_DEFINE_REF_CAST(desttype,srctype) \
static void conv_ ## desttype ## _ ## srctype ## _ref ( \
	type_ ## desttype *dest,	\
	int dest_stride,		\
	type_ ## srctype *src,		\
	int src_stride, int n)	 	\
{					\
	int i;				\
	for(i=0;i<n;i++){		\
		OIL_GET(dest,i*dest_stride, type_ ## desttype) = \
			OIL_GET(src,i*src_stride, type_ ## srctype);	\
	}				\
}					\
OIL_DEFINE_CLASS(conv_ ## desttype ## _ ## srctype,	\
	"type_" #desttype " *dest, "	\
	"int dstr, "			\
	"type_" #srctype " *src, "	\
	"int sstr, int n");	 	\
OIL_DEFINE_IMPL_REF(conv_ ## desttype ## _ ## srctype ## _ref,	\
	conv_ ## desttype ## _ ## srctype)

#define CONV_DEFINE_FLOAT_REF(desttype,srctype) \
static void conv_ ## desttype ## _ ## srctype ## _ref ( \
	type_ ## desttype *dest,	\
	int dest_stride,		\
	type_ ## srctype *src,		\
	int src_stride, int n) 		\
{					\
	int i;				\
	for(i=0;i<n;i++){		\
		OIL_GET(dest,i*dest_stride, type_ ## desttype) =		\
			rint(OIL_GET(src,i*src_stride, type_ ## srctype));	\
	}				\
}					\
OIL_DEFINE_CLASS(conv_ ## desttype ## _ ## srctype,	\
	"type_" #desttype " *dest, "	\
	"int dstr, "			\
	"type_" #srctype " *src, "	\
	"int sstr, int n");	 	\
OIL_DEFINE_IMPL_REF(conv_ ## desttype ## _ ## srctype ## _ref,	\
	conv_ ## desttype ## _ ## srctype)

CONV_DEFINE_REF_CAST(s8,u8);
CONV_DEFINE_REF_CAST(s8,s16);
CONV_DEFINE_REF_CAST(s8,u16);
CONV_DEFINE_REF_CAST(s8,s32);
CONV_DEFINE_REF_CAST(s8,u32);
CONV_DEFINE_FLOAT_REF(s8,f32);
CONV_DEFINE_FLOAT_REF(s8,f64);

CONV_DEFINE_REF_CAST(u8,s8);
CONV_DEFINE_REF_CAST(u8,s16);
CONV_DEFINE_REF_CAST(u8,u16);
CONV_DEFINE_REF_CAST(u8,s32);
CONV_DEFINE_REF_CAST(u8,u32);
CONV_DEFINE_FLOAT_REF(u8,f32);
CONV_DEFINE_FLOAT_REF(u8,f64);

CONV_DEFINE_REF_CAST(s16,s8);
CONV_DEFINE_REF_CAST(s16,u8);
CONV_DEFINE_REF_CAST(s16,u16);
CONV_DEFINE_REF_CAST(s16,s32);
CONV_DEFINE_REF_CAST(s16,u32);
CONV_DEFINE_FLOAT_REF(s16,f32);
CONV_DEFINE_FLOAT_REF(s16,f64);

CONV_DEFINE_REF_CAST(u16,s8);
CONV_DEFINE_REF_CAST(u16,u8);
CONV_DEFINE_REF_CAST(u16,s16);
CONV_DEFINE_REF_CAST(u16,s32);
CONV_DEFINE_REF_CAST(u16,u32);
CONV_DEFINE_FLOAT_REF(u16,f32);
CONV_DEFINE_FLOAT_REF(u16,f64);

CONV_DEFINE_REF_CAST(s32,s8);
CONV_DEFINE_REF_CAST(s32,s16);
CONV_DEFINE_REF_CAST(s32,u8);
CONV_DEFINE_REF_CAST(s32,u16);
CONV_DEFINE_REF_CAST(s32,u32);
CONV_DEFINE_FLOAT_REF(s32,f32);
CONV_DEFINE_FLOAT_REF(s32,f64);

CONV_DEFINE_REF_CAST(u32,s8);
CONV_DEFINE_REF_CAST(u32,s16);
CONV_DEFINE_REF_CAST(u32,u8);
CONV_DEFINE_REF_CAST(u32,u16);
CONV_DEFINE_REF_CAST(u32,s32);
CONV_DEFINE_FLOAT_REF(u32,f32);
CONV_DEFINE_FLOAT_REF(u32,f64);

CONV_DEFINE_REF_CAST(f32,s8);
CONV_DEFINE_REF_CAST(f32,s16);
CONV_DEFINE_REF_CAST(f32,u8);
CONV_DEFINE_REF_CAST(f32,u16);
CONV_DEFINE_REF_CAST(f32,s32);
CONV_DEFINE_REF_CAST(f32,u32);
CONV_DEFINE_REF_CAST(f32,f64);

CONV_DEFINE_REF_CAST(f64,s8);
CONV_DEFINE_REF_CAST(f64,u8);
CONV_DEFINE_REF_CAST(f64,s16);
CONV_DEFINE_REF_CAST(f64,u16);
CONV_DEFINE_REF_CAST(f64,s32);
CONV_DEFINE_REF_CAST(f64,u32);
CONV_DEFINE_REF_CAST(f64,f32);




#define CLIPCONV_DEFINE_BOTH_REF(desttype,srctype) \
static void clipconv_ ## desttype ## _ ## srctype ## _ref ( \
	type_ ## desttype *dest,	\
	int dest_stride,		\
	type_ ## srctype *src,		\
	int src_stride, int n)		\
{					\
	int i;				\
	type_ ## srctype x;		\
	for(i=0;i<n;i++){		\
		x = OIL_GET(src,i*src_stride, type_ ## srctype);			\
		if(x<type_min_ ## desttype) x=type_min_ ## desttype;	\
		if(x>type_max_ ## desttype) x=type_max_ ## desttype;	\
		OIL_GET(dest,i*dest_stride, type_ ## desttype) = x;			\
	}				\
}					\
OIL_DEFINE_CLASS(clipconv_ ## desttype ## _ ## srctype,	\
	"type_" #desttype " *dest, "	\
	"int dstr, "			\
	"type_" #srctype " *src, "	\
	"int sstr, int n");	 	\
OIL_DEFINE_IMPL_REF(clipconv_ ## desttype ## _ ## srctype ## _ref,	\
	clipconv_ ## desttype ## _ ## srctype)

#define CLIPCONV_DEFINE_UPPER_REF(desttype,srctype) \
static void clipconv_ ## desttype ## _ ## srctype ## _ref ( \
	type_ ## desttype *dest,	\
	int dest_stride,		\
	type_ ## srctype *src,		\
	int src_stride, int n)		\
{					\
	int i;				\
	type_ ## srctype x;		\
	for(i=0;i<n;i++){		\
		x = OIL_GET(src,i*src_stride, type_ ## srctype);			\
		if(x>type_max_ ## desttype) x=type_max_ ## desttype;	\
		OIL_GET(dest,i*dest_stride, type_ ## desttype) = x;			\
	}				\
}					\
OIL_DEFINE_CLASS(clipconv_ ## desttype ## _ ## srctype,	\
	"type_" #desttype " *dest, "	\
	"int dstr, "			\
	"type_" #srctype " *src, "	\
	"int sstr, int n");	 	\
OIL_DEFINE_IMPL_REF(clipconv_ ## desttype ## _ ## srctype ## _ref,	\
	clipconv_ ## desttype ## _ ## srctype)

#define CLIPCONV_DEFINE_LOWER_REF(desttype,srctype) \
static void clipconv_ ## desttype ## _ ## srctype ## _ref ( \
	type_ ## desttype *dest,	\
	int dest_stride,		\
	type_ ## srctype *src,		\
	int src_stride, int n)		\
{					\
	int i;				\
	type_ ## srctype x;		\
	for(i=0;i<n;i++){		\
		x = OIL_GET(src,i*src_stride, type_ ## srctype);			\
		if(x<type_min_ ## desttype) x=type_min_ ## desttype;	\
		OIL_GET(dest,i*dest_stride, type_ ## desttype) = x;			\
	}				\
}					\
OIL_DEFINE_CLASS(clipconv_ ## desttype ## _ ## srctype,	\
	"type_" #desttype " *dest, "	\
	"int dstr, "			\
	"type_" #srctype " *src, "	\
	"int sstr, int n");	 	\
OIL_DEFINE_IMPL_REF(clipconv_ ## desttype ## _ ## srctype ## _ref,	\
	clipconv_ ## desttype ## _ ## srctype)

#define CLIPCONV_DEFINE_FLOAT_REF(desttype,srctype) \
static void clipconv_ ## desttype ## _ ## srctype ## _ref ( \
	type_ ## desttype *dest,	\
	int dest_stride,		\
	type_ ## srctype *src,		\
	int src_stride, int n)		\
{					\
	int i;				\
	type_ ## srctype x;		\
	for(i=0;i<n;i++){		\
		x = OIL_GET(src,i*src_stride, type_ ## srctype);			\
		if(x<type_min_ ## desttype) x=type_min_ ## desttype;	\
		if(x>type_max_ ## desttype) x=type_max_ ## desttype;	\
		OIL_GET(dest,i*dest_stride, type_ ## desttype) = rint(x);		\
	}				\
}					\
OIL_DEFINE_CLASS(clipconv_ ## desttype ## _ ## srctype,	\
	"type_" #desttype " *dest, "	\
	"int dstr, "			\
	"type_" #srctype " *src, "	\
	"int sstr, int n");	 	\
OIL_DEFINE_IMPL_REF(clipconv_ ## desttype ## _ ## srctype ## _ref,	\
	clipconv_ ## desttype ## _ ## srctype)

/* clip upper */
CLIPCONV_DEFINE_UPPER_REF(s8,u8);
CLIPCONV_DEFINE_UPPER_REF(s8,u16);
CLIPCONV_DEFINE_UPPER_REF(s8,u32);
CLIPCONV_DEFINE_UPPER_REF(u8,u32);
CLIPCONV_DEFINE_UPPER_REF(u8,u16);
CLIPCONV_DEFINE_UPPER_REF(s16,u16);
CLIPCONV_DEFINE_UPPER_REF(s16,u32);
CLIPCONV_DEFINE_UPPER_REF(s32,u32);
CLIPCONV_DEFINE_UPPER_REF(u16,u32);

/* clip both */
CLIPCONV_DEFINE_BOTH_REF(s8,s16);
CLIPCONV_DEFINE_BOTH_REF(s8,s32);
CLIPCONV_DEFINE_BOTH_REF(u8,s16);
CLIPCONV_DEFINE_BOTH_REF(u8,s32);
CLIPCONV_DEFINE_BOTH_REF(s16,s32);
CLIPCONV_DEFINE_BOTH_REF(u16,s32);

/* clip lower */
CLIPCONV_DEFINE_LOWER_REF(u8,s8);
CLIPCONV_DEFINE_LOWER_REF(u16,s16);
CLIPCONV_DEFINE_LOWER_REF(u32,s32);

/* clip both, float */
CLIPCONV_DEFINE_FLOAT_REF(s8,f32);
CLIPCONV_DEFINE_FLOAT_REF(s8,f64);
CLIPCONV_DEFINE_FLOAT_REF(u8,f32);
CLIPCONV_DEFINE_FLOAT_REF(u8,f64);
CLIPCONV_DEFINE_FLOAT_REF(s16,f32);
CLIPCONV_DEFINE_FLOAT_REF(s16,f64);
CLIPCONV_DEFINE_FLOAT_REF(u16,f32);
CLIPCONV_DEFINE_FLOAT_REF(u16,f64);
CLIPCONV_DEFINE_FLOAT_REF(s32,f32);
CLIPCONV_DEFINE_FLOAT_REF(s32,f64);
CLIPCONV_DEFINE_FLOAT_REF(u32,f32);
CLIPCONV_DEFINE_FLOAT_REF(u32,f64);



#define SCALECONV_DEFINE_REF_RINT(desttype,srctype) \
static void scaleconv_ ## desttype ## _ ## srctype ## _ref ( \
	type_ ## desttype *dest,	\
	type_ ## srctype *src,		\
	int n, double *offset, double *multiplier) \
{					\
	int i;				\
        double x;                       \
	for(i=0;i<n;i++){		\
		x = *offset + *multiplier * src[i];	\
		if(x<type_min_ ## desttype) x=type_min_ ## desttype;	\
		if(x>type_max_ ## desttype) x=type_max_ ## desttype;	\
		dest[i] = rint(x);	\
	}				\
}					\
OIL_DEFINE_CLASS(scaleconv_ ## desttype ## _ ## srctype,	\
	"type_" #desttype " *dest, "	\
	"type_" #srctype " *src, "	\
	"int n, double *s2_1, double *s3_1"); \
OIL_DEFINE_IMPL_REF(scaleconv_ ## desttype ## _ ## srctype ## _ref,	\
	scaleconv_ ## desttype ## _ ## srctype)

#define SCALECONV_DEFINE_REF_CAST(desttype,srctype) \
static void scaleconv_ ## desttype ## _ ## srctype ## _ref ( \
	type_ ## desttype *dest,	\
	type_ ## srctype *src,		\
	int n, double *offset, double *multiplier) \
{					\
	int i;				\
	for(i=0;i<n;i++){		\
		dest[i] = *offset + *multiplier * src[i];	\
	}				\
}					\
OIL_DEFINE_CLASS(scaleconv_ ## desttype ## _ ## srctype,	\
	"type_" #desttype " *dest, "	\
	"type_" #srctype " *src, "	\
	"int n, double *s2_1, double *s3_1"); \
OIL_DEFINE_IMPL_REF(scaleconv_ ## desttype ## _ ## srctype ## _ref,	\
	scaleconv_ ## desttype ## _ ## srctype)

SCALECONV_DEFINE_REF_RINT(s8,f32);
SCALECONV_DEFINE_REF_RINT(u8,f32);
SCALECONV_DEFINE_REF_RINT(s16,f32);
SCALECONV_DEFINE_REF_RINT(u16,f32);
SCALECONV_DEFINE_REF_RINT(s32,f32);
SCALECONV_DEFINE_REF_RINT(u32,f32);

SCALECONV_DEFINE_REF_RINT(s8,f64);
SCALECONV_DEFINE_REF_RINT(u8,f64);
SCALECONV_DEFINE_REF_RINT(s16,f64);
SCALECONV_DEFINE_REF_RINT(u16,f64);
SCALECONV_DEFINE_REF_RINT(s32,f64);
SCALECONV_DEFINE_REF_RINT(u32,f64);

SCALECONV_DEFINE_REF_CAST(f32,s8);
SCALECONV_DEFINE_REF_CAST(f32,u8);
SCALECONV_DEFINE_REF_CAST(f32,s16);
SCALECONV_DEFINE_REF_CAST(f32,u16);
SCALECONV_DEFINE_REF_CAST(f32,s32);
SCALECONV_DEFINE_REF_CAST(f32,u32);

SCALECONV_DEFINE_REF_CAST(f64,s8);
SCALECONV_DEFINE_REF_CAST(f64,u8);
SCALECONV_DEFINE_REF_CAST(f64,s16);
SCALECONV_DEFINE_REF_CAST(f64,u16);
SCALECONV_DEFINE_REF_CAST(f64,s32);
SCALECONV_DEFINE_REF_CAST(f64,u32);


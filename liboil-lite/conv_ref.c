/* liboil - Library of Optimized Inner Loops
 * Copyright (C) 2001,2003  David A. Schleef <ds@schleef.org>
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
#include <liboil/liboilprofile.h>

#include <math.h>




#define CONV_DEFINE_REF_CAST(desttype,srctype) \
void conv_ ## desttype ## _ ## srctype ( \
	type_ ## desttype *dest,	\
	int dest_stride,		\
	type_ ## srctype *src,		\
	int src_stride, int n)	 	\
{					\
	int i;				\
	for(i=0;i<n;i++){		\
		*OIL_OFFSET(dest,i*dest_stride) = \
			*OIL_OFFSET(src,i*src_stride);	\
	}				\
}					\

#define CONV_DEFINE_FLOAT_REF(desttype,srctype) \
void conv_ ## desttype ## _ ## srctype ( \
	type_ ## desttype *dest,	\
	int dest_stride,		\
	type_ ## srctype *src,		\
	int src_stride, int n) 		\
{					\
	int i;				\
	for(i=0;i<n;i++){		\
		*OIL_OFFSET(dest,i*dest_stride) =		\
			rint(*OIL_OFFSET(src,i*src_stride));	\
	}				\
}					\

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

CONV_DEFINE_REF_CAST(s16,s8)
CONV_DEFINE_REF_CAST(s16,u8)
CONV_DEFINE_REF_CAST(s16,u16);
CONV_DEFINE_REF_CAST(s16,s32);
CONV_DEFINE_REF_CAST(s16,u32);
CONV_DEFINE_FLOAT_REF(s16,f32);
CONV_DEFINE_FLOAT_REF(s16,f64);

CONV_DEFINE_REF_CAST(u16,s8)
CONV_DEFINE_REF_CAST(u16,u8)
CONV_DEFINE_REF_CAST(u16,s16);
CONV_DEFINE_REF_CAST(u16,s32);
CONV_DEFINE_REF_CAST(u16,u32);
CONV_DEFINE_FLOAT_REF(u16,f32);
CONV_DEFINE_FLOAT_REF(u16,f64);

CONV_DEFINE_REF_CAST(s32,s8)
CONV_DEFINE_REF_CAST(s32,s16)
CONV_DEFINE_REF_CAST(s32,u8)
CONV_DEFINE_REF_CAST(s32,u16)
CONV_DEFINE_REF_CAST(s32,u32);
CONV_DEFINE_FLOAT_REF(s32,f32);
CONV_DEFINE_FLOAT_REF(s32,f64);

CONV_DEFINE_REF_CAST(u32,s8)
CONV_DEFINE_REF_CAST(u32,s16)
CONV_DEFINE_REF_CAST(u32,u8)
CONV_DEFINE_REF_CAST(u32,u16)
CONV_DEFINE_REF_CAST(u32,s32);
CONV_DEFINE_FLOAT_REF(u32,f32);
CONV_DEFINE_FLOAT_REF(u32,f64);

CONV_DEFINE_REF_CAST(f32,s8)
CONV_DEFINE_REF_CAST(f32,s16)
CONV_DEFINE_REF_CAST(f32,u8)
CONV_DEFINE_REF_CAST(f32,u16)
CONV_DEFINE_REF_CAST(f32,s32);
CONV_DEFINE_REF_CAST(f32,u32);
CONV_DEFINE_REF_CAST(f32,f64);

CONV_DEFINE_REF_CAST(f64,s8)
CONV_DEFINE_REF_CAST(f64,u8)
CONV_DEFINE_REF_CAST(f64,s16)
CONV_DEFINE_REF_CAST(f64,u16)
CONV_DEFINE_REF_CAST(f64,s32)
CONV_DEFINE_REF_CAST(f64,u32)
CONV_DEFINE_REF_CAST(f64,f32)




#define CLIPCONV_DEFINE_BOTH_REF(desttype,srctype) \
void clipconv_ ## desttype ## _ ## srctype ( \
	type_ ## desttype *dest,	\
	int dest_stride,		\
	type_ ## srctype *src,		\
	int src_stride, int n)		\
{					\
	int i;				\
	type_ ## srctype x;		\
	for(i=0;i<n;i++){		\
		x = *OIL_OFFSET(src,i*src_stride);			\
		if(x<type_min_ ## desttype) x=type_min_ ## desttype;	\
		if(x>type_max_ ## desttype) x=type_max_ ## desttype;	\
		*OIL_OFFSET(dest,i*dest_stride) = x;			\
	}				\
}					\

#define CLIPCONV_DEFINE_UPPER_REF(desttype,srctype) \
void clipconv_ ## desttype ## _ ## srctype ( \
	type_ ## desttype *dest,	\
	int dest_stride,		\
	type_ ## srctype *src,		\
	int src_stride, int n)		\
{					\
	int i;				\
	type_ ## srctype x;		\
	for(i=0;i<n;i++){		\
		x = *OIL_OFFSET(src,i*src_stride);			\
		if(x>type_max_ ## desttype) x=type_max_ ## desttype;	\
		*OIL_OFFSET(dest,i*dest_stride) = x;			\
	}				\
}					\

#define CLIPCONV_DEFINE_LOWER_REF(desttype,srctype) \
void clipconv_ ## desttype ## _ ## srctype ( \
	type_ ## desttype *dest,	\
	int dest_stride,		\
	type_ ## srctype *src,		\
	int src_stride, int n)		\
{					\
	int i;				\
	type_ ## srctype x;		\
	for(i=0;i<n;i++){		\
		x = *OIL_OFFSET(src,i*src_stride);			\
		if(x<type_min_ ## desttype) x=type_min_ ## desttype;	\
		*OIL_OFFSET(dest,i*dest_stride) = x;			\
	}				\
}					\

#define CLIPCONV_DEFINE_FLOAT_REF(desttype,srctype) \
void clipconv_ ## desttype ## _ ## srctype ( \
	type_ ## desttype *dest,	\
	int dest_stride,		\
	type_ ## srctype *src,		\
	int src_stride, int n)		\
{					\
	int i;				\
	type_ ## srctype x;		\
	for(i=0;i<n;i++){		\
		x = *OIL_OFFSET(src,i*src_stride);			\
		if(x<type_min_ ## desttype) x=type_min_ ## desttype;	\
		if(x>type_max_ ## desttype) x=type_max_ ## desttype;	\
		*OIL_OFFSET(dest,i*dest_stride) = rint(x);		\
	}				\
}					\

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




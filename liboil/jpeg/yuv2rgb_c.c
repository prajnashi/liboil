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

#include <liboil/liboil.h>
#include "jpeg.h"

OIL_DEFINE_CLASS (yuv2rgbx_u8,
    "uint8_t *dest, uint8_t *src1, uint8_t *src2, uint8_t *src3, int n");
OIL_DEFINE_CLASS (yuv2rgbx_sub2_u8,
    "uint8_t *dest, uint8_t *src1, uint8_t *src2, uint8_t *src3, int n");
OIL_DEFINE_CLASS (yuv2rgbx_sub4_u8,
    "uint8_t *dest, uint8_t *src1, uint8_t *src2, uint8_t *src3, int n");

#define clamp(x,a,b) clamp_lower(clamp_upper(x,b),a)
#define clamp_lower(x,a) ((x<a)?(a):(x))
#define clamp_upper(x,b) ((x>b)?(b):(x))

/* from the JFIF spec */
#define YUV_TO_R(y,u,v) clamp((y) + 1.402*((v)-128),0,255)
#define YUV_TO_G(y,u,v) clamp((y) - 0.34414*((u)-128) - 0.71414*((v)-128),0,255)
#define YUV_TO_B(y,u,v) clamp((y) + 1.772*((u)-128),0,255)

#define YUV_TO_R_INT(y,u,v) clamp(((y)*256 + 358*((v)-128))>>8,0,255)
#define YUV_TO_G_INT(y,u,v) clamp(((y)*256 - 88*((u)-128) - 183*((v)-128))>>8,0,255)
#define YUV_TO_B_INT(y,u,v) clamp(((y)*256 + 454*((u)-128))>>8,0,255)


static void
yuv2rgbx_u8_ref (uint8_t *rgbp, uint8_t *yp, uint8_t *up, uint8_t *vp, int n)
{
	int i;

	for(i=0;i<n;i++){
		rgbp[0] = YUV_TO_R(yp[0], up[0], vp[0]);
		rgbp[1] = YUV_TO_G(yp[0], up[0], vp[0]);
		rgbp[2] = YUV_TO_B(yp[0], up[0], vp[0]);
		rgbp[3] = 0;
		rgbp+=4;
		yp++;
		up++;
		vp++;
	}
}

OIL_DEFINE_IMPL_REF (yuv2rgbx_u8_ref, yuv2rgbx_u8_class);

static void
yuv2rgbx_u8_int (uint8_t *rgbp, uint8_t *yp, uint8_t *up, uint8_t *vp, int n)
{
	int i;

	for(i=0;i<n;i++){
		rgbp[0] = YUV_TO_R_INT(yp[0], up[0], vp[0]);
		rgbp[1] = YUV_TO_G_INT(yp[0], up[0], vp[0]);
		rgbp[2] = YUV_TO_B_INT(yp[0], up[0], vp[0]);
		rgbp[3] = 0;
		rgbp+=4;
		yp++;
		up++;
		vp++;
	}
}

OIL_DEFINE_IMPL (yuv2rgbx_u8_int, yuv2rgbx_u8_class);

static void
yuv2rgbx_sub2_u8_ref (uint8_t *rgbp, uint8_t *yp, uint8_t *up, uint8_t *vp, int n)
{
	int i;

	for(i=0;i<n/2;i++){
		rgbp[0] = YUV_TO_R(yp[0], up[0], vp[0]);
		rgbp[1] = YUV_TO_G(yp[0], up[0], vp[0]);
		rgbp[2] = YUV_TO_B(yp[0], up[0], vp[0]);
		rgbp[3] = 0;
		rgbp+=4;
		yp++;
		rgbp[0] = YUV_TO_R(yp[0], up[0], vp[0]);
		rgbp[1] = YUV_TO_G(yp[0], up[0], vp[0]);
		rgbp[2] = YUV_TO_B(yp[0], up[0], vp[0]);
		rgbp[3] = 0;
		rgbp+=4;
		yp++;
		up++;
		vp++;
	}
}

OIL_DEFINE_IMPL_REF (yuv2rgbx_sub2_u8_ref, yuv2rgbx_sub2_u8_class);

static void
yuv2rgbx_sub4_u8_ref (uint8_t *rgbp, uint8_t *yp, uint8_t *up, uint8_t *vp, int n)
{
	int i;

	for(i=0;i<n/4;i++){
		rgbp[0] = YUV_TO_R(yp[0], up[0], vp[0]);
		rgbp[1] = YUV_TO_G(yp[0], up[0], vp[0]);
		rgbp[2] = YUV_TO_B(yp[0], up[0], vp[0]);
		rgbp[3] = 0;
		rgbp+=4;
		yp++;
		rgbp[0] = YUV_TO_R(yp[0], up[0], vp[0]);
		rgbp[1] = YUV_TO_G(yp[0], up[0], vp[0]);
		rgbp[2] = YUV_TO_B(yp[0], up[0], vp[0]);
		rgbp[3] = 0;
		rgbp+=4;
		yp++;
		rgbp[0] = YUV_TO_R(yp[0], up[0], vp[0]);
		rgbp[1] = YUV_TO_G(yp[0], up[0], vp[0]);
		rgbp[2] = YUV_TO_B(yp[0], up[0], vp[0]);
		rgbp[3] = 0;
		rgbp+=4;
		yp++;
		rgbp[0] = YUV_TO_R(yp[0], up[0], vp[0]);
		rgbp[1] = YUV_TO_G(yp[0], up[0], vp[0]);
		rgbp[2] = YUV_TO_B(yp[0], up[0], vp[0]);
		rgbp[3] = 0;
		rgbp+=4;
		yp++;
		up++;
		vp++;
	}
}

OIL_DEFINE_IMPL_REF (yuv2rgbx_sub4_u8_ref, yuv2rgbx_sub4_u8_class);


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
#include <liboil/liboilfuncs.h>
#include <liboil/dct/dct.h>



#define C0_9808 0.980785280
#define C0_9239 0.923879532
#define C0_8315 0.831469612
#define C0_7071 0.707106781
#define C0_5556 0.555570233
#define C0_3827 0.382683432
#define C0_1951 0.195090322

/*
Alternate scaling used by RTjpeg.
*/


OIL_DEFINE_CLASS(fdct8x8s_s16,
    "int16_t *d_8x8, int ds, int16_t *s_8x8, int ss");

static void
fdct8x8s_s16_ref (int16_t *dest, int dstr, int16_t *src, int sstr)
{
	double s[64], d[64];
	double scale[8] = {
		4*C0_7071,
		4*C0_9808,
		4*C0_9239,
		4*C0_8315,
		4*C0_7071,
		4*C0_5556,
		4*C0_3827,
		4*C0_1951,
	};
	int i,j;

	for(i=0;i<8;i++){
		for(j=0;j<8;j++){
			s[8*i+j] = OIL_GET (src,sstr*i+j, int16_t);
		}
	}

	oil_fdct8x8_f64(d,8*sizeof(double),s,8*sizeof(double));

	for(i=0;i<8;i++){
		for(j=0;j<8;j++){
			d[8*i+j] *= scale[i] * scale[j];
		}
	}

	oil_conv8x8_s16_f64(dest,dstr,d,8*sizeof(double));
}

OIL_DEFINE_IMPL_REF (fdct8x8s_s16_ref, fdct8x8s_s16);



/* liboil - Library of Optimized Inner Loops
 * Copyright (C) 2001,2002,2003  David A. Schleef <ds@schleef.org>
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
#include <liboil/dct/dct.h>
#include <math.h>


#define BLOCK8x8_F64(ptr, stride, row, column) \
	(*((double *)((void *)ptr + stride*row) + column))

#define BLOCK8x8_PTR_F64(ptr, stride, row, column) \
	((double *)((void *)ptr + stride*row) + column)

#define BLOCK8x8_S16(ptr, stride, row, column) \
	(*((int16_t *)((void *)ptr + stride*row) + column))

OIL_DEFINE_CLASS (idct8x8_f64, "double *d_8x8, int dstr, double *s_8x8, int sstr");
OIL_DEFINE_CLASS (idct8x8_s16, "int16_t *d_8x8, int dstr, int16_t *s_8x8, int sstr");

static void
idct8x8_f64_slow (double *dest, int dstr, double *src, int sstr)
{
	static double idct_coeff[8][8];
	static int idct_coeff_init = 0;
	int i,j,k,l;
	double tmp1,tmp2;

	if(!idct_coeff_init){
		double scale;

		for(i=0;i<8;i++){
			scale = (i==0) ? sqrt(0.125) : 0.5;
			for(j=0;j<8;j++){
				idct_coeff[j][i] = scale *
					cos((M_PI/8)*i*(j+0.5));
			}
		}
		idct_coeff_init = 1;
	}

	for(i=0;i<8;i++){
		for(j=0;j<8;j++){
			tmp1 = 0;
			for(k=0;k<8;k++){
				tmp2 = 0;
				for(l=0;l<8;l++){
					tmp2 += idct_coeff[j][l] *
						BLOCK8x8_F64(src,sstr,k,l);
				}
				tmp1 += idct_coeff[i][k] * tmp2;
			}
			BLOCK8x8_F64(dest,dstr,i,j) = tmp1;
		}
	}
}

OIL_DEFINE_IMPL_REF (idct8x8_f64_slow, idct8x8_f64);

#if defined(oil_idct8_f64)
static void
idct8x8_f64_c (double *dest, int dstr, double *src, int sstr)
{
	int i;
	double tmp[64];
	int tmpstr = 8*sizeof(double);

	for(i=0;i<8;i++){
		oil_idct8_f64(
			BLOCK8x8_PTR_F64(tmp,tmpstr,i,0), sizeof(double),
			BLOCK8x8_PTR_F64(src,sstr,i,0), sizeof(double));
	}
	for(i=0;i<8;i++){
		oil_idct8_f64(
			BLOCK8x8_PTR_F64(dest,dstr,0,i), dstr,
			BLOCK8x8_PTR_F64(tmp,tmpstr,0,i), tmpstr);
	}
}

OIL_DEFINE_IMPL_DEPENDS (idct8x8_f64_c, idct8x8_f64, idct8_f64);
#endif

#if defined(oil_conv8x8_f64_s16) && defined(oil_idct8x8_f64) && \
    defined(oil_conv8x8_s16_f64)
static void
idct8x8_s16_slow (int16_t *dest, int dstr, int16_t *src, int sstr)
{
	double s[64], d[64];

	oil_conv8x8_f64_s16 (s,8*sizeof(double),src,sstr);
	oil_idct8x8_f64 (d,8*sizeof(double),s,8*sizeof(double));
	oil_conv8x8_s16_f64 (dest,dstr,d,8*sizeof(double));
}

OIL_DEFINE_IMPL_REF (idct8x8_s16_slow, idct8x8_s16);
#if 0
OIL_DEFINE_IMPL_DEPENDS (idct8x8_s16_slow, idct8x8_s16,
    conv8x8_f64_s16, idct8x8_f64, conv8x8_s16_f64);
#endif
#endif


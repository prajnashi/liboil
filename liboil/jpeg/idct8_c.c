/* liboil - Library of Optimized Inner Loops
 * Copyright (C) 2001,2001,2003  David A. Schleef <ds@schleef.org>
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

#include <math.h>

//OIL_DEFINE_CLASS_X (idct8_f64, "double *dest, int dstr, double *src, int sstr");
OIL_DEFINE_CLASS_X (idct8_f64, NULL);

static void
idct8_f64_c (double *dest, int dstr, double *src, int sstr)
{
	static double idct_coeff[8][8];
	static int idct_coeff_init = 0;
	int i,j;
	double x;

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
		x = 0;
		for(j=0;j<8;j++){
			x += idct_coeff[i][j] *
				*(double *)(((void *)src)+sstr*j);
		}
		*(double *)(((void *)dest)+dstr*i) = x;
	}
}

OIL_DEFINE_IMPL (idct8_f64_c, idct8_f64_class);


#define get_f64(ptr,str,i) (*(double *)((void *)(ptr) + (str)*(i)))
#define put_f64(ptr,str,i) (*(double *)((void *)(ptr) + (str)*(i)))

#define C0_9808 0.980785280
#define C0_9239 0.923879532
#define C0_8315 0.831469612
#define C0_7071 0.707106781
#define C0_5556 0.555570233
#define C0_3827 0.382683432
#define C0_1951 0.195090322


static void
idct8_f64_fast(double *dest, int dstr, double *src, int sstr)
{
	double s07, s16, s25, s34;
	double d07, d16, d25, d34;
	double ss07s34, ss16s25;
	double ds07s34, ds16s25;

	ss07s34 = C0_7071*(get_f64(src,sstr,0) + get_f64(src,sstr,4));
	ss16s25 = C0_7071*(get_f64(src,sstr,0) - get_f64(src,sstr,4));

	ds07s34 = C0_9239*get_f64(src,sstr,2) + C0_3827*get_f64(src,sstr,6);
	ds16s25 = C0_3827*get_f64(src,sstr,2) - C0_9239*get_f64(src,sstr,6);

	s07 = ss07s34 + ds07s34;
	s34 = ss07s34 - ds07s34;

	s16 = ss16s25 + ds16s25;
	s25 = ss16s25 - ds16s25;

	d07 = C0_9808*get_f64(src,sstr,1) + C0_8315*get_f64(src,sstr,3)
		+ C0_5556*get_f64(src,sstr,5) + C0_1951*get_f64(src,sstr,7);
	d16 = C0_8315*get_f64(src,sstr,1) - C0_1951*get_f64(src,sstr,3)
		- C0_9808*get_f64(src,sstr,5) - C0_5556*get_f64(src,sstr,7);
	d25 = C0_5556*get_f64(src,sstr,1) - C0_9808*get_f64(src,sstr,3)
		+ C0_1951*get_f64(src,sstr,5) + C0_8315*get_f64(src,sstr,7);
	d34 = C0_1951*get_f64(src,sstr,1) - C0_5556*get_f64(src,sstr,3)
		+ C0_8315*get_f64(src,sstr,5) - C0_9808*get_f64(src,sstr,7);

	put_f64(dest,dstr,0) = 0.5 * (s07 + d07);
	put_f64(dest,dstr,1) = 0.5 * (s16 + d16);
	put_f64(dest,dstr,2) = 0.5 * (s25 + d25);
	put_f64(dest,dstr,3) = 0.5 * (s34 + d34);
	put_f64(dest,dstr,4) = 0.5 * (s34 - d34);
	put_f64(dest,dstr,5) = 0.5 * (s25 - d25);
	put_f64(dest,dstr,6) = 0.5 * (s16 - d16);
	put_f64(dest,dstr,7) = 0.5 * (s07 - d07);

}

OIL_DEFINE_IMPL (idct8_f64_fast, idct8_f64_class);


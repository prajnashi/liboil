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
#include <liboil/liboilfunction.h>
#include <conv.h>

#include <math.h>


static void
conv_f64_s16_table(double *dest, int dest_stride, short *src,
	int src_stride, int n)
{
	static float ints_high[256];
	static float ints_low[256];
	static int init = 0;
	int i;
	unsigned int idx;
	if(!init){
		for(i=0;i<256;i++){
			ints_high[i]=256.0*((i<128)?i:i-256);
			ints_low[i]=i;
		}
		init = 1;
	}

	if(n&1){
		idx = (unsigned short)*src;
		*dest = ints_high[(idx>>8)] + ints_low[(idx&0xff)];
		OIL_INCREMENT(dest, dest_stride);
		OIL_INCREMENT(src, src_stride);
		n-=1;
	}
	for(i=0;i<n;i+=2){
		idx = (unsigned short)*src;
		*dest = ints_high[(idx>>8)] + ints_low[(idx&0xff)];
		OIL_INCREMENT(dest, dest_stride);
		OIL_INCREMENT(src, src_stride);
		idx = (unsigned short)*src;
		*dest = ints_high[(idx>>8)] + ints_low[(idx&0xff)];
		OIL_INCREMENT(dest, dest_stride);
		OIL_INCREMENT(src, src_stride);
	}
}
OIL_DEFINE_IMPL(conv_f64_s16_table, conv_f64_s16);



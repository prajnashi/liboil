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
#include <liboil/simdpack/simdpack.h>

OIL_DEFINE_CLASS (get8x8_f64,
    "double *dest, int dstr, double *src, int sstr, int x, int y");

static void get8x8_f64_ref(double *dest, int dstr, double *src, int sstr,
    int x, int y)
{
	int i,j;

	for(i=0;i<8;i++){
		for(j=0;j<8;j++){
			OIL_GET (dest,dstr*i+j, double) = 
				OIL_GET (src,sstr*(i+y)+j+x, double);
		}
	}
}

OIL_DEFINE_IMPL_REF (get8x8_f64_ref, get8x8_f64_class);



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
#include <math.h>

OIL_DEFINE_CLASS (sincos_f64,
    "double *dest1, double *dest2, int n, double *param1, double *param2");

static void
sincos_f64_ref (double *dest_sin, double *dest_cos, int n, double *offset,
    double *interval)
{
	int i;

	for(i=0;i<n;i++){
		dest_sin[i] = sin(*offset + *interval*i);
		dest_cos[i] = cos(*offset + *interval*i);
	}
}
OIL_DEFINE_IMPL_REF (sincos_f64_ref, sincos_f64);

/* XXX This gets more inaccurate as N increases */
static void
sincos_f64_i20_fast(double *dest_sin, double *dest_cos, int n, double *offset,
    double *interval)
{
	int i;
	double c,s,dc,ds,temp_s,temp_c;

	dest_sin[0] = s = sin(*offset);
	dest_cos[0] = c = cos(*offset);
	ds = sin(*interval);
	dc = cos(*interval);
	for(i=1;i<n;i++){
		temp_s = s*dc + c*ds;
		temp_c = c*dc - s*ds;
		dest_sin[i] = s = temp_s;
		dest_cos[i] = c = temp_c;
	}
}
OIL_DEFINE_IMPL (sincos_f64_i20_fast, sincos_f64);



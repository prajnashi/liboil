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
#include <math.h>


#define MULTSUM_DEFINE_REF(type)	\
static void multsum_ ## type ## _ref(	\
    type_ ## type *dest,		\
    type_ ## type *src1, int sstr1,	\
    type_ ## type *src2, int sstr2,	\
    int n)				\
{					\
  int i;				\
  double sum = 0;			\
  double errsum = 0;			\
  for(i=0;i<n;i++){			\
    type_ ## type x;                    \
    type_ ## type tmp;                  \
    x = OIL_GET(src1,sstr1*i,type_ ## type) * OIL_GET(src2,sstr2*i,type_ ## type);		\
    tmp = sum;				\
    sum += x;				\
    errsum += (tmp - sum) + x;		\
  }					\
  *dest = sum + errsum;			\
}					\
OIL_DEFINE_IMPL_REF (multsum_ ## type ## _ref, multsum_ ## type); \
OIL_DEFINE_CLASS (multsum_ ## type, \
    "type_" #type " *dest, "		\
    "type_" #type " *src1, int sstr1, "	\
    "type_" #type " *src2, int sstr2, "	\
    "int n")

MULTSUM_DEFINE_REF(f32);
MULTSUM_DEFINE_REF(f64);


static void multsum_f32_unroll2 (float *dest, float *src1, int sstr1,
    float *src2, int sstr2, int n)
{
  int i;
  double sum1 = 0;
  double sum2 = 0;

  if (n&1) {
    sum1 += OIL_GET(src1,0, float) * OIL_GET(src2,0, float);
    OIL_INCREMENT (src1, sstr1);
    OIL_INCREMENT (src2, sstr2);
  }
  for(i=0;i<n;i++){
    sum1 += OIL_GET(src1,sstr1*i, float) * OIL_GET(src2,sstr2*i, float);
    sum2 += OIL_GET(src1,sstr1*i, float) * OIL_GET(src2,sstr2*i, float);
  }

  *dest = sum1 + sum2;
}
OIL_DEFINE_IMPL (multsum_f32_unroll2, multsum_f32);


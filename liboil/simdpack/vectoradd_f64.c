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


#define VECTORADD_DEFINE_IMPL(type)		\
static void vectoradd_ ## type ## _ref (	\
    type_ ## type *dest, int dstr,		\
    type_ ## type *src1, int sstr1,		\
    type_ ## type *src2, int sstr2,		\
    int n, type_ ## type *v1, type_ ## type *v2) \
{						\
  int i;					\
  for(i=0;i<n;i++) {				\
    OIL_GET(dest,i*dstr, type_ ## type) = *v1 * OIL_GET(src1,i*sstr1, type_ ## type) +	\
        *v2 * OIL_GET(src2,i*sstr2, type_ ## type);		\
  }						\
}						\
OIL_DEFINE_CLASS (vectoradd_ ## type,         \
    "type_" #type " *dest, int dstr, "		\
    "type_" #type " *src1, int sstr1, "		\
    "type_" #type " *src2, int sstr2, "		\
    "int n, type_" #type " *s3_1, type_" #type " *s4_1");	\
OIL_DEFINE_IMPL_REF (vectoradd_ ## type ## _ref, vectoradd_ ## type);

VECTORADD_DEFINE_IMPL (s8);
VECTORADD_DEFINE_IMPL (u8);
VECTORADD_DEFINE_IMPL (s16);
VECTORADD_DEFINE_IMPL (u16);
VECTORADD_DEFINE_IMPL (s32);
VECTORADD_DEFINE_IMPL (u32);
VECTORADD_DEFINE_IMPL (f32);
VECTORADD_DEFINE_IMPL (f64);





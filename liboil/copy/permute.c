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

#define PERMUTE_DEFINE_REF(type)		\
static void permute_ ## type ## _ref(		\
    type_ ## type *dest, int dstr,		\
    type_ ## type *src, int sstr,		\
    int32_t *perm, int pstr, int n)		\
{						\
  int i;					\
  for(i=0;i<n;i++){				\
    OIL_GET(dest,dstr*i, type_ ## type) = OIL_GET(src,sstr*	\
	OIL_GET(perm,pstr*i, int), type_ ## type);		\
  }						\
}						\
OIL_DEFINE_IMPL_REF (permute_ ## type ## _ref, permute_ ## type ## _class); \
OIL_DEFINE_CLASS_X (permute_ ## type, "type_" #type " *dest, int dstr, " \
    "type_" #type " *src, int sstr, int32_t *perm, int pstr, int n")

PERMUTE_DEFINE_REF (s8);
PERMUTE_DEFINE_REF (u8);
PERMUTE_DEFINE_REF (s16);
PERMUTE_DEFINE_REF (u16);
PERMUTE_DEFINE_REF (s32);
PERMUTE_DEFINE_REF (u32);
PERMUTE_DEFINE_REF (f32);
PERMUTE_DEFINE_REF (f64);


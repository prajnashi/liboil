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

#define TRANS8x8_DEFINE_REF(type)		\
static void trans8x8_ ## type ## _ref (		\
    type_ ## type *dest, int dstr,		\
    type_ ## type *src, int sstr)		\
{						\
  int i,j;					\
  for(i=0;i<8;i++){				\
    for(j=0;j<8;j++){				\
      OIL_GET(dest,dstr*i+j,type_ ## type) = OIL_GET(src,sstr*j+i,type_ ## type); \
    }						\
  }						\
}						\
OIL_DEFINE_CLASS (trans8x8_ ## type, "type_" #type " *d_8x8, int ds,	" \
    "type_" #type " *s_8x8, int ss"); \
OIL_DEFINE_IMPL_REF (trans8x8_ ## type ## _ref, trans8x8_ ## type)

TRANS8x8_DEFINE_REF(u8);
TRANS8x8_DEFINE_REF(u16);
TRANS8x8_DEFINE_REF(u32);
TRANS8x8_DEFINE_REF(f64);



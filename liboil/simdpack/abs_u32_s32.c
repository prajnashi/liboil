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

#include <liboil/liboilfunction.h>
#include <liboil/simdpack/simdpack.h>


#if 0
static void
abs_u32_s32_ref (uint32_t * dest, int32_t * src, int n)
{
  int i;

  for (i = 0; i < n; i++) {
    dest[i] = (src[i] < 0) ? -src[i] : src[i];
  }
}

OIL_DEFINE_IMPL_REF (abs_u32_s32_ref, abs_u32_s32);
#endif

/* Similar algorithm as clipping, but for absolute value.  Note the stunt
 * pulled to get around 2*y rolling over.  */

static void
abs_u32_s32_fast (uint32_t * dest, int dstr, int32_t * src, int sstr, int n)
{
  int i;
  int32_t x, y;

  for (i = 0; i < n; i++) {
    x = OIL_GET(src, i * sstr, int32_t);
    y = ((x >> 31) & x);
    OIL_GET(dest, i * dstr, uint32_t) = x - y - y;
  }
}

OIL_DEFINE_IMPL (abs_u32_s32_fast, abs_u32_s32);


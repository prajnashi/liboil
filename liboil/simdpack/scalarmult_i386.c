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

static void
scalarmult_f32_sse (float *dest, int dstr, float *src, int sstr,
    float *val, int n)
{
  float tmp[8];
  float *t = (void *)(((unsigned long)tmp & ~0xf) + 16);
  int i;

  t[0] = *val;
  t[1] = *val;
  t[2] = *val;
  t[3] = *val;
  asm volatile (
      "  movss (%0), %%xmm1 \n"
      : 
      : "r" (t));
  for(i=0;i<n;i+=4) {
    t[0] = OIL_GET(src,sstr*(i + 0), float);
    t[1] = OIL_GET(src,sstr*(i + 1), float);
    t[2] = OIL_GET(src,sstr*(i + 2), float);
    t[3] = OIL_GET(src,sstr*(i + 3), float);
    asm volatile (
        "  movss (%0), %%xmm0 \n"
        "  mulps %%xmm1, %%xmm0 \n"
        "  movss %%xmm0, (%0) \n"
        : 
        : "r" (t));
    OIL_GET(dest,dstr*(i + 0), float) = t[0];
    OIL_GET(dest,dstr*(i + 0), float) = t[1];
    OIL_GET(dest,dstr*(i + 0), float) = t[2];
    OIL_GET(dest,dstr*(i + 0), float) = t[3];
  }

}
OIL_DEFINE_IMPL_FULL (scalarmult_f32_sse, scalarmult_f32, OIL_IMPL_REQUIRES_SSE);



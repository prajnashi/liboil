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
#include <liboil/copy/copy.h>

OIL_DEFINE_CLASS_X(splat_u8,"uint8_t *dest, int dstr, uint8_t value, int n");
OIL_DEFINE_CLASS_X(splat_u32,"uint32_t *dest, int dstr, uint32_t *src, int n");


static void splat_u8_ref (uint8_t *dest, int dstr, uint8_t value, int n)
{
  int i;
  for(i=0;i<n;i++){
    OIL_GET(dest,i*dstr, uint8_t) = value;
  }
}
OIL_DEFINE_IMPL_REF(splat_u8_ref, splat_u8_class);

static void splat_u32_ref (uint32_t *dest, int dstr, uint32_t *src, int n)
{
  int i;
  for(i=0;i<n;i++){
    OIL_GET(dest,i*dstr, uint32_t) = *src;
  }
}
OIL_DEFINE_IMPL_REF(splat_u32_ref, splat_u32_class);



static void splat_u32_unroll2 (uint32_t *dest, int dstr, uint32_t *src, int n)
{
  int i;
  if (n&1) {
    *dest = *src;
    OIL_INCREMENT(dest,dstr);
  }
  n >>= 1;
  for(i=0;i<n;i++){
    *dest = *src;
    OIL_INCREMENT(dest,dstr);
    *dest = *src;
    OIL_INCREMENT(dest,dstr);
  }
}
OIL_DEFINE_IMPL(splat_u32_unroll2, splat_u32_class);


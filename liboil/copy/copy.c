/* liboil - Library of Optimized Inner Loops
 * Copyright (C) 2004  David A. Schleef <ds@schleef.org>
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

#include <string.h>

#include <liboil/liboilfunction.h>

OIL_DEFINE_CLASS (copy_u8, "uint8_t *dest, uint8_t *src, int n");

static void
copy_u8_ref (uint8_t *dest, uint8_t *src, int n)
{
  int i;
  for(i=0;i<n;i++){
    dest[i] = src[i];
  }
}
OIL_DEFINE_IMPL_REF (copy_u8_ref, copy_u8);

static void
copy_u8_libc (uint8_t *dest, uint8_t *src, int n)
{
  memcpy (dest, src, n);
}
OIL_DEFINE_IMPL (copy_u8_libc, copy_u8);



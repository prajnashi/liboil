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
#include <liboil/liboil.h>
#include "jpeg.h"

#include <math.h>


#define BLOCK8x8_F64(ptr, stride, row, column) \
	(*((double *)((void *)ptr + stride*row) + column))

#define BLOCK8x8_PTR_F64(ptr, stride, row, column) \
	((double *)((void *)ptr + stride*row) + column)

#define BLOCK8x8_S16(ptr, stride, row, column) \
	(*((int16_t *)((void *)ptr + stride*row) + column))

#define BLOCK8x8_U8(ptr, stride, row, column) \
	(*((uint8_t *)((void *)ptr + stride*row) + column))


static void
conv8x8_s16_f64_c (int16_t * dest, int dstr, double *src, int sstr)
{
  int i, j;

  for (i = 0; i < 8; i++) {
    for (j = 0; j < 8; j++) {
      BLOCK8x8_S16 (dest, dstr, i, j) = rint (BLOCK8x8_F64 (src, sstr, i, j));
    }
  }
}

OIL_DEFINE_IMPL (conv8x8_s16_f64_c, conv8x8_s16_f64_class);

static void
conv8x8_f64_s16_c (double *dest, int dstr, int16_t * src, int sstr)
{
  int i, j;

  for (i = 0; i < 8; i++) {
    for (j = 0; j < 8; j++) {
      BLOCK8x8_F64 (dest, dstr, i, j) = BLOCK8x8_S16 (src, sstr, i, j);
    }
  }
}

OIL_DEFINE_IMPL (conv8x8_f64_s16_c, conv8x8_f64_s16_class);

static void
clipconv8x8_u8_s16_c (uint8_t * dest, int dstr, int16_t * src, int sstr)
{
  int i, j;
  int16_t x;

  for (i = 0; i < 8; i++) {
    for (j = 0; j < 8; j++) {
      x = BLOCK8x8_S16 (src, sstr, i, j);
      if (x < 0)
	x = 0;
      if (x > 255)
	x = 255;
      BLOCK8x8_U8 (dest, dstr, i, j) = x;
    }
  }
}

OIL_DEFINE_IMPL (clipconv8x8_u8_s16_c, clipconv8x8_u8_s16_class);

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

OIL_DEFINE_CLASS_X (abs_u8_s8, "uint8_t *dest, int dstr, int8_t *src, int sstr, int n");
OIL_DEFINE_CLASS_X (abs_u16_s16, "uint16_t *dest, int dstr, int16_t *src, int sstr, int n");
OIL_DEFINE_CLASS_X (abs_u32_s32, "uint32_t *dest, int dstr, int32_t *src, int sstr, int n");
OIL_DEFINE_CLASS_X (abs_f32_f32, "float *dest, int dstr, float *src, int sstr, int n");
OIL_DEFINE_CLASS_X (abs_f64_f64, "double *dest, int dstr, double *src, int sstr, int n");

//static void abs_test (OilFunctionClass *klass, OilFunctionImpl *impl);

#define ABS(x) ((x)>0 ? (x) : -(x))


#if 0
static void
abs_test (OilFunctionClass *klass, OilFunctionImpl *impl)
{


}
#endif

static void
abs_u8_s8_ref (uint8_t *dest, int dstr, int8_t *src, int sstr, int n)
{
  int i;
  int x;

  for (i=0; i<n; i++) {
    x = OIL_GET(src, i*sstr, int8_t);
    x = ABS(x);
    OIL_GET(dest, i*dstr, uint8_t) = x;
  }
}
OIL_DEFINE_IMPL_REF (abs_u8_s8_ref, abs_u8_s8_class);

static void
abs_u16_s16_ref (uint16_t *dest, int dstr, int16_t *src, int sstr, int n)
{
  int i;
  int x;

  for (i=0; i<n; i++) {
    x = OIL_GET(src, i*sstr, int16_t);
    x = ABS(x);
    OIL_GET(dest, i*dstr, uint16_t) = x;
  }
}
OIL_DEFINE_IMPL_REF (abs_u16_s16_ref, abs_u16_s16_class);

static void
abs_u32_s32_ref (uint32_t *dest, int dstr, int32_t *src, int sstr, int n)
{
  int i;
  int x;

  for (i=0; i<n; i++) {
    x = OIL_GET(src, i*sstr, int32_t);
    x = ABS(x);
    OIL_GET(dest, i*dstr, uint32_t) = x;
  }
}
OIL_DEFINE_IMPL_REF (abs_u32_s32_ref, abs_u32_s32_class);

static void
abs_f32_f32_ref (float *dest, int dstr, float *src, int sstr, int n)
{
  int i;

  for (i=0; i<n; i++) {
    OIL_GET(dest, i*dstr, float) = fabs(OIL_GET(src, i*sstr, float));
  }
}

OIL_DEFINE_IMPL_REF (abs_f32_f32_ref, abs_f32_f32_class);

static void
abs_f64_f64_ref (double *dest, int dstr, double *src, int sstr, int n)
{
  int i;

  for (i=0; i<n; i++) {
    OIL_GET(dest, i*dstr, double) = fabs(OIL_GET(src, i*sstr, double));
  }
}

OIL_DEFINE_IMPL_REF (abs_f64_f64_ref, abs_f64_f64_class);

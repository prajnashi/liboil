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

OIL_DEFINE_IMPL_REF (abs_u32_s32_ref, abs_u32_s32_class);
#endif

/* Similar algorithm as clipping, but for absolute value.  Note the stunt
 * pulled to get around 2*y rolling over.  */

static void
abs_u32_s32_fast (uint32_t * dest, int32_t * src, int n)
{
  int i;
  int32_t x, y;

  for (i = 0; i < n; i++) {
    x = src[i];
    y = ((x >> 31) & x);
    dest[i] = x - y - y;
  }
}

OIL_DEFINE_IMPL_REF (abs_u32_s32_fast, abs_u32_s32_class);

#ifdef TEST_abs_u32_s32
int
TEST_abs_u32_s32 (void)
{
  int i;
  int pass;
  int failures = 0;
  uint32_t *dest_test;
  uint32_t *dest_ref;
  int32_t *src;
  struct sl_profile_struct t;

  src = sl_malloc (sizeof (int32_t) * N);
  dest_test = sl_malloc (sizeof (int32_t) * N);
  dest_ref = sl_malloc (sizeof (int32_t) * N);

  sl_profile_init (t);
  srand (20020305);

  printf ("I: " sl_stringify (abs_u32_s32_FUNC) "\n");

  for (pass = 0; pass < N_PASS; pass++) {
    for (i = 0; i < N; i++)
      src[i] = sl_rand_int32_t ();

    abs_u32_s32_ref (dest_ref, src, N);
    sl_profile_start (t);
    abs_u32_s32_FUNC (dest_test, src, N);
    sl_profile_stop (t);

    for (i = 0; i < N; i++) {
      if (dest_test[i] != dest_ref[i]) {
	printf ("%d %d %u %u\n", i, src[i], dest_ref[i], dest_test[i]);
	failures++;
      }
    }
  }

  sl_free (dest_ref);
  sl_free (dest_test);
  sl_free (src);

  if (failures) {
    printf ("E: %d failures\n", failures);
  }

  sl_profile_print (t);

  return failures;
}
#endif

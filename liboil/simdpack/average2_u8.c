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

OIL_DEFINE_CLASS (average2_u8, 
    "uint8_t * dest, int dstr, uint8_t *src1, int sstr1, "
    "uint8_t *src2, int sstr2, int n");

static void
average2_u8_ref (uint8_t * dest, int dstr, uint8_t *src1, int sstr1,
    uint8_t *src2, int sstr2, int n)
{
  int i;

  for (i = 0; i < n; i++) {
    dest[i] = (src1[sstr1 * i] + src2[sstr2 * i]) >> 1;
  }
}

OIL_DEFINE_IMPL_REF (average2_u8_ref, average2_u8_class);

static void
average2_u8_trick (uint8_t * dest, int dstr, uint8_t *src1, int sstr1,
    uint8_t *src2, int sstr2, int n)
{
  unsigned int x, y, d;

  if (sstr1 == 1 && sstr2 == 1) {
    while (n > 0) {
      x = *(unsigned int *) src1;
      y = *(unsigned int *) src2;
      *(unsigned int *) dest = (((x ^ y) & 0xfefefefe) >> 1) + (x & y);
      src1 += 4;
      src2 += 4;
      dest += 4;
      n -= 4;
    }
  }
  else {
    while (n > 0) {
      x = (src1[0] << 24) | (src1[sstr1] << 16) | (src1[2 *
	      sstr1] << 8) | (src1[3 * sstr1]);
      y = (src2[0] << 24) | (src2[sstr2] << 16) | (src2[2 *
	      sstr2] << 8) | (src2[3 * sstr2]);
      d = (((x ^ y) & 0xfefefefe) >> 1) + (x & y);
      dest[0] = (d >> 24);
      dest[1] = (d >> 16);
      dest[2] = (d >> 8);
      dest[3] = (d >> 0);
      src1 += 4 * sstr1;
      src2 += 4 * sstr2;
      dest += 4;
      n -= 4;
    }
  }
}

OIL_DEFINE_IMPL (average2_u8_trick, average2_u8_class);

static void
average2_u8_fast (uint8_t * dest, int dstr, uint8_t *src1, int sstr1,
    uint8_t *src2, int sstr2, int n)
{
  while (n > 0) {
    *dest++ = (*src1 + *src2) >> 1;
    src1 += sstr1;
    src2 += sstr2;
    n--;
  }
}

OIL_DEFINE_IMPL (average2_u8_fast, average2_u8_class);

static void
average2_u8_unroll4 (uint8_t * dest, int dstr, uint8_t *src1, int sstr1,
    uint8_t *src2, int sstr2, int n)
{
  while (n & 0x3) {
    *dest++ = (*src1 + *src2) >> 1;
    src1 += sstr1;
    src2 += sstr2;
    n--;
  }
  while (n > 0) {
    *dest++ = (*src1 + *src2) >> 1;
    src1 += sstr1;
    src2 += sstr2;
    *dest++ = (*src1 + *src2) >> 1;
    src1 += sstr1;
    src2 += sstr2;
    *dest++ = (*src1 + *src2) >> 1;
    src1 += sstr1;
    src2 += sstr2;
    *dest++ = (*src1 + *src2) >> 1;
    src1 += sstr1;
    src2 += sstr2;
    n -= 4;
  }
}

OIL_DEFINE_IMPL (average2_u8_unroll4, average2_u8_class);

#ifdef SIMDPACK_USE_I386
/* This doesn't work with sstr!=2 or dstr!=2 */
static void
average2_u8_i386asm (uint8_t * dest, int dstr, uint8_t *src1, int sstr1,
    uint8_t *src2, int sstr2, int n)
{
  __asm__ __volatile__ ("\n"
      "	.p2align 4,,15			\n"
      "1:	movzbl	(%%ebx), %%eax		\n"
      "	addl	$2, %%ebx		\n"
      "	movzbl	(%%ecx), %%edx		\n"
      "	addl	$2, %%ecx		\n"
      "	leal	1(%%edx, %%eax), %%eax	\n"
      "	sarl	$1, %%eax		\n"
      "	movb	%%al, 0(%%esi)		\n"
      "	incl	%%esi			\n"
      "	decl	%%edi			\n"
      "	jg	1b			\n":"+b"
      (src1), "+c" (src2), "+D" (n), "+S" (dest)
      ::"eax", "edx");

}

OIL_DEFINE_IMPL (average2_u8_i386asm, average2_u8_class);
#endif


#ifdef TEST_average2_u8
int
TEST_average2_u8 (void)
{
  int i;
  int pass;
  int failures = 0;
  u8 *src1, *src2;
  u8 *dest_test, *dest_ref;
  struct sl_profile_struct t;

  src1 = sl_malloc_u8 (N);
  src2 = sl_malloc_u8 (N);
  dest_ref = sl_malloc_u8 (N);
  dest_test = sl_malloc_u8 (N);

  sl_profile_init (t);
  srand (20021003);

  printf ("I: " sl_stringify (average2_u8_FUNC) "\n");

  for (pass = 0; pass < N_PASS; pass++) {
    for (i = 0; i < N; i++) {
      src1[i] = sl_rand_u8 ();
      src2[i] = sl_rand_u8 ();
    }

    average2_u8_ref (dest_ref, src1, 1, src2, 1, N);
    sl_profile_start (t);
    average2_u8_FUNC (dest_test, src1, 1, src2, 1, N);
    sl_profile_stop (t);

    for (i = 0; i < N / 2; i++) {
      if (dest_test[i] != dest_ref[i]) {
	printf ("%d %d %d %d %d\n", i, src1[i], src2[i], dest_ref[i],
	    dest_test[i]);
	failures++;
      }
    }
  }

  sl_free (src1);
  sl_free (src2);
  sl_free (dest_ref);
  sl_free (dest_test);

  if (failures) {
    printf ("E: %d failures\n", failures);
  }

  sl_profile_print (t);

  return failures;
}
#endif

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

#include <liboil/liboil.h>
#include <liboil/simdpack/simdpack.h>

#define ABS(x) ((x)>0 ? (x) : -(x))

static void
abs_u16_s16_ref (uint16_t * dest, int dstr, int16_t * src, int sstr, int n)
{
  int i;

  for (i = 0; i < n; i++) {
    *OIL_OFFSET (dest, dstr * i) = ABS (*OIL_OFFSET (src, sstr * i));
  }
}

OIL_DEFINE_IMPL_REF (abs_u16_s16_ref, abs_u16_s16_class);

static void
abs_u16_s16_unroll4 (uint16_t * dest, int dstr, int16_t * src, int sstr, int n)
{
  while (n & 3) {
    *dest = ABS (*src);
    OIL_INCREMENT (dest, dstr);
    OIL_INCREMENT (src, sstr);
    n--;
  }
  while (n > 0) {
    *dest = ABS (*src);
    OIL_INCREMENT (dest, dstr);
    OIL_INCREMENT (src, sstr);
    *dest = ABS (*src);
    OIL_INCREMENT (dest, dstr);
    OIL_INCREMENT (src, sstr);
    *dest = ABS (*src);
    OIL_INCREMENT (dest, dstr);
    OIL_INCREMENT (src, sstr);
    *dest = ABS (*src);
    OIL_INCREMENT (dest, dstr);
    OIL_INCREMENT (src, sstr);
    n -= 4;
  }
}

OIL_DEFINE_IMPL (abs_u16_s16_unroll4, abs_u16_s16_class);

static void
abs_u16_s16_fast (uint16_t * dest, int dstr, int16_t * src, int sstr, int n)
{
  int i;
  int16_t x, y;

  for (i = 0; i < n; i++) {
    x = *src;
    y = ((x >> 15) & x);
    *dest = x - y - y;
    OIL_INCREMENT (dest, dstr);
    OIL_INCREMENT (src, sstr);
  }
}

OIL_DEFINE_IMPL (abs_u16_s16_fast, abs_u16_s16_class);

#ifdef HAVE_CPU_I386
static void
abs_u16_s16_i386asm (uint16_t * dest, int dstr, int16_t * src, int sstr, int n)
{
  __asm__ __volatile__ ("\n"
      "	.p2align 4,,15			\n"
      "1:  movswl	(%0), %%eax		\n"
      "    addl	$2, %0			\n"
      "    movl	%%eax, %%edx		\n"
      "    negl	%%edx			\n"
      "    cmpl	$-1, %%eax		\n"
      "    cmovle	%%edx, %%eax		\n"
      "    movw	%%ax, (%1)		\n"
      "    addl	$2, %1			\n"
      "    decl	%2			\n"
      "    testl	%2, %2			\n"
      "    jg	1b			\n":"+r" (src), "+r" (dest), "+r" (n)
      ::"eax", "edx");
}

OIL_DEFINE_IMPL (abs_u16_s16_i386asm, abs_u16_s16_class);
#endif

#ifdef SIMPACK_USE_I386
static void
abs_u16_s16_i386asm2 (uint16_t * dest, int dstr, int16_t * src, int sstr, int n)
{
  __asm__ __volatile__ ("\n"
      "	pushl	%%ebp			\n"
      "	movl	%%eax, %%ebp		\n"
      "	.p2align 4,,15			\n"
      "1:	movswl	(%%edi), %%eax		\n"
      "	addl	$2, %%edi		\n"
      "	movl	%%eax, %%edx		\n"
      "	negl	%%edx			\n"
      "	cmpl	$-1, %%eax		\n"
      "	cmovle	%%edx, %%eax		\n"
      "	movw	%%ax, (%%ebp)		\n"
      "	addl	$2, %%ebp		\n"
      "	decl	%2			\n"
      "	testl	%2, %2			\n"
      "	jg	1b			\n"
      "	popl	%%ebp			\n":"+D" (src), "+a" (dest), "+S" (n)
      ::"ebx", "ecx", "edx", "ebp");
}

OIL_DEFINE_IMPL (abs_u16_s16_i386asm2, abs_u16_s16_class);
#endif

#ifdef SIMPACK_USE_I386
/* Weave two threads */
static void
abs_u16_s16_i386asm3 (uint16_t * dest, int dstr, int16_t * src, int sstr, int n)
{
  while (n & 1) {
    *dest = ABS (*src);
    OIL_INCREMENT (dest, dstr);
    OIL_INCREMENT (src, sstr);
    n--;
  }
  n /= 2;
  __asm__ __volatile__ ("\n"
      "	pushl	%%ebp			\n"
      "	movl	%%eax, %%ebp		\n"
      "	.p2align 4,,15			\n"
      "1:	movswl	(%%edi), %%eax		\n"
      "	addl	$2, %%edi		\n"
      "  	 movswl	(%%edi), %%ecx		\n"
      "	 addl	$2, %%edi		\n"
      "	movl	%%eax, %%ebx		\n"
      "	 movl	%%ecx, %%edx		\n"
      "	negl	%%ebx			\n"
      "	 negl	%%edx			\n"
      "	cmpl	$-1, %%eax		\n"
      "	cmovle	%%ebx, %%eax		\n"
      "	 cmpl	$-1, %%ecx		\n"
      "	 cmovle	%%edx, %%ecx		\n"
      "	movw	%%ax, (%%ebp)		\n"
      "	addl	$2, %%ebp		\n"
      "	 movw	%%cx, (%%ebp)		\n"
      "	 addl	$2, %%ebp		\n"
      "	decl	%2			\n"
      "	testl	%2, %2			\n"
      "	jg	1b			\n"
      "	popl	%%ebp			\n":"+D" (src), "+a" (dest), "+S" (n)
      ::"ebx", "ecx", "edx", "ebp");
}

OIL_DEFINE_IMPL (abs_u16_s16_i386asm3, abs_u16_s16_class);
#endif

#ifdef HAVE_CPU_I386
static void
abs_u16_s16_mmx (uint16_t * dest, int dstr, int16_t * src, int sstr, int n)
{
  short p[] = { -32768, -32768, -32768, -32768,
    32767, 32767, 32767, 32767
  };

  while (n & 3) {
    *dest = ABS (*src);
    OIL_INCREMENT (dest, dstr);
    OIL_INCREMENT (src, sstr);
    n--;
  }
  n /= 4;
  __asm__ __volatile__ ("\n"
      "	movq	(%3), %%mm2		\n"
      "	movq	8(%3), %%mm3		\n"
      "	.p2align 4,,15			\n"
      "1:	movq	(%%edi), %%mm0		\n"
      "	movq	(%%edi), %%mm1		\n"
      "	addl	$8, %%edi		\n"
      "	paddsw	%%mm2, %%mm0		\n"
      "	paddsw	%%mm3, %%mm1		\n"
      "	psubsw	%%mm2, %%mm0		\n"
      "	psubsw	%%mm3, %%mm1		\n"
      "	psubw	%%mm1, %%mm0		\n"
      "	movq	%%mm0, (%%eax)		\n"
      "	addl	$8, %%eax		\n"
      "	decl	%2			\n"
      "	testl	%2, %2			\n"
      "	jg	1b			\n":"+D" (src), "+a" (dest), "+S" (n)
      :"c" (p)
      :"ebp");
}

OIL_DEFINE_IMPL (abs_u16_s16_mmx, abs_u16_s16_class);
#endif

#ifdef HAVE_CPU_I386
static void
abs_u16_s16_mmxx (uint16_t * dest, int dstr, int16_t * src, int sstr, int n)
{
  short p[] = { -32768, -32768, -32768, -32768,
    32767, 32767, 32767, 32767
  };

  while (n & 7) {
    *dest = ABS (*src);
    OIL_INCREMENT (dest, dstr);
    OIL_INCREMENT (src, sstr);
    n--;
  }
  n /= 8;
  __asm__ __volatile__ ("\n"
      "	movq	(%3), %%mm2		\n"
      "	movq	8(%3), %%mm3		\n"
      "	.p2align 4,,15			\n"
      "1:	movq	(%%edi), %%mm0		\n"
      "	movq	(%%edi), %%mm1		\n"
      "	paddsw	%%mm2, %%mm0		\n"
      "	paddsw	%%mm3, %%mm1		\n"
      "	psubsw	%%mm2, %%mm0		\n"
      "	psubsw	%%mm3, %%mm1		\n"
      "	psubw	%%mm1, %%mm0		\n"
      "	movq	%%mm0, (%%eax)		\n"
      "	 movq	8(%%edi), %%mm4		\n"
      "	 movq	8(%%edi), %%mm5		\n"
      "	 addl	$16, %%edi		\n"
      "	 paddsw	%%mm2, %%mm4		\n"
      "	 paddsw	%%mm3, %%mm5		\n"
      "	 psubsw	%%mm2, %%mm4		\n"
      "	 psubsw	%%mm3, %%mm5		\n"
      "	 psubw	%%mm5, %%mm4		\n"
      "	 movq	%%mm4, 8(%%eax)		\n"
      "	 addl	$16, %%eax		\n"
      "	decl	%2			\n"
      "	testl	%2, %2			\n"
      "	jg	1b			\n":"+D" (src), "+a" (dest), "+S" (n)
      :"c" (p)
      :"ebp");
}

OIL_DEFINE_IMPL (abs_u16_s16_mmxx, abs_u16_s16_class);
#endif

#ifdef HAVE_CPU_MMXEXT
static void
abs_u16_s16_mmx2 (uint16_t * dest, int dstr, int16_t * src, int sstr, int n)
{
  while (n & 7) {
    *dest = ABS (*src);
    OIL_INCREMENT (dest, dstr);
    OIL_INCREMENT (src, sstr);
    n--;
  }
  n /= 8;
  __asm__ __volatile__ ("\n"
      "	pushl	%%ebp			\n"
      "	movl	%%eax, %%ebp		\n"
      "	.p2align 4,,15			\n"
      "1:	movq	(%%edi), %%mm0		\n"
      "	pxor	%%mm1, %%mm1		\n"
      "	 movq	8(%%edi), %%mm2		\n"
      "	 addl	$16, %%edi		\n"
      "	psubw	%%mm0, %%mm1		\n"
      "	 pxor	%%mm3, %%mm3		\n"
      "	pmaxsw	%%mm0, %%mm1		\n"
      "	 psubw	%%mm2, %%mm3		\n"
      "	movq	%%mm1, (%%ebp)		\n"
      "	 pmaxsw	%%mm2, %%mm3		\n"
      "	 movq	%%mm3, 8(%%ebp)		\n"
      "	 addl	$16, %%ebp		\n"
      "	decl	%2			\n"
      "	testl	%2, %2			\n"
      "	jg	1b			\n"
      "	popl	%%ebp			\n":"+D" (src), "+a" (dest), "+S" (n)
      ::"ebx", "ecx", "edx", "ebp");
}

OIL_DEFINE_IMPL (abs_u16_s16_mmx2, abs_u16_s16_class);
#endif

#ifdef HAVE_SSE2
static void
abs_u16_s16_sse2 (uint16_t * dest, int dstr, int16_t * src, int sstr, int n)
{
  while (n & 7) {
    *dest = ABS (*src);
    OIL_INCREMENT (dest, dstr);
    OIL_INCREMENT (src, sstr);
    n--;
  }
  n /= 8;
  __asm__ __volatile__ ("\n"
      "	pushl	%%ebp			\n"
      "	movl	%%eax, %%ebp		\n"
      "	.p2align 4,,15			\n"
      "1:	movq	(%%edi), %%xmm0		\n"
      "	addl	$16, %%edi		\n"
      "	pxor	%%xmm1, %%xmm1		\n"
      "	psubw	%%xmm0, %%xmm1		\n"
      "	pmaxsw	%%xmm0, %%xmm1		\n"
      "	movq	%%xmm1, (%%ebp)		\n"
      "	addl	$16, %%ebp		\n"
      "	decl	%2			\n"
      "	testl	%2, %2			\n"
      "	jg	1b			\n"
      "	popl	%%ebp			\n":"+D" (src), "+a" (dest), "+S" (n)
      ::"ebx", "ecx", "edx", "ebp");
}

OIL_DEFINE_IMPL (abs_u16_s16_sse2, abs_u16_s16_class);
#endif

#ifdef HAVE_CPU_POWERPC
static void
abs_u16_s16_a16_altivec (uint16_t * dest, int dstr, int16_t * src, int sstr,
    int n)
{
  int i;

  for (i = n & ~0x7; i < n; i++) {
    *dest = ABS (*src);
    OIL_INCREMENT (dest, dstr);
    OIL_INCREMENT (src, sstr);
  }
  n /= 8;
  __asm__ __volatile__ ("\n"
      "	li %%r10, 0			\n"
      "	vxor %%v2, %%v2, %%v2		\n"
      "	mtctr %2			\n"
      "1:	lvx %%v0,%%r10,%1		\n"
      "	vsubshs %%v1, %%v2, %%v0	\n"
      "	vmaxsh %%v1, %%v1, %%v0		\n"
      "	stvx %%v1,%%r10,%0		\n"
      "	addi %%r10, %%r10, 16		\n"
      "	bdnz 1b				\n":"+b" (dest), "+b" (src), "+b" (n)
      ::"10", "ctr");
}

OIL_DEFINE_IMPL (abs_u16_s16_altivec, abs_u16_s16_class);
#endif

#if 0
int
TEST_abs_u16_s16 (void)
{
  int i;
  int pass;
  int failures = 0;
  u16 *dest_test;
  u16 *dest_ref;
  s16 *src;
  struct sl_profile_struct t;

  src = sl_malloc (sizeof (s16) * N);
  dest_test = sl_malloc (sizeof (s16) * N);
  dest_ref = sl_malloc (sizeof (s16) * N);

  sl_profile_init (t);
  srand (20020305);

  printf ("I: " sl_stringify (abs_u16_s16_FUNC) "\n");

  for (pass = 0; pass < N_PASS; pass++) {
    for (i = 0; i < N; i++)
      src[i] = sl_rand_s16 ();

    abs_u16_s16_ref (dest_ref, src, sstr, N);
    sl_profile_start (t);
    abs_u16_s16_FUNC (dest_test, src, sstr, N);
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

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

#define ABS(x) ((x)>0 ? (x) : -(x))

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

OIL_DEFINE_IMPL (abs_u16_s16_i386asm, abs_u16_s16);

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
      ::"ecx", "edx");
}

OIL_DEFINE_IMPL (abs_u16_s16_i386asm2, abs_u16_s16);

#if 0
/* This doesn't work in PIC mode */
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

OIL_DEFINE_IMPL (abs_u16_s16_i386asm3, abs_u16_s16);
#endif

static void
abs_u16_s16_mmx (uint16_t * dest, int dstr, int16_t * src, int sstr, int n)
{
  const short p[] = { -32768, -32768, -32768, -32768,
    32767, 32767, 32767, 32767
  };
  short tmp[4];

  while (n & 3) {
    *dest = ABS (*src);
    OIL_INCREMENT (dest, dstr);
    OIL_INCREMENT (src, sstr);
    n--;
  }
  n /= 4;
  __asm__ __volatile__ ("\n"
      "	movq	(%0), %%mm2		\n"
      "	movq	8(%0), %%mm3		\n"
      :: "c" (p));
  while (n--) {
    tmp[0] = *src;
    OIL_INCREMENT (src, sstr);
    tmp[1] = *src;
    OIL_INCREMENT (src, sstr);
    tmp[2] = *src;
    OIL_INCREMENT (src, sstr);
    tmp[3] = *src;
    OIL_INCREMENT (src, sstr);
    __asm__ __volatile__ ("\n"
        "	movq	(%%eax), %%mm1		\n"
        "	paddsw	%%mm2, %%mm0		\n"
        "	paddsw	%%mm3, %%mm1		\n"
        "	psubsw	%%mm2, %%mm0		\n"
        "	psubsw	%%mm3, %%mm1		\n"
        "	psubw	%%mm1, %%mm0		\n"
        "	movq	%%mm0, (%%eax)		\n"
        :: "a" (&tmp)
    );
    *dest = tmp[0];
    OIL_INCREMENT (dest, dstr);
    *dest = tmp[1];
    OIL_INCREMENT (dest, dstr);
    *dest = tmp[2];
    OIL_INCREMENT (dest, dstr);
    *dest = tmp[3];
    OIL_INCREMENT (dest, dstr);
  }
  asm volatile ("emms");
}

OIL_DEFINE_IMPL_FULL (abs_u16_s16_mmx, abs_u16_s16, OIL_IMPL_REQUIRES_MMX);

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
      :"c" (p));
  asm volatile ("emms");
}

OIL_DEFINE_IMPL_FULL (abs_u16_s16_mmxx, abs_u16_s16, OIL_IMPL_REQUIRES_MMX);

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
      ::"ecx", "edx");
  asm volatile ("emms");
}

OIL_DEFINE_IMPL_FULL (abs_u16_s16_mmx2, abs_u16_s16, OIL_IMPL_REQUIRES_MMXEXT);

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
      ::"ecx", "edx");
}

OIL_DEFINE_IMPL_FULL (abs_u16_s16_sse2, abs_u16_s16, OIL_IMPL_REQUIRES_SSE2);


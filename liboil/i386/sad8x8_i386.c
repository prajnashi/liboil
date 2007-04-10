/*
 * LIBOIL - Library of Optimized Inner Loops
 * Copyright (c) 2003,2004 David A. Schleef <ds@schleef.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <liboil/liboilfunction.h>
#include <liboil/liboilclasses.h>


static void
sad8x8_u8_mmx (uint32_t * dest, uint8_t * src1, int sstr1, uint8_t * src2,
    int sstr2)
{
  uint32_t diff;

  __asm__ __volatile__ (
    "  pxor        %%mm6, %%mm6     \n\t"	/* zero out mm6 for unpack */
    "  pxor        %%mm7, %%mm7     \n\t" 	/* mm7 contains the result */

#define LOOP \
    "  movq        (%1), %%mm0      \n\t"	/* take 8 bytes */ \
    "  movq        (%2), %%mm1      \n\t" \
    "  movq        %%mm0, %%mm2     \n\t" \
 \
    "  psubusb     %%mm1, %%mm0     \n\t" 	/* A - B */ \
    "  psubusb     %%mm2, %%mm1     \n\t"	/* B - A */ \
    "  por         %%mm1, %%mm0     \n\t"      	/* and or gives abs difference */ \
    "  movq        %%mm0, %%mm1     \n\t" \
 \
    "  punpcklbw   %%mm6, %%mm0     \n\t"	/* unpack to higher precision for accumulation */ \
    "  paddw       %%mm0, %%mm7     \n\t"	/* accumulate difference... */ \
    "  punpckhbw   %%mm6, %%mm1     \n\t"	/* unpack high four bytes to higher precision */ \
    "  add         %3, %1           \n\t"	/* Inc pointer into the new data */ \
    "  paddw       %%mm1, %%mm7     \n\t"	/* accumulate difference... */ \
    "  add         %4, %2           \n\t"	/* Inc pointer into ref data */

    LOOP
    LOOP
    LOOP
    LOOP
    LOOP
    LOOP
    LOOP
    LOOP
#undef LOOP

    "  movq        %%mm7, %%mm0     \n\t"
    "  psrlq       $32, %%mm7       \n\t"
    "  paddw       %%mm0, %%mm7     \n\t"
    "  movq        %%mm7, %%mm0     \n\t"
    "  psrlq       $16, %%mm7       \n\t"
    "  paddw       %%mm0, %%mm7     \n\t"
    "  movd        %%mm7, %0        \n\t"
    "  andl        $0xffff, %0      \n\t"
    "  emms                         \n\t"

     : "=m" (diff),
       "+r" (src1), 
       "+r" (src2) 
     : "r" (sstr1),
       "r" (sstr2)
     : "memory"
  );
  *dest = diff;
}
OIL_DEFINE_IMPL_FULL (sad8x8_u8_mmx, sad8x8_u8, OIL_IMPL_FLAG_MMX);

static void
sad8x8_u8_mmxext (uint32_t * dest, uint8_t * src1, int sstr1, uint8_t * src2,
    int sstr2)
{
  uint32_t diff;

  __asm__ __volatile__ (
    "  pxor %%mm7, %%mm7            \n\t" 	/* mm7 contains the result */

#define LOOP \
    "  movq (%1), %%mm0             \n\t"	/* take 8 bytes */ \
    "  movq (%2), %%mm1             \n\t" \
    "  psadbw %%mm1, %%mm0          \n\t" \
    "  add %3, %1                   \n\t"	/* Inc pointer into the new data */ \
    "  paddw %%mm0, %%mm7           \n\t"	/* accumulate difference... */ \
    "  add %4, %2                   \n\t"	/* Inc pointer into ref data */

    LOOP
    LOOP
    LOOP
    LOOP
    LOOP
    LOOP
    LOOP
#undef LOOP

    "  movq (%1), %%mm0             \n\t"	/* take 8 bytes */
    "  movq (%2), %%mm1             \n\t"
    "  psadbw %%mm1, %%mm0          \n\t"
    "  paddw %%mm0, %%mm7           \n\t"	/* accumulate difference... */
    "  movd %%mm7, %0               \n\t"
    "  emms                         \n\t"

     : "=r" (diff),
       "+r" (src1), 
       "+r" (src2) 
     : "r" (sstr1),
       "r" (sstr2)
     : "memory"
  );
  *dest = diff;
}
OIL_DEFINE_IMPL_FULL (sad8x8_u8_mmxext, sad8x8_u8, OIL_IMPL_FLAG_MMX | OIL_IMPL_FLAG_MMXEXT);

static void
sad8x8_u8_mmxext_2 (uint32_t * dest, uint8_t * src1, int sstr1, uint8_t * src2,
    int sstr2)
{
  uint32_t diff;

  __asm__ __volatile__ (
    "  pxor %%mm7, %%mm7            \n\t" 	/* mm7 contains the result */

#define LOOP \
    "  movq (%1), %%mm0             \n\t"	/* take 8 bytes */ \
    "  psadbw (%2), %%mm0           \n\t" \
    "  add %3, %1                   \n\t"	/* Inc pointer into the new data */ \
    "  paddw %%mm0, %%mm7           \n\t"	/* accumulate difference... */ \
    "  add %4, %2                   \n\t"	/* Inc pointer into ref data */

    LOOP
    LOOP
    LOOP
    LOOP
    LOOP
    LOOP
    LOOP
#undef LOOP

    "  movq (%1), %%mm0             \n\t"	/* take 8 bytes */
    "  psadbw (%2), %%mm0           \n\t" \
    "  paddw %%mm0, %%mm7           \n\t"	/* accumulate difference... */
    "  movd %%mm7, %0               \n\t"
    "  emms                         \n\t"

     : "=r" (diff),
       "+r" (src1), 
       "+r" (src2) 
     : "r" (sstr1),
       "r" (sstr2)
     : "memory"
  );
  *dest = diff;
}
OIL_DEFINE_IMPL_FULL (sad8x8_u8_mmxext_2, sad8x8_u8, OIL_IMPL_FLAG_MMX | OIL_IMPL_FLAG_MMXEXT);

static void
sad8x8_u8_mmxext_3 (uint32_t * dest, uint8_t * src1, int sstr1, uint8_t * src2,
    int sstr2)
{
  uint32_t diff;

  __asm__ __volatile__ (
    "  pxor %%mm7, %%mm7            \n\t" 	/* mm7 contains the result */

    "  movq (%1), %%mm0             \n\t"	/* take 8 bytes */
    "  psadbw (%2), %%mm0           \n\t"
    "  movq (%1,%3), %%mm1          \n\t"	/* take 8 bytes */
    "  psadbw (%2,%4), %%mm1        \n\t"
    "  leal (%1,%3,2), %1           \n\t"	/* Inc pointer into the new data */
    "  paddw %%mm0, %%mm7           \n\t"	/* accumulate difference... */
    "  leal (%2,%4,2), %2           \n\t"	/* Inc pointer into ref data */
    "  paddw %%mm1, %%mm7           \n\t"	/* accumulate difference... */

    "  movq (%1), %%mm0             \n\t"	/* take 8 bytes */
    "  psadbw (%2), %%mm0           \n\t"
    "  movq (%1,%3), %%mm1          \n\t"	/* take 8 bytes */
    "  psadbw (%2,%4), %%mm1        \n\t"
    "  leal (%1,%3,2), %1           \n\t"	/* Inc pointer into the new data */
    "  paddw %%mm0, %%mm7           \n\t"	/* accumulate difference... */
    "  leal (%2,%4,2), %2           \n\t"	/* Inc pointer into ref data */
    "  paddw %%mm1, %%mm7           \n\t"	/* accumulate difference... */

    "  movq (%1), %%mm0             \n\t"	/* take 8 bytes */
    "  psadbw (%2), %%mm0           \n\t"
    "  movq (%1,%3), %%mm1          \n\t"	/* take 8 bytes */
    "  psadbw (%2,%4), %%mm1        \n\t"
    "  leal (%1,%3,2), %1           \n\t"	/* Inc pointer into the new data */
    "  paddw %%mm0, %%mm7           \n\t"	/* accumulate difference... */
    "  leal (%2,%4,2), %2           \n\t"	/* Inc pointer into ref data */
    "  paddw %%mm1, %%mm7           \n\t"	/* accumulate difference... */

    "  movq (%1), %%mm0             \n\t"	/* take 8 bytes */
    "  psadbw (%2), %%mm0           \n\t"
    "  movq (%1,%3), %%mm1          \n\t"	/* take 8 bytes */
    "  psadbw (%2,%4), %%mm1        \n\t"
    "  paddw %%mm0, %%mm7           \n\t"	/* accumulate difference... */
    "  paddw %%mm1, %%mm7           \n\t"	/* accumulate difference... */

    "  movd %%mm7, %0               \n\t"
    "  emms                         \n\t"

     : "=r" (diff),
       "+r" (src1), 
       "+r" (src2) 
     : "r" (sstr1),
       "r" (sstr2)
     : "memory"
  );
  *dest = diff;
}
OIL_DEFINE_IMPL_FULL (sad8x8_u8_mmxext_3, sad8x8_u8, OIL_IMPL_FLAG_MMX | OIL_IMPL_FLAG_MMXEXT);

static void
sad8x8_u8_mmxext_4 (uint32_t * dest, uint8_t * src1, int sstr1, uint8_t * src2,
    int sstr2)
{
  uint32_t diff;

  __asm__ __volatile__ (
    "  movq (%1), %%mm6             \n\t"
    "   movq (%1,%3,1), %%mm7          \n\t"
    "  psadbw (%2), %%mm6           \n\t"
    "   psadbw (%2,%4,1), %%mm7      \n\t"
    "  movq (%1,%3,2), %%mm0          \n\t"
    "   movq (%1,%3,4), %%mm1          \n\t"
    "  psadbw (%2,%4,2), %%mm0      \n\t"
    "   psadbw (%2,%4,4), %%mm1      \n\t"
    "  paddw %%mm0, %%mm6           \n\t"
    "   paddw %%mm1, %%mm7           \n\t"

    "  leal (%1,%3,8), %1           \n\t"
    "   leal (%2,%4,8), %2           \n\t"
    "  neg %3\n\t"
    "   neg %4\n\t"
    "  leal (%1,%3), %1           \n\t"
    "   leal (%2,%4), %2           \n\t"

    "  movq (%1), %%mm0             \n\t"
    "   movq (%1,%3,1), %%mm1          \n\t"
    "  psadbw (%2), %%mm0           \n\t"
    "   psadbw (%2,%4,1), %%mm1      \n\t"
    "  paddw %%mm0, %%mm6           \n\t"
    "   paddw %%mm1, %%mm7           \n\t"
    "  movq (%1,%3,2), %%mm0          \n\t"
    "   movq (%1,%3,4), %%mm1          \n\t"
    "  psadbw (%2,%4,2), %%mm0      \n\t"
    "   psadbw (%2,%4,4), %%mm1      \n\t"
    "  paddw %%mm0, %%mm6           \n\t"
    "   paddw %%mm1, %%mm7           \n\t"

    "  paddw %%mm6, %%mm7           \n\t"
    "  movd %%mm7, %0               \n\t"

    "  emms                         \n\t"

     : "=r" (diff),
       "+r" (src1), 
       "+r" (src2),
       "+r" (sstr1),
       "+r" (sstr2)
     :
     : "memory"
  );
  *dest = diff;
}
OIL_DEFINE_IMPL_FULL (sad8x8_u8_mmxext_4, sad8x8_u8, OIL_IMPL_FLAG_MMX | OIL_IMPL_FLAG_MMXEXT);

static void
sad8x8_8xn_u8_psadbw (uint32_t * dest, uint8_t * src1, int sstr1, uint8_t * src2,
    int sstr2, int n)
{
  int n_tmp = n;
  uint32_t *dest_tmp = dest;
  uint8_t *src2_tmp = src2;
  uint8_t *src1_tmp = src1;

  __asm__ __volatile__ ("\n"
    "1:\n"
    "  movq (%[src1]), %%mm7             \n\t"
    "  psadbw (%[src2]), %%mm7           \n\t"
    "   movq (%[src1],%[sstr1],1), %%mm1          \n\t"
    "   psadbw (%[src2],%[sstr2],1), %%mm1      \n\t"
    "   paddw %%mm1, %%mm7           \n\t"
    "  movq (%[src1],%[sstr1],2), %%mm0          \n\t"
    "  psadbw (%[src2],%[sstr2],2), %%mm0      \n\t"
    "  paddw %%mm0, %%mm7           \n\t"
    "   movq (%[src1],%[sstr1],4), %%mm1          \n\t"
    "   psadbw (%[src2],%[sstr2],4), %%mm1      \n\t"
    "   paddw %%mm1, %%mm7           \n\t"
    "  movd %%mm7, 0(%[dest])               \n\t"
    "  addl %[sstr2],%[src2]\n\t"
    "  addl $4, %[dest]\n\t"
    "  decl %[n]\n\t"
    "  jnz 1b\n\t"
    "  emms                         \n\t"
     : [src1] "+r" (src1_tmp), 
       [src2] "+r" (src2_tmp),
       [n] "+m" (n_tmp),
       [dest] "+r" (dest_tmp)
     : [sstr1] "r" (sstr1),
       [sstr2] "r" (sstr2)
     : "memory"
  );

  src1 = OIL_OFFSET(src1, 7 * sstr1);
  src2 = OIL_OFFSET(src2, 7 * sstr2);
  sstr1 = -sstr1;
  sstr2 = -sstr2;

  __asm__ __volatile__ ("\n"
    "  pushl %%ebx\n\t"
    "1:\n"
    "  movq (%[src1]), %%mm7             \n\t"
    "  psadbw (%[src2]), %%mm7           \n\t"
    "   movq (%[src1],%[sstr1],1), %%mm1          \n\t"
    "   psadbw (%[src2],%[sstr2],1), %%mm1      \n\t"
    "   paddw %%mm1, %%mm7           \n\t"
    "  movq (%[src1],%[sstr1],2), %%mm0          \n\t"
    "  psadbw (%[src2],%[sstr2],2), %%mm0      \n\t"
    "  paddw %%mm0, %%mm7           \n\t"
    "   movq (%[src1],%[sstr1],4), %%mm1          \n\t"
    "   psadbw (%[src2],%[sstr2],4), %%mm1      \n\t"
    "   paddw %%mm1, %%mm7           \n\t"
    "  movd %%mm7, %%ebx\n\t"
    "  addl %%ebx, 0(%[dest])\n\t"
    "  subl %[sstr2],%[src2]\n\t"
    "  addl $4, %[dest]\n\t"
    "  decl %[n]\n\t"
    "  jnz 1b\n\t"
    "  popl %%ebx\n\t"

    "  emms                         \n\t"
     : [src1] "+r" (src1), 
       [src2] "+r" (src2),
       [dest] "+r" (dest),
       [n] "+m" (n)
     : [sstr1] "r" (sstr1),
       [sstr2] "r" (sstr2)
     : "memory"
  );
}
OIL_DEFINE_IMPL_FULL (sad8x8_8xn_u8_psadbw, sad8x8_8xn_u8, OIL_IMPL_FLAG_MMX | OIL_IMPL_FLAG_MMXEXT);


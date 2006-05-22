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

OIL_DECLARE_CLASS (sad8x8_u8);

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

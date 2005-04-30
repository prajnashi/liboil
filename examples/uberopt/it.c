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
#include <math.h>

OIL_DECLARE_CLASS(trans8x8_u16);

UBER_START
static void
trans8x8_u16_mmx_UBER_INDEX (uint16_t *dest, int dstr, uint16_t *src, int sstr)
{
  asm volatile (
      "  leal (%3,%3,2),%%eax \n"         // UBER 0:
      "  movq (%1), %%mm0 \n"             // UBER 1:
      "  movq (%1,%3,2), %%mm2 \n"        // UBER 2: 
      "  movq %%mm0, %%mm4 \n"            // UBER 3: 1
      "  movq %%mm2, %%mm5 \n"            // UBER 4: 2
      "  punpcklwd (%1,%3), %%mm0 \n"     // UBER 5: 1
      "  punpcklwd (%1,%%eax), %%mm2 \n"  // UBER 6: 0 2
      "  punpckhwd (%1,%3), %%mm4 \n"     // UBER 7: 3
      "  punpckhwd (%1,%%eax), %%mm5 \n"  // UBER 8: 0 4
      "  movq %%mm0, %%mm1 \n"            // UBER 9: 5
      "  movq %%mm4, %%mm3 \n"            // UBER 10: 7
      "  punpckldq %%mm2, %%mm0 \n"       // UBER 11: 5 6
      "  punpckldq %%mm5, %%mm4 \n"       // UBER 12: 7 8
      "  punpckhdq %%mm2, %%mm1 \n"       // UBER 13: 6 9
      "  punpckhdq %%mm5, %%mm3 \n"       // UBER 14: 9 10
      "  leal (%2,%2,2),%%eax \n"         // UBER 15: 6 8
      "  movq %%mm0, 0(%0) \n"            // UBER 16: 11
      "  movq %%mm1, (%0,%2) \n"          // UBER 17: 13
      "  movq %%mm4, (%0,%2,2) \n"        // UBER 18: 12
      "  movq %%mm3, (%0,%%eax) \n"       // UBER 19: 14 15

      "  leal (%3,%3,2),%%eax \n"
      "  movq 8(%1), %%mm0 \n"
      "  movq 8(%1,%3,2), %%mm2 \n"
      "  movq %%mm0, %%mm4 \n"
      "  movq %%mm2, %%mm5 \n"
      "  punpcklwd 8(%1,%3), %%mm0 \n"
      "  punpcklwd 8(%1,%%eax), %%mm2 \n"
      "  punpckhwd 8(%1,%3), %%mm4 \n"
      "  punpckhwd 8(%1,%%eax), %%mm5 \n"
      "  movq %%mm0, %%mm1 \n"
      "  movq %%mm4, %%mm3 \n"
      "  punpckldq %%mm2, %%mm0 \n"
      "  punpckldq %%mm5, %%mm4 \n"
      "  punpckhdq %%mm2, %%mm1 \n"
      "  punpckhdq %%mm5, %%mm3 \n"
      "  leal (%2,%2,2),%%eax \n"
      "  leal (%0,%2,4),%0 \n"
      "  movq %%mm0, 0(%0) \n"
      "  movq %%mm1, (%0,%2) \n"
      "  movq %%mm4, (%0,%2,2) \n"
      "  movq %%mm3, (%0,%%eax) \n"

      "  leal (%1,%3,4),%1 \n"
      "  leal (%3,%3,2),%%eax \n"
      "  movq 0(%1), %%mm0 \n"
      "  movq 0(%1,%3,2), %%mm2 \n"
      "  movq %%mm0, %%mm4 \n"
      "  movq %%mm2, %%mm5 \n"
      "  punpcklwd 0(%1,%3), %%mm0 \n"
      "  punpcklwd 0(%1,%%eax), %%mm2 \n"
      "  punpckhwd 0(%1,%3), %%mm4 \n"
      "  punpckhwd 0(%1,%%eax), %%mm5 \n"
      "  movq %%mm0, %%mm1 \n"
      "  movq %%mm4, %%mm3 \n"
      "  punpckldq %%mm2, %%mm0 \n"
      "  punpckldq %%mm5, %%mm4 \n"
      "  punpckhdq %%mm2, %%mm1 \n"
      "  punpckhdq %%mm5, %%mm3 \n"
      "  leal (%2,%2,2),%%eax \n"
      "  neg %2 \n"
      "  leal (%0,%2,4),%0 \n"
      "  neg %2 \n"
      "  movq %%mm0, 8(%0) \n"
      "  movq %%mm1, 8(%0,%2) \n"
      "  movq %%mm4, 8(%0,%2,2) \n"
      "  movq %%mm3, 8(%0,%%eax) \n"

      "  leal (%3,%3,2),%%eax \n"
      "  movq 8(%1), %%mm0 \n"
      "  movq 8(%1,%3,2), %%mm2 \n"
      "  movq %%mm0, %%mm4 \n"
      "  movq %%mm2, %%mm5 \n"
      "  punpcklwd 8(%1,%3), %%mm0 \n"
      "  punpcklwd 8(%1,%%eax), %%mm2 \n"
      "  punpckhwd 8(%1,%3), %%mm4 \n"
      "  punpckhwd 8(%1,%%eax), %%mm5 \n"
      "  movq %%mm0, %%mm1 \n"
      "  movq %%mm4, %%mm3 \n"
      "  punpckldq %%mm2, %%mm0 \n"
      "  punpckldq %%mm5, %%mm4 \n"
      "  punpckhdq %%mm2, %%mm1 \n"
      "  punpckhdq %%mm5, %%mm3 \n"
      "  leal (%2,%2,2),%%eax \n"
      "  leal (%0,%2,4),%0 \n"
      "  movq %%mm0, 8(%0) \n"
      "  movq %%mm1, 8(%0,%2) \n"
      "  movq %%mm4, 8(%0,%2,2) \n"
      "  movq %%mm3, 8(%0,%%eax) \n"

      "  emms \n"
      : "+r" (dest), "+r" (src), "+r" (dstr), "+r" (sstr)
      :
      : "eax");

}
UBER_END

void
register_impls(void)
{
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_0, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_1, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_2, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_3, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_4, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_5, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_6, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_7, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_8, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_9, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_10, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_11, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_12, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_13, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_14, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_15, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_16, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_17, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_18, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_19, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_20, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_21, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_22, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_23, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_24, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_25, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_26, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_27, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_28, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_29, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_30, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_31, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_32, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_33, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_34, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_35, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_36, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_37, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_38, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_39, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_40, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_41, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_42, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_43, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_44, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_45, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_46, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_47, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_48, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_49, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_50, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_51, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_52, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_53, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_54, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_55, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_56, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_57, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_58, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_59, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_60, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_61, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_62, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_63, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_64, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_65, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_66, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_67, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_68, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_69, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_70, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_71, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_72, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_73, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_74, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_75, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_76, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_77, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_78, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_79, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_80, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_81, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_82, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_83, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_84, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_85, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_86, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_87, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_88, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_89, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_90, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_91, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_92, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_93, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_94, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_95, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_96, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_97, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_98, trans8x8_u16, 0);
  OIL_REGISTER_IMPL_FULL (trans8x8_u16_mmx_99, trans8x8_u16, 0);
}



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


static void
mult8x8_s16_mmx(int16_t *dest, int16_t *src1, int16_t *src2, int dstr, int sstr1,
    int sstr2)
{

        asm volatile(
            "  movq 0(%1), %%mm0 \n"
            "  pmullw 0(%2), %%mm0 \n"
            "  movq %%mm0, 0(%0) \n"
            "  movq 8(%1), %%mm1 \n"
            "  pmullw 8(%2), %%mm1 \n"
            "  movq %%mm1, 8(%0) \n"

            "  add %3, %0 \n"
            "  add %4, %1 \n"
            "  add %5, %2 \n"

            "  movq 0(%1), %%mm0 \n"
            "  pmullw 0(%2), %%mm0 \n"
            "  movq %%mm0, 0(%0) \n"
            "  movq 8(%1), %%mm1 \n"
            "  pmullw 8(%2), %%mm1 \n"
            "  movq %%mm1, 8(%0) \n"

            "  add %3, %0 \n"
            "  add %4, %1 \n"
            "  add %5, %2 \n"

            "  movq 0(%1), %%mm0 \n"
            "  pmullw 0(%2), %%mm0 \n"
            "  movq %%mm0, 0(%0) \n"
            "  movq 8(%1), %%mm1 \n"
            "  pmullw 8(%2), %%mm1 \n"
            "  movq %%mm1, 8(%0) \n"

            "  add %3, %0 \n"
            "  add %4, %1 \n"
            "  add %5, %2 \n"

            "  movq 0(%1), %%mm0 \n"
            "  pmullw 0(%2), %%mm0 \n"
            "  movq %%mm0, 0(%0) \n"
            "  movq 8(%1), %%mm1 \n"
            "  pmullw 8(%2), %%mm1 \n"
            "  movq %%mm1, 8(%0) \n"

            "  add %3, %0 \n"
            "  add %4, %1 \n"
            "  add %5, %2 \n"

            "  movq 0(%1), %%mm0 \n"
            "  pmullw 0(%2), %%mm0 \n"
            "  movq %%mm0, 0(%0) \n"
            "  movq 8(%1), %%mm1 \n"
            "  pmullw 8(%2), %%mm1 \n"
            "  movq %%mm1, 8(%0) \n"

            "  add %3, %0 \n"
            "  add %4, %1 \n"
            "  add %5, %2 \n"

            "  movq 0(%1), %%mm0 \n"
            "  pmullw 0(%2), %%mm0 \n"
            "  movq %%mm0, 0(%0) \n"
            "  movq 8(%1), %%mm1 \n"
            "  pmullw 8(%2), %%mm1 \n"
            "  movq %%mm1, 8(%0) \n"

            "  add %3, %0 \n"
            "  add %4, %1 \n"
            "  add %5, %2 \n"

            "  movq 0(%1), %%mm0 \n"
            "  pmullw 0(%2), %%mm0 \n"
            "  movq %%mm0, 0(%0) \n"
            "  movq 8(%1), %%mm1 \n"
            "  pmullw 8(%2), %%mm1 \n"
            "  movq %%mm1, 8(%0) \n"

            "  add %3, %0 \n"
            "  add %4, %1 \n"
            "  add %5, %2 \n"

            "  movq 0(%1), %%mm0 \n"
            "  pmullw 0(%2), %%mm0 \n"
            "  movq %%mm0, 0(%0) \n"
            "  movq 8(%1), %%mm1 \n"
            "  pmullw 8(%2), %%mm1 \n"
            "  movq %%mm1, 8(%0) \n"
            "  emms\n "

            : "+r" (dest), "+r" (src1), "+r" (src2)
            : "m" (dstr), "m" (sstr1), "m" (sstr2));
}

OIL_DEFINE_IMPL_FULL (mult8x8_s16_mmx, mult8x8_s16, OIL_IMPL_REQUIRES_MMX);


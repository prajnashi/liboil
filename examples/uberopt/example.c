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


UBER_START
static void
abs_u16_s16_i386asm_UBER_INDEX (uint16_t * dest, int dstr, int16_t * src,
    int sstr, int n)
{
  __asm__ __volatile__ ("\n"
      "	.p2align 4,,15			\n"
      "1:                               \n"
      "    movswl	(%0), %%eax	\n" /* UBER 0:     */
      "    addl	$2, %0			\n" /* UBER 1: 0   */
      "    movl	%%eax, %%edx		\n" /* UBER 2: 0   */
      "    negl	%%edx			\n" /* UBER 3: 2   */
      "    cmpl	$-1, %%eax ; cmovle %%edx, %%eax \n" /* UBER 4: 3 */
      "    movw	%%ax, (%1)		\n" /* UBER 5: 4   */
      "    addl	$2, %1			\n" /* UBER 6: 5   */
      "    decl	%2			\n" /* UBER 7:     */
      "    testl	%2, %2		\n"
      "    jg	1b			\n"
      :"+r" (src), "+r" (dest), "+r" (n)
      ::"eax", "edx");
}
OIL_DEFINE_IMPL_ASM (abs_u16_s16_i386asm_UBER_INDEX, abs_u16_s16);
UBER_END


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


#if 0
static void
sad8x8_s16_a16_altivec (uint32_t *dest, int16_t *src1, int16_t *src2, int s1str, int s2str)
{
	static uint32_t x[4] __attribute__ ((__aligned__ (16)));

	sl_altivec_load8_0(src1, s1str);
	sl_altivec_load8_8(src2, s2str);

	__asm__ __volatile__(
		"\n"
		"\tvspltisw 19, 0\n"
		"\tvspltisw 22, 0\n"

		"\tvminsh 16, 0, 8\n"
		"\tvmaxsh 17, 0, 8\n"
		"\tvsubuhm 18, 17, 16\n"
		"\tvmrghh 20, 19, 18\n"
		"\tvmrglh 21, 19, 18\n"
		"\tvsumsws 22, 20, 22\n"
		"\tvsumsws 22, 21, 22\n"

		"\tvminsh 16, 1, 9\n"
		"\tvmaxsh 17, 1, 9\n"
		"\tvsubuhm 18, 17, 16\n"
		"\tvmrghh 20, 19, 18\n"
		"\tvmrglh 21, 19, 18\n"
		"\tvsumsws 22, 20, 22\n"
		"\tvsumsws 22, 21, 22\n"

		"\tvminsh 16, 2, 10\n"
		"\tvmaxsh 17, 2, 10\n"
		"\tvsubuhm 18, 17, 16\n"
		"\tvmrghh 20, 19, 18\n"
		"\tvmrglh 21, 19, 18\n"
		"\tvsumsws 22, 20, 22\n"
		"\tvsumsws 22, 21, 22\n"

		"\tvminsh 16, 3, 11\n"
		"\tvmaxsh 17, 3, 11\n"
		"\tvsubuhm 18, 17, 16\n"
		"\tvmrghh 20, 19, 18\n"
		"\tvmrglh 21, 19, 18\n"
		"\tvsumsws 22, 20, 22\n"
		"\tvsumsws 22, 21, 22\n"

		"\tvminsh 16, 4, 12\n"
		"\tvmaxsh 17, 4, 12\n"
		"\tvsubuhm 18, 17, 16\n"
		"\tvmrghh 20, 19, 18\n"
		"\tvmrglh 21, 19, 18\n"
		"\tvsumsws 22, 20, 22\n"
		"\tvsumsws 22, 21, 22\n"

		"\tvminsh 16, 5, 13\n"
		"\tvmaxsh 17, 5, 13\n"
		"\tvsubuhm 18, 17, 16\n"
		"\tvmrghh 20, 19, 18\n"
		"\tvmrglh 21, 19, 18\n"
		"\tvsumsws 22, 20, 22\n"
		"\tvsumsws 22, 21, 22\n"

		"\tvminsh 16, 6, 14\n"
		"\tvmaxsh 17, 6, 14\n"
		"\tvsubuhm 18, 17, 16\n"
		"\tvmrghh 20, 19, 18\n"
		"\tvmrglh 21, 19, 18\n"
		"\tvsumsws 22, 20, 22\n"
		"\tvsumsws 22, 21, 22\n"

		"\tvminsh 16, 7, 15\n"
		"\tvmaxsh 17, 7, 15\n"
		"\tvsubuhm 18, 17, 16\n"
		"\tvmrghh 20, 19, 18\n"
		"\tvmrglh 21, 19, 18\n"
		"\tvsumsws 22, 20, 22\n"
		"\tvsumsws 22, 21, 22\n"

		"\tli 0, 0\n"
		"\tstvx 22, %0, 0\n"
	:
	: "r" (x)
	);
	*dest = x[3];
}

static void
sad8x8_s16_l15_a16_altivec(uint32_t *dest, int16_t *src1, int16_t *src2, int s1str, int s2str)
{
	static uint32_t x[4] __attribute__ ((__aligned__ (16)));

	sl_altivec_load8_0(src1, s1str);
	sl_altivec_load8_8(src2, s2str);

	__asm__ __volatile__(
		"\n"
		"\tvspltisw 19, 0\n"
		"\tvspltisw 22, 0\n"

		"\tvminsh 16, 0, 8\n"
		"\tvmaxsh 17, 0, 8\n"
		"\tvsubuhm 18, 17, 16\n"
		"\tvsum4shs 22, 18, 22\n"

		"\tvminsh 16, 1, 9\n"
		"\tvmaxsh 17, 1, 9\n"
		"\tvsubuhm 18, 17, 16\n"
		"\tvsum4shs 22, 18, 22\n"

		"\tvminsh 16, 2, 10\n"
		"\tvmaxsh 17, 2, 10\n"
		"\tvsubuhm 18, 17, 16\n"
		"\tvsum4shs 22, 18, 22\n"

		"\tvminsh 16, 3, 11\n"
		"\tvmaxsh 17, 3, 11\n"
		"\tvsubuhm 18, 17, 16\n"
		"\tvsum4shs 22, 18, 22\n"

		"\tvminsh 16, 4, 12\n"
		"\tvmaxsh 17, 4, 12\n"
		"\tvsubuhm 18, 17, 16\n"
		"\tvsum4shs 22, 18, 22\n"

		"\tvminsh 16, 5, 13\n"
		"\tvmaxsh 17, 5, 13\n"
		"\tvsubuhm 18, 17, 16\n"
		"\tvsum4shs 22, 18, 22\n"

		"\tvminsh 16, 6, 14\n"
		"\tvmaxsh 17, 6, 14\n"
		"\tvsubuhm 18, 17, 16\n"
		"\tvsum4shs 22, 18, 22\n"

		"\tvminsh 16, 7, 15\n"
		"\tvmaxsh 17, 7, 15\n"
		"\tvsubuhm 18, 17, 16\n"
		"\tvsum4shs 22, 18, 22\n"

		"\tvsumsws 22, 22, 19\n"

		"\tli 0, 0\n"
		"\tstvx 22, %0, 0\n"
	:
	: "r" (x)
	);
	*dest = x[3];
}
#endif



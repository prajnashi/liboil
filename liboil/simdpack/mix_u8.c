
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

OIL_DEFINE_CLASS_X (mix_u8,
    "uint8_t *dest, uint8_t *src1, uint8_t *src2, uint8_t *param, int n");

static void mix_u8_ref(uint8_t *dest, uint8_t *src1, uint8_t *src2, uint8_t *scale, int n)
{
	int i;

	for(i=0;i<n;i++){
		dest[i] = (src1[i]*scale[i] + src2[i]*(255-scale[i]))>>8;
	}
}

OIL_DEFINE_IMPL_REF (mix_u8_ref, mix_u8_class);

static void mix_u8_fast(uint8_t *dest, uint8_t *src1, uint8_t *src2, uint8_t *scale, int n)
{
	while(n>0){
		*dest++ = (*src1 * *scale + *src2 * (255 - *scale))>>8;
		src1++;
		src2++;
		scale++;
		n--;
	}
}
OIL_DEFINE_IMPL (mix_u8_fast, mix_u8_class);

static void mix_u8_fast2 (uint8_t *dest, uint8_t *src1, uint8_t *src2, uint8_t *scale, int n)
{
	while(n>0){
		*dest++ = ((*src1 - *src2) * *scale + *src2 * 255)>>8;
		src1++;
		src2++;
		scale++;
		n--;
	}
}
OIL_DEFINE_IMPL (mix_u8_fast2, mix_u8_class);

/* PowerPC likes this one */
static void
mix_u8_fast3(uint8_t *dest, uint8_t *src1, uint8_t *src2, uint8_t *scale, int n)
{
	int i;

	for(i=0;i<n;i++){
		dest[i] = ((src1[i] - src2[i]) * scale[i] + src2[i] * 255)>>8;
	}
}
OIL_DEFINE_IMPL (mix_u8_fast3, mix_u8_class);

static void
mix_u8_unroll4(uint8_t *dest, uint8_t *src1, uint8_t *src2, uint8_t *scale, int n)
{
	while(n&0x3){
		*dest++ = (*src1 * *scale + *src2 * (255 - *scale))>>8;
		src1++;
		src2++;
		scale++;
		n--;
	}
	while(n>0){
		*dest++ = (*src1 * *scale + *src2 * (255 - *scale))>>8;
		src1++;
		src2++;
		scale++;
		*dest++ = (*src1 * *scale + *src2 * (255 - *scale))>>8;
		src1++;
		src2++;
		scale++;
		*dest++ = (*src1 * *scale + *src2 * (255 - *scale))>>8;
		src1++;
		src2++;
		scale++;
		*dest++ = (*src1 * *scale + *src2 * (255 - *scale))>>8;
		src1++;
		src2++;
		scale++;
		n-=4;
	}
}
OIL_DEFINE_IMPL (mix_u8_unroll4, mix_u8_class);

#ifdef SIMDPACK_USE_ALTIVEC
static void mix_u8_a16_altivec(uint8_t *dest, uint8_t *src1, uint8_t *src2, uint8_t *scale, int n)
{
	static const uint8_t permute[] __attribute__ ((__aligned__ (16))) = {
		0, 2, 4, 6, 8,10,12,14,16,18,20,22,24,26,28,30 };
	const uint8_t *perm = permute;
	int i;

	for(i=(n&~0xf);i<n;i++){
		dest[i] = (src1[i]*scale[i] + src2[i]*(255-scale[i]))>>8;
	}
	__asm__ __volatile__("\n"
		"	li	%%r12, 0		\n"
		"	vxor	%%v3, %%v3, %%v3	\n"
		"	lvx	%%v12, %4, %%r12	\n"
		"	mtctr	%5			\n"
		"1:	lvx	%%v0, %1, %%r12		\n"
		"	lvx	%%v1, %2, %%r12		\n"
		"	lvx	%%v2, %3, %%r12		\n"

		"	vmrghb	%%v4, %%v3, %%v0	\n"
		"	vmrghb	%%v5, %%v3, %%v1	\n"
		"	vmrghb	%%v6, %%v3, %%v2	\n"
		"	vmrghb	%%v7, %%v1, %%v3	\n"
		"	vsubuhs	%%v8, %%v7, %%v5	\n"
		"	vsubshs	%%v9, %%v4, %%v5	\n"
		"	vmladduhm	%%v10, %%v9, %%v6, %%v8	\n"

		"	vmrglb	%%v4, %%v3, %%v0	\n"
		"	vmrglb	%%v5, %%v3, %%v1	\n"
		"	vmrglb	%%v6, %%v3, %%v2	\n"
		"	vmrglb	%%v7, %%v1, %%v3	\n"
		"	vsubuhs	%%v8, %%v7, %%v5	\n"
		"	vsubshs	%%v9, %%v4, %%v5	\n"
		"	vmladduhm	%%v11, %%v9, %%v6, %%v8	\n"

		"	vperm	%%v0, %%v10, %%v11, %%v12	\n"
		"	stvx	%%v0, %0, %%r12		\n"
		"	addi	%%r12, %%r12, 16	\n"
		"	bdnz	1b			\n"
	: "+b" (dest), "+b" (src1), "+b" (src2), "+b" (scale), "+b" (perm)
	: "r" (n/16)
	: "12", "ctr");
}
OIL_DEFINE_IMPL (mix_u8_a16_altivec, mix_u8_class);
#endif


#ifdef TEST_mix_u8
int TEST_mix_u8(void)
{
	int i;
	int pass;
	int failures = 0;
	uint8_t *src1, *src2, *scale;
	uint8_t *dest_test, *dest_ref;
	struct sl_profile_struct t;

	src1 = sl_malloc_u8_a16(N);
	src2 = sl_malloc_u8_a16(N);
	scale = sl_malloc_u8_a16(N);
	dest_ref = sl_malloc_u8_a16(N);
	dest_test = sl_malloc_u8_a16(N);

	sl_profile_init(t);
	srand(20021003);

	printf("I: " sl_stringify(mix_u8_FUNC) "\n");

	for(pass=0;pass<N_PASS;pass++){
		for(i=0;i<N;i++){
			src1[i] = sl_rand_u8();
			src2[i] = sl_rand_u8();
			scale[i] = sl_rand_u8();
		}

		mix_u8_ref(dest_ref,src1,src2,scale,N);
		sl_profile_start(t);
		mix_u8_FUNC(dest_test,src1,src2,scale,N);
		sl_profile_stop(t);

		for(i=0;i<N;i++){
			if(dest_test[i] != dest_ref[i]){
				printf("%d %d %d %d %d %d\n",i,src1[i],src2[i],scale[i],
					dest_ref[i],
					dest_test[i]);
				failures++;
			}
		}
	}

	sl_free(src1);
	sl_free(src2);
	sl_free(scale);
	sl_free(dest_ref);
	sl_free(dest_test);

	if(failures){
		printf("E: %d failures\n",failures);
	}

	sl_profile_print(t);

	return failures;
}
#endif



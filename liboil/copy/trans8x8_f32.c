/* transpose 8x8 block
 * Copyright (C) 2001,2002  David A. Schleef <ds@schleef.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */


/*
Kernel: trans8x8_f32
Description: transpose 8x8 block

XXX
*/

#ifndef _trans8x8_f32_h_
#define _trans8x8_f32_h_

#include <math.h>

#include <sl_types.h>
#include <sl_block8x8.h>
#include <sl_altivec.h>

/* storage class */
#ifndef SL_trans8x8_f32_storage
 #ifdef SL_storage
  #define SL_trans8x8_f32_storage SL_storage
 #else
  #define SL_trans8x8_f32_storage static inline
 #endif
#endif


/* IMPL trans8x8_f32_ref */
SL_trans8x8_f32_storage
void trans8x8_f32_ref(f32 *dest, f32 *src, int dstr, int sstr)
{
	int i,j;

	for(i=0;i<8;i++){
		for(j=0;j<8;j++){
			block8x8_f32(dest,dstr,i,j) =
				block8x8_f32(src,sstr,j,i);
		}
	}
}


#ifdef SIMDPACK_USE_ALTIVEC
/* should be moved */
SL_trans8x8_f32_storage
void trans4x4_f32_a16_altivec(f32 *dest, f32 *src, int dstr, int sstr)
{
	/* load vectors */
	sl_altivec_load4_0(src,sstr);

	/* transpose 4x4 */
	__asm__ __volatile__(
		"vmrghw 4, 0, 2\n"
		"\tvmrglw 5, 0, 2\n"
		"\tvmrghw 6, 1, 3\n"
		"\tvmrglw 7, 1, 3\n"
		/* 00 20 01 21
		 * 02 22 03 23
		 * 10 30 11 31
		 * 12 32 13 33 */
		"\tvmrghw 0, 4, 6\n"
		"\tvmrglw 1, 4, 6\n"
		"\tvmrghw 2, 5, 7\n"
		"\tvmrglw 3, 5, 7\n"
	);

	/* store vectors */
	sl_altivec_store4_0(dest,dstr);
}

#define f32_addr(a,b,x) ((f32 *)((void *)(a) + (b)*(x)))

/* IMPL trans8x8_f32_a16_altivec defined(SIMDPACK_USE_ALTIVEC) */
SL_trans8x8_f32_storage
void trans8x8_f32_a16_altivec(f32 *dest, f32 *src, int dstr, int sstr)
{
	trans4x4_f32_a16_altivec(dest,src,dstr,sstr);
	trans4x4_f32_a16_altivec(dest+4,f32_addr(src,sstr,4),dstr,sstr);
	trans4x4_f32_a16_altivec(f32_addr(dest,dstr,4),src+4,dstr,sstr);
	trans4x4_f32_a16_altivec(f32_addr(dest,dstr,4)+4,
			f32_addr(src,sstr,4)+4,dstr,sstr);
}
#endif

#endif


#ifdef TEST_trans8x8_f32
int TEST_trans8x8_f32(void)
{
	int i;
	int pass;
	int failures = 0;
	f32 *src, *dest_ref, *dest_test;
	struct sl_profile_struct t;

#ifdef ALIGNED16
	src = sl_malloc_f32_a16(64);
	dest_ref = sl_malloc_f32_a16(64);
	dest_test = sl_malloc_f32_a16(64);
#else
	src = sl_malloc_f32(64);
	dest_ref = sl_malloc_f32(64);
	dest_test = sl_malloc_f32(64);
#endif
	
	sl_profile_init(t);
	srand(20020326);

	printf("I: " sl_stringify(trans8x8_f32_FUNC) "\n");

	for(pass=0;pass<N_PASS;pass++){
		for(i=0;i<64;i++)src[i] = sl_rand_f32_0_1();

		trans8x8_f32_ref(dest_test, src, 8*sizeof(f32), 8*sizeof(f32));
		sl_profile_start(t);
		trans8x8_f32_FUNC(dest_ref, src, 8*sizeof(f32), 8*sizeof(f32));
		sl_profile_stop(t);

		for(i=0;i<64;i++){
			if(dest_test[i]!=dest_ref[i]){
				failures++;
			}
		}
	}

	sl_free(src);
	sl_free(dest_ref);
	sl_free(dest_test);

	if(failures){
		printf("E: %d failures\n",failures);
	}

	sl_profile_print(t);

	return failures;
}
#endif


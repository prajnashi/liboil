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
Kernel: trans8x8_s16
Description: transpose 8x8 block

XXX
*/

#ifndef _trans8x8_s16_h_
#define _trans8x8_s16_h_

#include <math.h>

#include <sl_types.h>
#include <sl_block8x8.h>
#include <sl_altivec.h>

/* storage class */
#ifndef SL_trans8x8_s16_storage
 #ifdef SL_storage
  #define SL_trans8x8_s16_storage SL_storage
 #else
  #define SL_trans8x8_s16_storage static inline
 #endif
#endif


/* IMPL trans8x8_s16_ref */
SL_trans8x8_s16_storage
void trans8x8_s16_ref(s16 *dest, s16 *src, int dstr, int sstr)
{
	int i,j;

	for(i=0;i<8;i++){
		for(j=0;j<8;j++){
			block8x8_s16(dest,dstr,i,j) =
				block8x8_s16(src,sstr,j,i);
		}
	}
}


#ifdef SIMDPACK_USE_ALTIVEC
/* IMPL trans8x8_s16_a16_altivec defined(SIMDPACK_USE_ALTIVEC) */
SL_trans8x8_s16_storage
void trans8x8_s16_a16_altivec(s16 *dest, s16 *src, int dstr, int sstr)
{
	sl_altivec_load8_0(src,sstr);

	__asm__ __volatile__(
		"vmrghh    8, 0, 4\n"
		"\tvmrglh  9, 0, 4\n"
		"\tvmrghh 10, 1, 5\n"
		"\tvmrglh 11, 1, 5\n"
		"\tvmrghh 12, 2, 6\n"
		"\tvmrglh 13, 2, 6\n"
		"\tvmrghh 14, 3, 7\n"
		"\tvmrglh 15, 3, 7\n"

		"\tvmrghh 16,  8, 12\n"
		"\tvmrglh 17,  8, 12\n"
		"\tvmrghh 18,  9, 13\n"
		"\tvmrglh 19,  9, 13\n"
		"\tvmrghh 20, 10, 14\n"
		"\tvmrglh 21, 10, 14\n"
		"\tvmrghh 22, 11, 15\n"
		"\tvmrglh 23, 11, 15\n"

		"\tvmrghh  0, 16, 20\n"
		"\tvmrglh  1, 16, 20\n"
		"\tvmrghh  2, 17, 21\n"
		"\tvmrglh  3, 17, 21\n"
		"\tvmrghh  4, 18, 22\n"
		"\tvmrglh  5, 18, 22\n"
		"\tvmrghh  6, 19, 23\n"
		"\tvmrglh  7, 19, 23\n"
	);

	sl_altivec_store8_0(dest,dstr);
}
#endif

#ifdef SIMDPACK_USE_ALTIVEC
/* IMPL trans8x8_s16_altivecwrap defined(SIMDPACK_USE_ALTIVEC) */
SL_trans8x8_s16_storage
void trans8x8_s16_altivecwrap(s16 *dest, s16 *src, int dstr, int sstr)
{
	static s16 s[64] __attribute__ ((__aligned__ (16)));
	static s16 d[64] __attribute__ ((__aligned__ (16)));
	int i,j;

	for(i=0;i<8;i++){
		for(j=0;j<8;j++){
			block8x8_s16(s,16,i,j) = block8x8_s16(src,sstr,i,j);
		}       
	}       

	trans8x8_s16_a16_altivec(d,s,16,16);

	for(i=0;i<8;i++){
		for(j=0;j<8;j++){
			block8x8_s16(dest,dstr,i,j) = block8x8_s16(d,16,i,j);
		}       
	}

}
#endif

#endif

#ifdef TEST_trans8x8_s16
int TEST_trans8x8_s16(void)
{
	int i;
	int pass;
	int failures = 0;
	s16 *src, *dest_ref, *dest_test;
	struct sl_profile_struct t;

#ifdef ALIGNED16
	src = sl_malloc_s16_a16(64);
	dest_ref = sl_malloc_s16_a16(64);
	dest_test = sl_malloc_s16_a16(64);
#else
	src = sl_malloc_s16(64);
	dest_ref = sl_malloc_s16(64);
	dest_test = sl_malloc_s16(64);
#endif
	
	sl_profile_init(t);
	srand(20020326);

	printf("I: " sl_stringify(trans8x8_s16_FUNC) "\n");

	for(pass=0;pass<N_PASS;pass++){
		for(i=0;i<64;i++)src[i] = sl_rand_s16();

		trans8x8_s16_ref(dest_test, src, 16, 16);
		sl_profile_start(t);
		trans8x8_s16_FUNC(dest_ref, src, 16, 16);
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


/* convert block8x8 f64 to s16 functions
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
Kernel: conv8x8_f64_s16
Description: round source value to nearest integer and convert to s16.

Result is undefined if src<-32768 or src>32767.
Half integers may be rounded to either nearby integer.
*/

#ifndef _conv8x8_f64_s16_h_
#define _conv8x8_f64_s16_h_

#include <math.h>
#include <sl_types.h>

#include <sl_block8x8.h>


/* storage class */
#ifndef SL_conv8x8_f64_s16_storage
 #ifdef SL_storage
  #define SL_conv8x8_f64_s16_storage SL_storage
 #else
  #define SL_conv8x8_f64_s16_storage static inline
 #endif
#endif

/* IMPL conv8x8_f64_s16_ref */
SL_conv8x8_f64_s16_storage
void conv8x8_f64_s16_ref(s16 *dest, f64 *src, int dstr, int sstr)
{
	int i,j;

	for(i=0;i<8;i++){
		for(j=0;j<8;j++){
			block8x8_s16(dest,dstr,i,j) =
				floor(block8x8_f64(src,sstr,i,j)+0.5);
		}
	}
}

#include <conv_f64_s16.h>
/* IMPL conv8x8_f64_s16_fast */
SL_conv8x8_f64_s16_storage
void conv8x8_f64_s16_fast(s16 *dest, f64 *src, int dstr, int sstr)
{
	int i;

	for(i=0;i<8;i++){
		conv_f64_s16((s16 *)((void *)dest + dstr*i),
			(f64 *)((void *)src + sstr*i), 8);
	}
}
#endif

#ifdef TEST_conv8x8_f64_s16
int TEST_conv8x8_f64_s16(void)
{
	int i;
	int pass;
	int failures = 0;
	f64 *src;
	s16 *dest_test, *dest_ref;
	struct sl_profile_struct t;

	src = sl_malloc_f64(64);
	dest_ref = sl_malloc_s16(64);
	dest_test = sl_malloc_s16(64);

	sl_profile_init(t);
	srand(20020330);

	printf("I: " sl_stringify(conv8x8_f64_s16_FUNC) "\n");

	for(pass=0;pass<N_PASS;pass++){
		for(i=0;i<64;i++){
			src[i] = sl_rand_f64_s16();
		}
		conv8x8_f64_s16_ref(dest_ref,src,8*sizeof(s16),8*sizeof(f64));
		sl_profile_start(t);
		conv8x8_f64_s16_FUNC(dest_test,src,8*sizeof(s16),8*sizeof(f64));	
		sl_profile_stop(t);

		for(i=0;i<64;i++){
			if(dest_test[i] != dest_ref[i]){
				printf("%d %g %d %d\n",i,src[i],dest_ref[i],
					dest_test[i]);
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


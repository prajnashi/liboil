/* scalarmult_f64.h
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
Kernel: scalarmult_f64
Description: scalarmult f64

XXX
*/

#ifndef _scalarmult_f64_h_
#define _scalarmult_f64_h_

#include <sl_types.h>

/* storage class */
#ifndef SL_scalarmult_f64_storage
 #ifdef SL_storage
  #define SL_scalarmult_f64_storage SL_storage
 #else
  #define SL_scalarmult_f64_storage static inline
 #endif
#endif


/* IMPL scalarmult_f64_ref */
SL_scalarmult_f64_storage void scalarmult_f64_ref(f64 *dest, f64 *src, int n, f64 mult)
{
	int i;

	for(i=0;i<n;i++){
		dest[i] = mult * src[i];
	}
}

/* IMPL scalarmult_f64_unroll4 */
SL_scalarmult_f64_storage void scalarmult_f64_unroll4(f64 *dest, f64 *src, int n, f64 mult)
{
	SL_UNROLL4(*dest++ = mult * *src++,n);
}


#endif /* _scalarmult_f64_h_ */

#ifdef TEST_scalarmult_f64
int TEST_scalarmult_f64(void)
{
	int i;
	int pass;
	int failures = 0;
	f64 *src, *dest_ref, *dest_test;
	struct sl_profile_struct t;
	double mult;

	src = sl_malloc_f64(N);
	dest_ref = sl_malloc_f64(N);
	dest_test = sl_malloc_f64(N);

	sl_profile_init(t);
	srand(20021001);

	printf("I: " sl_stringify(scalarmult_f64_FUNC) "\n");

	for(pass=0;pass<N_PASS;pass++){
		for(i=0;i<N;i++)src[i]=sl_rand_f64_s16();
		mult=sl_rand_f64_s16();

		scalarmult_f64_ref(dest_ref,src,N,mult);
		sl_profile_start(t);
		scalarmult_f64_FUNC(dest_test,src,N,mult);
		sl_profile_stop(t);

		for(i=0;i<N;i++){
			if(dest_test[i] != dest_ref[i]){
				printf("%d %g %g %g\n",i,src[i],dest_ref[i],
						dest_test[i]);
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


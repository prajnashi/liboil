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
#include <math.h>

#define TRANS8x8_DEFINE_REF(type)		\
static void trans8x8_ ## type ## _ref (		\
    type_ ## type *dest, int dstr,		\
    type_ ## type *src, int sstr)		\
{						\
  int i,j;					\
  for(i=0;i<8;i++){				\
    for(j=0;j<8;j++){				\
      OIL_GET(dest,dstr*i+j,type_ ## type) = OIL_GET(src,sstr*j+i,type_ ## type); \
    }						\
  }						\
}						\
OIL_DEFINE_CLASS (trans8x8_ ## type, "type_" #type " *dest, int dstr,	" \
    "type_" #type " *src, int sstr"); \
OIL_DEFINE_IMPL_REF (trans8x8_ ## type ## _ref, trans8x8_ ## type)

TRANS8x8_DEFINE_REF(u8);
TRANS8x8_DEFINE_REF(u16);
TRANS8x8_DEFINE_REF(u32);
TRANS8x8_DEFINE_REF(f64);


#ifdef TEST_trans8x8_f64
int TEST_trans8x8_f64(void)
{
	int i;
	int pass;
	int failures = 0;
	f64 *src, *dest_ref, *dest_test;
	struct sl_profile_struct t;

	src = sl_malloc_f64(64);
	dest_ref = sl_malloc_f64(64);
	dest_test = sl_malloc_f64(64);
	
	sl_profile_init(t);
	srand(20020326);

	printf("I: " sl_stringify(trans8x8_f64_FUNC) "\n");

	for(pass=0;pass<N_PASS;pass++){
		for(i=0;i<64;i++)src[i] = sl_rand_f64_0_1();

		trans8x8_f64_ref(dest_test, src, 64, 64);
		sl_profile_start(t);
		trans8x8_f64_FUNC(dest_ref, src, 64, 64);
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


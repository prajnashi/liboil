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

#define SCALARADD_DEFINE_REF(type)		\
static void scalaradd_ ## type ## _ref(		\
    type_ ## type *dest, int dstr,		\
    type_ ## type *src, int sstr,		\
    type_ ## type val, int n)			\
{						\
  int i;					\
  for(i=0;i<n;i++){				\
    OIL_GET(dest,dstr*i, type_ ## type) =       \
      OIL_GET(src,sstr*i, type_ ## type) + val; \
  }						\
}						\
OIL_DEFINE_CLASS (scalaradd_ ## type,         \
    "type_" #type " *dest, int dstr, "		\
    "type_" #type " *src, int sstr, "		\
    "type_" #type " param int n");		\
OIL_DEFINE_IMPL_REF (scalaradd_ ## type ## _ref, scalaradd_ ## type ## _class);


SCALARADD_DEFINE_REF (s8);
SCALARADD_DEFINE_REF (u8);
SCALARADD_DEFINE_REF (s16);
SCALARADD_DEFINE_REF (u16);
SCALARADD_DEFINE_REF (s32);
SCALARADD_DEFINE_REF (u32);
SCALARADD_DEFINE_REF (f32);
SCALARADD_DEFINE_REF (f64);

#define SCALARADD_DEFINE_UNROLL2(type)		\
static void scalaradd_ ## type ## _unroll2(	\
    type_ ## type *dest, int dstr,		\
    type_ ## type *src, int sstr,		\
    type_ ## type val, int n)			\
{						\
  if(n&1) {					\
    *dest = *src + val;				\
    OIL_INCREMENT(dest, dstr);			\
    OIL_INCREMENT(src, sstr);			\
  }						\
  n /= 2;					\
  while(n>0){					\
    *dest = *src + val;				\
    OIL_INCREMENT(dest,dstr);			\
    OIL_INCREMENT(src,sstr);			\
    n--;					\
  }						\
}						\
OIL_DEFINE_IMPL (scalaradd_ ## type ## _unroll2, scalaradd_ ## type ## _class);

SCALARADD_DEFINE_UNROLL2 (s8);
SCALARADD_DEFINE_UNROLL2 (u8);
SCALARADD_DEFINE_UNROLL2 (s16);
SCALARADD_DEFINE_UNROLL2 (u16);
SCALARADD_DEFINE_UNROLL2 (s32);
SCALARADD_DEFINE_UNROLL2 (u32);
SCALARADD_DEFINE_UNROLL2 (f32);
SCALARADD_DEFINE_UNROLL2 (f64);

#define SCALARADD_DEFINE_UNROLL2x(type)		\
static void scalaradd_ ## type ## _unroll2x(	\
    type_ ## type *dest, int dstr,		\
    type_ ## type *src, int sstr,		\
    type_ ## type val, int n)			\
{						\
  type_ ## type *dest2;				\
  type_ ## type *src2;				\
  int i;					\
  if(n&1) {					\
    *dest = *src + val;				\
    OIL_INCREMENT(dest, dstr);			\
    OIL_INCREMENT(src, sstr);			\
  }						\
  src2 = OIL_OFFSET(src, sstr);			\
  dest2 = OIL_OFFSET(dest, dstr);		\
  n /= 2;					\
  sstr *= 2;					\
  dstr *= 2;					\
  for(i=0;i<n;i++){				\
    OIL_GET(dest,dstr*i, type_ ## type) = OIL_GET(src,sstr*i, type_ ## type) + val; \
    OIL_GET(dest2,dstr*i, type_ ## type) = OIL_GET(src2,sstr*i, type_ ## type) + val; \
  }						\
}						\
OIL_DEFINE_IMPL (scalaradd_ ## type ## _unroll2x, scalaradd_ ## type ## _class);

SCALARADD_DEFINE_UNROLL2x (s8);
SCALARADD_DEFINE_UNROLL2x (u8);
SCALARADD_DEFINE_UNROLL2x (s16);
SCALARADD_DEFINE_UNROLL2x (u16);
SCALARADD_DEFINE_UNROLL2x (s32);
SCALARADD_DEFINE_UNROLL2x (u32);
SCALARADD_DEFINE_UNROLL2x (f32);
SCALARADD_DEFINE_UNROLL2x (f64);

#define SCALARADD_DEFINE_UNROLL4(type)		\
static void scalaradd_ ## type ## _unroll4(	\
    type_ ## type *dest, int dstr,		\
    type_ ## type *src, int sstr,		\
    type_ ## type val, int n)			\
{						\
  if(n&1) {					\
    *dest = *src + val;				\
    OIL_INCREMENT(dest, dstr);			\
    OIL_INCREMENT(src, sstr);			\
  }						\
  if(n&2) {					\
    *dest = *src + val;				\
    OIL_INCREMENT(dest, dstr);			\
    OIL_INCREMENT(src, sstr);			\
    *dest = *src + val;				\
    OIL_INCREMENT(dest, dstr);			\
    OIL_INCREMENT(src, sstr);			\
  }						\
  n /= 4;					\
  while(n>0){					\
    *dest = *src + val;				\
    OIL_INCREMENT(dest,dstr);			\
    OIL_INCREMENT(src,sstr);			\
    *dest = *src + val;				\
    OIL_INCREMENT(dest,dstr);			\
    OIL_INCREMENT(src,sstr);			\
    *dest = *src + val;				\
    OIL_INCREMENT(dest,dstr);			\
    OIL_INCREMENT(src,sstr);			\
    *dest = *src + val;				\
    OIL_INCREMENT(dest,dstr);			\
    OIL_INCREMENT(src,sstr);			\
    n--;					\
  }						\
}						\
OIL_DEFINE_IMPL (scalaradd_ ## type ## _unroll4, scalaradd_ ## type ## _class);

SCALARADD_DEFINE_UNROLL4 (s8);
SCALARADD_DEFINE_UNROLL4 (u8);
SCALARADD_DEFINE_UNROLL4 (s16);
SCALARADD_DEFINE_UNROLL4 (u16);
SCALARADD_DEFINE_UNROLL4 (s32);
SCALARADD_DEFINE_UNROLL4 (u32);
SCALARADD_DEFINE_UNROLL4 (f32);
SCALARADD_DEFINE_UNROLL4 (f64);



#ifdef TEST_scalaradd_f64
int TEST_scalaradd_f64(void)
{
	int i;
	int pass;
	int failures = 0;
	f64 *src, *dest_ref, *dest_test;
	struct sl_profile_struct t;
	double offset;

	src = sl_malloc_f64(N);
	dest_ref = sl_malloc_f64(N);
	dest_test = sl_malloc_f64(N);

	sl_profile_init(t);
	srand(20021001);

	printf("I: " sl_stringify(scalaradd_f64_FUNC) "\n");

	for(pass=0;pass<N_PASS;pass++){
		for(i=0;i<N;i++)src[i]=sl_rand_f64_s16();
		offset=sl_rand_f64_s16();

		scalaradd_f64_ref(dest_ref,src,N,offset);
		sl_profile_start(t);
		scalaradd_f64_FUNC(dest_test,src,N,offset);
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


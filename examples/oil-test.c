/*
 * LIBOIL - Library of Optimized Inner Loops
 * Copyright (c) 2004 David A. Schleef <ds@schleef.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <liboil/liboil.h>
#include <liboil/liboilfunction.h>
#include <liboil/liboiltest.h>
#include <liboil/liboilrandom.h>
#include <liboil/liboilcpu.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

void register_impls(void);

void test(void)
{
  int32_t dest[1];
  uint8_t src[100];
  int i;

  for(i=0;i<100;i++){
    src[i] = oil_rand_u8() & 0x7f;
  }
  dest[0] = 0;

  oil_utf8_validate (dest, src, 100);

#if 0
  for(i=0;i<100;i++){
    printf("%d %d\n",dest[i],src[i]);
  }
#endif
  printf("%d\n", dest[0]);

}

void
dump_array (void *data, void *ref_data, OilType type, int pre_n, int stride,
    int post_n)
{
  int i, j;
  int s2 = oil_type_sizeof (type);
  double x;

#define DUMP(type, format) do { \
  for(i=0;i<post_n;i++){ \
    printf("    "); \
    for(j=0;j<pre_n;j++){ \
      x = fabs(OIL_GET(data, i*stride + j*s2, type) - \
          OIL_GET(ref_data, i*stride + j*s2, type)); \
      if (x >= 0.00001) { \
        printf("*" format "* ", OIL_GET(data, i*stride + j*s2, type)); \
      } else { \
        printf(format " ", OIL_GET(data, i*stride + j*s2, type)); \
      } \
    } \
    printf("\n"); \
  } \
} while(0)

  switch(type) {
    case OIL_TYPE_s8p:
      DUMP(int8_t, "%d");
      break;
    case OIL_TYPE_u8p:
      DUMP(uint8_t, "%d");
      break;
    case OIL_TYPE_s16p:
      DUMP(int16_t, "%d");
      break;
    case OIL_TYPE_u16p:
      DUMP(uint16_t, "%d");
      break;
    case OIL_TYPE_s32p:
      DUMP(int32_t, "%d");
      break;
    case OIL_TYPE_u32p:
      DUMP(uint32_t, "%u");
      //DUMP(uint32_t, "%08x");
      break;
    case OIL_TYPE_s64p:
      DUMP(int64_t, "%lld");
      break;
    case OIL_TYPE_u64p:
      DUMP(uint64_t, "%llu");
      break;
    case OIL_TYPE_f32p:
      DUMP(float, "%g");
      break;
    case OIL_TYPE_f64p:
      DUMP(double, "%g");
      break;
    default:
      break;
  }
}

void
dump_test (OilTest *test)
{
  int i;
  for(i=0;i<OIL_ARG_LAST;i++){
    OilParameter *p = &test->params[i];
    if (p->is_pointer) {
      if (p->direction == 'i' || p->direction == 'd') {
        printf ("  %s:\n", p->parameter_name);
        dump_array (p->test_data + OIL_TEST_HEADER,
            p->ref_data + OIL_TEST_HEADER,
            p->type, p->pre_n, p->stride, p->post_n);
      }
    }
  }
}

void
dump_source (OilTest *test)
{
  int i;
  for(i=0;i<OIL_ARG_LAST;i++){
    OilParameter *p = &test->params[i];
    if (p->is_pointer) {
      if (p->direction == 'i' || p->direction == 's') {
        printf ("  %s:\n", p->parameter_name);
        dump_array (p->src_data + OIL_TEST_HEADER,
            p->src_data + OIL_TEST_HEADER,
            p->type, p->pre_n, p->stride, p->post_n);
      }
    }
  }
}

int main (int argc, char *argv[])
{
  OilFunctionClass *klass;
  OilFunctionImpl *impl;
  OilTest *test;
  double ave, std;

  oil_init ();

  if (argc < 2) {
    printf("oil-test <class_name>\n");
    exit(0);
  }

  klass = oil_class_get (argv[1]);
  if (klass == NULL) {
    printf("class not found: %s\n", argv[1]);
    exit(0);
  }
  oil_class_optimize (klass);

  test = oil_test_new(klass);
  oil_test_set_iterations(test, 1);
  test->n = 10;
  test->m = 10;

  impl = klass->reference_impl;
  ave = impl->profile_ave;
  std = impl->profile_std;
  oil_test_check_impl (test, impl);
  printf ("source array\n");
  dump_source(test);
  printf ("reference impl %s\n", impl->name);
  printf("  ave=%g std=%g\n", ave, std);
  printf("  (this test) ave=%g std=%g\n", impl->profile_ave, impl->profile_std);
  dump_test(test);

  for (impl = klass->first_impl; impl; impl = impl->next) {
    if (impl == klass->reference_impl) continue;
    printf ("impl %s\n", impl->name);
    if (oil_impl_is_runnable (impl)) {
      printf("  ave=%g std=%g\n", impl->profile_ave, impl->profile_std);
      oil_test_check_impl (test, impl);
      printf("  (this test) ave=%g std=%g\n", impl->profile_ave, impl->profile_std);
      dump_test(test);
    }
  }

  return 0;
}


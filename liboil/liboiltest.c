/*
 * LIBOIL - Library of Optimized Inner Loops
 * Copyright (c) 2003,2004 David A. Schleef <ds@schleef.org>
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
#include <config.h>
#endif

#include <liboil/liboiltest.h>
#include <liboil/liboildebug.h>
#include <liboil/liboilrandom.h>
#include <liboil/liboilprofile.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

static void fill_array (void *ptr, OilType type, int pre_n, int stride,
    int post_n);
static double check_array (void *data, void *ref, OilType type, int pre_n,
    int stride, int post_n);
static int check_holes (void *data, OilType type, int pre_n,
    int stride, int post_n, int guard);

OilTest *
oil_test_new (OilFunctionClass *klass)
{
  OilTest *test;
  OilPrototype *proto;
  int i;

  if (klass == NULL) return NULL;

  proto = oil_prototype_from_string (klass->prototype);
  if (proto == NULL) return NULL;

  test = malloc (sizeof (OilTest));
  memset (test, 0, sizeof (OilTest));

  test->klass = klass;
  test->proto = proto;
  test->impl = klass->reference_impl;

  for (i=0;i<proto->n_params;i++){
    if (proto->params[i].parameter_type == OIL_ARG_UNKNOWN) {
      return NULL;
    }
    memcpy (&test->params[proto->params[i].parameter_type], &proto->params[i],
        sizeof(OilParameter));
  }
  for (i=0;i<OIL_ARG_LAST;i++){
    test->params[i].src_data = NULL;
    test->params[i].ref_data = NULL;
    test->params[i].test_data = NULL;
  }

  test->iterations = 10;
  test->n = 100;
  test->m = 100;

  return test;
}

void
oil_test_free (OilTest *test)
{
  int i;

  if (test->proto) {
    oil_prototype_free (test->proto);
  }

  for(i=0;i<OIL_ARG_LAST;i++){
    if (test->params[i].src_data) {
      free (test->params[i].src_data);
    }
    if (test->params[i].ref_data) {
      free (test->params[i].ref_data);
    }
    if (test->params[i].test_data) {
      free (test->params[i].test_data);
    }
  }

  free(test);
}

void
oil_test_set_impl (OilTest *test, OilFunctionImpl *impl)
{
  test->impl = impl;
}

void
oil_test_set_iterations (OilTest *test, int iterations)
{
  test->iterations = iterations;
}

static void
oil_test_check_function (OilTest *test)
{
  int i;
  int j;
  unsigned long args[10];
  unsigned int pointer_mask;

  if (!test->inited) {
    oil_test_init_params(test);

    test->params[OIL_ARG_N].value = test->n;
    
    test->inited = 1;

    if (test->klass->test_func) {
      test->klass->test_func (test);
    }
  }

  OIL_LOG("calling reference function %s", test->impl->name);

  pointer_mask = 1;
  for(i=0;i<test->proto->n_params;i++){
    OilParameter *p;
    j = test->proto->params[i].parameter_type;
    p = &test->params[j];

    pointer_mask <<= 1;
    OIL_LOG("  %s: 0x%08lx (%ld)", oil_arg_type_name (j), p->value, p->value);
    if (p->is_pointer) {
      pointer_mask |= 1;
      if (p->direction == 's') {
        args[i] = (unsigned long)p->src_data + OIL_TEST_HEADER;
      } else if (p->direction == 'i') {
        memcpy (p->test_data, p->src_data, p->size);
        args[i] = (unsigned long)p->test_data + OIL_TEST_HEADER;
      } else if (p->direction == 'd') {
        memset (p->test_data, p->guard, p->size);
        args[i] = (unsigned long)p->test_data + OIL_TEST_HEADER;
      } else {
        OIL_ERROR ("not reached");
      }
    } else {
      args[i] = p->value;
    }
  }

  oil_profile_init (&test->prof);
  for(i=0;i<test->iterations;i++){
    _oil_test_marshal_function (test->impl->func, args, test->proto->n_params,
        pointer_mask, &test->prof);
  }

  oil_profile_get_ave_std (&test->prof, &test->impl->profile_ave,
      &test->impl->profile_std);
}

void
oil_test_check_ref (OilTest *test)
{
  int i;

  if (test->proto->n_params > 10) {
    OIL_ERROR ("function has too many parameters");
    return;
  }

  test->impl = test->klass->reference_impl;

  oil_test_check_function (test);

  for(i=0;i<OIL_ARG_LAST;i++){
    OilParameter *p = &test->params[i];

    if (p->is_pointer) {
      if (p->direction == 'i' || p->direction == 'd') {
        memcpy (p->ref_data, p->test_data, p->size);
      }
    }
  }

  test->tested_ref = 1;
}

static int
check_guard (uint8_t *data, int n, int guard)
{
  int i;
  for(i=0;i<n;i++) {
    if (data[i] != guard) return 0;
  }
  return 1;
}

int
oil_test_check_impl (OilTest *test, OilFunctionImpl *impl)
{
  double x;
  int i;
  int n;
  int fail = 0;

  if (test->proto->n_params > 10) {
    OIL_ERROR ("function has too many parameters");
    return 0;
  }

  if (!test->inited || !test->tested_ref) {
    oil_test_check_ref(test);
  }

  test->impl = impl;
  oil_test_check_function (test);

  x = 0;
  n = 0;
  for(i=0;i<OIL_ARG_LAST;i++){
    OilParameter *p = &test->params[i];

    if (p->is_pointer) {
      if (p->direction == 'i' || p->direction == 'd') {
        x += check_array (p->test_data, p->ref_data, p->type, p->pre_n,
            p->stride, p->post_n);
        n += p->pre_n * p->post_n;
        if (!check_guard (p->test_data, OIL_TEST_HEADER, p->guard)) {
          fail = 1;
          OIL_ERROR("function %s wrote before area for parameter %s",
              test->impl->name, p->parameter_name);
        }
        if (!check_guard ((uint8_t *)p->test_data + p->size - OIL_TEST_FOOTER,
              OIL_TEST_FOOTER, p->guard)) {
          fail = 1;
          OIL_ERROR("function %s wrote after area for parameter %s",
              test->impl->name, p->parameter_name);
        }
        if (!check_holes (p->test_data, p->type, p->pre_n, p->stride,
              p->post_n, p->guard)) {
          fail = 1;
          OIL_ERROR("function %s wrote in interstitial area for parameter %s",
              test->impl->name, p->parameter_name);
        }
      }
    }
  }
  OIL_DEBUG("sum of absolute differences %g for %d values", x, n);
  test->sum_abs_diff = x;
  test->n_points = n;

  if (x > n || fail) {
    OIL_ERROR ("function %s in class %s failed check (%g > %d) outside=%d",
        test->impl->name, test->klass->name, x, n, fail);
    return 0;
  }

  return 1;
}


void
oil_test_cleanup (OilTest *test)
{
  OilParameter *params = test->params;

  /* src1 */
  if(params[OIL_ARG_SRC1].type) {
    if (!params[OIL_ARG_SSTR1].type) {
      params[OIL_ARG_SSTR1].value = oil_type_sizeof (params[OIL_ARG_SRC1].type);
    }
  }

  /* src2 */
  if(params[OIL_ARG_SRC2].type) {
    if (!params[OIL_ARG_SSTR2].type) {
      params[OIL_ARG_SSTR2].value = oil_type_sizeof (params[OIL_ARG_SRC2].type);
    }
  }

  /* src3 */
  if(params[OIL_ARG_SRC3].type) {
    if (!params[OIL_ARG_SSTR3].type) {
      params[OIL_ARG_SSTR3].value = oil_type_sizeof (params[OIL_ARG_SRC3].type);
    }
  }

  /* dest1 */
  if(params[OIL_ARG_DEST1].type) {
    if (!params[OIL_ARG_DSTR1].type) {
      params[OIL_ARG_DSTR1].value = oil_type_sizeof (params[OIL_ARG_DEST1].type);
    }
  }

  /* dest2 */
  if(params[OIL_ARG_DEST2].type) {
    if (!params[OIL_ARG_DSTR2].type) {
      params[OIL_ARG_DSTR2].value = oil_type_sizeof (params[OIL_ARG_DEST1].type);
    }
  }

}


static void
init_parameter (OilTest *test, OilParameter *p, OilParameter *ps)
{
  if (!p->type) return;

  if (p->prestride_length) {
    p->pre_n = p->prestride_length;
  } else {
    if (p->prestride_var == 1) {
      p->pre_n = test->n;
    } else {
      p->pre_n = test->m;
    }
  }

  if (ps->value) {
    p->stride = ps->value;
  } else {
    p->stride = oil_type_sizeof (p->type) * p->pre_n;
    ps->value = p->stride;
  }

  if (p->poststride_length) {
    p->post_n = p->poststride_length;
  } else {
    if (p->poststride_var == 1) {
      p->post_n = test->n;
    } else {
      p->post_n = test->m;
    }
  }

  p->size = p->stride * p->post_n + OIL_TEST_HEADER + OIL_TEST_FOOTER;
  p->guard = oil_rand_u8();

  if (p->direction == 'i' || p->direction == 's') {
    if (p->src_data) free (p->src_data);

    OIL_DEBUG("allocating %d bytes for src_data for %s", p->size, p->parameter_name);
    p->src_data = malloc (p->size);
    memset (p->src_data, p->guard, p->size);
    fill_array (p->src_data + OIL_TEST_HEADER, p->type, p->pre_n, p->stride, p->post_n);
  }

  if (p->direction == 'i' || p->direction == 'd') {
    if (p->ref_data) free (p->ref_data);
    p->ref_data = malloc (p->size);
    memset (p->ref_data, p->guard, p->size);
    OIL_DEBUG("allocating %d bytes for ref_data and test_data for %s", p->size, p->parameter_name);

    if (p->test_data) free (p->test_data);
    p->test_data = malloc (p->size);
    memset (p->test_data, p->guard, p->size);
  }
}

void
oil_test_init_params (OilTest *test)
{
  init_parameter (test, &test->params[OIL_ARG_DEST1],
      &test->params[OIL_ARG_DSTR1]);
  init_parameter (test, &test->params[OIL_ARG_DEST2],
      &test->params[OIL_ARG_DSTR2]);

  init_parameter (test, &test->params[OIL_ARG_SRC1],
      &test->params[OIL_ARG_SSTR1]);
  init_parameter (test, &test->params[OIL_ARG_SRC2],
      &test->params[OIL_ARG_SSTR2]);
  init_parameter (test, &test->params[OIL_ARG_SRC3],
      &test->params[OIL_ARG_SSTR3]);
  init_parameter (test, &test->params[OIL_ARG_SRC4],
      &test->params[OIL_ARG_SSTR4]);
  init_parameter (test, &test->params[OIL_ARG_SRC5],
      &test->params[OIL_ARG_SSTR5]);

  init_parameter (test, &test->params[OIL_ARG_INPLACE1],
      &test->params[OIL_ARG_ISTR1]);
  init_parameter (test, &test->params[OIL_ARG_INPLACE2],
      &test->params[OIL_ARG_ISTR2]);
}

static void
fill_array (void *data, OilType type, int pre_n, int stride, int post_n)
{
  int i;
  int j;
  int s2 = oil_type_sizeof (type);

#define FILL(type,func) do {\
  for(i=0;i<post_n;i++){ \
    for(j=0;j<pre_n;j++){ \
      OIL_GET(data, i*stride + j*s2, type) = func ; \
    } \
  } \
}while(0)

  switch (type) {
    case OIL_TYPE_s8p:
      FILL(int8_t,oil_rand_s8());
      break;
    case OIL_TYPE_u8p:
      FILL(uint8_t,oil_rand_u8());
      break;
    case OIL_TYPE_s16p:
      FILL(int16_t,oil_rand_s16());
      break;
    case OIL_TYPE_u16p:
      FILL(uint16_t,oil_rand_u16());
      break;
    case OIL_TYPE_s32p:
      FILL(int32_t,oil_rand_s32());
      break;
    case OIL_TYPE_u32p:
      FILL(uint32_t,oil_rand_u32());
      break;
    case OIL_TYPE_f32p:
      FILL(float,oil_rand_f32_0_1());
      break;
    case OIL_TYPE_f64p:
      FILL(double,oil_rand_f64_0_1());
      break;
    default:
      OIL_ERROR ("should not be reached (type == %d)", type);
      return;
      break;
  }
}

static double
check_array (void *data, void *ref, OilType type, int pre_n, int stride, int post_n)
{
  int i;
  int j;
  int s2 = oil_type_sizeof (type);
  double x = 0;

#define CHECK(type) do {\
  for(i=0;i<post_n;i++){ \
    for(j=0;j<pre_n;j++){ \
      x += fabs((double)OIL_GET(data, i*stride + j*s2, type) - \
          (double)OIL_GET(ref, i*stride + j*s2, type)); \
    } \
  } \
}while(0)

  switch (type) {
    case OIL_TYPE_s8p:
      CHECK(int8_t);
      break;
    case OIL_TYPE_u8p:
      CHECK(uint8_t);
      break;
    case OIL_TYPE_s16p:
      CHECK(int16_t);
      break;
    case OIL_TYPE_u16p:
      CHECK(uint16_t);
      break;
    case OIL_TYPE_s32p:
      CHECK(int32_t);
      break;
    case OIL_TYPE_u32p:
      CHECK(uint32_t);
      break;
    case OIL_TYPE_f32p:
      CHECK(float);
      break;
    case OIL_TYPE_f64p:
      CHECK(double);
      break;
    default:
      OIL_ERROR ("should not be reached (type == %d)", type);
      return 1e9;
      break;
  }
  return x;
}

static int
check_holes (void *data, OilType type, int pre_n, int stride, int post_n,
    int guard)
{
  int i;
  int chunk_size;
  int hole_size;

  chunk_size = pre_n * oil_type_sizeof (type);
  hole_size = stride - chunk_size;
  if (hole_size == 0) {
    return 1;
  }

  for(i=0;i<post_n;i++){
    if (!check_guard (data + stride * i + chunk_size, hole_size, guard)) {
      return 0;
    }
  }

  return 1;
}


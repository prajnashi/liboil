/* liboil - Library of Optimized Inner Loops
 * Copyright (C) 2003,2004  David A. Schleef <ds@schleef.org>
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
#include <config.h>
#endif

#include <liboil/liboiltest.h>
#include <liboil/liboilrandom.h>
#include <liboil/liboilprofile.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>

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
    if (test->params[i].ptr) {
      free (test->params[i].ptr);
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

int
oil_test_go (OilTest *test)
{
  int i;
  int j;
  int args[10];

  if (test->proto->n_params > 10) return 0;

  if (!test->inited) {
    oil_test_init_params(test);

    test->params[OIL_ARG_N].value = test->n;
    
    test->inited = 1;
  }

  if (test->klass->test_func) {
    test->klass->test_func (test);
  }

  OIL_LOG("calling test function %s", test->impl->name);
  for(i=0;i<test->proto->n_params;i++){
    j = test->proto->params[i].parameter_type;
    OIL_LOG("  %s: 0x%08lx (%ld)",
        oil_arg_type_name (j),
        test->params[j].value,
        test->params[j].value);
    args[i] = test->params[j].value;
  }

  oil_profile_init (&test->prof);
  for(i=0;i<test->iterations;i++){
    oil_profile_start (&test->prof);
    ((void (*)(int,int,int,int,int,int,int,int,int,int))test->impl->func)
      (args[0],args[1],args[2],args[3],args[4],args[5],args[6],args[7],
       args[8],args[9]);
    oil_profile_stop (&test->prof);
  }

  oil_profile_get_ave_std (&test->prof, &test->impl->profile_ave,
      &test->impl->profile_std);

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
fill_array (void *ptr, OilType type, int pre_n, int stride, int post_n)
{
  int i;
  int j;
  int s2 = oil_type_sizeof (type);

  for(i=0;i<post_n;i++){
    for(j=0;j<pre_n;j++){
      switch (type) {
        case OIL_TYPE_s8p:
          OIL_GET(ptr, i*stride + j*s2, int8_t) = oil_rand_s8();
          break;
        case OIL_TYPE_u8p:
          OIL_GET(ptr, i*stride + j*s2, uint8_t) = oil_rand_u8();
          break;
        case OIL_TYPE_s16p:
          OIL_GET(ptr, i*stride + j*s2, int16_t) = oil_rand_s16();
          break;
        case OIL_TYPE_u16p:
          OIL_GET(ptr, i*stride + j*s2, uint16_t) = oil_rand_u16();
          break;
        case OIL_TYPE_s32p:
          OIL_GET(ptr, i*stride + j*s2, int32_t) = oil_rand_s32();
          break;
        case OIL_TYPE_u32p:
          OIL_GET(ptr, i*stride + j*s2, uint32_t) = oil_rand_u32();
          break;
        case OIL_TYPE_f32p:
          OIL_GET(ptr, i*stride + j*s2, float) = oil_rand_f32_0_1();
          break;
        case OIL_TYPE_f64p:
          OIL_GET(ptr, i*stride + j*s2, double) = oil_rand_f64_0_1();
          break;
        default:
          OIL_ERROR ("should not be reached (type == %d)", type);
          return;
          break;
      }
    }
  }
}

static void
init_parameter (OilTest *test, OilParameter *p, OilParameter *ps)
{
  int stride;
  void *ptr;
  int pre_n;
  int post_n;

  if (!p->type) return;

  if (p->prestride_length) {
    pre_n = p->prestride_length;
  } else {
    if (p->prestride_var == 1) {
      pre_n = test->n;
    } else {
      pre_n = test->m;
    }
  }

  if (ps->value) {
    stride = ps->value;
  } else {
    stride = oil_type_sizeof (p->type) * pre_n;
    ps->value = stride;
  }

  if (p->poststride_length) {
    post_n = p->poststride_length;
  } else {
    if (p->poststride_var == 1) {
      post_n = test->n;
    } else {
      post_n = test->m;
    }
  }

  ptr = malloc (stride * post_n + OIL_TEST_HEADER + OIL_TEST_FOOTER);
  memset (ptr, 0, stride * post_n + OIL_TEST_HEADER + OIL_TEST_FOOTER);
  p->ptr = ptr;
  p->value = (unsigned long) ptr + OIL_TEST_HEADER;

  if (p->direction == 'i' || p->direction == 's') {
    fill_array (ptr, p->type, pre_n, stride, post_n);
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


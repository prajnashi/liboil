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
    memcpy (&test->params[proto->params[i].parameter_type], &proto->params[i],
        sizeof(OilParameter));
  }

  test->iterations = 100;
  test->n = 100;

  return test;
}

void
oil_test_free (OilTest *test)
{
  if (test->proto) {
    oil_prototype_free (test->proto);
  }



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
  if (test->klass->test_func != NULL) return 0;
  if (!test->params[OIL_ARG_N].type) return 0;
  if (test->params[OIL_ARG_UNKNOWN].type) return 0;
  if (test->params[OIL_ARG_STATE].type) return 0;

  oil_test_init_src_arrays(test);
  oil_test_init_dest_arrays(test);
  oil_test_init_params(test);

  test->params[OIL_ARG_N].value = test->n;

  OIL_LOG("calling test function %s", test->impl->name);
  for(i=0;i<test->proto->n_params;i++){
    j = test->proto->params[i].parameter_type;
    OIL_LOG("  %s: 0x%08lx (%ld)",
        oil_arg_type_name (j),
        test->params[j].value,
        test->params[j].value);
    args[i] = test->params[j].value;
  }

  oil_profile_init (test->prof);
  for(i=0;i<test->iterations;i++){
    oil_profile_start (test->prof);
    ((void (*)(int,int,int,int,int,int,int,int,int,int))test->impl->func)
      (args[0],args[1],args[2],args[3],args[4],args[5],args[6],args[7],
       args[8],args[9]);
    oil_profile_stop (test->prof);
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
fill_array (void *ptr, int stride, OilType type, int n)
{
  int i;

  for(i=0;i<n;i++){
    switch (type) {
      case OIL_TYPE_s8p:
        OIL_GET(ptr, i*stride, int8_t) = oil_rand_s8();
        break;
      case OIL_TYPE_u8p:
        OIL_GET(ptr, i*stride, uint8_t) = oil_rand_u8();
        break;
      case OIL_TYPE_s16p:
        OIL_GET(ptr, i*stride, int16_t) = oil_rand_s16();
        break;
      case OIL_TYPE_u16p:
        OIL_GET(ptr, i*stride, uint16_t) = oil_rand_u16();
        break;
      case OIL_TYPE_s32p:
        OIL_GET(ptr, i*stride, int32_t) = oil_rand_s32();
        break;
      case OIL_TYPE_u32p:
        OIL_GET(ptr, i*stride, uint32_t) = oil_rand_u32();
        break;
      case OIL_TYPE_f32p:
        OIL_GET(ptr, i*stride, float) = oil_rand_f32_0_1();
        break;
      case OIL_TYPE_f64p:
        OIL_GET(ptr, i*stride, double) = oil_rand_f64_0_1();
        break;
      default:
        OIL_ERROR ("should not be reached (type == %d)", type);
        return;
        break;
    }
  }
}

static void
init_src_array (OilTest *test, OilArgType t, OilArgType ts)
{
  int n;
  int stride;
  void *ptr;

  if (!test->params[t].type) return;

  n = test->n;
  stride = test->params[ts].value;
  if (stride == 0) {
    stride = oil_type_sizeof (test->params[t].type);
    test->params[ts].value = stride;
  }

  ptr = malloc (stride * n);
  memset (ptr, 0, stride * n);
  test->params[t].value = (unsigned long) ptr;

  fill_array (ptr, stride, test->params[t].type, n);
}

void
oil_test_init_src_arrays (OilTest *test)
{
  init_src_array (test, OIL_ARG_SRC1, OIL_ARG_SSTR1);
  init_src_array (test, OIL_ARG_SRC2, OIL_ARG_SSTR2);
  init_src_array (test, OIL_ARG_SRC3, OIL_ARG_SSTR3);
}

static void
init_param (OilTest *test, OilArgType t)
{
  int size;
  void *ptr;

  if (!test->params[t].type) return;

  size = oil_type_sizeof (test->params[t].type);
  ptr = malloc (size);
  memset (ptr, 0, size);
  test->params[t].value = (unsigned long) ptr;

  fill_array (ptr, 0, test->params[t].type, 1);
}

void
oil_test_init_params (OilTest *test)
{
  init_param (test, OIL_ARG_PARAM1);
  init_param (test, OIL_ARG_PARAM2);
  init_param (test, OIL_ARG_PARAM3);
}

static void
init_dest_array (OilTest *test, OilArgType t, OilArgType ts)
{
  int n;
  int stride;
  void *ptr;

  if (!test->params[t].type) return;

  n = test->n;
  stride = test->params[ts].value;
  if (stride == 0) {
    stride = oil_type_sizeof (test->params[t].type);
    test->params[ts].value = stride;
  }

  ptr = malloc (stride * n);
  memset (ptr, 0, stride * n);
  test->params[t].value = (unsigned long) ptr;
}

void
oil_test_init_dest_arrays (OilTest *test)
{
  init_dest_array (test, OIL_ARG_DEST1, OIL_ARG_DSTR1);
  init_dest_array (test, OIL_ARG_DEST2, OIL_ARG_DSTR2);
}


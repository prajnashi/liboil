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
#include <liboil/liboildebug.h>
#include <liboil/liboilcpu.h>
#include <liboil/liboiltest.h>

#include <stdio.h>
#include <string.h>

extern OilFunctionClass *_oil_function_class_array[];
extern OilFunctionImpl *_oil_function_impl_array[];

int oil_n_function_impls;
int oil_n_function_classes;

static void oil_init_pointers (void);
static void oil_init_structs (void);

void _oil_cpu_init (void);

void
oil_init (void)
{
  static int inited = 0;
  unsigned long start, stop;

  if (inited) return;
  inited = 1;

  start = oil_profile_stamp_gtod ();

  _oil_debug_init ();
  _oil_cpu_init ();
  oil_init_pointers ();
  oil_init_structs ();
  oil_optimize_all ();

  stop = oil_profile_stamp_gtod ();

  OIL_INFO ("initialization completed in %ld usec", stop-start);
}

void
oil_optimize_all (void)
{
  OilFunctionClass *klass;
  int i;

  for (i = 0; i < oil_n_function_classes; i++) {
    klass = oil_class_get_by_index (i);

    oil_class_optimize (klass);
  }
}

void
oil_optimize (const char *class_name)
{
  OilFunctionClass *klass;

  klass = oil_class_get (class_name);
  if (klass == NULL) {
    OIL_ERROR ("could not find class %s", class_name);
    return;
  }

  oil_class_optimize (klass);
}

OilFunctionClass *
oil_class_get_by_index (int i)
{
  return _oil_function_class_array[i];
}

OilFunctionImpl *
oil_impl_get_by_index (int i)
{
  return _oil_function_impl_array[i];
}

OilFunctionClass *
oil_class_get (const char *class_name)
{
  OilFunctionClass *klass;
  int i;

  for (i = 0; i < oil_n_function_classes; i++) {
    klass = oil_class_get_by_index (i);

    if (strcmp (klass->name, class_name) == 0)
      return klass;
  }
  return NULL;
}

void
oil_class_choose_by_name (OilFunctionClass * klass, const char *name)
{
  OilFunctionImpl *impl;

  for(impl = klass->first_impl; impl; impl = impl->next) {
    if (impl->name && strcmp (impl->name, name) == 0) {
      klass->chosen_impl = impl;
      klass->func = impl->func;
      return;
    }
  }
}

void
oil_class_optimize (OilFunctionClass * klass)
{
  OilFunctionImpl *impl;
  OilFunctionImpl *min_impl;
  OilTest *test;
  int ret;

  OIL_DEBUG ("optimizing class %s", klass->name);

  if (klass->first_impl == NULL) {
    OIL_ERROR ("class %s has no implmentations", klass->name);
    return;
  }

  test = oil_test_new (klass);
  if (test == NULL) {
    OIL_ERROR ("failed to test function class %s", klass->name);
    return;
  }

  min_impl = NULL;
  for (impl = klass->first_impl; impl; impl = impl->next) {
    OIL_LOG ("testing impl %s", impl->name);
    if ((impl->flags & OIL_CPU_FLAG_MASK) & (~oil_cpu_flags))
      continue;

    oil_test_set_impl (test, impl);

    ret = oil_test_go (test);
    if (ret) {
      OIL_LOG ("impl %s ave=%g std=%g", impl->name, impl->profile_ave,
          impl->profile_std);
      if (min_impl == NULL) {
        min_impl = impl;
      } else {
        if (impl->profile_ave < min_impl->profile_ave) {
          min_impl = impl;
        }
      }
    } else {
      OIL_ERROR("uncaught error");
    }
  }
  klass->chosen_impl = min_impl;
  klass->func = min_impl->func;
}

static void
oil_init_pointers (void)
{
  int i;

  for(i=0;_oil_function_class_array[i];i++) {
    oil_n_function_classes++;
  }

  for(i=0;_oil_function_impl_array[i];i++) {
    oil_n_function_impls++;
  }

}

static void
oil_init_structs (void)
{
  int i;
  OilFunctionImpl *impl;

  for (i = 0; i < oil_n_function_impls; i++) {
    impl = oil_impl_get_by_index (i);
    OIL_DEBUG ("registering impl %p (%s)", impl,
          (impl->name != NULL) ? impl->name : "NULL");
    if (impl->klass == NULL) {
      OIL_ERROR ("impl->klass is NULL for impl %p (%s)", impl,
          (impl->name != NULL) ? impl->name : "NULL");
      continue;
    }
    impl->next = impl->klass->first_impl;
    impl->klass->first_impl = impl;
    if (impl->flags & OIL_IMPL_FLAG_REF) {
      impl->klass->reference_impl = impl;
      impl->klass->chosen_impl = impl;
      impl->klass->func = impl->func;
    }
  }
}

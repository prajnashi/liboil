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

#include <stdio.h>
#include <string.h>

extern OilFunctionClass _oil_begin_function_class;
extern OilFunctionClass _oil_next_function_class;
extern OilFunctionClass _oil_end_function_class;

extern OilFunctionImpl _oil_begin_function_impl;
extern OilFunctionImpl _oil_next_function_impl;
extern OilFunctionImpl _oil_end_function_impl;

OilFunctionClass *oil_function_classes;
int oil_function_class_stride;
OilFunctionImpl *oil_function_impls;
int oil_function_impl_stride;
int oil_n_function_impls;
int oil_n_function_classes;

static void oil_init_pointers (void);
static void oil_init_structs (void);

void _oil_cpu_init (void);

void
oil_init (void)
{
  static int inited = 0;

  if (inited) return;
  inited = 1;

  _oil_debug_init ();
  _oil_cpu_init ();
  oil_init_pointers ();
  oil_init_structs ();
  //oil_optimize_all ();

  if (0)
    oil_spill ();
}

void
oil_optimize_all (void)
{
  OilFunctionClass *klass;
  int i;

  for (i = 0; i < oil_n_function_classes; i++) {
    klass = (void *)oil_function_classes + i * oil_function_class_stride;

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
  return (void *)oil_function_classes + i * oil_function_class_stride;
}

OilFunctionClass *
oil_class_get (const char *class_name)
{
  OilFunctionClass *klass;
  int i;

  for (i = 0; i < oil_n_function_classes; i++) {
    klass = (void *)oil_function_classes + i * oil_function_class_stride;

    if (strcmp (klass->name, class_name) == 0)
      return klass;
  }
  return NULL;
}

void
oil_class_optimize (OilFunctionClass * klass)
{
  OilFunctionImpl *impl;
  OilFunctionImpl *min_impl;
  int min;

  OIL_DEBUG ("optimizing class %s", klass->name);

  if (klass->first_impl == NULL) {
    OIL_ERROR ("class %s has no implmentations", klass->name);
    return;
  }
#if 0
  if (klass->test_func == NULL) {
    //OIL_ERROR ("class %s has no test function", klass->name);
    OIL_LOG ("class %s has no test function", klass->name);
    return;
  }
#endif

  min_impl = NULL;
  min = 2147483647;
  for (impl = klass->first_impl; impl; impl = impl->next) {
#if 0
    printf ("  %p %08x %5.5s %s\n", impl->func,
	impl->flags, (impl == klass->reference_impl) ? "(ref)" : "",
        impl->name
        );
#endif
    OIL_LOG ("testing impl %s", impl->name);
    if ((impl->flags & OIL_ARCH_FLAGS) & (~oil_cpu_flags))
      continue;
    //klass->test_func (klass, impl);

    if (impl->prof < min) {
      min_impl = impl;
      min = impl->prof;
    }
  }
  klass->chosen_impl = min_impl;
  klass->func = min_impl->func;
}

static void
oil_init_pointers (void)
{
  unsigned long begin;
  unsigned long next;
  unsigned long end;

  begin = ((unsigned long) &_oil_begin_function_class);
  next = ((unsigned long) &_oil_next_function_class);
  end = ((unsigned long) &_oil_end_function_class);
  oil_function_class_stride = next - begin;
  OIL_DEBUG("classes: begin %p next %p end %p stride %d", begin, next, end,
      oil_function_class_stride);

  oil_function_classes = (OilFunctionClass *) (next + oil_function_class_stride);
  oil_n_function_classes = (end - next) / oil_function_class_stride - 1;
  OIL_DEBUG("classes: %d at %p", oil_n_function_classes, oil_function_classes);

  begin = ((unsigned long) &_oil_begin_function_impl);
  next = ((unsigned long) &_oil_next_function_impl);
  end = ((unsigned long) &_oil_end_function_impl);
  oil_function_impl_stride = next - begin;
  OIL_DEBUG("impls: begin %p next %p end %p stride %d", begin, next, end,
      oil_function_impl_stride);

  oil_function_impls = (OilFunctionImpl *) (next + oil_function_impl_stride);
  oil_n_function_impls = (end - next) / oil_function_impl_stride - 1;
  OIL_DEBUG("impls: %d at %p", oil_n_function_impls, oil_function_impls);
}

static void
oil_init_structs (void)
{
  int i;
  OilFunctionImpl *impl;

  for (i = 0; i < oil_n_function_impls; i++) {
    impl = oil_function_impls + i;
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

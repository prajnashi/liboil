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

#include <stdio.h>
#include <string.h>

OilFunctionClass *oil_function_classes;
OilFunctionImpl *oil_function_impls;
int oil_n_function_impls;
int oil_n_function_classes;

unsigned int oil_arch_flags = OIL_IMPL_REQUIRES_MMX;

static void oil_init_pointers (void);
static void oil_init_structs (void);

void
oil_init (void)
{
  static int inited = 0;

  if (inited) return;
  inited = 1;

  _oil_debug_init ();
  oil_init_pointers ();
  oil_init_structs ();
  oil_optimize_all ();

  if (0)
    oil_spill ();
}

void
oil_optimize_all (void)
{
  OilFunctionClass *klass;
  int i;

  for (i = 0; i < oil_n_function_classes; i++) {
    klass = oil_function_classes + i;

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
oil_class_get (const char *class_name)
{
  OilFunctionClass *klass;
  int i;

  for (i = 0; i < oil_n_function_classes; i++) {
    klass = oil_function_classes + i;

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

  OIL_LOG ("optimizing class %s", klass->name);

  if (klass->test_func == NULL) {
    //OIL_ERROR ("class %s has no test function", klass->name);
    return;
  }

  min_impl = NULL;
  min = 2147483647;
  for (impl = klass->first_impl; impl; impl = impl->next) {
#if 0
    printf ("  %p %08x %s\n", impl->func,
	impl->flags, (impl == klass->reference_impl) ? "(ref)" : "");
#endif
    if ((impl->flags & OIL_ARCH_FLAGS) & (~oil_arch_flags))
      continue;
    klass->test_func (klass, impl);

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
  unsigned long stride;

  begin = ((unsigned long) &_oil_begin_function_class);
  next = ((unsigned long) &_oil_next_function_class);
  end = ((unsigned long) &_oil_end_function_class);
  stride = next - begin;

  oil_function_classes = (OilFunctionClass *) (next + stride);
  oil_n_function_classes = (end - next) / stride - 1;

  begin = ((unsigned long) &_oil_begin_function_impl);
  next = ((unsigned long) &_oil_next_function_impl);
  end = ((unsigned long) &_oil_end_function_impl);
  stride = next - begin;

  oil_function_impls = (OilFunctionImpl *) (next + stride);
  oil_n_function_impls = (end - next) / stride - 1;
}

static void
oil_init_structs (void)
{
  int i;
  OilFunctionImpl *impl;

  for (i = 0; i < oil_n_function_impls; i++) {
    impl = oil_function_impls + i;
    impl->next = impl->klass->first_impl;
    impl->klass->first_impl = impl;
    if (impl->flags & OIL_IMPL_FLAG_REF) {
      impl->klass->reference_impl = impl;
      impl->klass->chosen_impl = impl;
      impl->klass->func = impl->func;
    }
  }
}

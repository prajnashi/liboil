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

#include <liboil/liboil.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

int retval = EXIT_SUCCESS;
static void
printerr (char *format, ...)
{
  va_list var_args;

  va_start (var_args, format);
  vfprintf (stderr, format, var_args);
  va_end (var_args);
  retval = EXIT_FAILURE;
}
  
static char *
string_append (char *str, const char *append)
{
  char *ret;
  if (str) {
    size_t size = strlen (str) + 2 + strlen (append) + 1;
    ret = malloc (size);
    snprintf (ret, size, "%s, %s", str, append);
    free (str);
  } else {
    ret = strdup (append);
  }
  return ret;
}
static char *
oil_flags_to_string (unsigned int flags)
{
  char *ret = NULL;
  
  if (flags & OIL_IMPL_FLAG_REF) 
    ret = string_append (ret, "REF");
  if (flags & OIL_IMPL_FLAG_OPT) 
    ret = string_append (ret, "altopt");
  if (flags & OIL_IMPL_FLAG_ASM) 
    ret = string_append (ret, "asm");
#ifdef HAVE_CPU_I386
  if (flags & OIL_IMPL_REQUIRES_CMOV) 
    ret = string_append (ret, "cmov");
  if (flags & OIL_IMPL_REQUIRES_MMX) 
    ret = string_append (ret, "mmx");
  if (flags & OIL_IMPL_REQUIRES_SSE) 
    ret = string_append (ret, "sse");
  if (flags & OIL_IMPL_REQUIRES_MMXEXT) 
    ret = string_append (ret, "mmxext");
  if (flags & OIL_IMPL_REQUIRES_SSE2) 
    ret = string_append (ret, "sse2");
  if (flags & OIL_IMPL_REQUIRES_3DNOW) 
    ret = string_append (ret, "3dnow");
  if (flags & OIL_IMPL_REQUIRES_3DNOWEXT) 
    ret = string_append (ret, "3dnowext");
#endif
#ifdef HAVE_CPU_PPC
  if (flags & OIL_IMPL_REQUIRES_ALTIVEC) 
    ret = string_append (ret, "altivec");
#endif
  return ret;
}

static void
oil_print_impl (OilFunctionImpl *impl, char* prefix)
{
  char *c;
  printf ("%s%s\n", prefix, impl->name);
  c = oil_flags_to_string (impl->flags);
  if (c) {
    printf ("%s  flags: %s\n", prefix, c);
    free (c);
  }
  if (impl->profile_ave) {
    printf ("%s  profile: %g ticks (st.dev. %g)\n", prefix, impl->profile_ave,
        impl->profile_std);
  }
  if ((impl->flags & OIL_CPU_FLAG_MASK) & (~oil_cpu_flags)) {
    printf ("%s  disabled\n", prefix);
  }
}

static int
impl_compare (const void *a, const void *b)
{
  const OilFunctionImpl *ia = *(OilFunctionImpl **)a;
  const OilFunctionImpl *ib = *(OilFunctionImpl **)b;

  if (ia->profile_ave < ib->profile_ave) return -1;
  if (ia->profile_ave > ib->profile_ave) return 1;
  return 0;
}

static void
oil_print_class (OilFunctionClass *klass, int verbose)
{
  OilFunctionImpl *impl;
  OilFunctionImpl **list;
  int n;
  int i;
  
  if (!verbose) {
    printf ("%s\n", klass->name);
    return;
  }
  printf ("%s (%s)\n", klass->name, klass->prototype);
  printf ("  all implementations: \n");

  n = 0;
  for (impl = klass->first_impl; impl; impl = impl->next) {
    n++;
  }
  list = malloc(n * sizeof(OilFunctionImpl *));
  i = 0;
  for (impl = klass->first_impl; impl; impl = impl->next) {
    list[i] = impl;
    i++;
  }

  qsort (list, n, sizeof(OilFunctionImpl *), impl_compare);

  for (i=0;i<n;i++){
    impl = list[i];
    if ((impl->flags & OIL_IMPL_FLAG_REF) &&
	klass->reference_impl != impl) {
      printerr ("warning: function %s is not reference implementation for class %s\n",
	  impl->name, klass->name);
    }
    oil_print_impl (impl, "    ");
    if (klass->chosen_impl == impl) {
      printf ("      currently chosen\n");
    }
  }

  free(list);
}

static void
oil_print_all (void)
{
  int i;

  for (i = 0; i < oil_n_function_classes; i++){
    OilFunctionClass *klass = oil_class_get_by_index (i);
    oil_print_class (klass, 0);
  }
}

int
main (int argc, char *argv[])
{
  oil_init();

  if (argc > 1) {
    int i;
    for (i = 1; i < argc; i++) {
      OilFunctionClass *klass = oil_class_get (argv[i]);
      if (!klass) {
	printerr ("error: no class \"%s\" found\n", argv[i]);
      } else {
	oil_print_class (klass, 1);
      }
    }
  } else {
    oil_print_all ();
  }

  return retval;
}


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
#include <liboil/liboilcpu.h>
#include <liboil/liboiltest.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#if 0
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
  if (flags & OIL_IMPL_FLAG_CMOV) 
    ret = string_append (ret, "cmov");
  if (flags & OIL_IMPL_FLAG_MMX) 
    ret = string_append (ret, "mmx");
  if (flags & OIL_IMPL_FLAG_SSE) 
    ret = string_append (ret, "sse");
  if (flags & OIL_IMPL_FLAG_MMXEXT) 
    ret = string_append (ret, "mmxext");
  if (flags & OIL_IMPL_FLAG_SSE2) 
    ret = string_append (ret, "sse2");
  if (flags & OIL_IMPL_FLAG_3DNOW) 
    ret = string_append (ret, "3dnow");
  if (flags & OIL_IMPL_FLAG_3DNOWEXT) 
    ret = string_append (ret, "3dnowext");
#endif
#ifdef HAVE_CPU_PPC
  if (flags & OIL_IMPL_FLAG_ALTIVEC) 
    ret = string_append (ret, "altivec");
#endif
  return ret;
}
#endif

#if 0
static void
oil_print_impl (OilFunctionImpl *impl, OilTest *test, char* prefix)
{
  char *c;
  unsigned int cpu_flags = oil_cpu_get_flags();

  oil_test_check_impl (test, impl);

  printf ("%s%s\n", prefix, impl->name);
  c = oil_flags_to_string (impl->flags);
  if (c) {
    printf ("%s  flags: %s\n", prefix, c);
    free (c);
  }
  if (impl->profile_ave) {
    printf ("%s  profile: %g ticks (std.dev. %g)\n", prefix, impl->profile_ave,
        impl->profile_std);
  }
  if (test && !(impl->flags & OIL_IMPL_FLAG_REF)) {
    printf ("%s  sum abs difference: %g (n=%d)\n", prefix,
        test->sum_abs_diff, test->n_points);
  }
  if ((impl->flags & OIL_CPU_FLAG_MASK) & (~cpu_flags)) {
    printf ("%s  disabled\n", prefix);
  }
}
#endif

#if 0
static int
impl_compare (const void *a, const void *b)
{
  const OilFunctionImpl *ia = *(OilFunctionImpl **)a;
  const OilFunctionImpl *ib = *(OilFunctionImpl **)b;

  if (ia->profile_ave < ib->profile_ave) return -1;
  if (ia->profile_ave > ib->profile_ave) return 1;
  return 0;
}
#endif

#if 0
static int
get_n_impls(OilFunctionClass *klass)
{
  OilFunctionImpl *impl;
  int n;

  n = 0;
  for(impl=klass->first_impl;impl;impl=impl->next)n++;

  return n;
}
#endif

static int
is_poorly_strided (OilFunctionClass *klass)
{
  OilPrototype *proto;
  int i;
  int j;
  OilParameter *param;

  proto = oil_prototype_from_string (klass->prototype);
  for(i=0;i<proto->n_params;i++) {
    if (proto->params[i].is_stride) {
      for(j=0;j<proto->n_params;j++) {
        param = proto->params + j;
        if (param->parameter_type == (proto->params[i].parameter_type - 1)) {
          if (param->prestride_length == 1 && param->poststride_length == 0) {
            oil_prototype_free (proto);
            return 1;
          }
        }
      }
    }
  }
  oil_prototype_free (proto);
  return 0;
}

static double
get_speed_score (OilFunctionClass *klass)
{
  OilFunctionImpl *impl;
  double max;
  double x;

  max = 1.0;
  for(impl = klass->first_impl; impl; impl = impl->next) {
    x =  klass->reference_impl->profile_ave / impl->profile_ave;
    if (x > max) max = x;
  }
  return max;
}

static void
oil_suggest_class (OilFunctionClass *klass, int verbose)
{
  double x;

  if (is_poorly_strided (klass)) return;

  x = get_speed_score (klass);

  if (x == 1.0) {
    printf("%s\n", klass->name);
  }

}

static void
oil_suggest_all (void)
{
  int i;
  int n;

  n = oil_class_get_n_classes ();
  for (i = 0; i < n; i++){
    OilFunctionClass *klass = oil_class_get_by_index (i);

    oil_suggest_class (klass, 0);
  }
}

int
main (int argc, char *argv[])
{
  oil_init();

  oil_suggest_all ();

  return 0;
}


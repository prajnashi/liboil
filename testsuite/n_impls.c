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
#include <stdio.h>

#include <liboil/liboil.h>
#include <liboil/liboilfunction.h>

static int
oil_class_get_n_impls (OilFunctionClass *klass)
{
  int n;
  OilFunctionImpl *impl = klass->first_impl;

  n = 0;
  while(impl) {
    n++;
    impl = impl->next;
  }

  return n;
}

int main (int argc, char *argv[])
{
  int failed = 0;
  int n_classes;
  int expected_classes;
  int n_impls;
  int expected_impls;
  int i;

  oil_init ();

  n_classes = oil_class_get_n_classes();
  expected_classes = 265;

  printf("number of classes %d\n", n_classes);
  if (n_classes < expected_classes) {
    printf("number of classes less than expected (%d < %d)\n",
        n_classes, expected_classes);
    failed = 1;
  }

  n_impls = 0;
  for(i=0;i<n_classes;i++){
    OilFunctionClass *klass = oil_class_get_by_index (i);

    n_impls += oil_class_get_n_impls (klass);
  }

  printf("number of total impls %d\n", n_impls);
  expected_impls = 265;
  /* C impls */
  expected_impls += 350;
#ifdef HAVE_GCC_I386
  /* i386 */
  expected_impls += 47;
#endif
#ifdef HAVE_MMX_INTRINSICS
  /* fb */
  expected_impls += 4;
#endif
#ifdef HAVE_SSE_INTRINSICS
  /* sse */
  expected_impls += 1;
#endif
#ifdef HAVE_GCC_POWERPC
  expected_impls += 0;
#endif
  if (n_impls < expected_impls) {
    printf("number of total impls less than expected (%d < %d)\n",
        n_impls, expected_impls);
    failed = 1;
  }

  return failed;
}


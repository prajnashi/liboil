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
#include <liboil/liboilrandom.h>
#include <liboil/liboilcpu.h>
#include <glib.h>
#include <string.h>
#include <math.h>

void register_impls(void);

void test(void)
{
  int16_t dest[100];
  int16_t src[100];
  int i;

  for(i=0;i<100;i++){
    src[i] = oil_rand_s16();
    dest[i] = 0;
  }

  oil_abs_u16_s16 (dest, 4, src, 4, 50);

  for(i=0;i<100;i++){
    g_print("%d %d\n",dest[i],src[i]);
  }

}

int main (int argc, char *argv[])
{
  OilFunctionClass *klass;
  OilFunctionImpl *impl;
  unsigned long cpu_flags;

  oil_init ();

  cpu_flags = oil_cpu_get_flags ();

  //register_impls();

  klass = oil_class_get ("abs_u16_s16");
  oil_class_optimize (klass);

  for (impl = klass->first_impl; impl; impl = impl->next) {
    if (((impl->flags & OIL_CPU_FLAG_MASK) & ~cpu_flags) == 0) {
      klass->chosen_impl = impl;
      klass->func = impl->func;
      g_print("impl %s %g %g\n", impl->name, impl->profile_ave,
          impl->profile_std);
      test();
    }
  }

  return 0;
}


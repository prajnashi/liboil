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
#include <glib.h>
#include <string.h>
#include <math.h>

void register_impls(void);

void test(void)
{
  uint8_t dest[100*4];
  uint8_t color[4];
  uint8_t alpha[100];
  int i;

  for(i=0;i<16;i++){
    dest[i*4+0] = 0;
    dest[i*4+1] = 0;
    dest[i*4+2] = 0;
    dest[i*4+3] = 255;
    alpha[i]=i*16;
  }
  color[0] = 255;
  color[1] = 128;
  color[2] = 10;
  color[3] = 128;

  oil_argb_paint_u8 (dest, color, alpha, 16);

  for(i=0;i<4*16;i+=4){
    g_print("%d %d %d %d\n",dest[i+0],dest[i+1],dest[i+2],dest[i+3]);
  }

}

int main (int argc, char *argv[])
{
  OilFunctionClass *klass;
  OilFunctionImpl *impl;

  oil_init ();

  register_impls();

  klass = oil_class_get ("argb_paint_u8");
  oil_class_optimize (klass);

  for (impl = klass->first_impl; impl; impl = impl->next) {
    klass->chosen_impl = impl;
    klass->func = impl->func;
    g_print("impl %s %g %g\n", impl->name, impl->profile_ave,
        impl->profile_std);
    test();
  }

  return 0;
}


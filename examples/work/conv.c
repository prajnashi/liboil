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
#include "liboil/conv/conv.h"


#define REGISTER_IMPL(name,klass) do { \
  static OilFunctionImpl impl = { \
    NULL, \
    NULL, \
    name, \
    0, \
    #name, \
    0, \
    0 \
  }; \
  oil_class_register_impl_by_name ( #klass , &impl); \
} while(0)


void
conv_s16_f32_asm1 (int16_t *dest, int dstr, float *src, int sstr, int n)
{
  int i;
  int16_t tmp[2];
  const float constants[][2] = {
    { -0.5, -0.5 },
    { -1.0, -1.0 }
  };

  for(i=0;i<n;i++){
    asm volatile(
        "  movq 0(%0), %%mm0 \n"
        "  pfadd 0(%2), %%mm0 \n"
        "  pf2id %%mm0, %%mm1 \n"
        "  pfcmpgt 0(%2), %%mm0 \n"
        "  psubd %%mm0, %%mm1 \n"
        "  movd %%mm1, 0(%1) \n"
        :
        : "r" (src), "r" (tmp), "r" (constants)
        : "mm0"
    );
    dest[0] = tmp[0];
    dest = OIL_OFFSET(dest, dstr);
    src = OIL_OFFSET(src, sstr);
  }
  asm volatile ("emms");
}

void
register_impls (void)
{
  REGISTER_IMPL(conv_s16_f32_asm1, conv_s16_f32);
}



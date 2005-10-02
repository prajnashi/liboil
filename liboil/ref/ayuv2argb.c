/*
 * LIBOIL - Library of Optimized Inner Loops
 * Copyright (c) 2003,2004 David A. Schleef <ds@schleef.org>
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

OIL_DEFINE_CLASS (ayuv2argb_u8, "uint8_t *d_4xn, uint8_t *s_4xn, int n");

#define clamp(x,a,b) clamp_lower(clamp_upper(x,b),a)
#define clamp_lower(x,a) ((x<a)?(a):(x))
#define clamp_upper(x,b) ((x>b)?(b):(x))

/* from the JFIF spec */
#define YUV_TO_R(y,u,v) clamp((y) + 1.402*((v)-128.0),0,255)
#define YUV_TO_G(y,u,v) clamp((y) - 0.34414*((u)-128.0) - 0.71414*((v)-128.0),0,255)
#define YUV_TO_B(y,u,v) clamp((y) + 1.772*((u)-128.0),0,255)

#define YUV_TO_R_INT(y,u,v) clamp(((y)*256 + 358*((v)-128))>>8,0,255)
#define YUV_TO_G_INT(y,u,v) clamp(((y)*256 - 88*((u)-128) - 183*((v)-128))>>8,0,255)
#define YUV_TO_B_INT(y,u,v) clamp(((y)*256 + 454*((u)-128))>>8,0,255)


static void
ayuv2argb_u8_ref (uint8_t *argb, const uint8_t *ayuv, int n)
{
  int i;

  for(i=0;i<n;i++){
    argb[0] = ayuv[0];
    argb[1] = YUV_TO_R(ayuv[1], ayuv[2], ayuv[3]);
    argb[2] = YUV_TO_G(ayuv[1], ayuv[2], ayuv[3]);
    argb[3] = YUV_TO_B(ayuv[1], ayuv[2], ayuv[3]);
    argb+=4;
    ayuv+=4;
  }

}
OIL_DEFINE_IMPL_REF (ayuv2argb_u8_ref, ayuv2argb_u8);


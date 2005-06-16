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

OIL_DECLARE_CLASS (ayuv2argb_u8);

#define clamp(x,a,b) clamp_lower(clamp_upper(x,b),a)
#define clamp_lower(x,a) ((x<a)?(a):(x))
#define clamp_upper(x,b) ((x>b)?(b):(x))

/* from the JFIF spec */
#define YUV_TO_R(y,u,v) clamp((y) + 1.402*((v)-128),0,255)
#define YUV_TO_G(y,u,v) clamp((y) - 0.34414*((u)-128) - 0.71414*((v)-128),0,255)
#define YUV_TO_B(y,u,v) clamp((y) + 1.772*((u)-128),0,255)

#define YUV_TO_R_INT(y,u,v) clamp(((y)*256 + 358*((v)-128))>>8,0,255)
#define YUV_TO_G_INT(y,u,v) clamp(((y)*256 - 88*((u)-128) - 183*((v)-128))>>8,0,255)
#define YUV_TO_B_INT(y,u,v) clamp(((y)*256 + 454*((u)-128))>>8,0,255)

static short constants[][4] = {
  { 0, 0, 128, 128 },
  { 128, 0, 0, 0 },
  { 0, 128, 128, 128 },
  { 0, 0, -44, 227 },
  { 0, 179, -91, 0 }
};

static void
ayuv2argb_u8_mmx (uint8_t *argb, uint8_t *ayuv, int n)
{
  asm volatile (
      "  pxor %%mm0, %%mm0\n"
      "1:\n"
      "  movq (%1), %%mm1\n"
      "  add $4, %1\n"
      "  punpcklbw %%mm0, %%mm1\n"
      "  psubw 0(%3), %%mm1\n"
      "  pshufw $0x55, %%mm1, %%mm2\n"
      "  pshufw $0xaa, %%mm1, %%mm3\n"
      "  pshufw $0xff, %%mm1, %%mm4\n"
      "  pshufw $0x00, %%mm1, %%mm1\n"
      "  pmullw 8(%3), %%mm1\n"
      "  pmullw 16(%3), %%mm2\n"
      "  pmullw 24(%3), %%mm3\n"
      "  pmullw 32(%3), %%mm4\n"
      "  paddsw %%mm2, %%mm1\n"
      "  paddsw %%mm4, %%mm3\n"
      "  paddsw %%mm3, %%mm1\n"
      "  psraw $7, %%mm1\n"
      "  packuswb %%mm0, %%mm1\n"
      "  movd %%mm1, (%0)\n"
      "  add $4, %0\n"
      "  decl %2\n"
      "  jne 1b\n"
      "  emms\n"
      : "+r" (argb), "+r" (ayuv), "+r" (n)
      : "r" (&constants));

}
OIL_DEFINE_IMPL_FULL (ayuv2argb_u8_mmx, ayuv2argb_u8, OIL_IMPL_FLAG_MMX|OIL_IMPL_FLAG_MMXEXT);


static short constants2[][4] = {
  { 0, 0, 128, 128 },
  { -1, 0, 0, 0 },
  { 0, -1, -1, -1 },
  { 0, 0, -44, 227 },
  { 0, 179, -91, 0 }
};


static void
ayuv2argb_u8_mmx2 (uint8_t *argb, uint8_t *ayuv, int n)
{
  asm volatile (
      "  pxor %%mm0, %%mm0\n"
      "1:\n"
      "  movq (%1), %%mm1\n"
      "  add $4, %1\n"
      "  punpcklbw %%mm0, %%mm1\n"
      "  psubw 0(%3), %%mm1\n"
      "  pshufw $0x55, %%mm1, %%mm2\n"
      "  pshufw $0xaa, %%mm1, %%mm3\n"
      "  pshufw $0xff, %%mm1, %%mm4\n"
      "  pshufw $0x00, %%mm1, %%mm1\n"
      "  pand 8(%3), %%mm1\n"
      "  pand 16(%3), %%mm2\n"
      "  pmullw 24(%3), %%mm3\n"
      "  pmullw 32(%3), %%mm4\n"
      "  paddsw %%mm4, %%mm3\n"
      "  psraw $7, %%mm3\n"
      "  paddsw %%mm2, %%mm1\n"
      "  paddsw %%mm3, %%mm1\n"
      "  packuswb %%mm0, %%mm1\n"
      "  movd %%mm1, (%0)\n"
      "  add $4, %0\n"
      "  decl %2\n"
      "  jne 1b\n"
      "  emms\n"
      : "+r" (argb), "+r" (ayuv), "+r" (n)
      : "r" (&constants2));

}

OIL_DEFINE_IMPL_FULL (ayuv2argb_u8_mmx2, ayuv2argb_u8, OIL_IMPL_FLAG_MMX|OIL_IMPL_FLAG_MMXEXT);

static void
ayuv2argb_u8_mmx3 (uint8_t *argb, uint8_t *ayuv, int n)
{
  asm volatile (
      "  pxor %%mm0, %%mm0\n"
      "  movq 8(%3), %%mm5\n"
      "  movq 16(%3), %%mm6\n"
      "  movq 24(%3), %%mm7\n"
      //"  movq 32(%3), %%mm8\n"
      "1:\n"
      "  movq (%1), %%mm1\n"
      "  add $4, %1\n"
      "  punpcklbw %%mm0, %%mm1\n"
      "  psubw 0(%3), %%mm1\n"
      "  pshufw $0x55, %%mm1, %%mm2\n"
      "  pshufw $0xaa, %%mm1, %%mm3\n"
      "  pshufw $0xff, %%mm1, %%mm4\n"
      "  pshufw $0x00, %%mm1, %%mm1\n"
      "  pand %%mm5, %%mm1\n"
      "  pand %%mm6, %%mm2\n"
      "  pmullw %%mm7, %%mm3\n"
      "  pmullw 32(%3), %%mm4\n"
      "  paddsw %%mm4, %%mm3\n"
      "  psraw $7, %%mm3\n"
      "  paddsw %%mm2, %%mm1\n"
      "  paddsw %%mm3, %%mm1\n"
      "  packuswb %%mm0, %%mm1\n"
      "  movd %%mm1, (%0)\n"
      "  add $4, %0\n"
      "  decl %2\n"
      "  jne 1b\n"
      "  emms\n"
      : "+r" (argb), "+r" (ayuv), "+r" (n)
      : "r" (&constants2));

}
OIL_DEFINE_IMPL_FULL (ayuv2argb_u8_mmx3, ayuv2argb_u8, OIL_IMPL_FLAG_MMX|OIL_IMPL_FLAG_MMXEXT);


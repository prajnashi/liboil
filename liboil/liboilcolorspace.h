/*
 * LIBOIL - Library of Optimized Inner Loops
 * Copyright (c) 2005 David A. Schleef <ds@schleef.org>
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

#ifndef _LIBOIL_COLORSPACE_H_
#define _LIBOIL_COLORSPACE_H_

#include <liboil/liboilrandom.h>

#define oil_max(x,y) ((x)>(y)?(x):(y))
#define oil_min(x,y) ((x)<(y)?(x):(y))

#define oil_clamp_255(x) oil_max(0,oil_min((x),255))

#define oil_argb(a,r,g,b) \
    ((oil_clamp_255(a)<<24) | \
     (oil_clamp_255(r)<<16) | \
     (oil_clamp_255(g)<<8) | \
     (oil_clamp_255(b)<<0))

#define oil_argb_noclamp(a,r,g,b) \
    (((a)<<24) | ((r)<<16) | ((g)<<8) | ((b)<<0))

#define oil_argb_A(color) (((color)>>24)&0xff)
#define oil_argb_R(color) (((color)>>16)&0xff)
#define oil_argb_G(color) (((color)>>8)&0xff)
#define oil_argb_B(color) (((color)>>0)&0xff)

#define oil_divide_255(x) ((((x)+128) + (((x)+128)>>8))>>8)
#define oil_muldiv_255(a,b) oil_divide_255((a)*(b))

#define oil_rand_rgba(a) \
    oil_argb_noclamp((a), \
      oil_muldiv_255((a),oil_rand_u8()), \
      oil_muldiv_255((a),oil_rand_u8()), \
      oil_muldiv_255((a),oil_rand_u8()))

#endif



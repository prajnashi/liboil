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


#define ARGB(a,r,g,b) (((a)<<24) | ((r)<<16) | ((g)<<8) | (b))
#define ARGB_A(color) (((color)>>24)&0xff)
#define ARGB_R(color) (((color)>>16)&0xff)
#define ARGB_G(color) (((color)>>8)&0xff)
#define ARGB_B(color) (((color)>>0)&0xff)

#define div255(x) (((x + 128) + ((x + 128)>>8))>>8)

#define COMPOSE_OVER(d,s,m) ((d) + (s) - div255((d)*(a)))
#define COMPOSE_ADD(d,s) ((d) + (s))
#define COMPOSE_IN(s,m) (div255((s)*(m)))

OIL_DEFINE_CLASS (composite_in_argb,
    "uint32_t *d_n, uint32_t *s1_n, uint8_t *s2_n, int n");
OIL_DEFINE_CLASS (composite_in_argb_const_src,
    "uint32_t *d_n, uint32_t *s1_1, uint8_t *s2_n, int n");
OIL_DEFINE_CLASS (composite_in_argb_const_mask,
    "uint32_t *d_n, uint32_t *s1_n, uint8_t *s2_1, int n");
OIL_DEFINE_CLASS (composite_over_argb,
    "uint32_t *i_n, uint32_t *s1_n, int n");
OIL_DEFINE_CLASS (composite_over_argb_const_src,
    "uint32_t *i_n, uint32_t *s1_1, int n");
OIL_DEFINE_CLASS (composite_add_argb,
    "uint32_t *i_n, uint32_t *s1_n, int n");
OIL_DEFINE_CLASS (composite_add_argb_const_src,
    "uint32_t *i_n, uint32_t *s1_1, int n");
OIL_DEFINE_CLASS (composite_in_over_argb,
    "uint32_t *i_n, uint32_t *s1_n, uint8_t *s2_n, int n");
OIL_DEFINE_CLASS (composite_in_over_argb_const_src,
    "uint32_t *i_n, uint32_t *s1_1, uint8_t *s2_n, int n");
OIL_DEFINE_CLASS (composite_in_over_argb_const_mask,
    "uint32_t *i_n, uint32_t *s1_n, uint8_t *s2_1, int n");

static void
composite_in_argb_ref (uint32_t *dest, uint32_t *src, uint8_t *mask, int n)
{
  int i;

  for(i=0;i<n;i++){
    dest[i] = ARGB(
        COMPOSE_IN(ARGB_A(src[i]), mask[i]),
        COMPOSE_IN(ARGB_R(src[i]), mask[i]),
        COMPOSE_IN(ARGB_G(src[i]), mask[i]),
        COMPOSE_IN(ARGB_B(src[i]), mask[i]));
  }
}
OIL_DEFINE_IMPL_REF (composite_in_argb_ref, composite_in_argb);

static void
composite_in_argb_const_src_ref (uint32_t *dest, uint32_t *src, uint8_t *mask, int n)
{
  int i;

  for(i=0;i<n;i++){
    dest[i] = ARGB(
        COMPOSE_IN(ARGB_A(src[0]), mask[i]),
        COMPOSE_IN(ARGB_R(src[0]), mask[i]),
        COMPOSE_IN(ARGB_G(src[0]), mask[i]),
        COMPOSE_IN(ARGB_B(src[0]), mask[i]));
  }
}
OIL_DEFINE_IMPL_REF (composite_in_argb_const_src_ref, composite_in_argb_const_src);

static void
composite_in_argb_const_mask_ref (uint32_t *dest, uint32_t *src, uint8_t *mask, int n)
{
  int i;

  for(i=0;i<n;i++){
    dest[i] = ARGB(
        COMPOSE_IN(ARGB_A(src[i]), mask[0]),
        COMPOSE_IN(ARGB_R(src[i]), mask[0]),
        COMPOSE_IN(ARGB_G(src[i]), mask[0]),
        COMPOSE_IN(ARGB_B(src[i]), mask[0]));
  }
}
OIL_DEFINE_IMPL_REF (composite_in_argb_const_mask_ref, composite_in_argb_const_mask);

static void
composite_over_argb_ref (uint32_t *dest, uint32_t *src, int n)
{
  int i;
  uint8_t a;

  for(i=0;i<n;i++){
    a = ARGB_A(src[i]);
    dest[i] = ARGB(
        COMPOSE_OVER(ARGB_A(dest[i]),ARGB_A(src[i]),a),
        COMPOSE_OVER(ARGB_R(dest[i]),ARGB_R(src[i]),a),
        COMPOSE_OVER(ARGB_G(dest[i]),ARGB_G(src[i]),a),
        COMPOSE_OVER(ARGB_B(dest[i]),ARGB_B(src[i]),a));
  }

}
OIL_DEFINE_IMPL_REF (composite_over_argb_ref, composite_over_argb);

static void
composite_over_argb_const_src_ref (uint32_t *dest, uint32_t *src, int n)
{
  int i;
  uint8_t a;

  a = ARGB_A(src[0]);
  for(i=0;i<n;i++){
    dest[i] = ARGB(
        COMPOSE_OVER(ARGB_A(dest[i]),ARGB_A(src[0]),a),
        COMPOSE_OVER(ARGB_R(dest[i]),ARGB_R(src[0]),a),
        COMPOSE_OVER(ARGB_G(dest[i]),ARGB_G(src[0]),a),
        COMPOSE_OVER(ARGB_B(dest[i]),ARGB_B(src[0]),a));
  }

}
OIL_DEFINE_IMPL_REF (composite_over_argb_const_src_ref, composite_over_argb_const_src);

static void
composite_add_argb_ref (uint32_t *dest, uint32_t *src, int n)
{
  int i;

  for(i=0;i<n;i++){
    dest[i] = ARGB(
        COMPOSE_ADD(ARGB_A(dest[i]),ARGB_A(src[i])),
        COMPOSE_ADD(ARGB_R(dest[i]),ARGB_R(src[i])),
        COMPOSE_ADD(ARGB_G(dest[i]),ARGB_G(src[i])),
        COMPOSE_ADD(ARGB_B(dest[i]),ARGB_B(src[i])));
  }

}
OIL_DEFINE_IMPL_REF (composite_add_argb_ref, composite_add_argb);

static void
composite_add_argb_const_src_ref (uint32_t *dest, uint32_t *src, int n)
{
  int i;

  for(i=0;i<n;i++){
    dest[i] = ARGB(
        COMPOSE_ADD(ARGB_A(dest[i]),ARGB_A(src[0])),
        COMPOSE_ADD(ARGB_R(dest[i]),ARGB_R(src[0])),
        COMPOSE_ADD(ARGB_G(dest[i]),ARGB_G(src[0])),
        COMPOSE_ADD(ARGB_B(dest[i]),ARGB_B(src[0])));
  }

}
OIL_DEFINE_IMPL_REF (composite_add_argb_const_src_ref, composite_add_argb_const_src);

static void
composite_in_over_argb_ref (uint32_t *dest, uint32_t *src, uint8_t *mask, int n)
{
  int i;
  uint8_t a;
  uint32_t color;

  for(i=0;i<n;i++){
    color = ARGB(
        COMPOSE_IN(ARGB_A(src[i]), mask[i]),
        COMPOSE_IN(ARGB_R(src[i]), mask[i]),
        COMPOSE_IN(ARGB_G(src[i]), mask[i]),
        COMPOSE_IN(ARGB_B(src[i]), mask[i]));
    a = ARGB_A(color);
    dest[i] = ARGB(
        COMPOSE_OVER(ARGB_A(dest[i]),ARGB_A(color),a),
        COMPOSE_OVER(ARGB_R(dest[i]),ARGB_R(color),a),
        COMPOSE_OVER(ARGB_G(dest[i]),ARGB_G(color),a),
        COMPOSE_OVER(ARGB_B(dest[i]),ARGB_B(color),a));
  }

}
OIL_DEFINE_IMPL_REF (composite_in_over_argb_ref, composite_in_over_argb);

static void
composite_in_over_argb_const_src_ref (uint32_t *dest, uint32_t *src, uint8_t *mask, int n)
{
  int i;
  uint8_t a;
  uint32_t color;

  for(i=0;i<n;i++){
    color = ARGB(
        COMPOSE_IN(ARGB_A(src[0]), mask[i]),
        COMPOSE_IN(ARGB_R(src[0]), mask[i]),
        COMPOSE_IN(ARGB_G(src[0]), mask[i]),
        COMPOSE_IN(ARGB_B(src[0]), mask[i]));
    a = ARGB_A(color);
    dest[i] = ARGB(
        COMPOSE_OVER(ARGB_A(dest[i]),ARGB_A(color),a),
        COMPOSE_OVER(ARGB_R(dest[i]),ARGB_R(color),a),
        COMPOSE_OVER(ARGB_G(dest[i]),ARGB_G(color),a),
        COMPOSE_OVER(ARGB_B(dest[i]),ARGB_B(color),a));
  }

}
OIL_DEFINE_IMPL_REF (composite_in_over_argb_const_src_ref, composite_in_over_argb_const_src);

static void
composite_in_over_argb_const_mask_ref (uint32_t *dest, uint32_t *src, uint8_t *mask, int n)
{
  int i;
  uint8_t a;
  uint32_t color;

  for(i=0;i<n;i++){
    color = ARGB(
        COMPOSE_IN(ARGB_A(src[i]), mask[0]),
        COMPOSE_IN(ARGB_R(src[i]), mask[0]),
        COMPOSE_IN(ARGB_G(src[i]), mask[0]),
        COMPOSE_IN(ARGB_B(src[i]), mask[0]));
    a = ARGB_A(color);
    dest[i] = ARGB(
        COMPOSE_OVER(ARGB_A(dest[i]),ARGB_A(color),a),
        COMPOSE_OVER(ARGB_R(dest[i]),ARGB_R(color),a),
        COMPOSE_OVER(ARGB_G(dest[i]),ARGB_G(color),a),
        COMPOSE_OVER(ARGB_B(dest[i]),ARGB_B(color),a));
  }

}
OIL_DEFINE_IMPL_REF (composite_in_over_argb_const_mask_ref, composite_in_over_argb_const_mask);


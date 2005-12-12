/*
 * Copyright (c) 2005
 *	Eric Anholt.  All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <liboil/liboilclasses.h>
#include <liboil/liboilfunction.h>
#include <liboil/simdpack/simdpack.h>

#if 0

static void
add_f32_sse (float *dest, float *src1, float *src2, int n)
{
  int i;

  /* Initial operations to align the destination pointer */
  for (i = 0; i < ((long)dest & 15) >> 2; i++) {
    *dest++ = *src1++ + *src2++;
  }
  for (; i < n - 3; i += 4) {
    asm volatile (
        "  movups (%0), %%xmm0 \n"
        "  movups (%1), %%xmm1 \n"
        "  addps %%xmm1, %%xmm0 \n"
        "  movaps %%xmm0, (%2) \n"
        : 
        : "r" (src1),  "r" (src2),  "r" (dest));
    dest += 4;
    src1 += 4;
    src2 += 4;
  }
  for(;i<n;i++){
    *dest++ = *src1++ + *src2++;
  }
}
OIL_DEFINE_IMPL_FULL (add_f32_sse, add_f32, OIL_IMPL_FLAG_SSE);

static void
subtract_f32_sse (float *dest, float *src1, float *src2, int n)
{
  int i;

  /* Initial operations to align the destination pointer */
  for (i = 0; i < ((long)dest & 15) >> 2; i++) {
    *dest++ = *src1++ - *src2++;
  }
  for (; i < n - 3; i += 4) {
    asm volatile (
        "  movups (%0), %%xmm0 \n"
        "  movups (%1), %%xmm1 \n"
        "  subps %%xmm1, %%xmm0 \n"
        "  movaps %%xmm0, (%2) \n"
        : 
        : "r" (src1),  "r" (src2),  "r" (dest));
    dest += 4;
    src1 += 4;
    src2 += 4;
  }
  for(;i<n;i++){
    *dest++ = *src1++ - *src2++;
  }
}
OIL_DEFINE_IMPL_FULL (subtract_f32_sse, subtract_f32, OIL_IMPL_FLAG_SSE);

static void
multiply_f32_sse (float *dest, float *src1, float *src2, int n)
{
  int i;

  /* Initial operations to align the destination pointer */
  for (i = 0; i < ((long)dest & 15) >> 2; i++) {
    *dest++ = *src1++ * *src2++;
  }
  for (i = 0; i < n - 3; i += 4) {
    asm volatile (
        "  movups (%0), %%xmm0 \n"
        "  movups (%1), %%xmm1 \n"
        "  mulps %%xmm1, %%xmm0 \n"
        "  movaps %%xmm0, (%2) \n"
        : 
        : "r" (src1),  "r" (src2),  "r" (dest));
    dest += 4;
    src1 += 4;
    src2 += 4;
  }
  for(;i<n;i++){
    *dest++ = *src1++ * *src2++;
  }
}
OIL_DEFINE_IMPL_FULL (multiply_f32_sse, multiply_f32, OIL_IMPL_FLAG_SSE);

static void
divide_f32_sse (float *dest, float *src1, float *src2, int n)
{
  int i;

  /* Initial operations to align the destination pointer */
  for (i = 0; i < ((long)dest & 15) >> 2; i++) {
    *dest++ = *src1++ / *src2++;
  }
  for (; i < n - 3; i += 4) {
    asm volatile (
        "  movups (%0), %%xmm0 \n"
        "  movups (%1), %%xmm1 \n"
        "  divps %%xmm1, %%xmm0 \n"
        "  movaps %%xmm0, (%2) \n"
        : 
        : "r" (src1),  "r" (src2),  "r" (dest));
    dest += 4;
    src1 += 4;
    src2 += 4;
  }
  for(;i<n;i++){
    *dest++ = *src1++ / *src2++;
  }
}
OIL_DEFINE_IMPL_FULL (divide_f32_sse, divide_f32, OIL_IMPL_FLAG_SSE);

static void
negative_f32_sse (float *dest, float *src1, int n)
{
  int i;

  /* Initial operations to align the destination pointer */
  for (i = 0; i < ((long)dest & 15) >> 2; i++) {
    *dest++ = -(*src1++);
  }
  for (; i < n - 3; i += 4) {
    asm volatile (
        "  xorps %%xmm0, %%xmm0 \n"
        "  movups (%0), %%xmm1 \n"
        "  subps %%xmm1, %%xmm0 \n"
        "  movaps %%xmm0, (%1) \n"
        : 
        : "r" (src1),  "r" (dest));
    dest += 4;
    src1 += 4;
  }
  for(;i<n;i++){
    *dest++ = -(*src1++);
  }
}
OIL_DEFINE_IMPL_FULL (negative_f32_sse, negative_f32, OIL_IMPL_FLAG_SSE);

static void
scalaradd_f32_ns_sse (float *dest, float *src1, float *val, int n)
{
  float tmp[8];
  float *valtmp = (void *)(((unsigned long)tmp & ~0xf) + 16);
  int i;

  valtmp[0] = *val;
  valtmp[1] = *val;
  valtmp[2] = *val;
  valtmp[3] = *val;
  /* Initial operations to align the destination pointer */
  for (i = 0; i < ((long)dest & 15) >> 2; i++) {
    *dest++ = *src1++ + *val;
  }
  for (; i < n - 3; i += 4) {
    asm volatile (
        "  movups (%0), %%xmm0 \n"
        "  movaps (%1), %%xmm1 \n"
        "  addps %%xmm1, %%xmm0 \n"
        "  movaps %%xmm0, (%2) \n"
        : 
        : "r" (src1),  "r" (valtmp),  "r" (dest));
    dest += 4;
    src1 += 4;
  }
  for(;i<n;i++){
    *dest++ = *src1++ + *val;
  }
}
OIL_DEFINE_IMPL_FULL (scalaradd_f32_ns_sse, scalaradd_f32_ns, OIL_IMPL_FLAG_SSE);

static void
scalarmultiply_f32_ns_sse (float *dest, float *src1, float *val, int n)
{
  float tmp[8];
  float *valtmp = (void *)(((unsigned long)tmp & ~0xf) + 16);
  int i;

  valtmp[0] = *val;
  valtmp[1] = *val;
  valtmp[2] = *val;
  valtmp[3] = *val;
  /* Initial operations to align the destination pointer */
  for (i = 0; i < ((long)dest & 15) >> 2; i++) {
    *dest++ = *src1++ * *val;
  }
  for (; i < n - 3; i += 4) {
    asm volatile (
        "  movups (%0), %%xmm0 \n"
        "  movaps (%1), %%xmm1 \n"
        "  mulps %%xmm1, %%xmm0 \n"
        "  movaps %%xmm0, (%2) \n"
        : 
        : "r" (src1),  "r" (valtmp),  "r" (dest));
    dest += 4;
    src1 += 4;
  }
  for(;i<n;i++){
    *dest++ = *src1++ * *val;
  }
}
OIL_DEFINE_IMPL_FULL (scalarmultiply_f32_ns_sse, scalarmultiply_f32_ns, OIL_IMPL_FLAG_SSE);
#endif

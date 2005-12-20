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
#include <emmintrin.h>
#include <xmmintrin.h>

static void
clamp_f32_sse (float *dest, const float *src1, int n, const float *src2_1,
    const float *src3_1)
{
  __m128 xmm1, xmm2;
  float min = *src2_1;
  float max = *src3_1;

  /* Initial operations to align the destination pointer */
  for (; ((long)dest & 15) && (n > 0); n--) {
    float x = *src1++;
    if (x < min)
      x = min;
    if (x > max)
      x = max;
    *dest++ = x;
  }
  xmm1 = _mm_set_ps1(min);
  xmm2 = _mm_set_ps1(max);
  for (; n >= 4; n -= 4) {
    __m128 xmm0;
    xmm0 = _mm_loadu_ps(src1);
    xmm0 = _mm_max_ps(xmm0, xmm1);
    xmm0 = _mm_min_ps(xmm0, xmm2);
    _mm_store_ps(dest, xmm0);
    dest += 4;
    src1 += 4;
  }
  for (; n > 0; n--) {
    float x = *src1++;
    if (x < min)
      x = min;
    if (x > max)
      x = max;
    *dest++ = x;
  }
}
OIL_DEFINE_IMPL_FULL (clamp_f32_sse, clamp_f32, OIL_IMPL_FLAG_SSE);

static void
clamp_f64_sse (double *dest, const double *src1, int n, const double *src2_1,
    const double *src3_1)
{
  __m128d xmm1, xmm2;
  double min = *src2_1;
  double max = *src3_1;

  /* Initial operations to align the destination pointer */
  for (; ((long)dest & 15) && (n > 0); n--) {
    double x = *src1++;
    if (x < min)
      x = min;
    if (x > max)
      x = max;
    *dest++ = x;
  }
  xmm1 = _mm_set_pd1(min);
  xmm2 = _mm_set_pd1(max);
  for (; n >= 2; n -= 2) {
    __m128d xmm0;
    xmm0 = _mm_loadu_pd(src1);
    xmm0 = _mm_max_pd(xmm0, xmm1);
    xmm0 = _mm_min_pd(xmm0, xmm2);
    _mm_store_pd(dest, xmm0);
    dest += 2;
    src1 += 2;
  }
  for (; n > 0; n--) {
    double x = *src1++;
    if (x < min)
      x = min;
    if (x > max)
      x = max;
    *dest++ = x;
  }
}
OIL_DEFINE_IMPL_FULL (clamp_f64_sse, clamp_f64,
    OIL_IMPL_FLAG_SSE | OIL_IMPL_FLAG_SSE2);

static void
clamplow_f32_sse (float *dest, const float *src1, int n, const float *src2_1)
{
  __m128 xmm1;
  float min = *src2_1;

  /* Initial operations to align the destination pointer */
  for (; ((long)dest & 15) && (n > 0); n--) {
    float x = *src1++;
    if (x < min)
      x = min;
    *dest++ = x;
  }
  xmm1 = _mm_set_ps1(min);
  for (; n >= 4; n -= 4) {
    __m128 xmm0;
    xmm0 = _mm_loadu_ps(src1);
    xmm0 = _mm_max_ps(xmm0, xmm1);
    _mm_store_ps(dest, xmm0);
    dest += 4;
    src1 += 4;
  }
  for (; n > 0; n--) {
    float x = *src1++;
    if (x < min)
      x = min;
    *dest++ = x;
  }
}
OIL_DEFINE_IMPL_FULL (clamplow_f32_sse, clamplow_f32, OIL_IMPL_FLAG_SSE);

static void
clamplow_f64_sse (double *dest, const double *src1, int n, const double *src2_1)
{
  __m128d xmm1;
  double min = *src2_1;

  /* Initial operations to align the destination pointer */
  for (; ((long)dest & 15) && (n > 0); n--) {
    double x = *src1++;
    if (x < min)
      x = min;
    *dest++ = x;
  }
  xmm1 = _mm_set_pd1(min);
  for (; n >= 2; n -= 2) {
    __m128d xmm0;
    xmm0 = _mm_loadu_pd(src1);
    xmm0 = _mm_max_pd(xmm0, xmm1);
    _mm_store_pd(dest, xmm0);
    dest += 2;
    src1 += 2;
  }
  for (; n > 0; n--) {
    double x = *src1++;
    if (x < min)
      x = min;
    *dest++ = x;
  }
}
OIL_DEFINE_IMPL_FULL (clamplow_f64_sse, clamplow_f64,
    OIL_IMPL_FLAG_SSE | OIL_IMPL_FLAG_SSE2);

static void
clamphigh_f32_sse (float *dest, const float *src1, int n, const float *src2_1)
{
  __m128 xmm1;
  float max = *src2_1;

  /* Initial operations to align the destination pointer */
  for (; ((long)dest & 15) && (n > 0); n--) {
    float x = *src1++;
    if (x > max)
      x = max;
    *dest++ = x;
  }
  xmm1 = _mm_set_ps1(max);
  for (; n >= 4; n -= 4) {
    __m128 xmm0;
    xmm0 = _mm_loadu_ps(src1);
    xmm0 = _mm_min_ps(xmm0, xmm1);
    _mm_store_ps(dest, xmm0);
    dest += 4;
    src1 += 4;
  }
  for (; n > 0; n--) {
    float x = *src1++;
    if (x > max)
      x = max;
    *dest++ = x;
  }
}
OIL_DEFINE_IMPL_FULL (clamphigh_f32_sse, clamphigh_f32, OIL_IMPL_FLAG_SSE);

static void
clamphigh_f64_sse (double *dest, const double *src1, int n, const double *src2_1)
{
  __m128d xmm1;
  double max = *src2_1;

  /* Initial operations to align the destination pointer */
  for (; ((long)dest & 15) && (n > 0); n--) {
    double x = *src1++;
    if (x > max)
      x = max;
    *dest++ = x;
  }
  xmm1 = _mm_set_pd1(max);
  for (; n >= 2; n -= 2) {
    __m128d xmm0;
    xmm0 = _mm_loadu_pd(src1);
    xmm0 = _mm_min_pd(xmm0, xmm1);
    _mm_store_pd(dest, xmm0);
    dest += 2;
    src1 += 2;
  }
  for (; n > 0; n--) {
    double x = *src1++;
    if (x > max)
      x = max;
    *dest++ = x;
  }
}
OIL_DEFINE_IMPL_FULL (clamphigh_f64_sse, clamphigh_f64,
    OIL_IMPL_FLAG_SSE | OIL_IMPL_FLAG_SSE2);

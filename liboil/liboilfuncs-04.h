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

/* This file is automatically generated.  Do not edit. */

#ifndef _LIBOIL_FUNCS_H_
#define _LIBOIL_FUNCS_H_

#include <liboil/liboiltypes.h>

#ifdef __cplusplus
extern "C" {
#endif

void oil_abs_f32_f32 (float * dest, int dstr, const float * src, int sstr, int n);
void oil_abs_f64_f64 (double * dest, int dstr, const double * src, int sstr, int n);
void oil_abs_u16_s16 (uint16_t * dest, int dstr, const int16_t * src, int sstr, int n);
void oil_abs_u32_s32 (uint32_t * dest, int dstr, const int32_t * src, int sstr, int n);
void oil_abs_u8_s8 (uint8_t * dest, int dstr, const int8_t * src, int sstr, int n);
void oil_add_const_rshift_s16 (int16_t * d1, const int16_t * s1, const int16_t * s2_2, int n);
void oil_add_f32 (float * d, const float * s1, const float * s2, int n);
void oil_add_f64 (double * d, const double * s1, const double * s2, int n);
void oil_add_s16 (int16_t * d, const int16_t * src1, const int16_t * src2, int n);
void oil_add_s16_u8 (int16_t * d, const int16_t * src1, const uint8_t * src2, int n);
void oil_argb_paint_u8 (uint8_t * i_4xn, const uint8_t * s1_4, const uint8_t * s2_n, int n);
void oil_average2_u8 (uint8_t * d, int dstr, const uint8_t * s1, int sstr1, const uint8_t * s2, int sstr2, int n);
void oil_ayuv2argb_u8 (uint8_t * d_4xn, const uint8_t * s_4xn, int n);
void oil_ayuv2uyvy (uint32_t * d_n, const uint32_t * s_n, int n);
void oil_ayuv2yuyv (uint32_t * d_n, const uint32_t * s_n, int n);
void oil_ayuv2yvyu (uint32_t * d_n, const uint32_t * s_n, int n);
void oil_clamp_f32 (float * dest, const float * src, int n, const float * s2_1, const float * s3_1);
void oil_clamp_f64 (double * dest, const double * src, int n, const double * s2_1, const double * s3_1);
void oil_clamp_s16 (int16_t * dest, const int16_t * src, int n, const int16_t * s2_1, const int16_t * s3_1);
void oil_clamp_s32 (int32_t * dest, const int32_t * src, int n, const int32_t * s2_1, const int32_t * s3_1);
void oil_clamp_s8 (int8_t * dest, const int8_t * src, int n, const int8_t * s2_1, const int8_t * s3_1);
void oil_clamp_u16 (uint16_t * dest, const uint16_t * src, int n, const uint16_t * s2_1, const uint16_t * s3_1);
void oil_clamp_u32 (uint32_t * dest, const uint32_t * src, int n, const uint32_t * s2_1, const uint32_t * s3_1);
void oil_clamp_u8 (uint8_t * dest, const uint8_t * src, int n, const uint8_t * s2_1, const uint8_t * s3_1);
void oil_clamphigh_f32 (float * dest, const float * src, int n, const float * s2_1);
void oil_clamphigh_f64 (double * dest, const double * src, int n, const double * s2_1);
void oil_clamphigh_s16 (int16_t * dest, const int16_t * src, int n, const int16_t * s2_1);
void oil_clamphigh_s32 (int32_t * dest, const int32_t * src, int n, const int32_t * s2_1);
void oil_clamphigh_s8 (int8_t * dest, const int8_t * src, int n, const int8_t * s2_1);
void oil_clamphigh_u16 (uint16_t * dest, const uint16_t * src, int n, const uint16_t * s2_1);
void oil_clamphigh_u32 (uint32_t * dest, const uint32_t * src, int n, const uint32_t * s2_1);
void oil_clamphigh_u8 (uint8_t * dest, const uint8_t * src, int n, const uint8_t * s2_1);
void oil_clamplow_f32 (float * dest, const float * src, int n, const float * s2_1);
void oil_clamplow_f64 (double * dest, const double * src, int n, const double * s2_1);
void oil_clamplow_s16 (int16_t * dest, const int16_t * src, int n, const int16_t * s2_1);
void oil_clamplow_s32 (int32_t * dest, const int32_t * src, int n, const int32_t * s2_1);
void oil_clamplow_s8 (int8_t * dest, const int8_t * src, int n, const int8_t * s2_1);
void oil_clamplow_u16 (uint16_t * dest, const uint16_t * src, int n, const uint16_t * s2_1);
void oil_clamplow_u32 (uint32_t * dest, const uint32_t * src, int n, const uint32_t * s2_1);
void oil_clamplow_u8 (uint8_t * dest, const uint8_t * src, int n, const uint8_t * s2_1);
void oil_clip_f32 (float * dest, int dstr, const float * src, int sstr, int n, const float * s2_1, const float * s3_1);
void oil_clip_f64 (double * dest, int dstr, const double * src, int sstr, int n, const double * s2_1, const double * s3_1);
void oil_clip_s16 (int16_t * dest, int dstr, const int16_t * src, int sstr, int n, const int16_t * s2_1, const int16_t * s3_1);
void oil_clip_s32 (int32_t * dest, int dstr, const int32_t * src, int sstr, int n, const int32_t * s2_1, const int32_t * s3_1);
void oil_clip_s8 (int8_t * dest, int dstr, const int8_t * src, int sstr, int n, const int8_t * s2_1, const int8_t * s3_1);
void oil_clip_u16 (uint16_t * dest, int dstr, const uint16_t * src, int sstr, int n, const uint16_t * s2_1, const uint16_t * s3_1);
void oil_clip_u32 (uint32_t * dest, int dstr, const uint32_t * src, int sstr, int n, const uint32_t * s2_1, const uint32_t * s3_1);
void oil_clip_u8 (uint8_t * dest, int dstr, const uint8_t * src, int sstr, int n, const uint8_t * s2_1, const uint8_t * s3_1);
void oil_clipconv8x8_u8_s16 (uint8_t * d_8x8, int dstr, const int16_t * s_8x8, int sstr);
void oil_clipconv_s16_f32 (int16_t * dest, int dstr, const float * src, int sstr, int n);
void oil_clipconv_s16_f64 (int16_t * dest, int dstr, const double * src, int sstr, int n);
void oil_clipconv_s16_s32 (int16_t * dest, int dstr, const int32_t * src, int sstr, int n);
void oil_clipconv_s16_u16 (int16_t * dest, int dstr, const uint16_t * src, int sstr, int n);
void oil_clipconv_s16_u32 (int16_t * dest, int dstr, const uint32_t * src, int sstr, int n);
void oil_clipconv_s32_f32 (int32_t * dest, int dstr, const float * src, int sstr, int n);
void oil_clipconv_s32_f64 (int32_t * dest, int dstr, const double * src, int sstr, int n);
void oil_clipconv_s32_u32 (int32_t * dest, int dstr, const uint32_t * src, int sstr, int n);
void oil_clipconv_s8_f32 (int8_t * dest, int dstr, const float * src, int sstr, int n);
void oil_clipconv_s8_f64 (int8_t * dest, int dstr, const double * src, int sstr, int n);
void oil_clipconv_s8_s16 (int8_t * dest, int dstr, const int16_t * src, int sstr, int n);
void oil_clipconv_s8_s32 (int8_t * dest, int dstr, const int32_t * src, int sstr, int n);
void oil_clipconv_s8_u16 (int8_t * dest, int dstr, const uint16_t * src, int sstr, int n);
void oil_clipconv_s8_u32 (int8_t * dest, int dstr, const uint32_t * src, int sstr, int n);
void oil_clipconv_s8_u8 (int8_t * dest, int dstr, const uint8_t * src, int sstr, int n);
void oil_clipconv_u16_f32 (uint16_t * dest, int dstr, const float * src, int sstr, int n);
void oil_clipconv_u16_f64 (uint16_t * dest, int dstr, const double * src, int sstr, int n);
void oil_clipconv_u16_s16 (uint16_t * dest, int dstr, const int16_t * src, int sstr, int n);
void oil_clipconv_u16_s32 (uint16_t * dest, int dstr, const int32_t * src, int sstr, int n);
void oil_clipconv_u16_u32 (uint16_t * dest, int dstr, const uint32_t * src, int sstr, int n);
void oil_clipconv_u32_f32 (uint32_t * dest, int dstr, const float * src, int sstr, int n);
void oil_clipconv_u32_f64 (uint32_t * dest, int dstr, const double * src, int sstr, int n);
void oil_clipconv_u32_s32 (uint32_t * dest, int dstr, const int32_t * src, int sstr, int n);
void oil_clipconv_u8_f32 (uint8_t * dest, int dstr, const float * src, int sstr, int n);
void oil_clipconv_u8_f64 (uint8_t * dest, int dstr, const double * src, int sstr, int n);
void oil_clipconv_u8_s16 (uint8_t * dest, int dstr, const int16_t * src, int sstr, int n);
void oil_clipconv_u8_s32 (uint8_t * dest, int dstr, const int32_t * src, int sstr, int n);
void oil_clipconv_u8_s8 (uint8_t * dest, int dstr, const int8_t * src, int sstr, int n);
void oil_clipconv_u8_u16 (uint8_t * dest, int dstr, const uint16_t * src, int sstr, int n);
void oil_clipconv_u8_u32 (uint8_t * dest, int dstr, const uint32_t * src, int sstr, int n);
void oil_colorspace_argb (uint32_t * d, const uint32_t * s, const int16_t * s2_24, int n);
void oil_colsad8x8_u8 (uint32_t * d_1, const uint8_t * s1_8x8, int ss1, const uint8_t * s2_8x8, int ss2);
void oil_compare_u8 (uint32_t * d_1, const uint8_t * s1, const uint8_t * s2, int n);
void oil_composite_add_argb (uint32_t * i_n, const uint32_t * s1_n, int n);
void oil_composite_add_argb_const_src (uint32_t * i_n, const uint32_t * s1_1, int n);
void oil_composite_add_u8 (uint8_t * i_n, const uint8_t * s1_n, int n);
void oil_composite_add_u8_const_src (uint8_t * i_n, const uint8_t * s1_1, int n);
void oil_composite_in_argb (uint32_t * d_n, const uint32_t * s1_n, const uint8_t * s2_n, int n);
void oil_composite_in_argb_const_mask (uint32_t * d_n, const uint32_t * s1_n, const uint8_t * s2_1, int n);
void oil_composite_in_argb_const_src (uint32_t * d_n, const uint32_t * s1_1, const uint8_t * s2_n, int n);
void oil_composite_in_over_argb (uint32_t * i_n, const uint32_t * s1_n, const uint8_t * s2_n, int n);
void oil_composite_in_over_argb_const_mask (uint32_t * i_n, const uint32_t * s1_n, const uint8_t * s2_1, int n);
void oil_composite_in_over_argb_const_src (uint32_t * i_n, const uint32_t * s1_1, const uint8_t * s2_n, int n);
void oil_composite_over_argb (uint32_t * i_n, const uint32_t * s1_n, int n);
void oil_composite_over_argb_const_src (uint32_t * i_n, const uint32_t * s1_1, int n);
void oil_composite_over_u8 (uint8_t * i_n, const uint8_t * s1_n, int n);
void oil_conv8x8_f64_s16 (double * d_8x8, int dstr, const int16_t * s_8x8, int sstr);
void oil_conv8x8_s16_f64 (int16_t * d_8x8, int dstr, const double * s_8x8, int sstr);
void oil_conv_f32_f64 (float * dest, int dstr, const double * src, int sstr, int n);
void oil_conv_f32_s16 (float * dest, int dstr, const int16_t * src, int sstr, int n);
void oil_conv_f32_s32 (float * dest, int dstr, const int32_t * src, int sstr, int n);
void oil_conv_f32_s8 (float * dest, int dstr, const int8_t * src, int sstr, int n);
void oil_conv_f32_u16 (float * dest, int dstr, const uint16_t * src, int sstr, int n);
void oil_conv_f32_u32 (float * dest, int dstr, const uint32_t * src, int sstr, int n);
void oil_conv_f32_u8 (float * dest, int dstr, const uint8_t * src, int sstr, int n);
void oil_conv_f64_f32 (double * dest, int dstr, const float * src, int sstr, int n);
void oil_conv_f64_s16 (double * dest, int dstr, const int16_t * src, int sstr, int n);
void oil_conv_f64_s32 (double * dest, int dstr, const int32_t * src, int sstr, int n);
void oil_conv_f64_s8 (double * dest, int dstr, const int8_t * src, int sstr, int n);
void oil_conv_f64_u16 (double * dest, int dstr, const uint16_t * src, int sstr, int n);
void oil_conv_f64_u32 (double * dest, int dstr, const uint32_t * src, int sstr, int n);
void oil_conv_f64_u8 (double * dest, int dstr, const uint8_t * src, int sstr, int n);
void oil_conv_s16_f32 (int16_t * dest, int dstr, const float * src, int sstr, int n);
void oil_conv_s16_f64 (int16_t * dest, int dstr, const double * src, int sstr, int n);
void oil_conv_s16_s32 (int16_t * dest, int dstr, const int32_t * src, int sstr, int n);
void oil_conv_s16_s8 (int16_t * dest, int dstr, const int8_t * src, int sstr, int n);
void oil_conv_s16_u16 (int16_t * dest, int dstr, const uint16_t * src, int sstr, int n);
void oil_conv_s16_u32 (int16_t * dest, int dstr, const uint32_t * src, int sstr, int n);
void oil_conv_s16_u8 (int16_t * dest, int dstr, const uint8_t * src, int sstr, int n);
void oil_conv_s32_f32 (int32_t * dest, int dstr, const float * src, int sstr, int n);
void oil_conv_s32_f64 (int32_t * dest, int dstr, const double * src, int sstr, int n);
void oil_conv_s32_s16 (int32_t * dest, int dstr, const int16_t * src, int sstr, int n);
void oil_conv_s32_s8 (int32_t * dest, int dstr, const int8_t * src, int sstr, int n);
void oil_conv_s32_u16 (int32_t * dest, int dstr, const uint16_t * src, int sstr, int n);
void oil_conv_s32_u32 (int32_t * dest, int dstr, const uint32_t * src, int sstr, int n);
void oil_conv_s32_u8 (int32_t * dest, int dstr, const uint8_t * src, int sstr, int n);
void oil_conv_s8_f32 (int8_t * dest, int dstr, const float * src, int sstr, int n);
void oil_conv_s8_f64 (int8_t * dest, int dstr, const double * src, int sstr, int n);
void oil_conv_s8_s16 (int8_t * dest, int dstr, const int16_t * src, int sstr, int n);
void oil_conv_s8_s32 (int8_t * dest, int dstr, const int32_t * src, int sstr, int n);
void oil_conv_s8_u16 (int8_t * dest, int dstr, const uint16_t * src, int sstr, int n);
void oil_conv_s8_u32 (int8_t * dest, int dstr, const uint32_t * src, int sstr, int n);
void oil_conv_s8_u8 (int8_t * dest, int dstr, const uint8_t * src, int sstr, int n);
void oil_conv_u16_f32 (uint16_t * dest, int dstr, const float * src, int sstr, int n);
void oil_conv_u16_f64 (uint16_t * dest, int dstr, const double * src, int sstr, int n);
void oil_conv_u16_s16 (uint16_t * dest, int dstr, const int16_t * src, int sstr, int n);
void oil_conv_u16_s32 (uint16_t * dest, int dstr, const int32_t * src, int sstr, int n);
void oil_conv_u16_s8 (uint16_t * dest, int dstr, const int8_t * src, int sstr, int n);
void oil_conv_u16_u32 (uint16_t * dest, int dstr, const uint32_t * src, int sstr, int n);
void oil_conv_u16_u8 (uint16_t * dest, int dstr, const uint8_t * src, int sstr, int n);
void oil_conv_u32_f32 (uint32_t * dest, int dstr, const float * src, int sstr, int n);
void oil_conv_u32_f64 (uint32_t * dest, int dstr, const double * src, int sstr, int n);
void oil_conv_u32_s16 (uint32_t * dest, int dstr, const int16_t * src, int sstr, int n);
void oil_conv_u32_s32 (uint32_t * dest, int dstr, const int32_t * src, int sstr, int n);
void oil_conv_u32_s8 (uint32_t * dest, int dstr, const int8_t * src, int sstr, int n);
void oil_conv_u32_u16 (uint32_t * dest, int dstr, const uint16_t * src, int sstr, int n);
void oil_conv_u32_u8 (uint32_t * dest, int dstr, const uint8_t * src, int sstr, int n);
void oil_conv_u8_f32 (uint8_t * dest, int dstr, const float * src, int sstr, int n);
void oil_conv_u8_f64 (uint8_t * dest, int dstr, const double * src, int sstr, int n);
void oil_conv_u8_s16 (uint8_t * dest, int dstr, const int16_t * src, int sstr, int n);
void oil_conv_u8_s32 (uint8_t * dest, int dstr, const int32_t * src, int sstr, int n);
void oil_conv_u8_s8 (uint8_t * dest, int dstr, const int8_t * src, int sstr, int n);
void oil_conv_u8_u16 (uint8_t * dest, int dstr, const uint16_t * src, int sstr, int n);
void oil_conv_u8_u32 (uint8_t * dest, int dstr, const uint32_t * src, int sstr, int n);
void oil_convert_s16_f32 (int16_t * dest, const float * src, int n);
void oil_convert_s16_f64 (int16_t * dest, const double * src, int n);
void oil_convert_s16_s32 (int16_t * dest, const int32_t * src, int n);
void oil_convert_s16_s8 (int16_t * dest, const int8_t * src, int n);
void oil_convert_s16_u16 (int16_t * dest, const uint16_t * src, int n);
void oil_convert_s16_u32 (int16_t * dest, const uint32_t * src, int n);
void oil_convert_s16_u8 (int16_t * dest, const uint8_t * src, int n);
void oil_convert_s32_f64 (int32_t * dest, const double * src, int n);
void oil_convert_s32_s16 (int32_t * dest, const int16_t * src, int n);
void oil_convert_s32_s8 (int32_t * dest, const int8_t * src, int n);
void oil_convert_s32_u16 (int32_t * dest, const uint16_t * src, int n);
void oil_convert_s32_u32 (int32_t * dest, const uint32_t * src, int n);
void oil_convert_s32_u8 (int32_t * dest, const uint8_t * src, int n);
void oil_convert_s8_f32 (int8_t * dest, const float * src, int n);
void oil_convert_s8_f64 (int8_t * dest, const double * src, int n);
void oil_convert_s8_s16 (int8_t * dest, const int16_t * src, int n);
void oil_convert_s8_s32 (int8_t * dest, const int32_t * src, int n);
void oil_convert_s8_u16 (int8_t * dest, const uint16_t * src, int n);
void oil_convert_s8_u32 (int8_t * dest, const uint32_t * src, int n);
void oil_convert_s8_u8 (int8_t * dest, const uint8_t * src, int n);
void oil_convert_u16_f32 (uint16_t * dest, const float * src, int n);
void oil_convert_u16_f64 (uint16_t * dest, const double * src, int n);
void oil_convert_u16_s16 (uint16_t * dest, const int16_t * src, int n);
void oil_convert_u16_s32 (uint16_t * dest, const int32_t * src, int n);
void oil_convert_u16_u32 (uint16_t * dest, const uint32_t * src, int n);
void oil_convert_u16_u8 (uint16_t * dest, const uint8_t * src, int n);
void oil_convert_u32_f64 (uint32_t * dest, const double * src, int n);
void oil_convert_u32_s32 (uint32_t * dest, const int32_t * src, int n);
void oil_convert_u32_u16 (uint32_t * dest, const uint16_t * src, int n);
void oil_convert_u32_u8 (uint32_t * dest, const uint8_t * src, int n);
void oil_convert_u8_f32 (uint8_t * dest, const float * src, int n);
void oil_convert_u8_f64 (uint8_t * dest, const double * src, int n);
void oil_convert_u8_s16 (uint8_t * dest, const int16_t * src, int n);
void oil_convert_u8_s32 (uint8_t * dest, const int32_t * src, int n);
void oil_convert_u8_s8 (uint8_t * dest, const int8_t * src, int n);
void oil_convert_u8_u16 (uint8_t * dest, const uint16_t * src, int n);
void oil_convert_u8_u32 (uint8_t * dest, const uint32_t * src, int n);
void oil_copy8x8_u8 (uint8_t * d_8x8, int ds, const uint8_t * s_8x8, int ss);
void oil_copy_u8 (uint8_t * dest, const uint8_t * src, int n);
void oil_dct36_f32 (float * d_36, int dstr, const float * s_36, int sstr);
void oil_deinterleave (int16_t * d_2xn, const int16_t * s_2xn, int n);
void oil_deinterleave2_s16 (int16_t * d1_n, int16_t * d2_n, const int16_t * s_2xn, int n);
void oil_dequantize8x8_s16 (int16_t * d_8x8, int dstr, const int16_t * s1_8x8, int sstr1, const int16_t * s2_8x8, int sstr2);
void oil_diff8x8_average_s16_u8 (int16_t * d_8x8, const uint8_t * s1_8x8, int ss1, const uint8_t * s2_8x8, int ss2, const uint8_t * s3_8x8, int ss3);
void oil_diff8x8_const128_s16_u8 (int16_t * d_8x8, const uint8_t * s1_8x8, int ss1);
void oil_diff8x8_s16_u8 (int16_t * d_8x8, const uint8_t * s1_8x8, int ss1, const uint8_t * s2_8x8, int ss2);
void oil_diffsquaresum_f64 (double * d_1, const double * src1, int sstr1, const double * src2, int sstr2, int n);
void oil_divide_f32 (float * d, const float * s1, const float * s2, int n);
void oil_divide_f64 (double * d, const double * s1, const double * s2, int n);
void oil_err_inter8x8_u8 (uint32_t * d_1, const uint8_t * s1_8x8, int ss1, const uint8_t * s2_8x8, int ss2);
void oil_err_inter8x8_u8_avg (uint32_t * d_1, const uint8_t * s1_8x8, int ss1, const uint8_t * s2_8x8, const uint8_t * s3_8x8, int ss2);
void oil_err_intra8x8_u8 (uint32_t * d_1, const uint8_t * s1_8x8, int ss1);
void oil_fdct8_f64 (double * d_8, const double * s_8, int dstr, int sstr);
void oil_fdct8x8_f64 (double * d_8x8, int dstr, const double * s_8x8, int sstr);
void oil_fdct8x8s_s16 (int16_t * d_8x8, int ds, const int16_t * s_8x8, int ss);
void oil_fdct8x8theora (const int16_t * s_8x8, int16_t * d_8x8);
void oil_floor_f32 (float * d, const float * s, int n);
void oil_idct8_f64 (double * d_8, int dstr, const double * s_8, int sstr);
void oil_idct8theora_s16 (int16_t * d_8, int dstr, const int16_t * s_8, int sstr);
void oil_idct8x8_f64 (double * d_8x8, int dstr, const double * s_8x8, int sstr);
void oil_idct8x8_s16 (int16_t * d_8x8, int dstr, const int16_t * s_8x8, int sstr);
void oil_idct8x8lim10_f64 (double * d_8x8, int dstr, const double * s_8x8, int sstr);
void oil_idct8x8lim10_s16 (int16_t * d_8x8, int dstr, const int16_t * s_8x8, int sstr);
void oil_idct8x8theora_s16 (int16_t * d_8x8, int dstr, const int16_t * s_8x8, int sstr);
void oil_imdct12_f64 (double * d_12, const double * s_6);
void oil_imdct32_f32 (float * d_32, const float * s_32);
void oil_imdct36_f64 (double * d_36, const double * s_18);
void oil_interleave (int16_t * d_2xn, const int16_t * s_2xn, int n);
void oil_interleave2_s16 (int16_t * d_2xn, const int16_t * s1_n, const int16_t * s2_n, int n);
void oil_inverse_f32 (float * d, const float * s, int n);
void oil_lift_add_135 (int16_t * d, const int16_t * s1, const int16_t * s2, const int16_t * s3, const int16_t * s4, const int16_t * s5, int n);
void oil_lift_add_mult_shift12 (int16_t * d, const int16_t * s1, const int16_t * s2, const int16_t * s3, const int16_t * s4_1, int n);
void oil_lift_add_shift1 (int16_t * d, const int16_t * s1, const int16_t * s2, const int16_t * s3, int n);
void oil_lift_add_shift2 (int16_t * d, const int16_t * s1, const int16_t * s2, const int16_t * s3, int n);
void oil_lift_sub_135 (int16_t * d, const int16_t * s1, const int16_t * s2, const int16_t * s3, const int16_t * s4, const int16_t * s5, int n);
void oil_lift_sub_mult_shift12 (int16_t * d, const int16_t * s1, const int16_t * s2, const int16_t * s3, const int16_t * s4_1, int n);
void oil_lift_sub_shift1 (int16_t * d, const int16_t * s1, const int16_t * s2, const int16_t * s3, int n);
void oil_lift_sub_shift2 (int16_t * d, const int16_t * s1, const int16_t * s2, const int16_t * s3, int n);
void oil_lshift_s16 (int16_t * d1, const int16_t * s1, const int16_t * s2_1, int n);
void oil_mas2_across_add_s16 (int16_t * d, const int16_t * s1, const int16_t * s2, const int16_t * s3, const int16_t * s4_2, const int16_t * s5_2, int n);
void oil_mas2_add_s16 (int16_t * d, const int16_t * s1, const int16_t * s2_np1, const int16_t * s3_2, const int16_t * s4_2, int n);
void oil_mas4_across_add_s16 (int16_t * d, const int16_t * s1, const int16_t * s2_nx4, int sstr2, const int16_t * s3_4, const int16_t * s4_2, int n);
void oil_mas4_add_s16 (int16_t * d, const int16_t * s1, const int16_t * s2_np3, const int16_t * s3_4, const int16_t * s4_2, int n);
void oil_mas8_across_add_s16 (int16_t * d, const int16_t * s1, const int16_t * s2_nx8, int sstr2, const int16_t * s3_8, const int16_t * s4_2, int n);
void oil_mas8_add_s16 (int16_t * d, const int16_t * s1, const int16_t * s2_np7, const int16_t * s3_8, const int16_t * s4_2, int n);
void oil_max_f32 (float * d, const float * s1, int n);
void oil_max_f64 (double * d, const double * s1, int n);
void oil_maximum_f32 (float * d, const float * s1, const float * s2, int n);
void oil_md5 (uint32_t * i_4, const uint32_t * s_16);
void oil_mdct12_f64 (double * d_6, const double * s_12);
void oil_mdct36_f64 (double * d_18, const double * s_36);
void oil_merge_linear_argb (uint32_t * d_n, const uint32_t * s_n, const uint32_t * s2_n, const uint32_t * s3_1, int n);
void oil_merge_linear_u8 (uint8_t * d_n, const uint8_t * s_n, const uint8_t * s2_n, const uint32_t * s3_1, int n);
void oil_minimum_f32 (float * d, const float * s1, const float * s2, int n);
void oil_mix_u8 (uint8_t * dest, const uint8_t * src1, const uint8_t * src2, const uint8_t * src3, int n);
void oil_mt19937 (uint32_t * d_624, uint32_t * i_624);
void oil_mult8x8_s16 (int16_t * d_8x8, const int16_t * s1_8x8, const int16_t * s2_8x8, int ds, int ss1, int ss2);
void oil_multiply_and_acc_12xn_s16_u8 (int16_t * i1_12xn, int is1, const int16_t * s1_12xn, int ss1, const uint8_t * s2_12xn, int ss2, int n);
void oil_multiply_and_add_s16 (int16_t * d, const int16_t * src1, const int16_t * src2, const int16_t * src3, int n);
void oil_multiply_and_add_s16_u8 (int16_t * d, const int16_t * src1, const int16_t * src2, const uint8_t * src3, int n);
void oil_multiply_f32 (float * d, const float * s1, const float * s2, int n);
void oil_multiply_f64 (double * d, const double * s1, const double * s2, int n);
void oil_multsum_f32 (float * dest, const float * src1, int sstr1, const float * src2, int sstr2, int n);
void oil_multsum_f64 (double * dest, const double * src1, int sstr1, const double * src2, int sstr2, int n);
void oil_negative_f32 (float * d, const float * s, int n);
void oil_null (void);
void oil_permute_f32 (float * dest, int dstr, const float * src1, int sstr1, const int32_t * src2, int sstr2, int n);
void oil_permute_f64 (double * dest, int dstr, const double * src1, int sstr1, const int32_t * src2, int sstr2, int n);
void oil_permute_s16 (int16_t * dest, int dstr, const int16_t * src1, int sstr1, const int32_t * src2, int sstr2, int n);
void oil_permute_s32 (int32_t * dest, int dstr, const int32_t * src1, int sstr1, const int32_t * src2, int sstr2, int n);
void oil_permute_s8 (int8_t * dest, int dstr, const int8_t * src1, int sstr1, const int32_t * src2, int sstr2, int n);
void oil_permute_u16 (uint16_t * dest, int dstr, const uint16_t * src1, int sstr1, const int32_t * src2, int sstr2, int n);
void oil_permute_u32 (uint32_t * dest, int dstr, const uint32_t * src1, int sstr1, const int32_t * src2, int sstr2, int n);
void oil_permute_u8 (uint8_t * dest, int dstr, const uint8_t * src1, int sstr1, const int32_t * src2, int sstr2, int n);
void oil_recon8x8_inter (uint8_t * d_8x8, int ds, const uint8_t * s1_8x8, int ss1, const int16_t * s2_8x8);
void oil_recon8x8_inter2 (uint8_t * d_8x8, int ds, const uint8_t * s1_8x8, int ss1, const uint8_t * s2_8x8, int ss2, const int16_t * s3_8x8);
void oil_recon8x8_intra (uint8_t * d_8x8, int ds, const int16_t * s_8x8);
void oil_resample_linear_argb (uint32_t * d_n, const uint32_t * s_2xn, int n, uint32_t * i_2);
void oil_resample_linear_u8 (uint8_t * d_n, const uint8_t * s_2xn, int n, uint32_t * i_2);
void oil_rgb2bgr (uint8_t * d_3xn, const uint8_t * s_3xn, int n);
void oil_rgb2rgba (uint8_t * d_4xn, const uint8_t * s_3xn, int n);
void oil_rgb565_to_argb (uint32_t * d, const uint16_t * s, int n);
void oil_rowsad8x8_u8 (uint32_t * d_1, const uint8_t * s1_8x8, const uint8_t * s2_8x8);
void oil_sad8x8_f64 (double * d_8x8, int ds, const double * s1_8x8, int ss1, const double * s2_8x8, int ss2);
void oil_sad8x8_f64_2 (double * d_1, const double * s1_8x8, int ss1, const double * s2_8x8, int ss2);
void oil_sad8x8_s16 (uint32_t * d_8x8, int ds, const int16_t * s1_8x8, int ss1, const int16_t * s2_8x8, int ss2);
void oil_sad8x8_s16_2 (uint32_t * d_1, const int16_t * s1_8x8, int ss1, const int16_t * s2_8x8, int ss2);
void oil_sad8x8_u8 (uint32_t * d_1, const uint8_t * s1_8x8, int ss1, const uint8_t * s2_8x8, int ss2);
void oil_sad8x8_u8_avg (uint32_t * d_1, const uint8_t * s1_8x8, int ss1, const uint8_t * s2_8x8, const uint8_t * s3_8x8, int ss2);
void oil_scalaradd_f32 (float * d, int dstr, const float * s1, int sstr, const float * s2_1, int n);
void oil_scalaradd_f32_ns (float * d, const float * s1, const float * s2_1, int n);
void oil_scalaradd_f64 (double * d, int dstr, const double * s1, int sstr, const double * s2_1, int n);
void oil_scalaradd_s16 (int16_t * d, int dstr, const int16_t * s1, int sstr, const int16_t * s2_1, int n);
void oil_scalaradd_s32 (int32_t * d, int dstr, const int32_t * s1, int sstr, const int32_t * s2_1, int n);
void oil_scalaradd_s8 (int8_t * d, int dstr, const int8_t * s1, int sstr, const int8_t * s2_1, int n);
void oil_scalaradd_u16 (uint16_t * d, int dstr, const uint16_t * s1, int sstr, const uint16_t * s2_1, int n);
void oil_scalaradd_u32 (uint32_t * d, int dstr, const uint32_t * s1, int sstr, const uint32_t * s2_1, int n);
void oil_scalaradd_u8 (uint8_t * d, int dstr, const uint8_t * s1, int sstr, const uint8_t * s2_1, int n);
void oil_scalarmult_f32 (float * d, int dstr, const float * s1, int sstr, const float * s2_1, int n);
void oil_scalarmult_f64 (double * d, int dstr, const double * s1, int sstr, const double * s2_1, int n);
void oil_scalarmult_s16 (int16_t * d, int dstr, const int16_t * s1, int sstr, const int16_t * s2_1, int n);
void oil_scalarmult_s32 (int32_t * d, int dstr, const int32_t * s1, int sstr, const int32_t * s2_1, int n);
void oil_scalarmult_s8 (int8_t * d, int dstr, const int8_t * s1, int sstr, const int8_t * s2_1, int n);
void oil_scalarmult_u16 (uint16_t * d, int dstr, const uint16_t * s1, int sstr, const uint16_t * s2_1, int n);
void oil_scalarmult_u32 (uint32_t * d, int dstr, const uint32_t * s1, int sstr, const uint32_t * s2_1, int n);
void oil_scalarmult_u8 (uint8_t * d, int dstr, const uint8_t * s1, int sstr, const uint8_t * s2_1, int n);
void oil_scalarmultiply_f32_ns (float * d, const float * s1, const float * s2_1, int n);
void oil_scaleconv_f32_s16 (float * dest, const int16_t * src, int n, const double * s2_1, const double * s3_1);
void oil_scaleconv_f32_s32 (float * dest, const int32_t * src, int n, const double * s2_1, const double * s3_1);
void oil_scaleconv_f32_s8 (float * dest, const int8_t * src, int n, const double * s2_1, const double * s3_1);
void oil_scaleconv_f32_u16 (float * dest, const uint16_t * src, int n, const double * s2_1, const double * s3_1);
void oil_scaleconv_f32_u32 (float * dest, const uint32_t * src, int n, const double * s2_1, const double * s3_1);
void oil_scaleconv_f32_u8 (float * dest, const uint8_t * src, int n, const double * s2_1, const double * s3_1);
void oil_scaleconv_f64_s16 (double * dest, const int16_t * src, int n, const double * s2_1, const double * s3_1);
void oil_scaleconv_f64_s32 (double * dest, const int32_t * src, int n, const double * s2_1, const double * s3_1);
void oil_scaleconv_f64_s8 (double * dest, const int8_t * src, int n, const double * s2_1, const double * s3_1);
void oil_scaleconv_f64_u16 (double * dest, const uint16_t * src, int n, const double * s2_1, const double * s3_1);
void oil_scaleconv_f64_u32 (double * dest, const uint32_t * src, int n, const double * s2_1, const double * s3_1);
void oil_scaleconv_f64_u8 (double * dest, const uint8_t * src, int n, const double * s2_1, const double * s3_1);
void oil_scaleconv_s16_f32 (int16_t * dest, const float * src, int n, const double * s2_1, const double * s3_1);
void oil_scaleconv_s16_f64 (int16_t * dest, const double * src, int n, const double * s2_1, const double * s3_1);
void oil_scaleconv_s32_f32 (int32_t * dest, const float * src, int n, const double * s2_1, const double * s3_1);
void oil_scaleconv_s32_f64 (int32_t * dest, const double * src, int n, const double * s2_1, const double * s3_1);
void oil_scaleconv_s8_f32 (int8_t * dest, const float * src, int n, const double * s2_1, const double * s3_1);
void oil_scaleconv_s8_f64 (int8_t * dest, const double * src, int n, const double * s2_1, const double * s3_1);
void oil_scaleconv_u16_f32 (uint16_t * dest, const float * src, int n, const double * s2_1, const double * s3_1);
void oil_scaleconv_u16_f64 (uint16_t * dest, const double * src, int n, const double * s2_1, const double * s3_1);
void oil_scaleconv_u32_f32 (uint32_t * dest, const float * src, int n, const double * s2_1, const double * s3_1);
void oil_scaleconv_u32_f64 (uint32_t * dest, const double * src, int n, const double * s2_1, const double * s3_1);
void oil_scaleconv_u8_f32 (uint8_t * dest, const float * src, int n, const double * s2_1, const double * s3_1);
void oil_scaleconv_u8_f64 (uint8_t * dest, const double * src, int n, const double * s2_1, const double * s3_1);
void oil_scanlinescale2_u8 (uint8_t * d, const uint8_t * s, int n);
void oil_sign_f32 (float * d, const float * s, int n);
void oil_sincos_f64 (double * dest1, double * dest2, int n, const double * s1_1, const double * s2_1);
void oil_splat_u16_ns (uint16_t * dest, const uint16_t * s1_1, int n);
void oil_splat_u32 (uint32_t * dest, int dstr, const uint32_t * s1_1, int n);
void oil_splat_u32_ns (uint32_t * dest, const uint32_t * s1_1, int n);
void oil_splat_u8 (uint8_t * dest, int dstr, const uint8_t * s1_1, int n);
void oil_splat_u8_ns (uint8_t * dest, const uint8_t * s1_1, int n);
void oil_split_135 (int16_t * d_2xn, const int16_t * s_2xn, int n);
void oil_split_53 (int16_t * d_2xn, const int16_t * s_2xn, int n);
void oil_split_approx97 (int16_t * d_2xn, const int16_t * s_2xn, int n);
void oil_split_daub97 (int16_t * d_2xn, const int16_t * s_2xn, int n);
void oil_squaresum_f64 (double * d, const double * s, int n);
void oil_squaresum_shifted_s16 (uint32_t * d, const int16_t * s, int n);
void oil_subtract_f32 (float * d, const float * s1, const float * s2, int n);
void oil_subtract_f64 (double * d, const double * s1, const double * s2, int n);
void oil_subtract_s16 (int16_t * d, const int16_t * src1, const int16_t * src2, int n);
void oil_subtract_s16_u8 (int16_t * d, const int16_t * src1, const uint8_t * src2, int n);
void oil_sum_f64 (double * d_1, const double * s, int sstr, int n);
void oil_sum_s16 (int16_t * d_1, const int16_t * s, int n);
void oil_swab_u16 (uint16_t * d_n, const uint16_t * s_n, int n);
void oil_swab_u32 (uint32_t * d_n, const uint32_t * s_n, int n);
void oil_synth_135 (int16_t * d_2xn, const int16_t * s_2xn, int n);
void oil_synth_53 (int16_t * d_2xn, const int16_t * s_2xn, int n);
void oil_synth_approx97 (int16_t * d_2xn, const int16_t * s_2xn, int n);
void oil_synth_daub97 (int16_t * d_2xn, const int16_t * s_2xn, int n);
void oil_tablelookup_u8 (uint8_t * d, int ds, const uint8_t * s1, int ss1, const uint8_t * s2_256, int ss2, int n);
void oil_testzero_u8 (uint32_t * d_1, const uint8_t * s, int n);
void oil_trans8x8_f64 (double * d_8x8, int ds, const double * s_8x8, int ss);
void oil_trans8x8_u16 (uint16_t * d_8x8, int ds, const uint16_t * s_8x8, int ss);
void oil_trans8x8_u32 (uint32_t * d_8x8, int ds, const uint32_t * s_8x8, int ss);
void oil_trans8x8_u8 (uint8_t * d_8x8, int ds, const uint8_t * s_8x8, int ss);
void oil_unzigzag8x8_s16 (int16_t * d_8x8, int ds, const int16_t * s_8x8, int ss);
void oil_utf8_validate (int32_t * d_1, const uint8_t * s, int n);
void oil_uyvy2ayuv (uint32_t * d_n, const uint32_t * s_n, int n);
void oil_vectoradd_f32 (float * d, int dstr, const float * s1, int sstr1, const float * s2, int sstr2, int n, const float * s3_1, const float * s4_1);
void oil_vectoradd_f64 (double * d, int dstr, const double * s1, int sstr1, const double * s2, int sstr2, int n, const double * s3_1, const double * s4_1);
void oil_vectoradd_s16 (int16_t * d, int dstr, const int16_t * s1, int sstr1, const int16_t * s2, int sstr2, int n, const int16_t * s3_1, const int16_t * s4_1);
void oil_vectoradd_s32 (int32_t * d, int dstr, const int32_t * s1, int sstr1, const int32_t * s2, int sstr2, int n, const int32_t * s3_1, const int32_t * s4_1);
void oil_vectoradd_s8 (int8_t * d, int dstr, const int8_t * s1, int sstr1, const int8_t * s2, int sstr2, int n, const int8_t * s3_1, const int8_t * s4_1);
void oil_vectoradd_s_f32 (float * d, int dstr, const float * s1, int sstr1, const float * s2, int sstr2, int n);
void oil_vectoradd_s_f64 (double * d, int dstr, const double * s1, int sstr1, const double * s2, int sstr2, int n);
void oil_vectoradd_s_s16 (int16_t * d, int dstr, const int16_t * s1, int sstr1, const int16_t * s2, int sstr2, int n);
void oil_vectoradd_s_s8 (int8_t * d, int dstr, const int8_t * s1, int sstr1, const int8_t * s2, int sstr2, int n);
void oil_vectoradd_s_u16 (uint16_t * d, int dstr, const uint16_t * s1, int sstr1, const uint16_t * s2, int sstr2, int n);
void oil_vectoradd_s_u8 (uint8_t * d, int dstr, const uint8_t * s1, int sstr1, const uint8_t * s2, int sstr2, int n);
void oil_vectoradd_u16 (uint16_t * d, int dstr, const uint16_t * s1, int sstr1, const uint16_t * s2, int sstr2, int n, const uint16_t * s3_1, const uint16_t * s4_1);
void oil_vectoradd_u32 (uint32_t * d, int dstr, const uint32_t * s1, int sstr1, const uint32_t * s2, int sstr2, int n, const uint32_t * s3_1, const uint32_t * s4_1);
void oil_vectoradd_u8 (uint8_t * d, int dstr, const uint8_t * s1, int sstr1, const uint8_t * s2, int sstr2, int n, const uint8_t * s3_1, const uint8_t * s4_1);
void oil_yuv2rgbx_sub2_u8 (uint8_t * d_4xn, const uint8_t * src1, const uint8_t * src2, const uint8_t * src3, int n);
void oil_yuv2rgbx_sub4_u8 (uint8_t * d_4xn, const uint8_t * src1, const uint8_t * src2, const uint8_t * src3, int n);
void oil_yuv2rgbx_u8 (uint8_t * d_4xn, const uint8_t * src1, const uint8_t * src2, const uint8_t * src3, int n);
void oil_yuyv2ayuv (uint32_t * d_n, const uint32_t * s_n, int n);
void oil_yvyu2ayuv (uint32_t * d_n, const uint32_t * s_n, int n);
void oil_zigzag8x8_s16 (int16_t * d_8x8, int ds, const int16_t * s_8x8, int ss);

#ifdef __cplusplus
}
#endif

#endif


/* liboil - Library of Optimized Inner Loops
 * Copyright (C) 2003  David A. Schleef <ds@schleef.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of version 2.1 of the GNU Lesser General
 * Public License as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307 USA.
 */

#ifndef _LIBOIL_JPEG_H_
#define _LIBOIL_JPEG_H_

#include <liboil/liboilfunction.h>

OIL_DECLARE_CLASS(conv8x8_s16_f64);
OIL_DECLARE_CLASS(conv8x8_f64_s16);
OIL_DECLARE_CLASS(clipconv8x8_u8_s16);

OIL_DECLARE_CLASS(idct8_f64);

OIL_DECLARE_CLASS(idct8x8_f64);
OIL_DECLARE_CLASS(idct8x8_s16);

OIL_DECLARE_CLASS(scanlinescale2_u8);

OIL_DECLARE_CLASS(dequantize8x8_s16);

OIL_DECLARE_CLASS(yuv2rgbx_u8);
OIL_DECLARE_CLASS(yuv2rgbx_sub2_u8);
OIL_DECLARE_CLASS(yuv2rgbx_sub4_u8);

OIL_DECLARE_CLASS(unzigzag8x8_s16);

#define idct8_f64 ((void (*)(double *, int, double *, int)) \
                    _oil_function_idct8_f64_class.func)
#define idct8x8_f64 ((void (*)(double *, int, double *, int)) \
                    _oil_function_idct8x8_f64_class.func)
#define conv8x8_f64_s16 ((void (*)(double *, int, int16_t *, int)) \
                    _oil_function_conv8x8_f64_s16_class.func)
#define conv8x8_s16_f64 ((void (*)(int16_t *, int, double *, int)) \
                    _oil_function_conv8x8_s16_f64_class.func)

#endif


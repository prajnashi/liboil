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

#ifndef _LIBOIL_FUNCS_H_
#define _LIBOIL_FUNCS_H_

extern OilFunctionClass _oil_function_conv_f32_f64_class;
#define conv_f32_f64 ((void (*)(float *, int, double *, int, int)) \
		_oil_function_conv_f32_f64_class.func)
extern OilFunctionClass _oil_function_conv_f64_f32_class;
#define conv_f64_f32 ((void (*)(double *, int, float *, int, int)) \
		_oil_function_conv_f64_f32_class.func)
extern OilFunctionClass _oil_function_conv_s16_f64_class;
#define conv_s16_f64 ((void (*)(short *, int, double *, int, int)) \
		_oil_function_conv_s16_f64_class.func)
extern OilFunctionClass _oil_function_conv_f64_s16_class;
#define conv_f64_s16 ((void (*)(double *, int, short *, int, int)) \
		_oil_function_conv_f64_s16_class.func)

#endif


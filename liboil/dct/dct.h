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

#ifndef _LIBOIL_DCT_H_
#define _LIBOIL_DCT_H_

#include <liboil/liboilfunction.h>

OIL_DECLARE_CLASS(dct12_f32);
OIL_DECLARE_CLASS(dct36_f32);
OIL_DECLARE_CLASS(fdct8_f64);
OIL_DECLARE_CLASS(fdct8x8_f64);
OIL_DECLARE_CLASS(fdct8x8s_s16);
OIL_DECLARE_CLASS(idct8_f64);
OIL_DECLARE_CLASS(imdct32_f32);

#endif


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

#ifndef _LIBOIL_PARAMETER_H_
#define _LIBOIL_PARAMETER_H_

#include <liboil/liboiltypes.h>

enum _OilType {
  OIL_TYPE_UNKNOWN = 0,
  OIL_TYPE_INT,
  OIL_TYPE_s8,
  OIL_TYPE_u8,
  OIL_TYPE_s16,
  OIL_TYPE_u16,
  OIL_TYPE_s32,
  OIL_TYPE_u32,
  OIL_TYPE_f32,
  OIL_TYPE_f64,
  OIL_TYPE_s8p,
  OIL_TYPE_u8p,
  OIL_TYPE_s16p,
  OIL_TYPE_u16p,
  OIL_TYPE_s32p,
  OIL_TYPE_u32p,
  OIL_TYPE_f32p,
  OIL_TYPE_f64p,
};

enum _OilArgType {
  OIL_ARG_UNKNOWN = 0,
  OIL_ARG_N,
  OIL_ARG_M,
  OIL_ARG_DEST1,
  OIL_ARG_DSTR1,
  OIL_ARG_DEST2,
  OIL_ARG_DSTR2,
  OIL_ARG_SRC1,
  OIL_ARG_SSTR1,
  OIL_ARG_SRC2,
  OIL_ARG_SSTR2,
  OIL_ARG_SRC3,
  OIL_ARG_SSTR3,
  OIL_ARG_SRC4,
  OIL_ARG_SSTR4,
  OIL_ARG_SRC5,
  OIL_ARG_SSTR5,
  OIL_ARG_INPLACE1,
  OIL_ARG_ISTR1,
  OIL_ARG_INPLACE2,
  OIL_ARG_ISTR2,

  OIL_ARG_LAST
};

struct _OilParameter {
  char *type_name;
  char *parameter_name;

  int order;
  OilType type;

  int direction;
  int is_stride;
  int index;
  int prestride_length;
  int prestride_var;
  int poststride_length;
  int poststride_var;

  OilArgType parameter_type;

  void *ptr;
  unsigned long value;
  int size;
};

#endif


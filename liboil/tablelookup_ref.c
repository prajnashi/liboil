/* liboil - Library of Optimized Inner Loops
 * Copyright (C) 2004  David A. Schleef <ds@schleef.org>
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <liboil/liboilfunction.h>
//#include <liboil/.h>


tablelookup_u8_ref (uint8_t *dest, int dstr, uint8_t *src, int sstr,
    uint8_t *table, int tablestride, int n)
{
  int i;
  for(i=0;i<n;i++){
    *dest = *OIL_OFFSET(table,tablestride*(int)src);
    dest = OIL_OFFSET(dest, dstr);
    src = OIL_OFFSET(src, sstr);
  }
}

OIL_DEFINE_IMPL_REF (tablelookup_u8_ref, tablelookup_u8_class);

OIL_DEFINE_CLASS (tablelookup_u8, NULL);


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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <liboil/liboil.h>

#include "jpeg.h"

OIL_DEFINE_CLASS_X (scanlinescale2_u8, "unsigned char *dest, unsigned char *src, int len");

static void
scanlinescale2_u8_ref (unsigned char *dest, unsigned char *src, int len)
{
	int i;

	for(i=0;i<len;i++){
		dest[i] = src[i/2];
	}
}

OIL_DEFINE_IMPL_REF (scanlinescale2_u8_ref, scanlinescale2_u8_class);


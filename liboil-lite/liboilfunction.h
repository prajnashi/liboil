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

#ifndef _LIBOIL_FUNCTION_H_
#define _LIBOIL_FUNCTION_H_

#include <liboil/liboiltypes.h>

typedef struct _OilFunctionClass OilFunctionClass;
typedef struct _OilFunctionImpl OilFunctionImpl;
typedef struct _OilFunctionArg OilFunctionArg;

typedef void (*OilTestFunction) (OilFunctionClass *klass,
		OilFunctionImpl *impl);

struct _OilFunctionClass {
  int ignore;
};

struct _OilFunctionImpl {
  int ignore;
};


void oil_init (void);
void oil_optimize_all (void);
void oil_optimize (const char *class_name);

OilFunctionClass *oil_class_get (const char *class_name);
void oil_class_optimize (OilFunctionClass *klass);


#endif


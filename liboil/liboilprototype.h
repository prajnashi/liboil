/* liboil - Library of Optimized Inner Loops
 * Copyright (C) 2003,2004  David A. Schleef <ds@schleef.org>
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

#ifndef _LIBOIL_PROTOTYPE_H_
#define _LIBOIL_PROTOTYPE_H_

#include <liboil/liboilfunction.h>
#include <liboil/liboilparameter.h>

typedef struct _OilPrototype OilPrototype;
struct _OilPrototype {
  int n_params;
  OilParameter *params;
  OilFunctionClass *klass;
};


OilPrototype *oil_prototype_from_string (const char *s);
char *oil_prototype_to_string (OilPrototype *proto);
void oil_prototype_free (OilPrototype *proto);
void oil_prototype_append_param (OilPrototype *proto, OilParameter *param);

int oil_type_sizeof (OilType type);
const char * oil_type_name (OilType type);
const char * oil_arg_type_name (OilArgType type);


#endif


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

#ifndef _LIBOIL_TEST_H_
#define _LIBOIL_TEST_H_

#include <liboil/liboil.h>
#include <liboil/liboilprototype.h>
#include <liboil/liboilprofile.h>

//typedef struct _OilTest OilTest;
struct _OilTest {
  OilFunctionClass *klass;
  OilFunctionImpl *impl;
  OilPrototype *proto;
  OilParameter params[OIL_ARG_LAST];
  OilProfile prof;

  int iterations;
  int n;
};


OilTest *oil_test_new (OilFunctionClass *klass);
void oil_test_free (OilTest *test);

void oil_test_set_impl (OilTest *test, OilFunctionImpl *impl);
void oil_test_set_iterations (OilTest *test, int iterations);

int oil_test_go (OilTest *test);

void oil_test_cleanup (OilTest *test);
void oil_test_init_src_arrays (OilTest *test);
void oil_test_init_dest_arrays (OilTest *test);

#endif


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

#if 0
#define LIBOIL_ATTRIBUTE_ALIGNED_16 __attribute__ ((aligned (16)))
#define LIBOIL_ATTRIBUTE_SECTION(section_name) \
  __attribute__ ((section (section_name))) = {
#endif
#define LIBOIL_ATTRIBUTE_ALIGNED_16
#define LIBOIL_ATTRIBUTE_SECTION(section_name)

struct _OilFunctionClass {
	void *func;
	const char *name;
	const char *desc;
	OilTestFunction test_func;

	OilFunctionImpl *first_impl;
	OilFunctionImpl *reference_impl;

	OilFunctionImpl *chosen_impl;

	const char *prototype;
} LIBOIL_ATTRIBUTE_ALIGNED_16;

struct _OilFunctionImpl {
	void *next;
	OilFunctionClass *klass;
	void *func;
	unsigned int flags;
	const char *name;
	unsigned int prof;
} LIBOIL_ATTRIBUTE_ALIGNED_16;

#define OIL_GET(ptr, offset, type) (*(type *)((uint8_t *)ptr + offset) )
#define OIL_OFFSET(ptr, offset) ((void *)((uint8_t *)ptr + offset) )
#define OIL_INCREMENT(ptr, offset) (ptr = (void *)((uint8_t *)ptr + offset) )

#define OIL_IMPL_FLAG_REF	(1<<0)

#define OIL_ARCH_FLAGS 0xffff0000

/* i386 */
#define OIL_IMPL_REQUIRES_CMOV 	        (1<<16)
#define OIL_IMPL_REQUIRES_MMX	        (1<<17)
#define OIL_IMPL_REQUIRES_SSE	        (1<<18)
#define OIL_IMPL_REQUIRES_MMXEXT        (1<<19)
#define OIL_IMPL_REQUIRES_SSE2          (1<<20)
#define OIL_IMPL_REQUIRES_3DNOW	        (1<<21)
#define OIL_IMPL_REQUIRES_3DNOWEXT	(1<<22)

/* powerpc */
#define OIL_IMPL_REQUIRES_ALTIVEC       (1<<16)

#define OIL_DECLARE_CLASS(klass) \
	extern OilFunctionClass _oil_function_ ## klass ## _class


#define OIL_DEFINE_CLASS(klass, test) \
OilFunctionClass _oil_function_ ## klass ## _class \
		LIBOIL_ATTRIBUTE_SECTION(".oil_function_class") = { \
	NULL, \
	#klass , \
	NULL, \
        (OilTestFunction) test, \
        NULL, \
        NULL, \
        NULL, \
        NULL \
}

#define OIL_DEFINE_CLASS_X(klass, string) \
OilFunctionClass _oil_function_ ## klass ## _class \
		LIBOIL_ATTRIBUTE_SECTION(".oil_function_class") = { \
	NULL, \
	#klass , \
	NULL, \
        (OilTestFunction) NULL, \
	NULL, \
	NULL, \
	NULL, \
	string \
}

#define OIL_ARG(a,b,c,d) { a, b, c, d }

#define OIL_DEFINE_IMPL_FULL(function,klass,flags) \
OilFunctionImpl _oil_function_impl_ ## function \
		LIBOIL_ATTRIBUTE_SECTION(".oil_function_impl") /* unused */ = { \
	NULL, \
	&_oil_function_ ## klass , \
	(void *)function, \
	flags, \
	#function \
}

#define OIL_DEFINE_IMPL(function,klass) \
	OIL_DEFINE_IMPL_FULL(function,klass,0)
#define OIL_DEFINE_IMPL_REF(function,klass) \
	OIL_DEFINE_IMPL_FULL(function,klass,OIL_IMPL_FLAG_REF)
#define OIL_DEFINE_IMPL_DEPENDS(function,klass,...) \
	OIL_DEFINE_IMPL_FULL(function,klass,OIL_IMPL_FLAG_REF)

extern OilFunctionClass _oil_begin_function_class;
extern OilFunctionClass _oil_next_function_class;
extern OilFunctionClass _oil_end_function_class;

extern OilFunctionImpl _oil_begin_function_impl;
extern OilFunctionImpl _oil_next_function_impl;
extern OilFunctionImpl _oil_end_function_impl;

extern OilFunctionClass *oil_function_classes;
extern OilFunctionImpl *oil_function_impls;
extern int oil_n_function_impls;
extern int oil_n_function_classes;

extern unsigned int oil_arch_flags;

void oil_init (void);
void oil_optimize_all (void);
void oil_optimize (const char *class_name);

OilFunctionClass *oil_class_get (const char *class_name);
void oil_class_optimize (OilFunctionClass *klass);


#endif


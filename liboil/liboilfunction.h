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
typedef struct _OilParameter OilParameter;
typedef struct _OilTest OilTest;

typedef enum _OilType OilType;
typedef enum _OilArgType OilArgType;

typedef void (*OilTestFunction) (OilTest *test);

#ifdef LIBOIL_STRICT_PROTOTYPES
#include <liboil/liboilfuncs.h>
#define LIBOIL_CHECK_PROTOTYPE(a) a
#else
#define LIBOIL_CHECK_PROTOTYPE(a)
#endif

#ifndef OIL_OPT_MANGLE
#define OIL_OPT_MANGLE(a) a
#else
#define OIL_NO_CLASSES
#endif
#ifndef OIL_OPT_SUFFIX
#define OIL_OPT_SUFFIX
#endif

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
  OIL_ARG_STATE,
  OIL_ARG_PARAM1,
  OIL_ARG_PARAM2,
  OIL_ARG_PARAM3,

  OIL_ARG_LAST
};

struct _OilFunctionClass {
	void *func;
	const char *name;
	const char *desc;
	OilTestFunction test_func;

	OilFunctionImpl *first_impl;
	OilFunctionImpl *reference_impl;

	OilFunctionImpl *chosen_impl;

	const char *prototype;
};

struct _OilFunctionImpl {
	void *next;
	OilFunctionClass *klass;
	void *func;
	unsigned int flags;
	const char *name;
	unsigned int prof;
};

struct _OilParameter {
  char *type_name;
  char *parameter_name;
  OilType type;
  int order;
  OilArgType parameter_type;

  unsigned long value;
  int size;
};

#define OIL_GET(ptr, offset, type) (*(type *)((uint8_t *)ptr + offset) )
#define OIL_OFFSET(ptr, offset) ((void *)((uint8_t *)ptr + offset) )
#define OIL_INCREMENT(ptr, offset) (ptr = (void *)((uint8_t *)ptr + offset) )

#define OIL_IMPL_FLAG_REF	(1<<0)

#define OIL_CPU_FLAG_MASK 0xffff0000

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
	extern OilFunctionClass _oil_function_class_ ## klass


#ifndef OIL_NO_CLASSES
#define OIL_DEFINE_CLASS_FULL(klass, string, test) \
OilFunctionClass _oil_function_class_ ## klass = { \
	NULL, \
	#klass , \
	NULL, \
        test, \
        NULL, \
        NULL, \
        NULL, \
        string \
}; \
OilFunctionClass *oil_function_class_ptr_ ## klass = \
  &_oil_function_class_ ## klass
#else
#define OIL_DEFINE_CLASS_FULL(klass, string, test) \
  OIL_DECLARE_CLASS(klass)
#endif

#define OIL_DEFINE_CLASS(klass, string) \
  OIL_DEFINE_CLASS_FULL (klass, string, NULL)

#define OIL_DEFINE_IMPL_FULL(function,klass,flags) \
OilFunctionImpl OIL_OPT_MANGLE(_oil_function_impl_ ## function) = { \
	NULL, \
	&_oil_function_class_ ## klass , \
	(void *)function, \
	flags, \
        #function OIL_OPT_SUFFIX \
} \
LIBOIL_CHECK_PROTOTYPE(;_oil_type_ ## klass _ignore_me_ ## function = function)

#define OIL_DEFINE_IMPL(function,klass) \
	OIL_DEFINE_IMPL_FULL(function,klass,0)
#define OIL_DEFINE_IMPL_REF(function,klass) \
	OIL_DEFINE_IMPL_FULL(function,klass,OIL_IMPL_FLAG_REF)
#define OIL_DEFINE_IMPL_DEPENDS(function,klass,...) \
	OIL_DEFINE_IMPL_FULL(function,klass,0)

extern unsigned int oil_arch_flags;
extern int oil_n_function_impls;
extern int oil_n_function_classes;

void oil_init (void);
void oil_optimize_all (void);
void oil_optimize (const char *class_name);

OilFunctionClass * oil_class_get_by_index (int i);
OilFunctionClass *oil_class_get (const char *class_name);
void oil_class_optimize (OilFunctionClass *klass);

OilFunctionImpl * oil_impl_get_by_index (int i);

void oil_class_choose_by_name (OilFunctionClass * klass, const char *name);

#endif


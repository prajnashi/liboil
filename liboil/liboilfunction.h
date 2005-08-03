/*
 * LIBOIL - Library of Optimized Inner Loops
 * Copyright (c) 2003,2004 David A. Schleef <ds@schleef.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _LIBOIL_FUNCTION_H_
#define _LIBOIL_FUNCTION_H_

#include <liboil/liboiltypes.h>

#ifdef LIBOIL_STRICT_PROTOTYPES
#include <liboil/liboilfuncs.h>
#define LIBOIL_CHECK_PROTOTYPE(a) a
#else
#define LIBOIL_CHECK_PROTOTYPE(a)
#endif

#ifndef OIL_OPT_MANGLE
#define OIL_OPT_MANGLE(a) a
#define OIL_OPT_FLAG_MANGLE(a) a
#else
#define OIL_NO_CLASSES
#define OIL_OPT_FLAG_MANGLE(a) (((a)&(~OIL_IMPL_FLAG_REF)) | OIL_IMPL_FLAG_OPT)
#endif
#ifndef OIL_OPT_SUFFIX
#define OIL_OPT_SUFFIX
#endif

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
        double profile_ave;
        double profile_std;
};

#define OIL_GET(ptr, offset, type) (*(type *)((uint8_t *)ptr + (offset)) )
#define OIL_OFFSET(ptr, offset) ((void *)((uint8_t *)ptr + (offset)) )
#define OIL_INCREMENT(ptr, offset) (ptr = (void *)((uint8_t *)ptr + (offset)) )

#define OIL_IMPL_FLAG_REF	(1<<0)
#define OIL_IMPL_FLAG_OPT	(1<<1)
#define OIL_IMPL_FLAG_ASM	(1<<2)
#define OIL_IMPL_FLAG_DISABLED	(1<<3)

#define OIL_CPU_FLAG_MASK 0xffff0000

/* i386 */
#define OIL_IMPL_FLAG_CMOV              (1<<16)
#define OIL_IMPL_FLAG_MMX               (1<<17)
#define OIL_IMPL_FLAG_SSE               (1<<18)
#define OIL_IMPL_FLAG_MMXEXT            (1<<19)
#define OIL_IMPL_FLAG_SSE2              (1<<20)
#define OIL_IMPL_FLAG_3DNOW             (1<<21)
#define OIL_IMPL_FLAG_3DNOWEXT          (1<<22)
#define OIL_IMPL_FLAG_SSE3              (1<<23)

/* powerpc */
#define OIL_IMPL_FLAG_ALTIVEC       (1<<16)

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
	OIL_OPT_FLAG_MANGLE(flags), \
        #function OIL_OPT_SUFFIX \
} \
LIBOIL_CHECK_PROTOTYPE(;_oil_type_ ## klass _ignore_me_ ## function = function)

#define OIL_DEFINE_IMPL(function,klass) \
	OIL_DEFINE_IMPL_FULL(function,klass,0)
#define OIL_DEFINE_IMPL_REF(function,klass) \
	OIL_DEFINE_IMPL_FULL(function,klass,OIL_IMPL_FLAG_REF)
#define OIL_DEFINE_IMPL_ASM(function,klass) \
	OIL_DEFINE_IMPL_FULL(function,klass,OIL_IMPL_FLAG_ASM)
#define OIL_DEFINE_IMPL_DEPENDS(function,klass,...) \
	OIL_DEFINE_IMPL_FULL(function,klass,0)

void oil_optimize_all (void);
void oil_optimize (const char *class_name);

OilFunctionClass * oil_class_get_by_index (int i);
OilFunctionClass *oil_class_get (const char *class_name);
void oil_class_optimize (OilFunctionClass *klass);
int oil_class_get_n_classes (void);

OilFunctionImpl * oil_impl_get_by_index (int i);
int oil_impl_is_runnable (OilFunctionImpl *impl);

void oil_class_choose_by_name (OilFunctionClass * klass, const char *name);

void oil_class_register_impl_full (OilFunctionClass * klass,
    void (*func)(void), const char *name, unsigned int flags);
void oil_class_register_impl (OilFunctionClass * klass, OilFunctionImpl *impl);
void oil_class_register_impl_by_name (const char *klass_name,
    OilFunctionImpl *impl);

void oil_init_no_optimize(void);

#endif


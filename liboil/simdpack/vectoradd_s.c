#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <liboil/liboilfunction.h>
#include <liboil/simdpack/simdpack.h>
#include <liboil/liboilgcc.h>

#define CLAMP(x, low, high)  (((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))
#define type_min_f32 -1.0
#define type_max_f32 1.0
#define type_min_f64 -1.0
#define type_max_f64 1.0

#define VECTORADD_S_DEFINE_IMPL(type,bigger)		\
static void vectoradd_s_ ## type ## _ref (	\
    type_ ## type *dest, int dstr,		\
    type_ ## type *src1, int sstr1,		\
    type_ ## type *src2, int sstr2,		\
    int n) \
{						\
  int i;					\
  for(i=0;i<n;i++) {				\
    OIL_GET(dest,i*dstr, type_ ## type) = CLAMP((type_ ## bigger)OIL_GET(src1,i*sstr1, type_ ## type) +	\
        (type_ ## bigger)OIL_GET(src2,i*sstr2, type_ ## type),type_min_ ## type, type_max_ ## type);		\
  }						\
}						\
OIL_DEFINE_CLASS (vectoradd_s_ ## type,         \
    "type_" #type " *dest, int dstr, "		\
    "type_" #type " *src1, int sstr1, "		\
    "type_" #type " *src2, int sstr2, "		\
    "int n");	\
OIL_DEFINE_IMPL_REF (vectoradd_s_ ## type ## _ref, vectoradd_s_ ## type);

VECTORADD_S_DEFINE_IMPL (s8,s16);
VECTORADD_S_DEFINE_IMPL (u8,u16);
VECTORADD_S_DEFINE_IMPL (s16,s32);
VECTORADD_S_DEFINE_IMPL (u16,u32);
//VECTORADD_S_DEFINE_IMPL (s32,s64);
//VECTORADD_S_DEFINE_IMPL (u32,u64);
VECTORADD_S_DEFINE_IMPL (f32,f32);
VECTORADD_S_DEFINE_IMPL (f64,f64);


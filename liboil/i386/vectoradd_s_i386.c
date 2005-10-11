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

#define __padd_s8 __builtin_ia32_paddsb
#define __padd_u8 __builtin_ia32_paddusb
#define __padd_s16 __builtin_ia32_paddsw
#define __padd_u16 __builtin_ia32_paddusw

#define vec_u8 vec_s8
#define vec_u16 vec_s16

#define VECTORADD_MMX_IMPL(type,bigger,pad)  \
static void       \
vectoradd_s_ ## type ## _mmx (type_ ## type *dest, int dstr, type_ ## type *src1, int sstr1, type_ ## type *src2, int sstr2, int n) \
{             \
  vec_ ## type *vdest;          \
  vec_ ## type *vsrc1;          \
  vec_ ## type *vsrc2;          \
                                   \
  while (n & (pad-1)) {               \
    *dest = CLAMP((type_ ## bigger)(*src1)+(type_ ## bigger)(*src2),type_min_ ## bigger,type_max_ ## bigger);  \
    OIL_INCREMENT (dest, dstr);          \
    OIL_INCREMENT (src1, sstr1);         \
    OIL_INCREMENT (src2, sstr2);         \
    n--;                                 \
  }	\
  n /= pad;         \
  vsrc1=(vec_ ## type*)src1;      \
  vsrc2=(vec_ ## type*)src2;       \
  vdest=(vec_ ## type*)dest;       \
                     \
  while (n>0)         \
   {                   \
    *vdest=__padd_ ##type (*vsrc1,*vsrc2);	\
    OIL_INCREMENT (vdest, 8);                   \
    OIL_INCREMENT (vsrc1, 8);                 \
    OIL_INCREMENT (vsrc2, 8);                  \
   n--;                               \
  }                                   \
}                                  \
                                   \
OIL_DEFINE_IMPL_FULL(vectoradd_s_ ## type ## _mmx,vectoradd_s_ ## type,OIL_IMPL_FLAG_MMX);

#ifdef ENABLE_BROKEN_IMPLS
VECTORADD_MMX_IMPL(s8,s16,8)
VECTORADD_MMX_IMPL(u8,s16,8)
VECTORADD_MMX_IMPL(s16,s32,4)
VECTORADD_MMX_IMPL(u16,s32,4)
#endif


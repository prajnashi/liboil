#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <liboil/liboilfunction.h>
#include <liboil/simdpack/simdpack.h>
#include <math.h>
#include <liboil/liboilgcc.h>

#define ABS(x) ((x)>0 ? (x) : -(x))


static void
abs_u8_s8_sse (uint8_t *dest, int dstr, int8_t *src, int sstr, int n)
{
  register vec_s8 zero;
  vec_s8 *vdest;
  vec_s8 *vsrc;
  
  while (n & 7) {
    *dest = ABS (*src);
    OIL_INCREMENT (dest, dstr);
    OIL_INCREMENT (src, sstr);
    n--;
  }
  n /= 8;
  vsrc=(vec_s8*)src;
  vdest=(vec_s8*)dest;
  
  while (n>0) 
   {
    zero = (vec_s8)((long long) 0);
    zero=zero-(*vsrc);
    *vdest=__builtin_ia32_pminub(*vsrc,zero);	
    OIL_INCREMENT (vdest, 8);
    OIL_INCREMENT (vsrc, 8);
   n--;
  }
}
OIL_DEFINE_IMPL_FULL (abs_u8_s8_sse, abs_u8_s8,OIL_IMPL_FLAG_SSE);

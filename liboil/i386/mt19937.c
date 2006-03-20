

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <liboil/liboilfunction.h>
#include <liboil/liboilclasses.h>


#define TWIST(next,pair) \
      "  movd 0(%1), %%mm0 \n" \
      "  movd " #next "(%1), %%mm1 \n" \
      "  pand %%mm7, %%mm0 \n" \
      "  pand %%mm6, %%mm1 \n" \
      "  por %%mm1, %%mm0 \n" \
      "  pslld $31, %%mm1 \n" \
      "  psrad $31, %%mm1 \n" \
      "  pand %%mm5, %%mm1 \n" \
      "  psrld $1, %%mm0 \n" \
      "  pxor %%mm1, %%mm0 \n" \
      "  pxor " #pair "(%1), %%mm0 \n" \
      "  movd %%mm0, 0(%1) \n"

#define TWIST2(next,pair) \
      "  movq 0(%1), %%mm0 \n" \
      "  movq " #next "(%1), %%mm1 \n" \
      "  pand %%mm7, %%mm0 \n" \
      "  pand %%mm6, %%mm1 \n" \
      "  por %%mm1, %%mm0 \n" \
      "  pslld $31, %%mm1 \n" \
      "  psrad $31, %%mm1 \n" \
      "  pand %%mm5, %%mm1 \n" \
      "  psrld $1, %%mm0 \n" \
      "  pxor %%mm1, %%mm0 \n" \
      "  pxor " #pair "(%1), %%mm0 \n" \
      "  movq %%mm0, 0(%1) \n"

#define TWIST4(next,pair) \
      "  movq 0(%1), %%mm0 \n" \
      "   movq 8(%1), %%mm2 \n" \
      "  movq " #next "(%1), %%mm1 \n" \
      "   movq (8+" #next ")(%1), %%mm3 \n" \
      "  pand %%mm7, %%mm0 \n" \
      "   pand %%mm7, %%mm2 \n" \
      "  pand %%mm6, %%mm1 \n" \
      "   pand %%mm6, %%mm3 \n" \
      "  por %%mm1, %%mm0 \n" \
      "   por %%mm3, %%mm2 \n" \
      "  pslld $31, %%mm1 \n" \
      "   pslld $31, %%mm3 \n" \
      "  psrad $31, %%mm1 \n" \
      "   psrad $31, %%mm3 \n" \
      "  pand %%mm5, %%mm1 \n" \
      "   pand %%mm5, %%mm3 \n" \
      "  psrld $1, %%mm0 \n" \
      "   psrld $1, %%mm2 \n" \
      "  pxor %%mm1, %%mm0 \n" \
      "   pxor %%mm3, %%mm2 \n" \
      "  pxor " #pair "(%1), %%mm0 \n" \
      "   pxor (8+" #pair ")(%1), %%mm2 \n" \
      "  movq %%mm0, 0(%1) \n" \
      "   movq %%mm2, 8(%1) \n"


static void
mt19937_i386_mmx (uint32_t *d, uint32_t *mt)
{
  asm volatile (
      "  mov $0x80000000, %2\n"
      "  movd %2, %%mm7\n"
      "  mov $0x7fffffff, %2\n"
      "  movd %2, %%mm6\n"
      "  mov $0x9908b0df, %2\n"
      "  movd %2, %%mm5\n"
      "  mov $0x1, %2\n"
      "  movd %2, %%mm4\n"
      "  mov $227, %2 \n"

      "1: \n"
      TWIST(4,1588)
      "  add $4, %1\n"
      "  decl %2 \n"
      "  jne 1b \n"

      "  mov $396, %2 \n"
      "2: \n"
      TWIST(4,-908)
      "  add $4, %1\n"
      "  decl %2 \n"
      "  jne 2b \n"

      TWIST(-2492,-908)

      "  add $-2492, %1 \n"

      "  mov $0x9d2c5680, %2\n"
      "  movd %2, %%mm7\n"
      "  mov $0xefc60000, %2\n"
      "  movd %2, %%mm6\n"
      "  mov $624, %2 \n"
      "3:\n"
      "  movd 0(%1), %%mm0 \n"
      "  movd 0(%1), %%mm1 \n"
      "  psrld $11, %%mm1 \n"
      "  pxor %%mm1, %%mm0 \n"
      "  movq %%mm0, %%mm1 \n"
      "  pslld $7, %%mm1 \n"
      "  pand %%mm7, %%mm1 \n"
      "  pxor %%mm1, %%mm0 \n"
      "  movq %%mm0, %%mm1 \n"
      "  pslld $15, %%mm1 \n"
      "  pand %%mm6, %%mm1 \n"
      "  pxor %%mm1, %%mm0 \n"
      "  movq %%mm0, %%mm1 \n"
      "  psrld $18, %%mm1 \n"
      "  pxor %%mm1, %%mm0 \n"
      "  movd %%mm0, 0(%0) \n"
      "  add $4, %0\n"
      "  add $4, %1\n"
      "  decl %2 \n"
      "  jne 3b \n"
      "  emms \n"

      : "+r" (d), "+r" (mt)
      : "r" (0)
      );

}
OIL_DEFINE_IMPL_FULL (mt19937_i386_mmx, mt19937, OIL_IMPL_FLAG_MMX);

static void
mt19937_i386_mmx_2 (uint32_t *d, uint32_t *mt)
{
  asm volatile (
      "  mov $0x80000000, %2\n"
      "  movd %2, %%mm7\n"
      "  pshufw $0x44, %%mm7, %%mm7 \n"
      "  mov $0x7fffffff, %2\n"
      "  movd %2, %%mm6\n"
      "  pshufw $0x44, %%mm6, %%mm6 \n"
      "  mov $0x9908b0df, %2\n"
      "  movd %2, %%mm5\n"
      "  pshufw $0x44, %%mm5, %%mm5 \n"
      "  mov $0x1, %2\n"
      "  movd %2, %%mm4\n"
      "  pshufw $0x44, %%mm4, %%mm4 \n"

      "  mov $113, %2 \n"
      "1: \n"
      TWIST2(4,1588)
      "  add $8, %1\n"
      "  decl %2 \n"
      "  jne 1b \n"

      TWIST(4,1588)
      "  add $4, %1\n"

      "  mov $198, %2 \n"
      "2: \n"
      TWIST2(4,-908)
      "  add $8, %1\n"
      "  decl %2 \n"
      "  jne 2b \n"

      TWIST(-2492,-908)

      "  add $-2492, %1 \n"

      "  mov $0x9d2c5680, %2\n"
      "  movd %2, %%mm7\n"
      "  pshufw $0x44, %%mm7, %%mm7 \n"
      "  mov $0xefc60000, %2\n"
      "  movd %2, %%mm6\n"
      "  pshufw $0x44, %%mm6, %%mm6 \n"
      "  mov $312, %2 \n"
      "3:\n"
      "  movq 0(%1), %%mm0 \n"
      "  movq 0(%1), %%mm1 \n"
      "  psrld $11, %%mm1 \n"
      "  pxor %%mm1, %%mm0 \n"
      "  movq %%mm0, %%mm1 \n"
      "  pslld $7, %%mm1 \n"
      "  pand %%mm7, %%mm1 \n"
      "  pxor %%mm1, %%mm0 \n"
      "  movq %%mm0, %%mm1 \n"
      "  pslld $15, %%mm1 \n"
      "  pand %%mm6, %%mm1 \n"
      "  pxor %%mm1, %%mm0 \n"
      "  movq %%mm0, %%mm1 \n"
      "  psrld $18, %%mm1 \n"
      "  pxor %%mm1, %%mm0 \n"
      "  movq %%mm0, 0(%0) \n"
      "  add $8, %0\n"
      "  add $8, %1\n"
      "  decl %2 \n"
      "  jne 3b \n"
      "  emms \n"

      : "+r" (d), "+r" (mt)
      : "r" (0)
      );

}
OIL_DEFINE_IMPL_FULL (mt19937_i386_mmx_2, mt19937, OIL_IMPL_FLAG_MMX | OIL_IMPL_FLAG_MMXEXT);


static void
mt19937_i386_mmx_3 (uint32_t *d, uint32_t *mt)
{
  asm volatile (
      "  mov $0x80000000, %2\n"
      "  movd %2, %%mm7\n"
      "  pshufw $0x44, %%mm7, %%mm7 \n"
      "  mov $0x7fffffff, %2\n"
      "  movd %2, %%mm6\n"
      "  pshufw $0x44, %%mm6, %%mm6 \n"
      "  mov $0x9908b0df, %2\n"
      "  movd %2, %%mm5\n"
      "  pshufw $0x44, %%mm5, %%mm5 \n"
      "  mov $0x1, %2\n"
      "  movd %2, %%mm4\n"

      "  mov $56, %2 \n"
      "1: \n"
      TWIST4(4,1588)
      "  add $16, %1\n"
      "  decl %2 \n"
      "  jne 1b \n"

      TWIST2(4,1588)
      "  add $8, %1\n"

      TWIST(4,1588)
      "  add $4, %1\n"

      "  mov $99, %2 \n"
      "2: \n"
      TWIST4(4,-908)
      "  add $16, %1\n"
      "  decl %2 \n"
      "  jne 2b \n"

      TWIST(-2492,-908)

      "  add $-2492, %1 \n"

      "  mov $0x9d2c5680, %2\n"
      "  movd %2, %%mm7\n"
      "  pshufw $0x44, %%mm7, %%mm7 \n"
      "  mov $0xefc60000, %2\n"
      "  movd %2, %%mm6\n"
      "  pshufw $0x44, %%mm6, %%mm6 \n"
      "  mov $156, %2 \n"
      "3:\n"
      "  movq 0(%1), %%mm0 \n"
      "   movq 8(%1), %%mm2 \n"
      "  movq 0(%1), %%mm1 \n"
      "   movq 8(%1), %%mm3 \n"
      "  psrld $11, %%mm1 \n"
      "   psrld $11, %%mm3 \n"
      "  pxor %%mm1, %%mm0 \n"
      "   pxor %%mm3, %%mm2 \n"
      "  movq %%mm0, %%mm1 \n"
      "   movq %%mm2, %%mm3 \n"
      "  pslld $7, %%mm1 \n"
      "   pslld $7, %%mm3 \n"
      "  pand %%mm7, %%mm1 \n"
      "   pand %%mm7, %%mm3 \n"
      "  pxor %%mm1, %%mm0 \n"
      "   pxor %%mm3, %%mm2 \n"
      "  movq %%mm0, %%mm1 \n"
      "   movq %%mm2, %%mm3 \n"
      "  pslld $15, %%mm1 \n"
      "   pslld $15, %%mm3 \n"
      "  pand %%mm6, %%mm1 \n"
      "   pand %%mm6, %%mm3 \n"
      "  pxor %%mm1, %%mm0 \n"
      "   pxor %%mm3, %%mm2 \n"
      "  movq %%mm0, %%mm1 \n"
      "   movq %%mm2, %%mm3 \n"
      "  psrld $18, %%mm1 \n"
      "   psrld $18, %%mm3 \n"
      "  pxor %%mm1, %%mm0 \n"
      "   pxor %%mm3, %%mm2 \n"
      "  movq %%mm0, 0(%0) \n"
      "   movq %%mm2, 8(%0) \n"
      "  add $16, %0\n"
      "  add $16, %1\n"
      "  decl %2 \n"
      "  jne 3b \n"
      "  emms \n"

      : "+r" (d), "+r" (mt)
      : "r" (0)
      );

}
OIL_DEFINE_IMPL_FULL (mt19937_i386_mmx_3, mt19937, OIL_IMPL_FLAG_MMX | OIL_IMPL_FLAG_MMXEXT);



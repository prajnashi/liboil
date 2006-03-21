
#include <liboil/liboilfunction.h>
#include <liboil/liboilclasses.h>


void
split_53_nomix (int16_t *d_2xn, int16_t *s_2xn, int n)
{
  int i;
    
  if (n == 0) return;
  /* predict */
  for(i=1;i<n*2-2;i+=2){
    d_2xn[i] = s_2xn[i] - ((s_2xn[i-1] + s_2xn[i+1]) >> 1);
  }
  d_2xn[n*2-1] = s_2xn[n*2-1] - s_2xn[n*2-2];

  /* update */
  d_2xn[0] = s_2xn[0] + (d_2xn[1] >> 1);
  for(i=2;i<n*2;i+=2){
    d_2xn[i] = s_2xn[i] + ((d_2xn[i-1] + d_2xn[i+1]) >> 2);
  }
} 
OIL_DEFINE_IMPL (split_53_nomix, split_53);
  
#if 0
void
synth_53_nomix (int16_t *d_2xn, int16_t *s_2xn, int n)
{ 
  int i;
    
  /* predict */ 
  i_n[0] -= i_n[1] >> 1;
  for(i=2;i<n*2;i+=2){
    i_n[i] -= (i_n[i-1] + i_n[i+1]) >> 2;
  }
  
  /* update */
  for(i=1;i<n*2-2;i+=2){
    i_n[i] += (i_n[i+1] + i_n[i-1]) >> 1;
  }
  i_n[n*2-1] += i_n[n*2-2];
}
#endif


void
split_53_c (int16_t *d_2xn, int16_t *s_2xn, int n)
{
  int i;

  if (n == 0) return;
  if (n == 1) {
    d_2xn[1] = s_2xn[1] - s_2xn[0];
    d_2xn[0] = s_2xn[0] + (d_2xn[1] >> 1);
  } else {
    d_2xn[1] = s_2xn[1] - ((s_2xn[0] + s_2xn[2]) >> 1);
    d_2xn[0] = s_2xn[0] + (d_2xn[1] >> 1);
    d_2xn+=2;
    s_2xn+=2;
    for(i=0;i<(n*2-4)/2;i++){
      d_2xn[1] = s_2xn[1] - ((s_2xn[0] + s_2xn[2]) >> 1);
      d_2xn[0] = s_2xn[0] + ((d_2xn[-1] + d_2xn[1]) >> 2);
      d_2xn+=2;
      s_2xn+=2;
    }
    d_2xn[1] = s_2xn[1] - s_2xn[0];
    d_2xn[0] = s_2xn[0] + ((d_2xn[-1] + d_2xn[1]) >> 2);
  }
}
OIL_DEFINE_IMPL (split_53_c, split_53);

void
synth_53_c (int16_t *d_2xn, int16_t *s_2xn, int n)
{
  int i;

  if (n == 0) return;
  if (n == 1) {
    d_2xn[0] = s_2xn[0] - (s_2xn[1] >> 1);
    d_2xn[1] = s_2xn[1] + d_2xn[0];
  } else {
    d_2xn[0] = s_2xn[0] - (s_2xn[1] >> 1);
    for(i=2;i<n*2-2;i+=2){
      d_2xn[i] = s_2xn[i] - ((s_2xn[i-1] + s_2xn[i+1]) >> 2);
      d_2xn[i-1] = s_2xn[i-1] + ((d_2xn[i] + d_2xn[i-2]) >> 1);
    }
    d_2xn[n*2-2] = s_2xn[n*2-2] - ((s_2xn[n*2-3] + s_2xn[n*2-1]) >> 2);
    d_2xn[n*2-3] = s_2xn[n*2-3] + ((d_2xn[n*2-2] + d_2xn[n*2-4]) >> 1);
    d_2xn[n*2-1] = s_2xn[n*2-1] + d_2xn[n*2-2];
  }
}
OIL_DEFINE_IMPL (synth_53_c, synth_53);

void
deinterleave_c_1 (int16_t *d_2xn, int16_t *s_2xn, int n)
{
  int i;

  for(i=0;i<n;i++) {
    d_2xn[i] = s_2xn[2*i];
    d_2xn[n + i] = s_2xn[2*i + 1];
  }
}
OIL_DEFINE_IMPL (deinterleave_c_1, deinterleave);

void
deinterleave_asm (int16_t *d_2xn, int16_t *s_2xn, int n)
{
  int16_t *d2;

  if (n == 0) return;

  d2 = d_2xn + n;
  while (n&1) {
    d_2xn[0] = s_2xn[0];
    d2[0] = s_2xn[1];
    d_2xn++;
    d2++;
    s_2xn+=2;
    n--;
  }

  asm volatile ("\n"
      "  mov %3, %%ecx\n"
      "  sub $2, %%ecx\n"
      "1:\n"
      "  movw (%1,%%ecx,4), %%ax\n"
      "  movw %%ax, (%0,%%ecx,2)\n"
      "  movw 2(%1,%%ecx,4), %%ax\n"
      "  movw %%ax, (%2,%%ecx,2)\n"
      "  movw 4(%1,%%ecx,4), %%ax\n"
      "  movw %%ax, 2(%0,%%ecx,2)\n"
      "  movw 6(%1,%%ecx,4), %%ax\n"
      "  movw %%ax, 2(%2,%%ecx,2)\n"
      "  sub $2, %%ecx\n"
      "  jge 1b\n"
      : "+r" (d_2xn), "+r" (s_2xn), "+r" (d2)
      : "m" (n)
      : "eax", "ecx");
}
OIL_DEFINE_IMPL (deinterleave_asm, deinterleave);

void
deinterleave_mmx (int16_t *d_2xn, int16_t *s_2xn, int n)
{
  int16_t *d2;

  d2 = d_2xn + n;

  while (n&3) {
    d_2xn[0] = s_2xn[0];
    d2[0] = s_2xn[1];
    d_2xn++;
    d2++;
    s_2xn+=2;
    n--;
  }
  if (n==0) return;

  asm volatile ("\n"
      "  xor %%ecx, %%ecx\n"
      "1:\n"
      "  movq (%1,%%ecx,4), %%mm0\n"
      "  movq 8(%1,%%ecx,4), %%mm1\n"
      "  pslld $16, %%mm0\n"
      "  pslld $16, %%mm1\n"
      "  psrad $16, %%mm0\n"
      "  psrad $16, %%mm1\n"
      "  packssdw %%mm1, %%mm0\n"
      "  movq %%mm0, (%0,%%ecx,2)\n"
      "  movq (%1,%%ecx,4), %%mm0\n"
      "  movq 8(%1,%%ecx,4), %%mm1\n"
      "  psrad $16, %%mm0\n"
      "  psrad $16, %%mm1\n"
      "  packssdw %%mm1, %%mm0\n"
      "  movq %%mm0, (%2,%%ecx,2)\n"
      "  add $4, %%ecx\n"
      "  cmp %3, %%ecx\n"
      "  jl 1b\n"
      "  emms\n"
      : "+r" (d_2xn), "+r" (s_2xn), "+r" (d2)
      : "m" (n)
      : "eax", "ecx");
}
OIL_DEFINE_IMPL_FULL (deinterleave_mmx, deinterleave, OIL_IMPL_FLAG_MMX);

void
deinterleave_mmx_2 (int16_t *d_2xn, int16_t *s_2xn, int n)
{
  int16_t *d2;

  d2 = d_2xn + n;

  while (n&3) {
    d_2xn[0] = s_2xn[0];
    d2[0] = s_2xn[1];
    d_2xn++;
    d2++;
    s_2xn+=2;
    n--;
  }
  if (n==0) return;

  asm volatile ("\n"
      "  xor %%ecx, %%ecx\n"
      "1:\n"
      "  pshufw $0xd8, (%1,%%ecx,4), %%mm0\n"
      "  movd %%mm0, (%0,%%ecx,2)\n"
      "  pshufw $0x8d, (%1,%%ecx,4), %%mm0\n"
      "  movd %%mm0, (%2,%%ecx,2)\n"
      "  add $2, %%ecx\n"
      "  cmp %3, %%ecx\n"
      "  jl 1b\n"
      "  emms\n"
      : "+r" (d_2xn), "+r" (s_2xn), "+r" (d2)
      : "m" (n)
      : "eax", "ecx");
}
OIL_DEFINE_IMPL_FULL (deinterleave_mmx_2, deinterleave, OIL_IMPL_FLAG_MMX | OIL_IMPL_FLAG_MMXEXT);

void
deinterleave_mmx_3 (int16_t *d_2xn, int16_t *s_2xn, int n)
{
  int16_t *d2;

  d2 = d_2xn + n;

  while (n&3) {
    d_2xn[0] = s_2xn[0];
    d2[0] = s_2xn[1];
    d_2xn++;
    d2++;
    s_2xn+=2;
    n--;
  }
  if (n==0) return;

  asm volatile ("\n"
      "  xor %%ecx, %%ecx\n"
      "1:\n"
      "  movq (%1,%%ecx,4), %%mm1\n"
      "  movq (%1,%%ecx,4), %%mm2\n"
      "  movq 8(%1,%%ecx,4), %%mm0\n"
      "  punpcklwd %%mm0, %%mm1\n"
      "  punpckhwd %%mm0, %%mm2\n"
      "  movq %%mm1, %%mm0\n"
      "  punpcklwd %%mm2, %%mm0\n"
      "  punpckhwd %%mm2, %%mm1\n"
      "  movq %%mm0, (%0,%%ecx,2)\n"
      "  movq %%mm1, (%2,%%ecx,2)\n"
      "  add $4, %%ecx\n"
      "  cmp %3, %%ecx\n"
      "  jl 1b\n"
      "  emms\n"
      : "+r" (d_2xn), "+r" (s_2xn), "+r" (d2)
      : "m" (n)
      : "eax", "ecx");
}
OIL_DEFINE_IMPL_FULL (deinterleave_mmx_3, deinterleave, OIL_IMPL_FLAG_MMX);

void
deinterleave_mmx_4 (int16_t *d_2xn, int16_t *s_2xn, int n)
{
  int16_t *d2;

  d2 = d_2xn + n;

  while (n&7) {
    d_2xn[0] = s_2xn[0];
    d2[0] = s_2xn[1];
    d_2xn++;
    d2++;
    s_2xn+=2;
    n--;
  }
  if (n==0) return;

  asm volatile ("\n"
      "  xor %%ecx, %%ecx\n"
      "1:\n"
      "  movq (%1,%%ecx,4), %%mm1\n"
      "  movq %%mm1, %%mm2\n"
      "  movq 8(%1,%%ecx,4), %%mm0\n"
      "   movq 16(%1,%%ecx,4), %%mm5\n"
      "  punpcklwd %%mm0, %%mm1\n"
      "   movq %%mm5, %%mm6\n"
      "  punpckhwd %%mm0, %%mm2\n"
      "   movq 24(%1,%%ecx,4), %%mm4\n"
      "  movq %%mm1, %%mm0\n"
      "   punpcklwd %%mm4, %%mm5\n"
      "  punpcklwd %%mm2, %%mm0\n"
      "   punpckhwd %%mm4, %%mm6\n"
      "  punpckhwd %%mm2, %%mm1\n"
      "   movq %%mm5, %%mm4\n"
      "  movq %%mm0, (%0,%%ecx,2)\n"
      "   punpcklwd %%mm6, %%mm4\n"
      "  movq %%mm1, (%2,%%ecx,2)\n"
      "   punpckhwd %%mm6, %%mm5\n"
      "   movq %%mm4, 8(%0,%%ecx,2)\n"
      "   movq %%mm5, 8(%2,%%ecx,2)\n"
      "  add $8, %%ecx\n"
      "  cmp %3, %%ecx\n"
      "  jl 1b\n"
      "  emms\n"
      : "+r" (d_2xn), "+r" (s_2xn), "+r" (d2)
      : "m" (n)
      : "eax", "ecx");
}
OIL_DEFINE_IMPL_FULL (deinterleave_mmx_4, deinterleave, OIL_IMPL_FLAG_MMX);

void
interleave_c (int16_t *d_2xn, int16_t *s_2xn, int n)
{
  int i;

  for(i=0;i<n;i++) {
    d_2xn[2*i] = s_2xn[i];
    d_2xn[2*i + 1] = s_2xn[n + i];
  }
}
OIL_DEFINE_IMPL (interleave_c, interleave);


#ifdef ENABLE_BROKEN_IMPLS
void
lift_add_mult_shift12_i386_mmx (int16_t *d, int16_t *s1, int16_t *s2,
    int16_t *s3, int16_t *s4, int n)
{
  uint32_t val = *s4;

  while (n&3) {
    d[0] = s1[0] + ((s4[0]*(s2[0] + s3[0]))>>12);
    d++;
    s1++;
    s2++;
    s3++;
    n--;
  }
  if (n==0) return;

  val = ((*(uint16_t *)s4)<<16) | (*(uint16_t *)s4);
  n>>=2;
  asm volatile ("\n"
      "  mov %4, %%ecx\n"
      "  movd %%ecx, %%mm7\n"
      "  punpcklwd %%mm7, %%mm7\n"
      "  mov %5, %%ecx\n"
      "1:\n"
      "  movq 0(%2), %%mm0\n"
      "  paddsw 0(%3), %%mm0\n"
      "  movq %%mm0, %%mm1\n"
      "  pmullw %%mm7, %%mm0\n"
      "  pmulhw %%mm7, %%mm1\n"
      "  psrlw $12, %%mm0\n"
      "  psllw $4, %%mm1\n"
      "  por %%mm1, %%mm0\n"
      "  paddsw 0(%1), %%mm0\n"
      "  movq %%mm0, 0(%0)\n"
      "  decl %%ecx\n"
      "  add $8, %0\n"
      "  add $8, %1\n"
      "  add $8, %2\n"
      "  add $8, %3\n"
      "  jne 1b\n"
      "  emms\n"
      : "+r" (d), "+r" (s1), "+r" (s2), "+r" (s3)
      : "m" (val), "m" (n)
      : "ecx");
}
OIL_DEFINE_IMPL (lift_add_mult_shift12_i386_mmx, lift_add_mult_shift12);
#endif


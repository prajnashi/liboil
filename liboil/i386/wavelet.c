
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
deinterleave2_c_1 (int16_t *d1, int16_t *d2, int16_t *s_2xn, int n)
{
  int i;

  for(i=0;i<n;i++) {
    d1[i] = s_2xn[2*i];
    d2[i] = s_2xn[2*i + 1];
  }
}
OIL_DEFINE_IMPL (deinterleave2_c_1, deinterleave2_s16);

void
deinterleave2_asm (int16_t *d1, int16_t *d2, int16_t *s_2xn, int n)
{
  if (n == 0) return;

  while (n&1) {
    d1[0] = s_2xn[0];
    d2[0] = s_2xn[1];
    d1++;
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
      : "+r" (d1), "+r" (s_2xn), "+r" (d2)
      : "m" (n)
      : "eax", "ecx");
}
OIL_DEFINE_IMPL (deinterleave2_asm, deinterleave2_s16);

void
deinterleave2_mmx (int16_t *d1, int16_t *d2, int16_t *s_2xn, int n)
{
  while (n&3) {
    d1[0] = s_2xn[0];
    d2[0] = s_2xn[1];
    d1++;
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
      : "+r" (d1), "+r" (s_2xn), "+r" (d2)
      : "m" (n)
      : "eax", "ecx");
}
OIL_DEFINE_IMPL_FULL (deinterleave2_mmx, deinterleave2_s16, OIL_IMPL_FLAG_MMX);

void
deinterleave2_mmx_2 (int16_t *d1, int16_t *d2, int16_t *s_2xn, int n)
{
  while (n&3) {
    d1[0] = s_2xn[0];
    d2[0] = s_2xn[1];
    d1++;
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
      : "+r" (d1), "+r" (s_2xn), "+r" (d2)
      : "m" (n)
      : "eax", "ecx");
}
OIL_DEFINE_IMPL_FULL (deinterleave2_mmx_2, deinterleave2_s16, OIL_IMPL_FLAG_MMX | OIL_IMPL_FLAG_MMXEXT);

void
deinterleave2_mmx_3 (int16_t *d1, int16_t *d2, int16_t *s_2xn, int n)
{
  while (n&3) {
    d1[0] = s_2xn[0];
    d2[0] = s_2xn[1];
    d1++;
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
      : "+r" (d1), "+r" (s_2xn), "+r" (d2)
      : "m" (n)
      : "eax", "ecx");
}
OIL_DEFINE_IMPL_FULL (deinterleave2_mmx_3, deinterleave2_s16, OIL_IMPL_FLAG_MMX);

void
deinterleave2_mmx_4 (int16_t *d1, int16_t *d2, int16_t *s_2xn, int n)
{
  while (n&7) {
    d1[0] = s_2xn[0];
    d2[0] = s_2xn[1];
    d1++;
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
      : "+r" (d1), "+r" (s_2xn), "+r" (d2)
      : "m" (n)
      : "eax", "ecx");
}
OIL_DEFINE_IMPL_FULL (deinterleave2_mmx_4, deinterleave2_s16, OIL_IMPL_FLAG_MMX);

void
interleave2_c (int16_t *d_2xn, int16_t *s1, int16_t *s2, int n)
{
  int i;

  for(i=0;i<n;i++) {
    d_2xn[2*i] = s1[i];
    d_2xn[2*i + 1] = s2[i];
  }
}
OIL_DEFINE_IMPL (interleave2_c, interleave2_s16);


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

void
interleave2_mmx (int16_t *d_2xn, int16_t *s1, int16_t *s2, int n)
{
  while (n&3) {
    d_2xn[0] = s1[0];
    d_2xn[1] = s2[0];
    s1++;
    s2++;
    d_2xn+=2;
    n--;
  }
  if (n==0) return;

  asm volatile ("\n"
      "  xor %%ecx, %%ecx\n"
      "1:\n"
      "  movq (%1,%%ecx,2), %%mm0\n"
      "  movq (%2,%%ecx,2), %%mm1\n"
      "  movq %%mm0, %%mm2\n"
      "  punpckhwd %%mm1, %%mm0\n"
      "  punpcklwd %%mm1, %%mm2\n"
      "  movq %%mm2, (%0,%%ecx,4)\n"
      "  movq %%mm0, 8(%0,%%ecx,4)\n"
      "  add $4, %%ecx\n"
      "  cmp %3, %%ecx\n"
      "  jl 1b\n"
      "  emms\n"
      : "+r" (d_2xn), "+r" (s1), "+r" (s2)
      : "m" (n)
      : "eax", "ecx");
}
OIL_DEFINE_IMPL_FULL (interleave2_mmx, interleave2_s16, OIL_IMPL_FLAG_MMX);

void
lift_add_shift1_mmx (int16_t *d, int16_t *s1, int16_t *s2, int16_t *s3, int n)
{
  while (n&3) {
    d[0] = s1[0] + ((s2[0] + s3[0])>>1);
    d++;
    s1++;
    s2++;
    s3++;
    n--;
  }
  if (n==0) return;

  asm volatile ("\n"
      "  xor %%ecx, %%ecx\n"
      "1:\n"
      "  movq (%2,%%ecx,2), %%mm1\n"
      "  movq (%3,%%ecx,2), %%mm2\n"
      "  paddw %%mm2, %%mm1\n"
      "  psraw $1, %%mm1\n"
      "  paddw (%1,%%ecx,2), %%mm1\n"
      "  movq %%mm1, (%0,%%ecx,2)\n"
      "  add $4, %%ecx\n"
      "  cmp %4, %%ecx\n"
      "  jl 1b\n"
      "  emms\n"
      : "+r" (d), "+r" (s1), "+r" (s2), "+r" (s3)
      : "m" (n)
      : "ecx");
}
OIL_DEFINE_IMPL_FULL (lift_add_shift1_mmx, lift_add_shift1, OIL_IMPL_FLAG_MMX);

void
lift_sub_shift1_mmx (int16_t *d, int16_t *s1, int16_t *s2, int16_t *s3, int n)
{
  while (n&3) {
    d[0] = s1[0] - ((s2[0] + s3[0])>>1);
    d++;
    s1++;
    s2++;
    s3++;
    n--;
  }
  if (n==0) return;

  asm volatile ("\n"
      "  xor %%ecx, %%ecx\n"
      "1:\n"
      "  movq (%2,%%ecx,2), %%mm1\n"
      "  movq (%3,%%ecx,2), %%mm2\n"
      "  movq (%1,%%ecx,2), %%mm0\n"
      "  paddw %%mm2, %%mm1\n"
      "  psraw $1, %%mm1\n"
      "  psubw %%mm1, %%mm0\n"
      "  movq %%mm0, (%0,%%ecx,2)\n"
      "  add $4, %%ecx\n"
      "  cmp %4, %%ecx\n"
      "  jl 1b\n"
      "  emms\n"
      : "+r" (d), "+r" (s1), "+r" (s2), "+r" (s3)
      : "m" (n)
      : "ecx");
}
OIL_DEFINE_IMPL_FULL (lift_sub_shift1_mmx, lift_sub_shift1, OIL_IMPL_FLAG_MMX);

void
lift_add_shift2_mmx (int16_t *d, int16_t *s1, int16_t *s2, int16_t *s3, int n)
{
  while (n&3) {
    d[0] = s1[0] + ((s2[0] + s3[0])>>2);
    d++;
    s1++;
    s2++;
    s3++;
    n--;
  }
  if (n==0) return;

  asm volatile ("\n"
      "  xor %%ecx, %%ecx\n"
      "1:\n"
      "  movq (%2,%%ecx,2), %%mm1\n"
      "  movq (%3,%%ecx,2), %%mm2\n"
      "  paddw %%mm2, %%mm1\n"
      "  psraw $2, %%mm1\n"
      "  paddw (%1,%%ecx,2), %%mm1\n"
      "  movq %%mm1, (%0,%%ecx,2)\n"
      "  add $4, %%ecx\n"
      "  cmp %4, %%ecx\n"
      "  jl 1b\n"
      "  emms\n"
      : "+r" (d), "+r" (s1), "+r" (s2), "+r" (s3)
      : "m" (n)
      : "ecx");
}
OIL_DEFINE_IMPL_FULL (lift_add_shift2_mmx, lift_add_shift2, OIL_IMPL_FLAG_MMX);

void
lift_sub_shift2_mmx (int16_t *d, int16_t *s1, int16_t *s2, int16_t *s3, int n)
{
  while (n&3) {
    d[0] = s1[0] - ((s2[0] + s3[0])>>2);
    d++;
    s1++;
    s2++;
    s3++;
    n--;
  }
  if (n==0) return;

  asm volatile ("\n"
      "  xor %%ecx, %%ecx\n"
      "1:\n"
      "  movq (%2,%%ecx,2), %%mm1\n"
      "  movq (%3,%%ecx,2), %%mm2\n"
      "  movq (%1,%%ecx,2), %%mm0\n"
      "  paddw %%mm2, %%mm1\n"
      "  psraw $2, %%mm1\n"
      "  psubw %%mm1, %%mm0\n"
      "  movq %%mm0, (%0,%%ecx,2)\n"
      "  add $4, %%ecx\n"
      "  cmp %4, %%ecx\n"
      "  jl 1b\n"
      "  emms\n"
      : "+r" (d), "+r" (s1), "+r" (s2), "+r" (s3)
      : "m" (n)
      : "ecx");
}
OIL_DEFINE_IMPL_FULL (lift_sub_shift2_mmx, lift_sub_shift2, OIL_IMPL_FLAG_MMX);

#ifdef ENABLE_BROKEN_IMPLS
void
synth_53_mmx (int16_t *d_2xn, int16_t *s_2xn, int n)
{
  int i;
    
  if (n==0) return;
  if (n == 1) {
    d_2xn[0] = s_2xn[0] - (s_2xn[1] >> 1);
    d_2xn[1] = s_2xn[1] + d_2xn[0]; 
  } else {
    int i;

    d_2xn[0] = s_2xn[0] - (s_2xn[1] >> 1);

    if (n > 6) {
      n-=5;

      asm volatile ("\n"
          "  xor %%ecx, %%ecx\n"
          "  movw 2(%1), %%ecx\n"
          "  movd %%ecx, %%mm7\n"
          "  movw 0(%0), %%ecx\n"
          "  movd %%ecx, %%mm6\n"
          "  movw 0(%1), %%ecx\n"
          "  movd %%ecx, %%mm5\n"

          "  xor %%ecx, %%ecx\n"
          "1:\n"
          "  movq 4(%1,%%ecx,4), %%mm1\n"  // mm1 = s5 s4 s3 s2
          "  movq %%mm1, %%mm2\n"          // mm2 = s5 s4 s3 s2
          "  movq 12(%1,%%ecx,4), %%mm0\n" // mm0 = s9 s8 s7 s6
          "  punpcklwd %%mm0, %%mm1\n"     // mm1 = s7 s3 s6 s2
          "  punpckhwd %%mm0, %%mm2\n"     // mm2 = s9 s5 s8 s4
          "  movq %%mm1, %%mm0\n"          // mm0 = s7 s3 s6 s2
          "  punpcklwd %%mm2, %%mm0\n"     // mm0 = s8 s6 s4 s2
          "  punpckhwd %%mm2, %%mm1\n"     // mm1 = s9 s7 s5 s3
          //"  movq %%mm0, %%mm3\n"          // mm0 = s8 s6 s4 s2

          "  movq %%mm1, %%mm2\n"          // mm2 = s9 s7 s5 s3
          "  psllq $16, %%mm2\n"           // mm2 = s7 s5 s3 00
          "  por %%mm7, %%mm2\n"           // mm2 = s7 s5 s3 s1
          "  movq %%mm2, %%mm4\n"          // mm4 = s7 s5 s3 s1
          "  paddw %%mm1, %%mm2\n"         // mm2 = s9+s7 ...
          "  psraw $2, %%mm2\n"            // mm2 = (s9+s7)>>2 ...
          "  movq %%mm1, %%mm7\n"          // mm7 = s9 s7 s5 s3
          "  psrlq $48, %%mm7\n"           // mm7 = 00 00 00 s9
          "  psubw %%mm2, %%mm0\n"         // mm0 = d8 d6 d4 d2

          "  movq %%mm0, %%mm1\n"          // mm1 = d8 d6 d4 d2
          "  movq %%mm0, %%mm3\n"          // mm1 = d8 d6 d4 d2
          "  psllq $16, %%mm0\n"           // mm0 = d6 d4 d2 00
          "  por %%mm6, %%mm0\n"           // mm0 = d6 d4 d2 d0
          "  psrlq $48, %%mm1\n"           // mm1 = 00 00 00 d8
          "  movq %%mm1, %%mm6\n"          // mm6 = 00 00 00 d8

          "  movq %%mm0, %%mm1\n"
          "  paddw %%mm3, %%mm1\n"         // mm0 = d8+d6 ...
          "  psraw $1, %%mm1\n"            // mm1 = (d8+d6)>>1 ...
          "  paddw %%mm4, %%mm1\n"         // mm1 = d7 d5 d3 d1

          "  movq %%mm1, %%mm2\n"

          "  movq %%mm0, %%mm1\n"
          "  punpcklwd %%mm2, %%mm0\n"
          "  punpckhwd %%mm2, %%mm1\n"

          "  movq %%mm0, (%0, %%ecx, 4)\n"
          "  movq %%mm1, 8(%0, %%ecx, 4)\n"

          "  add $4, %%ecx\n"
          "  cmp %3, %%ecx\n"
          "  jl 1b\n"
          "  emms\n"
          : "+r" (d_2xn), "+r" (s_2xn), "+ecx" (i)
          : "m" (n));

      i*=2;
      n+=5;
      d_2xn[i] = s_2xn[i] - ((s_2xn[i-1] + s_2xn[i+1]) >> 2);
      i+=2;
    } else {
      i = 2;
    }
    for(;i<n*2-2;i+=2){
      d_2xn[i] = s_2xn[i] - ((s_2xn[i-1] + s_2xn[i+1]) >> 2);
      d_2xn[i-1] = s_2xn[i-1] + ((d_2xn[i] + d_2xn[i-2]) >> 1);
    }
    d_2xn[n*2-2] = s_2xn[n*2-2] - ((s_2xn[n*2-3] + s_2xn[n*2-1]) >> 2);
    d_2xn[n*2-3] = s_2xn[n*2-3] + ((d_2xn[n*2-2] + d_2xn[n*2-4]) >> 1);
    d_2xn[n*2-1] = s_2xn[n*2-1] + d_2xn[n*2-2];
  } 
}
OIL_DEFINE_IMPL_FULL (synth_53_mmx, synth_53, OIL_IMPL_FLAG_MMX);
#endif


void
mas2_add_s16_mmx (int16_t *d1, int16_t *s1, int16_t *s2, int16_t *s3_2,
    int16_t *s4_2, int n)
{
  int shift = s4_2[1];
    
  while (n&3) {
    int x;

    x = s4_2[0] + s2[0]*s3_2[0] + s2[1]*s3_2[1];
    x >>= s4_2[1];
    d1[0] = s1[0] + x;

    d1++;
    s1++;
    s2++;
    n--;
  }
  if (n==0) return;

  n>>=2;
  asm volatile ("\n"
      "  xor %%ecx, %%ecx\n"
      "  movw 0(%0), %%cx\n"
      "  movd %%ecx, %%mm7\n"
      "  pshufw $0x00, %%mm7, %%mm7\n"
      "  movw 2(%0), %%cx\n"
      "  movd %%ecx, %%mm6\n"
      "  pshufw $0x00, %%mm6, %%mm6\n"
      "  movw 0(%1), %%cx\n"
      "  movd %%ecx, %%mm5\n"
      "  pshufw $0x00, %%mm5, %%mm5\n"
      "  movw 2(%1), %%cx\n"
      "  movd %%ecx, %%mm4\n"
      :: "r" (s3_2), "r" (s4_2)
      : "ecx"
      );
  asm volatile ("\n"
      "1:\n"
      "  movq 0(%2), %%mm0\n"
      "  pmullw %%mm7, %%mm0\n"
      "  movq 2(%2), %%mm1\n"
      "  pmullw %%mm6, %%mm1\n"
      "  paddw %%mm1, %%mm0\n"
      "  paddw %%mm5, %%mm0\n"
      "  psraw %%mm4, %%mm0\n"
      "  paddw 0(%1), %%mm0\n"
      "  movq %%mm0, 0(%0)\n"
      "  add $8, %0\n"
      "  add $8, %1\n"
      "  add $8, %2\n"
      "  decl %3\n"
      "  jnz 1b\n"
      "  emms\n"
      : "+r" (d1), "+r" (s1), "+r" (s2), "+r" (n)
      );
}
OIL_DEFINE_IMPL_FULL (mas2_add_s16_mmx, mas2_add_s16, OIL_IMPL_FLAG_MMX | OIL_IMPL_FLAG_MMXEXT);


void
mas4_add_s16_mmx (int16_t *d1, int16_t *s1, int16_t *s2, int16_t *s3_2,
    int16_t *s4_2, int n)
{
  int shift = s4_2[1];
    
  while (n&3) {
    int x;

    x = s4_2[0] + s2[0]*s3_2[0] + s2[1]*s3_2[1] + 
      s2[2]*s3_2[2] + s2[2]*s3_2[2];
    x >>= s4_2[1];
    d1[0] = s1[0] + x;

    d1++;
    s1++;
    s2++;
    n--;
  }
  if (n==0) return;

  n>>=2;
  asm volatile ("\n"
      "  xor %%ecx, %%ecx\n"
      "  movw 0(%0), %%cx\n"
      "  movd %%ecx, %%mm7\n"
      "  pshufw $0x00, %%mm7, %%mm7\n"
      "  movw 2(%0), %%cx\n"
      "  movd %%ecx, %%mm6\n"
      "  pshufw $0x00, %%mm6, %%mm6\n"
      "  movw 2(%0), %%cx\n"
      "  movd %%ecx, %%mm5\n"
      "  pshufw $0x00, %%mm5, %%mm5\n"
      "  movw 2(%0), %%cx\n"
      "  movd %%ecx, %%mm4\n"
      "  pshufw $0x00, %%mm4, %%mm4\n"
      "  movw 0(%1), %%cx\n"
      "  movd %%ecx, %%mm3\n"
      "  pshufw $0x00, %%mm3, %%mm3\n"
      "  movw 2(%1), %%cx\n"
      "  movd %%ecx, %%mm2\n"
      :: "r" (s3_2), "r" (s4_2)
      : "ecx"
      );
  asm volatile ("\n"
      "1:\n"
      "  movq 0(%2), %%mm0\n"
      "  pmullw %%mm7, %%mm0\n"
      "  movq 2(%2), %%mm1\n"
      "  pmullw %%mm6, %%mm1\n"
      "  paddw %%mm1, %%mm0\n"
      "  movq 4(%2), %%mm1\n"
      "  pmullw %%mm5, %%mm1\n"
      "  paddw %%mm1, %%mm0\n"
      "  movq 6(%2), %%mm1\n"
      "  pmullw %%mm4, %%mm1\n"
      "  paddw %%mm1, %%mm0\n"
      "  paddw %%mm3, %%mm0\n"
      "  psraw %%mm2, %%mm0\n"
      "  paddw 0(%1), %%mm0\n"
      "  movq %%mm0, 0(%0)\n"
      "  add $8, %0\n"
      "  add $8, %1\n"
      "  add $8, %2\n"
      "  decl %3\n"
      "  jnz 1b\n"
      "  emms\n"
      : "+r" (d1), "+r" (s1), "+r" (s2), "+r" (n)
      );
}
OIL_DEFINE_IMPL_FULL (mas4_add_s16_mmx, mas4_add_s16, OIL_IMPL_FLAG_MMX | OIL_IMPL_FLAG_MMXEXT);



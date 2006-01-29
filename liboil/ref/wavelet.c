
#include <liboil/liboilfunction.h>
#include <liboil/liboiltest.h>
#include <liboil/liboilrandom.h>


static void
wavelet_test (OilTest *test)
{
  int16_t *data = (int16_t *)oil_test_get_source_data (test, OIL_ARG_SRC1);
  int i;

  for(i=0;i<test->n*2;i++){
    data[i] = oil_rand_u8();
  }
}


OIL_DEFINE_CLASS_FULL (deinterleave,
    "int16_t *d_2xn, int16_t *s_2xn, int n", wavelet_test);
OIL_DEFINE_CLASS_FULL (interleave,
    "int16_t *d_2xn, int16_t *s_2xn, int n", wavelet_test);
OIL_DEFINE_CLASS_FULL (synth_daub97,
    "int16_t *d_2xn, int16_t *s_2xn, int n", wavelet_test);
OIL_DEFINE_CLASS_FULL (split_daub97,
    "int16_t *d_2xn, int16_t *s_2xn, int n", wavelet_test);
OIL_DEFINE_CLASS_FULL (split_approx97,
    "int16_t *d_2xn, int16_t *s_2xn, int n", wavelet_test);
OIL_DEFINE_CLASS_FULL (synth_approx97,
    "int16_t *d_2xn, int16_t *s_2xn, int n", wavelet_test);
OIL_DEFINE_CLASS_FULL (split_53,
    "int16_t *d_2xn, int16_t *s_2xn, int n", wavelet_test);
OIL_DEFINE_CLASS_FULL (synth_53,
    "int16_t *d_2xn, int16_t *s_2xn, int n", wavelet_test);
OIL_DEFINE_CLASS_FULL (split_135,
    "int16_t *d_2xn, int16_t *s_2xn, int n", wavelet_test);
OIL_DEFINE_CLASS_FULL (synth_135,
    "int16_t *d_2xn, int16_t *s_2xn, int n", wavelet_test);

void
deinterleave_ref (int16_t *d_2xn, int16_t *s_2xn, int n)
{
  int i;

  for(i=0;i<n;i++) {
    d_2xn[i] = s_2xn[2*i];
    d_2xn[n + i] = s_2xn[2*i + 1];
  }
}
OIL_DEFINE_IMPL_REF (deinterleave_ref, deinterleave);

void
interleave_ref (int16_t *d_2xn, int16_t *s_2xn, int n)
{
  int i;

  for(i=0;i<n;i++) {
    d_2xn[2*i] = s_2xn[i];
    d_2xn[2*i + 1] = s_2xn[n + i];
  }
}
OIL_DEFINE_IMPL_REF (interleave_ref, interleave);

void
synth_daub97_ref (int16_t *d_2xn, int16_t *s_2xn, int n)
{
  int i;

  if (n==0) return;
  /* predict */
  d_2xn[0] = s_2xn[0] - ((1817 * s_2xn[1]) >> 11);
  for(i=2;i<n*2;i+=2){
    d_2xn[i] = s_2xn[i] - ((1817 * (s_2xn[i-1] + s_2xn[i+1])) >> 12);
  }
  for(i=1;i<n*2-2;i+=2){
    d_2xn[i] = s_2xn[i] - ((3616 * (d_2xn[i-1] + d_2xn[i+1])) >> 12);
  }
  d_2xn[n*2-1] = s_2xn[n*2-1] - ((3616 * d_2xn[n*2-2]) >> 11);

  /* update */
  d_2xn[0] += (217 * d_2xn[1]) >> 11;
  for(i=2;i<n*2;i+=2){
    d_2xn[i] += (217 * (d_2xn[i-1] + d_2xn[i+1])) >> 12;
  }
  for(i=1;i<n*2-2;i+=2){
    d_2xn[i] += (6497 * (d_2xn[i-1] + d_2xn[i+1])) >> 12;
  }
  d_2xn[n*2-1] += (6497 * d_2xn[n*2-2]) >> 11;
}
OIL_DEFINE_IMPL_REF (synth_daub97_ref, synth_daub97);

void
split_daub97_ref (int16_t *d_2xn, int16_t *s_2xn, int n)
{
  int i;

  if (n==0) return;
  /* predict */
  for(i=1;i<n*2-2;i+=2){
    d_2xn[i] = s_2xn[i] - ((6497 * (s_2xn[i-1] + s_2xn[i+1])) >> 12);
  }
  d_2xn[n*2-1] = s_2xn[n*2-1] - ((6497 * s_2xn[n*2-2]) >> 11);
  d_2xn[0] = s_2xn[0] - ((217 * d_2xn[1]) >> 11);
  for(i=2;i<n*2;i+=2){
    d_2xn[i] = s_2xn[i] - ((217 * (d_2xn[i-1] + d_2xn[i+1])) >> 12);
  }

  /* update */
  for(i=1;i<n*2-2;i+=2){
    d_2xn[i] += (3616 * (d_2xn[i-1] + d_2xn[i+1])) >> 12;
  }
  d_2xn[n*2-1] += (3616 * d_2xn[n*2-2]) >> 11;
  d_2xn[0] += (1817 * d_2xn[1]) >> 11;
  for(i=2;i<n*2;i+=2){
    d_2xn[i] += (1817 * (d_2xn[i-1] + d_2xn[i+1])) >> 12;
  }
}
OIL_DEFINE_IMPL_REF (split_daub97_ref, split_daub97);


void
split_approx97_ref (int16_t *d_2xn, int16_t *s_2xn, int n)
{
  int i;

  if (n==0) return;
  if (n==1) {
    d_2xn[1] = s_2xn[1] - s_2xn[0];
    d_2xn[0] = s_2xn[0] + (d_2xn[1] >> 1);
  } else if (n==2) {
    /* predict */
    d_2xn[1] = s_2xn[1] - ((9*(s_2xn[0] + s_2xn[2]) - (s_2xn[2] + s_2xn[2])) >> 4);
    d_2xn[3] = s_2xn[3] - ((9*s_2xn[2] - s_2xn[0]) >> 3);

    /* update */
    d_2xn[0] = s_2xn[0] + (d_2xn[1] >> 1);
    d_2xn[2] = s_2xn[2] + ((d_2xn[1] + d_2xn[3]) >> 2);
  } else {
    /* predict */
    d_2xn[1] = s_2xn[1] - ((9*(s_2xn[0] + s_2xn[2]) - (s_2xn[2] + s_2xn[4])) >> 4);
    for(i=3;i<n*2-4;i+=2){
      d_2xn[i] = s_2xn[i] - ((9*(s_2xn[i-1] + s_2xn[i+1]) - (s_2xn[i-3] + s_2xn[i+3])) >> 4);
    }
    d_2xn[n*2-3] = s_2xn[n*2-3] - ((9*(s_2xn[n*2-4] + s_2xn[n*2-2]) - (s_2xn[n*2-6] + s_2xn[n*2-2])) >> 4);
    d_2xn[n*2-1] = s_2xn[n*2-1] - ((9*s_2xn[n*2-2] - s_2xn[n*2-4]) >> 3);

    /* update */
    d_2xn[0] = s_2xn[0] + (d_2xn[1] >> 1);
    for(i=2;i<n*2;i+=2){
      d_2xn[i] = s_2xn[i] + ((d_2xn[i-1] + d_2xn[i+1]) >> 2);
    }
  }

}
OIL_DEFINE_IMPL_REF (split_approx97_ref, split_approx97);

void
synth_approx97_ref (int16_t *d_2xn, int16_t *s_2xn, int n)
{
  int i;

  if (n==0) return;
  if (n==1) {
    d_2xn[0] = s_2xn[0] - (s_2xn[1] >> 1);
    d_2xn[1] = s_2xn[1] + d_2xn[0];
  } else if (n==2) {
    /* predict */
    d_2xn[0] = s_2xn[0] - (s_2xn[1] >> 1);
    d_2xn[2] = s_2xn[2] - ((s_2xn[1] + s_2xn[3]) >> 2);

    /* update */
    d_2xn[1] = s_2xn[1] + ((9*(d_2xn[0] + d_2xn[2]) - (d_2xn[2] + d_2xn[2])) >> 4);
    d_2xn[3] = s_2xn[3] + ((9*d_2xn[2] - d_2xn[0]) >> 3);
  } else {
    /* predict */
    d_2xn[0] = s_2xn[0] - (s_2xn[1] >> 1);
    for(i=2;i<n*2;i+=2){
      d_2xn[i] = s_2xn[i] - ((s_2xn[i-1] + s_2xn[i+1]) >> 2);
    }

    /* update */
    d_2xn[1] = s_2xn[1] + ((9*(d_2xn[0] + d_2xn[2]) - (d_2xn[2] + d_2xn[4])) >> 4);
    for(i=3;i<n*2-4;i+=2){
      d_2xn[i] = s_2xn[i] + ((9*(d_2xn[i-1] + d_2xn[i+1]) - (d_2xn[i-3] + d_2xn[i+3])) >> 4);
    }
    d_2xn[n*2-3] = s_2xn[n*2-3] + ((9*(d_2xn[n*2-4] + d_2xn[n*2-2]) - (d_2xn[n*2-6] + d_2xn[n*2-2])) >> 4);
    d_2xn[n*2-1] = s_2xn[n*2-1] + ((9*d_2xn[n*2-2] - d_2xn[n*2-4]) >> 3);
  }
}
OIL_DEFINE_IMPL_REF (synth_approx97_ref, synth_approx97);

void
split_53_ref (int16_t *d_2xn, int16_t *s_2xn, int n)
{
  int i;

  if (n==0) return;
  if (n == 1) {
    d_2xn[1] = s_2xn[1] - s_2xn[0];
    d_2xn[0] = s_2xn[0] + (d_2xn[1] >> 1);
  } else {
    d_2xn[1] = s_2xn[1] - ((s_2xn[0] + s_2xn[2]) >> 1);
    d_2xn[0] = s_2xn[0] + (d_2xn[1] >> 1);
    for(i=2;i<n*2-2;i+=2){
      d_2xn[i+1] = s_2xn[i+1] - ((s_2xn[i] + s_2xn[i+2]) >> 1);
      d_2xn[i] = s_2xn[i] + ((d_2xn[i-1] + d_2xn[i+1]) >> 2);
    }
    d_2xn[n*2-1] = s_2xn[n*2-1] - s_2xn[n*2-2];
    d_2xn[n*2-2] = s_2xn[n*2-2] + ((d_2xn[n*2-3] + d_2xn[n*2-1]) >> 2);
  }
}
OIL_DEFINE_IMPL_REF (split_53_ref, split_53);

void
synth_53_ref (int16_t *d_2xn, int16_t *s_2xn, int n)
{
  int i;

  if (n==0) return;
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
OIL_DEFINE_IMPL_REF (synth_53_ref, synth_53);


void
split_135_ref (int16_t *d_2xn, int16_t *s_2xn, int n)
{
  int i;

  if (n==0) return;
  if (n==1) {
    d_2xn[1] = s_2xn[1] - (s_2xn[0]);
    d_2xn[0] = s_2xn[0] + (d_2xn[1]>>1);
  } else if (n==2) {
    /* predict */
    d_2xn[1] = s_2xn[1] - ((9*(s_2xn[0] + s_2xn[2]) - (s_2xn[2] + s_2xn[2])) >> 4);
    d_2xn[3] = s_2xn[3] - ((9*s_2xn[2] - s_2xn[0]) >> 3);

    /* update */
    d_2xn[0] = s_2xn[0] + ((9*d_2xn[1] - d_2xn[3]) >> 4);
    d_2xn[2] = s_2xn[2] + ((9*(d_2xn[1] + d_2xn[3]) - (d_2xn[1] + d_2xn[1])) >> 5);
  } else {
    /* predict */
    d_2xn[1] = s_2xn[1] - ((9*(s_2xn[0] + s_2xn[2]) - (s_2xn[2] + s_2xn[4])) >> 4);
    for(i=3;i<n*2-4;i+=2){
      d_2xn[i] = s_2xn[i] - ((9*(s_2xn[i-1] + s_2xn[i+1]) - (s_2xn[i-3] + s_2xn[i+3])) >> 4);
    }
    d_2xn[n*2-3] = s_2xn[n*2-3] - ((9*(s_2xn[n*2-4] + s_2xn[n*2-2]) - (s_2xn[n*2-6] + s_2xn[n*2-2])) >> 4);
    d_2xn[n*2-1] = s_2xn[n*2-1] - ((9*s_2xn[n*2-2] - s_2xn[n*2-4]) >> 3);

    /* update */
    d_2xn[0] = s_2xn[0] + ((9*d_2xn[1] - d_2xn[3]) >> 4);
    d_2xn[2] = s_2xn[2] + ((9*(d_2xn[1] + d_2xn[3]) - (d_2xn[1] + d_2xn[5])) >> 5);
    for(i=4;i<n*2-2;i+=2){
      d_2xn[i] = s_2xn[i] + ((9*(d_2xn[i-1] + d_2xn[i+1]) - (d_2xn[i-3] + d_2xn[i+3])) >> 5);
    }
    d_2xn[n*2-2] = s_2xn[n*2-2] + ((9*(d_2xn[n*2-3] + d_2xn[n*2-1]) - (d_2xn[n*2-5] + d_2xn[n*2-1])) >> 5);
  }

}
OIL_DEFINE_IMPL_REF (split_135_ref, split_135);

void
synth_135_ref (int16_t *d_2xn, int16_t *s_2xn, int n)
{
  int i;

  if (n==0) return;
  if (n==1) {
    d_2xn[0] = s_2xn[0] - (s_2xn[1]>>1);
    d_2xn[1] = s_2xn[1] + (d_2xn[0]);
  } else if (n==2) {
    /* predict */
    d_2xn[0] = s_2xn[0] - ((9*s_2xn[1] - s_2xn[3]) >> 4);
    d_2xn[2] = s_2xn[2] - ((9*(s_2xn[1] + s_2xn[3]) - (s_2xn[1] + s_2xn[1])) >> 5);

    /* update */
    d_2xn[1] = s_2xn[1] + ((9*(d_2xn[0] + d_2xn[2]) - (d_2xn[2] + d_2xn[2])) >> 4);
    d_2xn[3] = s_2xn[3] + ((9*d_2xn[2] - d_2xn[0]) >> 3);
  } else {
    /* predict */
    d_2xn[0] = s_2xn[0] - ((9*s_2xn[1] - s_2xn[3]) >> 4);
    d_2xn[2] = s_2xn[2] - ((9*(s_2xn[1] + s_2xn[3]) - (s_2xn[1] + s_2xn[5])) >> 5);
    for(i=4;i<n*2-2;i+=2){
      d_2xn[i] = s_2xn[i] - ((9*(s_2xn[i-1] + s_2xn[i+1]) - (s_2xn[i-3] + s_2xn[i+3])) >> 5);
    }
    d_2xn[n*2-2] = s_2xn[n*2-2] - ((9*(s_2xn[n*2-3] + s_2xn[n*2-1]) - (s_2xn[n*2-5] + s_2xn[n*2-1])) >> 5);

    /* update */
    d_2xn[1] = s_2xn[1] + ((9*(d_2xn[0] + d_2xn[2]) - (d_2xn[2] + d_2xn[4])) >> 4);
    for(i=3;i<n*2-4;i+=2){
      d_2xn[i] = s_2xn[i] + ((9*(d_2xn[i-1] + d_2xn[i+1]) - (d_2xn[i-3] + d_2xn[i+3])) >> 4);
    }
    d_2xn[n*2-3] = s_2xn[n*2-3] + ((9*(d_2xn[n*2-4] + d_2xn[n*2-2]) - (d_2xn[n*2-6] + d_2xn[n*2-2])) >> 4);
    d_2xn[n*2-1] = s_2xn[n*2-1] + ((9*d_2xn[n*2-2] - d_2xn[n*2-4]) >> 3);
  }
}
OIL_DEFINE_IMPL_REF (synth_135_ref, synth_135);




static void
lift_test (OilTest *test)
{
  int16_t *data;
  int i;

  data = (int16_t *)oil_test_get_source_data (test, OIL_ARG_SRC1);
  for(i=0;i<test->n;i++){
    data[i] = oil_rand_u8();
  }
  data = (int16_t *)oil_test_get_source_data (test, OIL_ARG_SRC2);
  for(i=0;i<test->n;i++){
    data[i] = oil_rand_u8();
  }
  data = (int16_t *)oil_test_get_source_data (test, OIL_ARG_SRC3);
  for(i=0;i<test->n;i++){
    data[i] = oil_rand_u8();
  }
#if 0
  data = (int16_t *)oil_test_get_source_data (test, OIL_ARG_SRC4);
  if (data) {
    data[0] = oil_rand_u8();
    data[1] = oil_rand_u8();
  }
#endif
}

OIL_DEFINE_CLASS_FULL (lift_add_shift1,
    "int16_t *d, int16_t *s1, int16_t *s2, int16_t *s3, int n", lift_test);
OIL_DEFINE_CLASS_FULL (lift_sub_shift1,
    "int16_t *d, int16_t *s1, int16_t *s2, int16_t *s3, int n", lift_test);
OIL_DEFINE_CLASS_FULL (lift_add_shift2,
    "int16_t *d, int16_t *s1, int16_t *s2, int16_t *s3, int n", lift_test);
OIL_DEFINE_CLASS_FULL (lift_sub_shift2,
    "int16_t *d, int16_t *s1, int16_t *s2, int16_t *s3, int n", lift_test);
OIL_DEFINE_CLASS_FULL (lift_add_mult,
    "int16_t *d, int16_t *s1, int16_t *s2, int16_t *s3, int16_t *s4_2, int n", lift_test);
OIL_DEFINE_CLASS_FULL (lift_sub_mult,
    "int16_t *d, int16_t *s1, int16_t *s2, int16_t *s3, int16_t *s4_2, int n", lift_test);


void
lift_add_shift1_ref (int16_t *d, int16_t *s1, int16_t *s2, int16_t *s3, int n)
{
  int i;
  for(i=0;i<n;i++) {
    d[i] = s1[i] + ((s2[i] + s3[i])>>1);
  }
}
OIL_DEFINE_IMPL_REF (lift_add_shift1_ref, lift_add_shift1);

void
lift_sub_shift1_ref (int16_t *d, int16_t *s1, int16_t *s2, int16_t *s3, int n)
{
  int i;
  for(i=0;i<n;i++) {
    d[i] = s1[i] - ((s2[i] + s3[i])>>1);
  }
}
OIL_DEFINE_IMPL_REF (lift_sub_shift1_ref, lift_sub_shift1);

void
lift_add_shift2_ref (int16_t *d, int16_t *s1, int16_t *s2, int16_t *s3, int n)
{
  int i;
  for(i=0;i<n;i++) {
    d[i] = s1[i] + ((s2[i] + s3[i])>>2);
  }
}
OIL_DEFINE_IMPL_REF (lift_add_shift2_ref, lift_add_shift2);

void
lift_sub_shift2_ref (int16_t *d, int16_t *s1, int16_t *s2, int16_t *s3, int n)
{
  int i;
  for(i=0;i<n;i++) {
    d[i] = s1[i] - ((s2[i] + s3[i])>>2);
  }
}
OIL_DEFINE_IMPL_REF (lift_sub_shift2_ref, lift_sub_shift2);

void
lift_add_mult_ref (int16_t *d, int16_t *s1, int16_t *s2, int16_t *s3, int16_t *s4, int n)
{
  int i;
  for(i=0;i<n;i++) {
    d[i] = s1[i] + ((s4[0]*s2[i] + s4[1]*s3[i])>>16);
  }
}
OIL_DEFINE_IMPL_REF (lift_add_mult_ref, lift_add_mult);

void
lift_sub_mult_ref (int16_t *d, int16_t *s1, int16_t *s2, int16_t *s3, int16_t *s4, int n)
{
  int i;
  for(i=0;i<n;i++) {
    d[i] = s1[i] + ((s4[0]*s2[i] + s4[1]*s3[i])>>16);
  }
}
OIL_DEFINE_IMPL_REF (lift_sub_mult_ref, lift_sub_mult);

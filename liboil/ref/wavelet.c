
#include <liboil/liboilfunction.h>
#include <liboil/liboiltest.h>
#include <liboil/liboilrandom.h>


static void
wavelet_test (OilTest *test)
{
  int16_t *data = (int16_t *)oil_test_get_source_data (test, OIL_ARG_SRC1);
  int i;

  for(i=0;i<test->n;i++){
    data[i] = oil_rand_u8();
  }
}

static void
mas_test (OilTest *test)
{
  int16_t *data;
  int i;
  int n;

  data = (int16_t *)oil_test_get_source_data (test, OIL_ARG_SRC1);
  for(i=0;i<test->n;i++){
    //data[i] = oil_rand_s16()>>1;
    data[i] = 0;
  }

  data = (int16_t *)oil_test_get_source_data (test, OIL_ARG_SRC2);
  for(i=0;i<test->n;i++){
    data[i] = oil_rand_s16();
  }

  data = (int16_t *)oil_test_get_source_data (test, OIL_ARG_SRC3);
  n = oil_test_get_arg_post_n (test, OIL_ARG_SRC3);
  for(i=0;i<n;i++){
    data[i] = (oil_rand_s16()>>4)/n;
  }

  data = (int16_t *)oil_test_get_source_data (test, OIL_ARG_SRC4);
  data[0] = (1<<11);
  data[1] = 12;
}

static void
mas2_across_test (OilTest *test)
{
  int16_t *data;
  int i;
  int n;

  data = (int16_t *)oil_test_get_source_data (test, OIL_ARG_SRC1);
  for(i=0;i<test->n;i++){
    //data[i] = oil_rand_s16()>>1;
    data[i] = 0;
  }

  data = (int16_t *)oil_test_get_source_data (test, OIL_ARG_SRC2);
  for(i=0;i<test->n;i++){
    data[i] = oil_rand_s16()>>4;
  }

  data = (int16_t *)oil_test_get_source_data (test, OIL_ARG_SRC3);
  for(i=0;i<test->n;i++){
    data[i] = oil_rand_s16()>>4;
  }

  data = (int16_t *)oil_test_get_source_data (test, OIL_ARG_SRC4);
  n = oil_test_get_arg_post_n (test, OIL_ARG_SRC4);
  for(i=0;i<n;i++){
    data[i] = (oil_rand_s16()>>4)/n;
  }

  data = (int16_t *)oil_test_get_source_data (test, OIL_ARG_SRC5);
  data[0] = (1<<11);
  data[1] = 12;
}

static void
mas48_across_test (OilTest *test)
{
  int16_t *data;
  int stride;
  int i;
  int j;
  int n;
  int m;

  data = (int16_t *)oil_test_get_source_data (test, OIL_ARG_SRC1);
  for(i=0;i<test->n;i++){
    //data[i] = oil_rand_s16()>>1;
    data[i] = 0;
  }

  data = (int16_t *)oil_test_get_source_data (test, OIL_ARG_SRC2);
  m = oil_test_get_arg_post_n (test, OIL_ARG_SRC2);
  stride = oil_test_get_arg_stride (test, OIL_ARG_SRC2);
  for(j=0;j<m;j++){
    for(i=0;i<test->n;i++){
      data[i] = oil_rand_s16()>>4;
    }
    data = OIL_OFFSET(data, stride);
  }

  data = (int16_t *)oil_test_get_source_data (test, OIL_ARG_SRC3);
  n = oil_test_get_arg_post_n (test, OIL_ARG_SRC3);
  for(i=0;i<n;i++){
    data[i] = (oil_rand_s16()>>4)/n;
  }

  data = (int16_t *)oil_test_get_source_data (test, OIL_ARG_SRC4);
  data[0] = (1<<11);
  data[1] = 12;
}

static void
rshift_test (OilTest *test)
{
  int16_t *data;
  int i;

  data = (int16_t *)oil_test_get_source_data (test, OIL_ARG_SRC1);
  for(i=0;i<test->n;i++){
    data[i] = oil_rand_s16()>>1;
  }

  data = (int16_t *)oil_test_get_source_data (test, OIL_ARG_SRC2);
  data[0] = (1<<3);
  data[1] = 4;
}

static void
lshift_test (OilTest *test)
{
  int16_t *data;

  data = (int16_t *)oil_test_get_source_data (test, OIL_ARG_SRC2);
  data[0] = 12;
}

OIL_DEFINE_CLASS_FULL (deinterleave,
    "int16_t *d_2xn, int16_t *s_2xn, int n", wavelet_test);
OIL_DEFINE_CLASS (deinterleave2_s16,
    "int16_t *d1_n, int16_t *d2_n, int16_t *s_2xn, int n");
OIL_DEFINE_CLASS_FULL (interleave,
    "int16_t *d_2xn, int16_t *s_2xn, int n", wavelet_test);
OIL_DEFINE_CLASS (interleave2_s16,
    "int16_t *d_2xn, int16_t *s1_n, int16_t *s2_n, int n");
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
OIL_DEFINE_CLASS_FULL (mas2_add_s16,
    "int16_t *d, int16_t *s1, int16_t *s2_np1, int16_t *s3_2, int16_t *s4_2, "
    "int n", mas_test);
OIL_DEFINE_CLASS_FULL (mas4_add_s16,
    "int16_t *d, int16_t *s1, int16_t *s2_np3, int16_t *s3_4, int16_t *s4_2, "
    "int n", mas_test);
OIL_DEFINE_CLASS_FULL (mas8_add_s16,
    "int16_t *d, int16_t *s1, int16_t *s2_np7, int16_t *s3_8, int16_t *s4_2, "
    "int n", mas_test);
OIL_DEFINE_CLASS_FULL (add_const_rshift_s16,
    "int16_t *d1, int16_t *s1, int16_t *s2_2, int n", rshift_test);
OIL_DEFINE_CLASS_FULL (lshift_s16,
    "int16_t *d1, int16_t *s1, int16_t *s2_1, int n", lshift_test);
OIL_DEFINE_CLASS_FULL (mas2_across_add_s16,
    "int16_t *d, int16_t *s1, int16_t *s2, int16_t *s3, int16_t *s4_2, "
    "int16_t *s5_2, int n", mas2_across_test);
OIL_DEFINE_CLASS_FULL (mas4_across_add_s16,
    "int16_t *d, int16_t *s1, int16_t *s2_nx4, int sstr2, int16_t *s3_4, "
    "int16_t *s4_2, int n", mas48_across_test);
OIL_DEFINE_CLASS_FULL (mas8_across_add_s16,
    "int16_t *d, int16_t *s1, int16_t *s2_nx8, int sstr2, int16_t *s3_8, "
    "int16_t *s4_2, int n", mas48_across_test);
OIL_DEFINE_CLASS (multiply_and_add_s16,
    "int16_t *d, int16_t *src1, int16_t *src2, int16_t *src3, int n");
OIL_DEFINE_CLASS (multiply_and_add_s16_u8,
    "int16_t *d, int16_t *src1, int16_t *src2, uint8_t *src3, int n");
OIL_DEFINE_CLASS (add_s16,
    "int16_t *d, int16_t *src1, int16_t *src2, int n");
OIL_DEFINE_CLASS (multiply_and_acc_12xn_s16_u8, "int16_t *i1_12xn, int is1, "
    "int16_t *s1_12xn, int ss1, uint8_t *s2_12xn, int ss2, int n");

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
deinterleave2_s16_ref (int16_t *d1_n, int16_t *d2_n, int16_t *s_2xn, int n)
{
  int i;

  for(i=0;i<n;i++) {
    d1_n[i] = s_2xn[2*i];
    d2_n[i] = s_2xn[2*i + 1];
  }
}
OIL_DEFINE_IMPL_REF (deinterleave2_s16_ref, deinterleave2_s16);

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
interleave2_s16_ref (int16_t *d_2xn, int16_t *s1_n, int16_t *s2_n, int n)
{
  int i;

  for(i=0;i<n;i++) {
    d_2xn[2*i] = s1_n[i];
    d_2xn[2*i + 1] = s2_n[i];
  }
}
OIL_DEFINE_IMPL_REF (interleave2_s16_ref, interleave2_s16);

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
OIL_DEFINE_CLASS_FULL (lift_add_mult_shift12,
    "int16_t *d, int16_t *s1, int16_t *s2, int16_t *s3, int16_t *s4_1, int n", lift_test);
OIL_DEFINE_CLASS_FULL (lift_sub_mult_shift12,
    "int16_t *d, int16_t *s1, int16_t *s2, int16_t *s3, int16_t *s4_1, int n", lift_test);
OIL_DEFINE_CLASS_FULL (lift_add_135,
    "int16_t *d, int16_t *s1, int16_t *s2, int16_t *s3, int16_t *s4, int16_t *s5, int n", lift_test);
OIL_DEFINE_CLASS_FULL (lift_sub_135,
    "int16_t *d, int16_t *s1, int16_t *s2, int16_t *s3, int16_t *s4, int16_t *s5, int n", lift_test);


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
lift_add_mult_shift12_ref (int16_t *d, int16_t *s1, int16_t *s2, int16_t *s3, int16_t *s4, int n)
{
  int i;
  for(i=0;i<n;i++) {
    d[i] = s1[i] + ((s4[0]*(s2[i] + s3[i]))>>12);
  }
}
OIL_DEFINE_IMPL_REF (lift_add_mult_shift12_ref, lift_add_mult_shift12);

void
lift_sub_mult_shift12_ref (int16_t *d, int16_t *s1, int16_t *s2, int16_t *s3, int16_t *s4, int n)
{
  int i;
  for(i=0;i<n;i++) {
    d[i] = s1[i] - ((s4[0]*(s2[i] + s3[i]))>>12);
  }
}
OIL_DEFINE_IMPL_REF (lift_sub_mult_shift12_ref, lift_sub_mult_shift12);

void
lift_add_135_ref (int16_t *d, int16_t *s1, int16_t *s2, int16_t *s3,
    int16_t *s4, int16_t *s5, int n)
{
  int i;
  for(i=0;i<n;i++) {
    d[i] = s1[i] + ((9*(s3[i-1] + s4[i+1]) - (s2[i-3] + s5[i+3])) >> 4);
  }
}
OIL_DEFINE_IMPL_REF (lift_add_135_ref, lift_add_135);

void
lift_sub_135_ref (int16_t *d, int16_t *s1, int16_t *s2, int16_t *s3,
    int16_t *s4, int16_t *s5, int n)
{
  int i;
  for(i=0;i<n;i++) {
    d[i] = s1[i] - ((9*(s3[i-1] + s4[i+1]) - (s2[i-3] + s5[i+3])) >> 4);
  }
}
OIL_DEFINE_IMPL_REF (lift_sub_135_ref, lift_sub_135);


void
mas2_add_s16_ref(int16_t *d1, int16_t *s1, int16_t *s2, int16_t *s3_2,
    int16_t *s4_2, int n)
{
  int i;
  int x;

  for(i=0;i<n;i++){
    x = s4_2[0] + s2[i]*s3_2[0] + s2[i+1]*s3_2[1];
    x >>= s4_2[1];
    d1[i] = s1[i] + x;
  }
}
OIL_DEFINE_IMPL_REF (mas2_add_s16_ref, mas2_add_s16);

void
mas4_add_s16_ref(int16_t *d1, int16_t *s1, int16_t *s2, int16_t *s3_4,
    int16_t *s4_2, int n)
{
  int i;
  int x;

  for(i=0;i<n;i++){
    x = s4_2[0] + s2[i]*s3_4[0] + s2[i+1]*s3_4[1] + s2[i+2]*s3_4[2] +
        s2[i+3]*s3_4[3];
    x >>= s4_2[1];
    d1[i] = s1[i] + x;
  }
}
OIL_DEFINE_IMPL_REF (mas4_add_s16_ref, mas4_add_s16);

void
mas8_add_s16_ref(int16_t *d1, int16_t *s1, int16_t *s2, int16_t *s3_8,
    int16_t *s4_2, int n)
{
  int i;
  int j;
  int x;

  for(i=0;i<n;i++){
    x = s4_2[0];
    for(j=0;j<8;j++){
      x += s2[i+j]*s3_8[j];
    }
    x >>= s4_2[1];
    d1[i] = s1[i] + x;
  }
}
OIL_DEFINE_IMPL_REF (mas8_add_s16_ref, mas8_add_s16);

void
add_const_rshift_s16_ref(int16_t *d1, int16_t *s1, int16_t *s3_2, int n)
{
  int i;
  for(i=0;i<n;i++){
    d1[i] = (s1[i] + s3_2[0])>>s3_2[1];
  }
}
OIL_DEFINE_IMPL_REF (add_const_rshift_s16_ref, add_const_rshift_s16);

void
lshift_s16_ref(int16_t *d1, int16_t *s1, int16_t *s3_1, int n)
{
  int i;
  for(i=0;i<n;i++){
    d1[i] = s1[i]<<s3_1[0];
  }
}
OIL_DEFINE_IMPL_REF (lshift_s16_ref, lshift_s16);

void
mas2_across_add_s16_ref (int16_t *d, int16_t *s1, int16_t *s2, int16_t *s3,
    int16_t *s4_2, int16_t *s5_2, int n)
{
  int i;
  int x;
  for(i=0;i<n;i++){
    x = s5_2[0];
    x += s2[i]*s4_2[0] + s3[i]*s4_2[1];
    x >>= s5_2[1];
    d[i] = s1[i] + x;
  }
}
OIL_DEFINE_IMPL_REF (mas2_across_add_s16_ref, mas2_across_add_s16);

void
mas4_across_add_s16_ref (int16_t *d, int16_t *s1, int16_t *s2_nx4, int sstr2,
    int16_t *s3_4, int16_t *s4_2, int n)
{
  int i;
  int j;
  int x;
  for(i=0;i<n;i++){
    x = s4_2[0];
    for(j=0;j<4;j++){
      x += OIL_GET(s2_nx4, i*sizeof(int16_t) + j*sstr2, int16_t)*s3_4[j];
    }
    x >>= s4_2[1];
    d[i] = s1[i] + x;
  }
}
OIL_DEFINE_IMPL_REF (mas4_across_add_s16_ref, mas4_across_add_s16);

void
mas8_across_add_s16_ref (int16_t *d, int16_t *s1, int16_t *s2_nx8, int sstr2,
    int16_t *s3_8, int16_t *s4_2, int n)
{
  int i;
  int j;
  int x;
  for(i=0;i<n;i++){
    x = s4_2[0];
    for(j=0;j<8;j++){
      x += OIL_GET(s2_nx8, i*sizeof(int16_t) + j*sstr2, int16_t)*s3_8[j];
    }
    x >>= s4_2[1];
    d[i] = s1[i] + x;
  }
}
OIL_DEFINE_IMPL_REF (mas8_across_add_s16_ref, mas8_across_add_s16);

void
multiply_and_add_s16_ref (int16_t *d, int16_t *src1, int16_t *src2, int16_t *src3, int n)
{
  int i;
  for(i=0;i<n;i++){
    d[i] = src1[i] + src2[i]*src3[i];
  }
}
OIL_DEFINE_IMPL_REF (multiply_and_add_s16_ref, multiply_and_add_s16);

void
multiply_and_add_s16_u8_ref (int16_t *d, int16_t *src1, int16_t *src2,
    uint8_t *src3, int n)
{
  int i;
  for(i=0;i<n;i++){
    d[i] = src1[i] + src2[i]*src3[i];
  }
}
OIL_DEFINE_IMPL_REF (multiply_and_add_s16_u8_ref, multiply_and_add_s16_u8);

void
add_s16_ref (int16_t *d, int16_t *src1, int16_t *src2, int n)
{
  int i;
  for(i=0;i<n;i++){
    d[i] = src1[i] + src2[i];
  }
}
OIL_DEFINE_IMPL_REF (add_s16_ref, add_s16);

void
multiply_and_acc_12xn_s16_u8_ref (int16_t *i1, int is1, int16_t *s1,
    int ss1, uint8_t *s2, int ss2, int n)
{
  int i, j;
  for(j=0;j<n;j++){
    for(i=0;i<12;i++){
      i1[i] += s1[i]*s2[i];
    }
    i1 = OIL_OFFSET(i1,is1);
    s1 = OIL_OFFSET(s1,ss1);
    s2 = OIL_OFFSET(s2,ss2);
  }
}
OIL_DEFINE_IMPL_REF (multiply_and_acc_12xn_s16_u8_ref,
    multiply_and_acc_12xn_s16_u8);


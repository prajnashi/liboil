
#include <stdio.h>
#include <liboil/liboil.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <liboil/liboilprototype.h>
#include <liboil/liboiltest.h>
#include <liboil/liboilcpu.h>

void hist(OilTest *Test);

int main (int argc, char *argv[])
{
  OilFunctionClass *klass;
  OilFunctionImpl *impl;
  OilTest *test;
  int i;
  //int j;
  int ret;

  oil_init ();

  for (i=0;i<oil_n_function_classes; i++ ){
    klass = oil_class_get_by_index(i);

    printf("%s\n", klass->name);

    test = oil_test_new (klass);
    if (test == NULL) {
      printf("  bad prototype\n");
      continue;
    }

    for (impl = klass->first_impl; impl; impl = impl->next) {
      printf("  %s\n", impl->name);
      if ((impl->flags & OIL_CPU_FLAG_MASK) & ~oil_cpu_flags) {
        printf("    not supported\n");
      } else {
        oil_test_set_impl (test, impl);

        test->n = 1600;

        ret = oil_test_go (test);
        if (ret) {
#if 0
          printf("    %lu %g\n",test->prof.min,
              (double)test->prof.total/test->prof.n);
          for(j=0;j<test->prof.hist_n;j++){
            printf("    %lu %d\n",test->prof.hist_time[j],test->prof.hist_count[j]);
          }
#endif
          printf("    ave=%g std=%g\n", impl->profile_ave, impl->profile_std);
          //hist(test);
        } else {
          printf("  not tested\n");
          break;
        }
      }
    }

    oil_test_free (test);
  }

  return 0;
}


void
hist(OilTest *test)
{
  double ave;
  double std;
  int max_i;
  double off;
  double s;
  double s2;
  int i;
  int n;
  double x;

  do {
    s = s2 = 0;
    n = 0;
    max_i = -1;
    for(i=0;i<10;i++){
      x = test->prof.hist_time[i];
      s2 += x * x * test->prof.hist_count[i];
      s += x * test->prof.hist_count[i];
      n += test->prof.hist_count[i];
      if (test->prof.hist_count[i] > 0) {
        if (max_i == -1 || test->prof.hist_time[i] > test->prof.hist_time[max_i]) {
          max_i = i;
        }
      }
    }

    ave = s / n;
    std = sqrt (s2 - s * s / n + n*n) / (n-1);
    off = (test->prof.hist_time[max_i] - ave)/std;

    printf("    ave=%g std=%g max=%ld off=%g %s\n",
        ave, std, test->prof.hist_time[max_i], off, (off>4.0)?"yes":"no");

    if (off > 4.0) {
      test->prof.hist_count[max_i] = 0;
    }
  } while (off > 4.0);
  printf("    ave=%g std=%g\n", ave, std);
}




#include <stdio.h>
#include <liboil/liboil.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include <liboil/liboilprototype.h>
#include <liboil/liboiltest.h>
#include <liboil/liboilcpu.h>

int main (int argc, char *argv[])
{
  OilFunctionClass *klass;
  OilFunctionImpl *impl;
  OilTest *test;
  int i;
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

        ret = oil_test_go (test);
        if (ret) {
          printf("    %lu %lu %lu %lu\n",test->prof.tsum,test->prof.tmin,
              test->prof.t,test->prof.count);
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


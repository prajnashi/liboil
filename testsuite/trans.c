
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <liboil/liboil.h>
#include <liboil/liboilfuncs.h>
#include <glib.h>
#include <string.h>
#include <math.h>

void test(void)
{
  int16_t src2[80];
  int16_t src1[80];
  int16_t dest[80];
  int i;
  int j;

  for(i=0;i<8;i++){
    for(j=0;j<8;j++){
      dest[i*10+j] = 0;
      //src1[i*10+j] = i*8+j;
      //src1[i*10+j] = rint(100*cos((j+0.5)*i*M_PI/8));
      src1[i*10+j] = 100*(i+j);
      src2[i*10+j] = 2;
    }
  }

  oil_fdct8x8s_s16 (dest, 20, src1, 20);

  for(i=0;i<8;i++){
    for(j=0;j<8;j++){
      g_print("%4d ",src1[i*10+j]);
    }
    g_print("\n");
  }
  g_print("\n");
  for(i=0;i<8;i++){
    for(j=0;j<8;j++){
      g_print("%4d ",dest[i*10+j]);
    }
    g_print("\n");
  }
}

int main (int argc, char *argv[])
{
  OilFunctionClass *klass;
  OilFunctionImpl *impl;

  oil_init ();

  klass = oil_class_get ("fdct8x8s_s16");

  oil_class_choose_by_name (klass, "fdct8x8s_s16_mmx");
  impl = klass->chosen_impl;
  g_print("chosen=%p\n", impl);
  impl = klass->reference_impl;
  g_print("ref=%p\n", impl);
  test();

  return 0;
}


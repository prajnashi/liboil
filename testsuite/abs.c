
#include <liboil/liboil.h>
#include <liboil/liboilfuncs.h>
#include <glib.h>

void test(void)
{
  int i;
  float *src;
  float *dest;

  src = g_malloc(100*sizeof(float));
  dest = g_malloc(100*sizeof(float));

  for(i=0;i<100;i++){
    src[i] = (i-50)*100;
  }

  oil_abs_f32_f32 (dest, 4, src, 4, 100);

  for (i=0; i<100; i++) {
    g_print("%d %g %g\n", i, src[i], dest[i]);
  }
}

int main (int argc, char *argv[])
{
  OilFunctionClass *klass;
  OilFunctionImpl *impl;

  oil_init ();

  klass = oil_class_get ("abs_f32_f32");
  oil_class_optimize(klass);

  g_print("class=%s\n", klass->name);
  for (impl = klass->first_impl; impl; impl=impl->next) {
    g_print("impl=%p\n", impl);
    g_print("  func=%p\n", impl->func);
    g_print("  name=%s\n", impl->name);
    g_print("  flags=%08x\n", impl->flags);

  }
  impl = klass->chosen_impl;
  g_print("chosen=%p\n", impl);
  impl = klass->reference_impl;
  g_print("ref=%p\n", impl);

  test();

  return 0;
}


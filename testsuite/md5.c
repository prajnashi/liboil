
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <liboil/liboil.h>
#include <liboil/liboilfuncs.h>
#include <glib.h>
#include <string.h>

char *message = "liboil md5 test\n";
//char *message = "";

#ifdef WORDS_BIGENDIAN
#define uint32_from_host(a) \
  ((((a)&0xff)<<24)|(((a)&0xff00)<<8)|(((a)&0xff0000)>>8)|(((a)>>24)&0xff))
#else
#define uint32_from_host(a) (a)
#endif

void test(void)
{
  uint32_t state[4];
  uint32_t src[16];
  int len;

  state[0] = 0x67452301;
  state[1] = 0xefcdab89;
  state[2] = 0x98badcfe;
  state[3] = 0x10325476;

  memset (src, 0, 64);
  len = strlen (message);
  memcpy (src, message, len);
  ((uint8_t *)src)[len] = 0x80;
  src[14] = uint32_from_host(len << 3);
  src[15] = 0;

  oil_md5 (state, src);

  g_print("%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\n",
      state[0]&0xff, (state[0]>>8)&0xff, (state[0]>>16)&0xff,
      (state[0]>>24)&0xff,
      state[1]&0xff, (state[1]>>8)&0xff, (state[1]>>16)&0xff,
      (state[1]>>24)&0xff,
      state[2]&0xff, (state[2]>>8)&0xff, (state[2]>>16)&0xff,
      (state[2]>>24)&0xff,
      state[3]&0xff, (state[3]>>8)&0xff, (state[3]>>16)&0xff,
      (state[3]>>24)&0xff);
}

int main (int argc, char *argv[])
{
  OilFunctionClass *klass;
  OilFunctionImpl *impl;

  oil_init ();

  oil_optimize_all();

  klass = oil_class_get ("md5");
  oil_class_optimize(klass);

  g_print("class=%s\n", klass->name);
  for (impl = klass->first_impl; impl; impl=impl->next) {
    g_print("impl=%p\n", impl);
    g_print("  func=%p\n", impl->func);
    g_print("  name=%s\n", impl->name);
    g_print("  flags=%08x\n", impl->flags);
  }

  oil_class_choose_by_name (klass, "md5_c");
  impl = klass->chosen_impl;
  g_print("chosen=%p\n", impl);
  impl = klass->reference_impl;
  g_print("ref=%p\n", impl);
  test();

  oil_class_choose_by_name (klass, "md5_asm1");
  impl = klass->chosen_impl;
  g_print("chosen=%p\n", impl);
  impl = klass->reference_impl;
  g_print("ref=%p\n", impl);
  test();

  oil_class_choose_by_name (klass, "md5_asm2");
  impl = klass->chosen_impl;
  g_print("chosen=%p\n", impl);
  impl = klass->reference_impl;
  g_print("ref=%p\n", impl);
  test();

  return 0;
}


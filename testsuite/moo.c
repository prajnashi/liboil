
#include <liboil/liboil.h>


int main (int argc, char *argv[])
{
  _oil_debug_level = OIL_DEBUG_INFO;
  oil_init ();

  oil_optimize_all();

  return 0;
}


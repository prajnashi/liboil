
#include "config.h"

#include <glib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ojprogram.h"

int
main (int argc, char *argv[])
{
  OJProgram *p;
  int s1, s2, d1;

  p = oj_program_new ();

  d1 = oj_program_add_destination (p, "s16");
  s1 = oj_program_add_source (p, "s16");
  s2 = oj_program_add_source (p, "s16");

  oj_program_append (p, "add_s16", d1, s1, s2);

  oj_program_output_mmx (p);

  return 0;
}



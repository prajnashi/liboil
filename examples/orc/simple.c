
#include "config.h"

#include <glib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <orc/orcprogram.h>

#define N 10

int16_t src1[N];
int16_t src2[N];
int16_t dest[N];

void test1(void);

int
main (int argc, char *argv[])
{
  orc_init ();

  test1();
  exit(0);
}


void
test1(void)
{
  OrcProgram *p;
  OrcExecutor *ex;
  int s1, s2, d1;


  p = orc_program_new ();

  d1 = orc_program_add_destination (p, "s16", "d1");
  s1 = orc_program_add_source (p, "s16", "s1");
  s2 = orc_program_add_source (p, "s16", "s2");

  orc_program_append (p, "add_s16", d1, s1, s2);

  orc_program_compile (p);

  ex = orc_executor_new (p);
  orc_executor_set_n (ex, N);
  orc_executor_set_array (ex, s1, src1);
  orc_executor_set_array (ex, s2, src2);
  orc_executor_set_array (ex, d1, dest);

  if (1) {
    int i;

    for(i=0;i<N;i++){
      src1[i] = rand()&0xf;
      src2[i] = rand()&0x3;
    }

    orc_executor_run (ex);
    //orc_executor_emulate (ex);

    for(i=0;i<N;i++){
      printf("#  %4d %4d %4d %4d\n", src1[i], src2[i], dest[i],
          src1[i] + src2[i]);
    }
  }

  orc_executor_free (ex);
  orc_program_free (p);
}




#include "config.h"

#include <glib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ojprogram.h"

#define N 10

int16_t src1[N];
int16_t src2[N];
int16_t dest[N];

void test(OJExecutor *ex);

int
main (int argc, char *argv[])
{
  OJProgram *p;
  OJExecutor *ex;
  int s1, s2, d1, offset, shift;
  int t1;

  oj_opcode_init ();

  p = oj_program_new ();

  d1 = oj_program_add_destination (p, "s16", "d1");
  s1 = oj_program_add_source (p, "s16", "s1");
  s2 = oj_program_add_source (p, "s16", "s2");
  t1 = oj_program_add_temporary (p, "s16", "t1");
  offset = oj_program_add_constant (p, "s16", 1, "offset");
  shift = oj_program_add_constant (p, "s16", 1, "shift");

  oj_program_append (p, "add_s16", t1, s1, s2);
  oj_program_append (p, "add_s16", t1, t1, offset);
  oj_program_append (p, "rshift_s16", d1, t1, shift);

  ex = oj_executor_new (p);

  oj_executor_set_n (ex, N);
  oj_executor_set_array (ex, s1, src1);
  oj_executor_set_array (ex, s2, src2);
  oj_executor_set_array (ex, d1, dest);

  oj_program_compile_x86 (p);

#if 0
  if (1) {
    int i;

    for(i=0;i<N;i++){
      src1[i] = rand()&0xf;
      src2[i] = rand()&0xf;
    }

    test (ex);
    //oj_executor_emulate (ex);

    for(i=0;i<N;i++){
      printf("  %4d %4d %4d %4d\n", src1[i], src2[i], dest[i],
          (src1[i] + src2[i] + 1)>>1);
    }
  }
#endif

  return 0;
}



void
test1 (int16_t *dest, int16_t *src1, int16_t *src2, int n)
{
  int i;
  for(i=0;i<n;i++){
    dest[i] = (src1[i] + src2[i] + 1)>>1;
  }
}


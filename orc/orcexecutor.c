
#include "config.h"

#include <glib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <orc/orcprogram.h>


OJExecutor *
oj_executor_new (OJProgram *program)
{
  OJExecutor *ex;

  ex = g_malloc0(sizeof(OJExecutor));

  ex->program = program;

  memcpy (ex->vars, program->vars, 10*sizeof(OJVariable));

  return ex;
}

void
oj_executor_free (OJExecutor *ex)
{
  g_free (ex);
}

void
oj_executor_run (OJExecutor *ex)
{


}

void
oj_executor_set_array (OJExecutor *ex, int var, void *ptr)
{
  ex->arrays[var] = ptr;
}

void
oj_executor_set_n (OJExecutor *ex, int n)
{
  ex->n = n;
}

void
oj_executor_emulate (OJExecutor *ex)
{
  int i;
  int j;
  int k;
  OJProgram *program = ex->program;
  OJInstruction *insn;
  OJOpcode *opcode;

  printf("n %d\n", ex->n);
  printf("n_insns %d\n", program->n_insns);

  for(i=0;i<ex->n;i++){
    for(j=0;j<program->n_insns;j++){
      insn = program->insns + j;
      opcode = insn->opcode;

      printf("%d: %s\n", j, insn->opcode->name);

      /* set up args */
      for(k=0;k<opcode->n_src + opcode->n_dest;k++){
        ex->args[k] = ex->vars + insn->args[k];

        printf("setting up arg %d as var %d vartype %d\n",
            k, insn->args[k], ex->args[k]->vartype);

        if (ex->args[k]->vartype == OJ_VAR_TYPE_SRC) {
          void *ptr = ex->arrays[insn->args[k]] + 2*i;

          printf("load %p\n", ptr);
          ex->args[k]->s16 = *(int16_t *)ptr;
        }
      }

      opcode->emulate (ex, opcode->emulate_user);
      printf("emulate: %d %d %d\n", ex->args[0]->s16,
          ex->args[1]->s16, ex->args[2]->s16);

      for(k=0;k<opcode->n_src + opcode->n_dest;k++){
        if (ex->args[k]->vartype == OJ_VAR_TYPE_DEST) {
          void *ptr = ex->arrays[insn->args[k]] + 2*i;

          printf("store %p\n", ptr);
          *(int16_t *)ptr = ex->args[k]->s16;
        }
      }
    }
  }
}




#include "config.h"

#include <glib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <orc/orcprogram.h>


OrcProgram *
orc_program_new (void)
{
  OrcProgram *p;
  p = g_malloc0(sizeof(OrcProgram));
  return p;
}

int
orc_program_add_temporary (OrcProgram *program, const char *type, const char *name)
{
  int i = program->n_vars;

  program->vars[i].vartype = ORC_VAR_TYPE_TEMP;
  program->vars[i].type = orc_type_get(type);
  program->vars[i].name = strdup(name);
  program->n_vars++;

  return i;
}

int
orc_program_dup_temporary (OrcProgram *program, int var, int j)
{
  int i = program->n_vars;

  program->vars[i].vartype = ORC_VAR_TYPE_TEMP;
  program->vars[i].type = program->vars[var].type;
  program->vars[i].name = g_strdup_printf("%s.dup%d",
      program->vars[var].name, j);
  program->n_vars++;

  return i;
}

int
orc_program_add_source (OrcProgram *program, const char *type, const char *name)
{
  int i = program->n_vars;

  program->vars[i].vartype = ORC_VAR_TYPE_SRC;
  program->vars[i].type = orc_type_get(type);
  program->vars[i].name = strdup(name);
  program->n_vars++;

  return i;
}

int
orc_program_add_destination (OrcProgram *program, const char *type, const char *name)
{
  int i = program->n_vars;

  program->vars[i].vartype = ORC_VAR_TYPE_DEST;
  program->vars[i].type = orc_type_get(type);
  program->vars[i].name = strdup(name);
  program->n_vars++;

  return i;
}

int
orc_program_add_constant (OrcProgram *program, const char *type, int value, const char *name)
{
  int i = program->n_vars;

  program->vars[i].vartype = ORC_VAR_TYPE_CONST;
  program->vars[i].type = orc_type_get(type);
  program->vars[i].s16 = value;
  program->vars[i].name = strdup(name);
  program->n_vars++;

  return i;
}

int
orc_program_add_parameter (OrcProgram *program, OrcType *type, int value, const char *name)
{

  return 0;
}

void
orc_program_append (OrcProgram *program, const char *name, int arg0,
    int arg1, int arg2)
{
  OrcInstruction *insn;

  insn = program->insns + program->n_insns;

  insn->opcode = orc_opcode_find_by_name (name);
  if (!insn->opcode) {
    printf("unknown opcode: %s\n", name);
  }
  insn->args[0] = arg0;
  insn->args[1] = arg1;
  insn->args[2] = arg2;
  
  program->n_insns++;
}


void
orc_program_rewrite_vars (OrcProgram *program)
{
  int i;
  int j;
  int k;
  OrcInstruction *insn;
  OrcOpcode *opcode;
  int var;
  int actual_var;
  int alloc[8] = { 0, 1, 0, 0, 1, 1, 0, 0 };

  for(j=0;j<program->n_insns;j++){
    insn = program->insns + j;
    opcode = insn->opcode;

    /* set up args */
    for(k=opcode->n_dest;k<opcode->n_src + opcode->n_dest;k++){
      var = insn->args[k];
      if (program->vars[var].vartype == ORC_VAR_TYPE_DEST) {
        g_print("ERROR: using dest var as source\n");
      }

      actual_var = var;
      if (program->vars[var].replaced) {
        actual_var = program->vars[var].replacement;
        insn->args[k] = actual_var;
      }

      if (!program->vars[var].used) {
        if (program->vars[var].vartype == ORC_VAR_TYPE_TEMP) {
          g_print("ERROR: using uninitialized temp var\n");
        }
        program->vars[var].used = TRUE;
        program->vars[var].first_use = j;
      }
      program->vars[actual_var].last_use = j;
    }

    for(k=0;k<opcode->n_dest;k++){
      var = insn->args[k];

      if (program->vars[var].vartype == ORC_VAR_TYPE_SRC) {
        g_print("ERROR: using src var as dest\n");
      }
      if (program->vars[var].vartype == ORC_VAR_TYPE_CONST) {
        g_print("ERROR: using const var as dest\n");
      }
      if (program->vars[var].vartype == ORC_VAR_TYPE_PARAM) {
        g_print("ERROR: using param var as dest\n");
      }

      actual_var = var;
      if (program->vars[var].replaced) {
        actual_var = program->vars[var].replacement;
        insn->args[k] = actual_var;
      }

      if (!program->vars[var].used) {
        program->vars[actual_var].used = TRUE;
        program->vars[actual_var].first_use = j;
      } else {
        if (program->vars[var].vartype == ORC_VAR_TYPE_DEST) {
          g_print("ERROR: writing dest more than once\n");
        }
        if (program->vars[var].vartype == ORC_VAR_TYPE_TEMP) {
          actual_var = orc_program_dup_temporary (program, var, j);
          program->vars[var].replaced = TRUE;
          program->vars[var].replacement = actual_var;
          insn->args[k] = actual_var;
          program->vars[actual_var].used = TRUE;
          program->vars[actual_var].first_use = j;
        }
      }
      program->vars[actual_var].last_use = j;
    }
  }

  for(j=0;j<program->n_insns;j++){
    for(i=0;i<program->n_vars;i++){
      if (program->vars[i].first_use == j) {
        for(k=0;k<8;k++){
          if (!alloc[k]) {
            program->vars[i].alloc = k + ORC_GP_REG_BASE;
            alloc[k] = 1;
            break;
          }
        }
        if (k==8) {
          g_print("register overflow\n");
        }
      }
    }
    for(i=0;i<program->n_vars;i++){
      if (program->vars[i].last_use == j) {
        alloc[program->vars[i].alloc - ORC_GP_REG_BASE] = 0;
      }
    }
  }

#if 0
  for(i=0;i<program->n_vars;i++){
    g_print("%2d: %2d %2d %s\n",
        i,
        program->vars[i].first_use,
        program->vars[i].last_use,
        x86_get_regname(program->vars[i].alloc));
  }
#endif

}

void
orc_program_dump_code (OrcProgram *program)
{
  FILE *file;

  file = fopen("dump","w");

  fwrite (program->code, 1, program->codeptr - program->code, file);
  fclose (file);
}

void
orc_program_dump (OrcProgram *program)
{
  int i;
  int j;
  OrcOpcode *opcode;
  OrcInstruction *insn;

  for(i=0;i<program->n_insns;i++){
    insn = program->insns + i;
    opcode = insn->opcode;

    g_print("insn: %d\n", i);
    g_print("  opcode: %s\n", opcode->name);

    for(j=0;j<opcode->n_dest;j++){
      g_print("  dest%d: %d %s\n", j, insn->args[j],
          program->vars[insn->args[j]].name);
    }
    for(j=0;j<opcode->n_src;j++){
      g_print("  src%d: %d %s\n", j, insn->args[opcode->n_dest + j],
          program->vars[insn->args[opcode->n_dest + j]].name);
    }

    g_print("\n");
  }

  for(i=0;i<program->n_vars;i++){
    g_print("var: %d %s\n", i, program->vars[i].name);
    g_print("first_use: %d\n", program->vars[i].first_use);
    g_print("last_use: %d\n", program->vars[i].last_use);

    g_print("\n");
  }

}


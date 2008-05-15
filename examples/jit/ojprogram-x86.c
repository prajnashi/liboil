
#include "config.h"

#include <glib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ojprogram.h"


#if 0
void
emit (unsigned int x)
{
  g_print ("%02x ", x);
}

void
emit_modrm (OJArgument *reg, OJArgument *regmem)
{
  int bits_mod;
  int bits_rm;
  int bits_reg;

  if (regmem->is_indirect) {
    if (regmem->offset == 0) {
      bits_mod = 0;
      bits_rm = regmem->index;
      bits_reg = reg->index;
      emit ((bits_reg << 5) | (bits_mod << 3) | (bits_rm));
    } else {
      if (regmem->offset <= 127 && regmem->offset >= -128) {
        bits_mod = 1;
        bits_rm = regmem->index;
        bits_reg = reg->index;
        emit ((bits_reg << 5) | (bits_mod << 3) | (bits_rm));
        emit (regmem->offset);
      } else {
        bits_mod = 2;
        bits_rm = regmem->index;
        bits_reg = reg->index;
        emit ((bits_reg << 5) | (bits_mod << 3) | (bits_rm));
        emit ((regmem->offset>>0) & 0xff);
        emit ((regmem->offset>>8) & 0xff);
        emit ((regmem->offset>>16) & 0xff);
        emit ((regmem->offset>>24) & 0xff);
      }
    }
  } else {
    bits_mod = 3;
    bits_rm = regmem->index;
    bits_reg = reg->index;
    emit ((bits_reg << 5) | (bits_mod << 3) | (bits_rm));
  }

}

void
oj_program_output_mmx (OJProgram *program) 
{
  int i;
  char *dest_regs[] = { "error", "eax", "error", "error" };
  char *src_regs[] = { "error", "ecx", "edx", "error" };
  char *r_regs[] = { "mm0", "mm1", "mm2", "mm3" };
  OJInstruction *insn;

  g_print("  push %%ebp\n");
  g_print("  movl %%esp, %%ebp\n");
  g_print("  movl 0x8(%%ebp), %%%s\n", dest_regs[1]);
  g_print("  movl 0xc(%%ebp), %%%s\n", src_regs[1]);
  g_print("  movl 0x10(%%ebp), %%%s\n", src_regs[2]);

  for(i=0;i<program->n_insns;i++){
    insn = program->insns + i;
    switch (insn->opcode) {
      case 0:
        g_print ("  movq %d(%%%s), %%%s\n",
            insn->args[1].offset,
            src_regs[insn->args[1].index],
            r_regs[insn->args[0].index]);
        emit (0x0f);
        emit (0x6f);
        emit_modrm (insn->args + 0, insn->args + 1);
        g_print ("\n");
        break;
      case 1:
        g_print ("  movq %%%s, %d(%%%s)\n",
            r_regs[insn->args[1].index],
            insn->args[0].offset,
            dest_regs[insn->args[0].index]);
        emit (0x0f);
        emit (0x7f);
        emit_modrm (insn->args + 1, insn->args + 0);
        g_print ("\n");
        break;
      case 2:
        g_print ("  movq %%%s, %%%s\n",
            r_regs[insn->args[2].index],
            r_regs[insn->args[0].index]);
        emit (0x0f);
        emit (0x6f);
        emit_modrm (insn->args + 0, insn->args + 1);
        g_print ("\n");
        g_print ("  paddw %%%s, %%%s\n",
            r_regs[insn->args[1].index],
            r_regs[insn->args[0].index]);
        emit (0x0f);
        emit (0xfd);
        emit_modrm (insn->args + 0, insn->args + 1);
        g_print ("\n");
        break;
    }
  }

  g_print("  emms\n");
  g_print("  pop %%ebp\n");
  g_print("  ret\n");
}
#endif

#define X86_REG_BASE 8

static const char *
x86_get_regname(int i)
{
  static const char *x86_regs[] = { "eax", "ecx", "edx", "ebx",
    "esp", "ebp", "esi", "edi" };

  if (i>=X86_REG_BASE && i<X86_REG_BASE + 8) return x86_regs[i - X86_REG_BASE];
  switch (i) {
    case 0:
      return "UNALLOCATED";
    case 1:
      return "direct";
    default:
      return "ERROR";
  }
}

static const char *
x86_get_regname_16(int i)
{
  static const char *x86_regs[] = { "ax", "cx", "dx", "bx",
    "sp", "bp", "si", "di" };

  if (i>=X86_REG_BASE && i<X86_REG_BASE + 8) return x86_regs[i - X86_REG_BASE];
  switch (i) {
    case 0:
      return "UNALLOCATED";
    case 1:
      return "direct";
    default:
      return "ERROR";
  }
}


void oj_program_rewrite_vars (OJProgram *program);
void oj_program_allocate_regs (OJProgram *program);
void oj_program_dump (OJProgram *program);

void
oj_program_compile_x86 (OJProgram *program)
{
  int j;
  int k;
  OJInstruction *insn;
  OJOpcode *opcode;
  OJVariable *args[10];
  OJRuleList *list;
  OJRule *rule;

  list = oj_rule_list_new();
  oj_program_x86_register_rules (list);

  oj_program_rewrite_vars (program);
  oj_program_dump (program);

  printf("# n_insns %d\n", program->n_insns);
  g_print("  push %%ebp\n");
  g_print("  movl 0x8(%%esp), %%ebp\n");
  g_print("  push %%edi\n");
  g_print("  push %%esi\n");
  g_print("  push %%ebx\n");

  g_print("  movl 0x%02x(%%ebp), %%ecx\n", (int)G_STRUCT_OFFSET(OJExecutor,n));
  g_print("  movl %%ecx, 0x%02x(%%ebp)\n", (int)G_STRUCT_OFFSET(OJExecutor,counter));
  g_print("  testl %%ecx, %%ecx\n");
  g_print("  je 2f\n");
  g_print("1:\n");

  for(j=0;j<program->n_insns;j++){
    insn = program->insns + j;
    opcode = insn->opcode;

    printf("# %d: %s\n", j, insn->opcode->name);

    /* set up args */
    for(k=0;k<opcode->n_src + opcode->n_dest;k++){
      args[k] = program->vars + insn->args[k];
    }

    for(k=opcode->n_dest;k<opcode->n_src + opcode->n_dest;k++){
      switch (args[k]->vartype) {
        case OJ_VAR_TYPE_SRC:
          g_print("  movl 0x%02x(%%ebp), %%ecx\n",
              (int)G_STRUCT_OFFSET(OJExecutor, arrays[k]));
          g_print("  movw 0(%%ecx), %%%s\n",
              x86_get_regname_16(args[k]->alloc));
          break;
        case OJ_VAR_TYPE_CONST:
          g_print("  movl $%d, %%%s\n", args[k]->s16,
              x86_get_regname(args[k]->alloc));
          break;
        case OJ_VAR_TYPE_TEMP:
#if 0
          g_print("  movw temp, %%%s\n",
              x86_get_regname(args[k]->alloc));
#endif
          break;
        default:
          break;
      }
    }

    rule = oj_rule_list_get (list, opcode);
    if (rule) {
      if (rule->flags & OJ_RULE_MUST_CHAIN_SRC1 &&
          insn->args[0] != insn->args[1]) {
        g_print ("  movw %%%s, %%%s\n",
            x86_get_regname_16(args[1]->alloc),
            x86_get_regname_16(args[0]->alloc));
      }
      rule->emit (program, rule->emit_user, insn);
    } else {
      printf("No rule for: %s\n", opcode->name);
    }

    for(k=0;k<opcode->n_dest;k++){
      switch (args[k]->vartype) {
        case OJ_VAR_TYPE_DEST:
          g_print("  movl 0x%02x(%%ebp), %%ecx\n",
              (int)G_STRUCT_OFFSET(OJExecutor, arrays[k]));
          g_print("  movw %%%s, 0(%%ecx)\n",
              x86_get_regname_16(args[k]->alloc));
          break;
        case OJ_VAR_TYPE_TEMP:
#if 0
          g_print("  movw %%%s, temp\n",
              x86_get_regname(args[k]->alloc));
#endif
          break;
        default:
          break;
      }
    }
  }

  for(k=0;k<program->n_vars;k++){
    if (program->vars[k].vartype == OJ_VAR_TYPE_SRC ||
        program->vars[k].vartype == OJ_VAR_TYPE_DEST) {
      g_print("  addl $2, 0x%02x(%%ebp)\n",
          (int)G_STRUCT_OFFSET(OJExecutor, arrays[k]));
    }
  }
  g_print("  decl 0x%02x(%%ebp)\n", (int)G_STRUCT_OFFSET(OJExecutor,counter));
  g_print("  jne 1b\n");
  g_print("2:\n");

  g_print("  pop %%ebx\n");
  g_print("  pop %%esi\n");
  g_print("  pop %%edi\n");
  g_print("  pop %%ebp\n");
  g_print("  ret\n");
}


#if 0
static int
oj_program_get_reg (OJProgram *program, int insn, int var)
{
  int i;

  for(i=0;i<program->n_regs;i++) {
    if (program->regs[i].var == var && !program->regs[i].retired) {
      program->regs[i].last_use = insn;
      return i;
    }
  }

  program->regs[i].var = var;
  program->regs[i].first_use = insn;
  program->regs[i].last_use = insn;

  program->n_regs++;

  return i;
}

static void
oj_program_retire_reg (OJProgram *program, int var)
{
  int i;

  for(i=0;i<program->n_regs;i++) {
    if (program->regs[i].var == var && !program->regs[i].retired) {
      program->regs[i].retired = TRUE;
      return;
    }
  }
}
#endif

#if 0
void
oj_program_allocate_regs (OJProgram *program)
{
  int i;
  int j;
  int k;
  OJInstruction *insn;
  OJOpcode *opcode;
  OJVariable *args[10];
  int alloc[8] = { 0, 0, 0, 0, 1, 1, 1, 0 };

  for(j=0;j<program->n_insns;j++){
    insn = program->insns + j;
    opcode = insn->opcode;

    /* set up args */
    for(k=opcode->n_dest;k<opcode->n_src + opcode->n_dest;k++){
      args[k] = program->vars + insn->args[k];

      oj_program_get_reg (program, j, insn->args[k]);
    }

    for(k=0;k<opcode->n_dest;k++){
      args[k] = program->vars + insn->args[k];

      oj_program_retire_reg (program, insn->args[k]);
      oj_program_get_reg (program, j, insn->args[k]);
    }
  }

  for(j=0;j<program->n_insns;j++){
    for(i=0;i<program->n_regs;i++){
      if (program->regs[i].first_use == j) {
        for(k=0;k<8;k++){
          if (!alloc[k]) {
            program->regs[i].alloc = X86_REG_BASE + k;
            alloc[k] = 1;
            break;
          }
        }
        if (k==8) {
          g_print("register overflow\n");
        }
      }
    }
    for(i=0;i<program->n_regs;i++){
      if (program->regs[i].last_use == j) {
        alloc[program->regs[i].alloc - X86_REG_BASE] = 0;
      }
    }
  }

  for(i=0;i<program->n_regs;i++){
    g_print("%2d: %2d %2d %2d %d %s\n",
        i, program->regs[i].var,
        program->regs[i].first_use,
        program->regs[i].last_use,
        program->regs[i].retired,
        x86_get_regname(program->regs[i].alloc));
  }

}
#endif


void
oj_program_rewrite_vars (OJProgram *program)
{
  int i;
  int j;
  int k;
  OJInstruction *insn;
  OJOpcode *opcode;
  int var;
  int actual_var;
  int alloc[8] = { 0, 1, 0, 0, 1, 1, 0, 0 };

  for(j=0;j<program->n_insns;j++){
    insn = program->insns + j;
    opcode = insn->opcode;

    /* set up args */
    for(k=opcode->n_dest;k<opcode->n_src + opcode->n_dest;k++){
      var = insn->args[k];
      if (program->vars[var].vartype == OJ_VAR_TYPE_DEST) {
        g_print("ERROR: using dest var as source\n");
      }

      actual_var = var;
      if (program->vars[var].replaced) {
        actual_var = program->vars[var].replacement;
        insn->args[k] = actual_var;
      }

      if (!program->vars[var].used) {
        if (program->vars[var].vartype == OJ_VAR_TYPE_TEMP) {
          g_print("ERROR: using uninitialized temp var\n");
        }
        program->vars[var].used = TRUE;
        program->vars[var].first_use = j;
      }
      program->vars[actual_var].last_use = j;
    }

    for(k=0;k<opcode->n_dest;k++){
      var = insn->args[k];

      if (program->vars[var].vartype == OJ_VAR_TYPE_SRC) {
        g_print("ERROR: using src var as dest\n");
      }
      if (program->vars[var].vartype == OJ_VAR_TYPE_CONST) {
        g_print("ERROR: using const var as dest\n");
      }
      if (program->vars[var].vartype == OJ_VAR_TYPE_PARAM) {
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
        if (program->vars[var].vartype == OJ_VAR_TYPE_DEST) {
          g_print("ERROR: writing dest more than once\n");
        }
        if (program->vars[var].vartype == OJ_VAR_TYPE_TEMP) {
          actual_var = oj_program_dup_temporary (program, var, j);
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
            program->vars[i].alloc = k + X86_REG_BASE;
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
        alloc[program->vars[i].alloc - X86_REG_BASE] = 0;
      }
    }
  }

  for(i=0;i<program->n_vars;i++){
    g_print("%2d: %2d %2d %s\n",
        i,
        program->vars[i].first_use,
        program->vars[i].last_use,
        x86_get_regname(program->vars[i].alloc));
  }

}

void
oj_program_dump (OJProgram *program)
{
  int i;
  int j;
  OJOpcode *opcode;
  OJInstruction *insn;

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


/* rules */

static void
x86_rule_add_s16 (OJProgram *p, void *user, OJInstruction *insn)
{
  g_print("  addw %%%s, %%%s\n",
      x86_get_regname_16(p->vars[insn->args[2]].alloc),
      x86_get_regname_16(p->vars[insn->args[0]].alloc));
}

static void
x86_rule_sub_s16 (OJProgram *p, void *user, OJInstruction *insn)
{
  g_print("  subw %%%s, %%%s\n",
      x86_get_regname_16(p->vars[insn->args[2]].alloc),
      x86_get_regname_16(p->vars[insn->args[0]].alloc));
}

static void
x86_rule_mul_s16 (OJProgram *p, void *user, OJInstruction *insn)
{
  g_print("  imulw %%%s, %%%s\n",
      x86_get_regname_16(p->vars[insn->args[2]].alloc),
      x86_get_regname_16(p->vars[insn->args[0]].alloc));
}

static void
x86_rule_lshift_s16 (OJProgram *p, void *user, OJInstruction *insn)
{
  g_print("  shlw %%%s, %%%s\n",
      x86_get_regname_16(p->vars[insn->args[2]].alloc),
      x86_get_regname_16(p->vars[insn->args[0]].alloc));
}

static void
x86_rule_rshift_s16 (OJProgram *p, void *user, OJInstruction *insn)
{
  g_print("  movl %%%s, %%ecx\n",
      x86_get_regname(p->vars[insn->args[2]].alloc));
  g_print("  sarw %%cl, %%%s\n",
      x86_get_regname_16(p->vars[insn->args[0]].alloc));
}


void
oj_program_x86_register_rules (OJRuleList *list)
{
  oj_rule_list_register (list, "add_s16", x86_rule_add_s16, NULL,
      OJ_RULE_MUST_CHAIN_SRC1 | OJ_RULE_SYMMETRIC_SRC);
  oj_rule_list_register (list, "sub_s16", x86_rule_sub_s16, NULL,
      OJ_RULE_MUST_CHAIN_SRC1);
  oj_rule_list_register (list, "mul_s16", x86_rule_mul_s16, NULL,
      OJ_RULE_MUST_CHAIN_SRC1 | OJ_RULE_SYMMETRIC_SRC);
  oj_rule_list_register (list, "lshift_s16", x86_rule_lshift_s16, NULL,
      OJ_RULE_MUST_CHAIN_SRC1);
  oj_rule_list_register (list, "rshift_s16", x86_rule_rshift_s16, NULL,
      OJ_RULE_MUST_CHAIN_SRC1);
}



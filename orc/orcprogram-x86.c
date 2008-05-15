
#include "config.h"

#include <glib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>

#include <orc/orcprogram.h>

#define SIZE 65536

void x86_emit_push (OJProgram *program, int size, int reg);
void x86_emit_pop (OJProgram *program, int size, int reg);
void x86_emit_mov_memoffset_reg (OJProgram *program, int size, int offset, int reg1, int reg2);
void x86_emit_mov_reg_memoffset (OJProgram *program, int size, int reg1, int offset, int reg2);
void x86_emit_mov_imm_reg (OJProgram *program, int size, int value, int reg1);
void x86_emit_mov_reg_reg (OJProgram *program, int size, int reg1, int reg2);
void x86_emit_test_reg_reg (OJProgram *program, int size, int reg1, int reg2);
void x86_emit_dec_memoffset (OJProgram *program, int size, int offset, int reg);
void x86_emit_add_imm_memoffset (OJProgram *program, int size, int value, int offset, int reg);
void x86_emit_ret (OJProgram *program);
void x86_emit_je (OJProgram *program, int label);
void x86_emit_jne (OJProgram *program, int label);
void x86_emit_label (OJProgram *program, int label);

void x86_emit_modrm_memoffset (OJProgram *program, int reg1, int offset, int reg2);
void x86_emit_modrm_reg (OJProgram *program, int reg1, int reg2);
void x86_test (OJProgram *program);

enum {
  X86_EAX = OJ_GP_REG_BASE,
  X86_ECX,
  X86_EDX,
  X86_EBX,
  X86_ESP,
  X86_EBP,
  X86_ESI,
  X86_EDI
};

static const char *
x86_get_regname(int i)
{
  static const char *x86_regs[] = { "eax", "ecx", "edx", "ebx",
    "esp", "ebp", "esi", "edi" };

  if (i>=OJ_GP_REG_BASE && i<OJ_GP_REG_BASE + 8) return x86_regs[i - OJ_GP_REG_BASE];
  switch (i) {
    case 0:
      return "UNALLOCATED";
    case 1:
      return "direct";
    default:
      return "ERROR";
  }
}

static int
x86_get_regnum(int i)
{
  return i - OJ_GP_REG_BASE;
}

static const char *
x86_get_regname_16(int i)
{
  static const char *x86_regs[] = { "ax", "cx", "dx", "bx",
    "sp", "bp", "si", "di" };

  if (i>=OJ_GP_REG_BASE && i<OJ_GP_REG_BASE + 8) return x86_regs[i - OJ_GP_REG_BASE];
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
x86_emit_prologue (OJProgram *program)
{
  g_print(".global test\n");
  g_print("test:\n");
  x86_emit_push (program, 4, X86_EBP);
  x86_emit_mov_memoffset_reg (program, 4, 8, X86_ESP, X86_EBP);
  x86_emit_push (program, 4, X86_EDI);
  x86_emit_push (program, 4, X86_ESI);
  x86_emit_push (program, 4, X86_EBX);
}

void
x86_emit_epilogue (OJProgram *program)
{
  x86_emit_pop (program, 4, X86_EBX);
  x86_emit_pop (program, 4, X86_ESI);
  x86_emit_pop (program, 4, X86_EDI);
  x86_emit_pop (program, 4, X86_EBP);
  x86_emit_ret (program);
}

void
x86_do_fixups (OJProgram *program)
{
  int i;
  for(i=0;i<program->n_fixups;i++){
    if (program->fixups[i].type == 0) {
      unsigned char *label = program->labels[program->fixups[i].label];
      unsigned char *ptr = program->fixups[i].ptr;

      ptr[0] += label - ptr;
    }
  }
}

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

  oj_program_allocate_codemem (program);

  list = oj_rule_list_new();
  oj_program_x86_register_rules (list);

  oj_program_rewrite_vars (program);

  x86_emit_prologue (program);

  x86_emit_mov_memoffset_reg (program, 4, (int)G_STRUCT_OFFSET(OJExecutor,n),
      X86_EBP, X86_ECX);

  x86_emit_mov_reg_memoffset (program, 4, X86_ECX,
      (int)G_STRUCT_OFFSET(OJExecutor,counter), X86_EBP);

  x86_emit_test_reg_reg (program, 4, X86_ECX, X86_ECX);

  x86_emit_je (program, 1);
  x86_emit_label (program, 0);

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
          x86_emit_mov_memoffset_reg (program, 4,
              (int)G_STRUCT_OFFSET(OJExecutor, arrays[k]),
              X86_EBP, X86_ECX);
          x86_emit_mov_memoffset_reg (program, 2, 0, X86_ECX, args[k]->alloc);
          break;
        case OJ_VAR_TYPE_CONST:
          x86_emit_mov_imm_reg (program, 2, args[k]->s16, args[k]->alloc);
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
      if (!(rule->flags & OJ_RULE_3REG) && insn->args[0] != insn->args[1]) {
        x86_emit_mov_reg_reg (program, 2, args[1]->alloc, args[0]->alloc);
      }
      rule->emit (program, rule->emit_user, insn);
    } else {
      printf("No rule for: %s\n", opcode->name);
    }

    for(k=0;k<opcode->n_dest;k++){
      switch (args[k]->vartype) {
        case OJ_VAR_TYPE_DEST:
          x86_emit_mov_memoffset_reg (program, 4,
              (int)G_STRUCT_OFFSET(OJExecutor, arrays[k]),
              X86_EBP, X86_ECX);
          x86_emit_mov_reg_memoffset (program, 2, args[k]->alloc, 0, X86_ECX);
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
      x86_emit_add_imm_memoffset (program, 4, 2, 
          (int)G_STRUCT_OFFSET(OJExecutor, arrays[k]),
          X86_EBP);
    }
  }

  x86_emit_dec_memoffset (program, 4, (int)G_STRUCT_OFFSET(OJExecutor,counter),
      X86_EBP);
  x86_emit_jne (program, 0);
  x86_emit_label (program, 1);

  x86_emit_epilogue (program);

  x86_test (program);


  x86_do_fixups (program);

  oj_program_dump_code (program);
}


/* rules */

static void
x86_rule_add_s16 (OJProgram *p, void *user, OJInstruction *insn)
{
  g_print("  addw %%%s, %%%s\n",
      x86_get_regname_16(p->vars[insn->args[2]].alloc),
      x86_get_regname_16(p->vars[insn->args[0]].alloc));

  *p->codeptr++ = 0x66;
  *p->codeptr++ = 0x01;
  x86_emit_modrm_reg (p, p->vars[insn->args[0]].alloc,
      p->vars[insn->args[2]].alloc);

}

static void
x86_rule_sub_s16 (OJProgram *p, void *user, OJInstruction *insn)
{
  g_print("  subw %%%s, %%%s\n",
      x86_get_regname_16(p->vars[insn->args[2]].alloc),
      x86_get_regname_16(p->vars[insn->args[0]].alloc));

  *p->codeptr++ = 0x66;
  *p->codeptr++ = 0x29;
  x86_emit_modrm_reg (p, p->vars[insn->args[0]].alloc,
      p->vars[insn->args[2]].alloc);
}

static void
x86_rule_mul_s16 (OJProgram *p, void *user, OJInstruction *insn)
{
  g_print("  imulw %%%s, %%%s\n",
      x86_get_regname_16(p->vars[insn->args[2]].alloc),
      x86_get_regname_16(p->vars[insn->args[0]].alloc));

  *p->codeptr++ = 0x66;
  *p->codeptr++ = 0x0f;
  *p->codeptr++ = 0xaf;
  x86_emit_modrm_reg (p, p->vars[insn->args[0]].alloc,
      p->vars[insn->args[2]].alloc);
}

static void
x86_rule_lshift_s16 (OJProgram *p, void *user, OJInstruction *insn)
{
  x86_emit_mov_reg_reg(p, 4, p->vars[insn->args[2]].alloc, X86_ECX);

  g_print("  shlw %%cl, %%%s\n",
      x86_get_regname_16(p->vars[insn->args[0]].alloc));

  *p->codeptr++ = 0x66;
  *p->codeptr++ = 0xd3;
  x86_emit_modrm_reg (p, p->vars[insn->args[0]].alloc, 4);
}

static void
x86_rule_rshift_s16 (OJProgram *p, void *user, OJInstruction *insn)
{
  x86_emit_mov_reg_reg(p, 4, p->vars[insn->args[2]].alloc, X86_ECX);

  g_print("  sarw %%cl, %%%s\n",
      x86_get_regname_16(p->vars[insn->args[0]].alloc));

  *p->codeptr++ = 0x66;
  *p->codeptr++ = 0xd3;
  x86_emit_modrm_reg (p, p->vars[insn->args[0]].alloc, 7);
}


void
oj_program_x86_register_rules (OJRuleList *list)
{
  oj_rule_list_register (list, "add_s16", x86_rule_add_s16, NULL,
      OJ_RULE_REG_REG);
  oj_rule_list_register (list, "sub_s16", x86_rule_sub_s16, NULL,
      OJ_RULE_REG_REG);
  oj_rule_list_register (list, "mul_s16", x86_rule_mul_s16, NULL,
      OJ_RULE_REG_REG);
  oj_rule_list_register (list, "lshift_s16", x86_rule_lshift_s16, NULL,
      OJ_RULE_REG_REG);
  oj_rule_list_register (list, "rshift_s16", x86_rule_rshift_s16, NULL,
      OJ_RULE_REG_REG);
}


/* code generation */

void
x86_emit_push (OJProgram *program, int size, int reg)
{

  if (size == 1) {
    program->error = 1;
  } else if (size == 2) {
    g_print("  pushw %%%s\n", x86_get_regname_16(reg));
    *program->codeptr++ = 0x66;
    *program->codeptr++ = 0x50 + x86_get_regnum(reg);
  } else {
    g_print("  pushl %%%s\n", x86_get_regname(reg));
    *program->codeptr++ = 0x50 + x86_get_regnum(reg);
  }
}

void
x86_emit_pop (OJProgram *program, int size, int reg)
{

  if (size == 1) {
    program->error = 1;
  } else if (size == 2) {
    g_print("  popw %%%s\n", x86_get_regname_16(reg));
    *program->codeptr++ = 0x66;
    *program->codeptr++ = 0x58 + x86_get_regnum(reg);
  } else {
    g_print("  popl %%%s\n", x86_get_regname(reg));
    *program->codeptr++ = 0x58 + x86_get_regnum(reg);
  }
}

#define X86_MODRM(mod, rm, reg) ((((mod)&3)<<6)|(((rm)&7)<<0)|(((reg)&7)<<3))
#define X86_SIB(ss, ind, reg) ((((ss)&3)<<6)|(((ind)&7)<<3)|((reg)&7))

void
x86_emit_modrm_memoffset (OJProgram *program, int reg1, int offset, int reg2)
{
  if (offset == 0 && reg2 != X86_EBP) {
    if (reg2 == X86_ESP) {
      *program->codeptr++ = X86_MODRM(0, 4, reg1);
      *program->codeptr++ = X86_SIB(0, 4, reg2);
    } else {
      *program->codeptr++ = X86_MODRM(0, reg2, reg1);
    }
  } else if (offset >= -128 && offset < 128) {
    *program->codeptr++ = X86_MODRM(1, reg2, reg1);
    if (reg2 == X86_ESP) {
      *program->codeptr++ = X86_SIB(0, 4, reg2);
    }
    *program->codeptr++ = (offset & 0xff);
  } else {
    *program->codeptr++ = X86_MODRM(2, reg2, reg1);
    if (reg2 == X86_ESP) {
      *program->codeptr++ = X86_SIB(0, 4, reg2);
    }
    *program->codeptr++ = (offset & 0xff);
    *program->codeptr++ = ((offset>>8) & 0xff);
    *program->codeptr++ = ((offset>>16) & 0xff);
    *program->codeptr++ = ((offset>>24) & 0xff);
  }
}

void
x86_emit_modrm_reg (OJProgram *program, int reg1, int reg2)
{
  *program->codeptr++ = X86_MODRM(3, reg1, reg2);
}

void
x86_emit_mov_memoffset_reg (OJProgram *program, int size, int offset,
    int reg1, int reg2)
{
  if (size == 2) {
    g_print("  movw %d(%%%s), %%%s\n", offset, x86_get_regname(reg1),
        x86_get_regname_16(reg2));
    *program->codeptr++ = 0x66;
  } else {
    g_print("  movl %d(%%%s), %%%s\n", offset, x86_get_regname(reg1),
        x86_get_regname(reg2));
  }

  *program->codeptr++ = 0x8b;
  x86_emit_modrm_memoffset (program, reg2, offset, reg1);
}

void
x86_emit_mov_reg_memoffset (OJProgram *program, int size, int reg1, int offset,
    int reg2)
{
  if (size == 2) {
    g_print("  movw %%%s, %d(%%%s)\n", x86_get_regname_16(reg1), offset,
        x86_get_regname(reg2));
    *program->codeptr++ = 0x66;
  } else {
    g_print("  movl %%%s, %d(%%%s)\n", x86_get_regname(reg1), offset,
        x86_get_regname(reg2));
  }

  *program->codeptr++ = 0x89;
  x86_emit_modrm_memoffset (program, reg1, offset, reg2);
}

void
x86_emit_mov_imm_reg (OJProgram *program, int size, int value, int reg1)
{
  if (size == 2) {
    g_print("  movw $%d, %%%s\n", value, x86_get_regname_16(reg1));
    *program->codeptr++ = 0x66;
    *program->codeptr++ = 0xb8 + x86_get_regnum(reg1);
    *program->codeptr++ = (value & 0xff);
    *program->codeptr++ = ((value>>8) & 0xff);
  } else {
    g_print("  movl $%d, %%%s\n", value, x86_get_regname(reg1));
    *program->codeptr++ = 0xb8 + x86_get_regnum(reg1);
    *program->codeptr++ = (value & 0xff);
    *program->codeptr++ = ((value>>8) & 0xff);
    *program->codeptr++ = ((value>>16) & 0xff);
    *program->codeptr++ = ((value>>24) & 0xff);
  }

}

void x86_emit_mov_reg_reg (OJProgram *program, int size, int reg1, int reg2)
{
  if (size == 2) {
    g_print("  movw %%%s, %%%s\n", x86_get_regname_16(reg1),
        x86_get_regname_16(reg2));
    *program->codeptr++ = 0x66;
  } else {
    g_print("  movl %%%s, %%%s\n", x86_get_regname(reg1),
        x86_get_regname(reg2));
  }

  *program->codeptr++ = 0x89;
  x86_emit_modrm_reg (program, reg2, reg1);
}


void
x86_emit_test_reg_reg (OJProgram *program, int size, int reg1, int reg2)
{
  if (size == 2) {
    g_print("  testw %%%s, %%%s\n", x86_get_regname_16(reg1),
        x86_get_regname_16(reg2));
    *program->codeptr++ = 0x66;
  } else {
    g_print("  testl %%%s, %%%s\n", x86_get_regname(reg1),
        x86_get_regname(reg2));
  }

  *program->codeptr++ = 0x85;
  x86_emit_modrm_reg (program, reg2, reg1);
}

void
x86_emit_add_imm_memoffset (OJProgram *program, int size, int value,
    int offset, int reg)
{
  if (size == 2) {
    g_print("  addw $%d, %d(%%%s)\n", value, offset,
        x86_get_regname(reg));
    *program->codeptr++ = 0x66;
  } else {
    g_print("  addl $%d, %d(%%%s)\n", value, offset,
        x86_get_regname(reg));
  }

  if (value >= -128 && value < 128) {
    *program->codeptr++ = 0x83;
    x86_emit_modrm_memoffset (program, 0, offset, reg);
    *program->codeptr++ = (value & 0xff);
  } else {
    *program->codeptr++ = 0x81;
    x86_emit_modrm_memoffset (program, 0, offset, reg);
    *program->codeptr++ = (value & 0xff);
    *program->codeptr++ = ((value>>8) & 0xff);
    if (size == 4) {
      *program->codeptr++ = ((value>>16) & 0xff);
      *program->codeptr++ = ((value>>24) & 0xff);
    }
  }
}

void
x86_emit_dec_memoffset (OJProgram *program, int size,
    int offset, int reg)
{
  if (size == 2) {
    g_print("  decw %d(%%%s)\n", offset, x86_get_regname(reg));
    *program->codeptr++ = 0x66;
  } else {
    g_print("  decl %d(%%%s)\n", offset, x86_get_regname(reg));
  }

  *program->codeptr++ = 0xff;
  x86_emit_modrm_memoffset (program, 1, offset, reg);
}

void x86_emit_ret (OJProgram *program)
{
  g_print("  ret\n");
  *program->codeptr++ = 0xc3;
}

void
x86_add_fixup (OJProgram *program, unsigned char *ptr, int label)
{
  program->fixups[program->n_fixups].ptr = ptr;
  program->fixups[program->n_fixups].label = label;
  program->fixups[program->n_fixups].type = 0;
  program->n_fixups++;
}

void
x86_add_label (OJProgram *program, unsigned char *ptr, int label)
{
  program->labels[label] = ptr;
}

void x86_emit_je (OJProgram *program, int label)
{
  g_print("  je .L%d\n", label);

  *program->codeptr++ = 0x74;
  x86_add_fixup (program, program->codeptr, label);
  *program->codeptr++ = -1;
}

void x86_emit_jne (OJProgram *program, int label)
{
  g_print("  jne .L%d\n", label);
  *program->codeptr++ = 0x75;
  x86_add_fixup (program, program->codeptr, label);
  *program->codeptr++ = -1;
}

void x86_emit_label (OJProgram *program, int label)
{
  g_print(".L%d:\n", label);

  x86_add_label (program, program->codeptr, label);
}

void
x86_test (OJProgram *program)
{
  int size;
  int i;
  int j;
  int reg;

  for(size=2;size<=4;size+=2) {
    for(i=0;i<8;i++){
      reg = OJ_GP_REG_BASE + i;
      x86_emit_push (program, size, reg);
      x86_emit_pop (program, size, reg);
      x86_emit_mov_imm_reg (program, size, 0, reg);
      x86_emit_mov_imm_reg (program, size, 1, reg);
      x86_emit_mov_imm_reg (program, size, 256, reg);
      x86_emit_dec_memoffset (program, size, 0, reg);
      x86_emit_dec_memoffset (program, size, 1, reg);
      x86_emit_dec_memoffset (program, size, 256, reg);
      x86_emit_add_imm_memoffset (program, size, 1, 0, reg);
      x86_emit_add_imm_memoffset (program, size, 1, 1, reg);
      x86_emit_add_imm_memoffset (program, size, 1, 256, reg);
      x86_emit_add_imm_memoffset (program, size, 256, 0, reg);
      x86_emit_add_imm_memoffset (program, size, 256, 1, reg);
      x86_emit_add_imm_memoffset (program, size, 256, 256, reg);
      for(j=0;j<8;j++){
        int reg2 = OJ_GP_REG_BASE + j;
        x86_emit_mov_reg_reg (program, size, reg, reg2);
        x86_emit_mov_memoffset_reg (program, size, 0, reg, reg2);
        x86_emit_mov_memoffset_reg (program, size, 1, reg, reg2);
        x86_emit_mov_memoffset_reg (program, size, 256, reg, reg2);
        x86_emit_mov_reg_memoffset (program, size, reg, 0, reg2);
        x86_emit_mov_reg_memoffset (program, size, reg, 1, reg2);
        x86_emit_mov_reg_memoffset (program, size, reg, 256, reg2);
      }
    }
  }

}


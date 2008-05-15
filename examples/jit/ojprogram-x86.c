
#include "config.h"

#include <glib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>

#include "ojprogram.h"

#define SIZE 65536

#define X86_REG_BASE 8

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

void oj_program_dump_code (OJProgram *program);

enum {
  X86_EAX = X86_REG_BASE,
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

static int
x86_get_regnum(int i)
{
  return i - X86_REG_BASE;
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

  oj_program_allocate_codemem (program);

  list = oj_rule_list_new();
  oj_program_x86_register_rules (list);

  oj_program_rewrite_vars (program);

  printf(".global test\n");
  printf("test:\n");
  printf("# n_insns %d\n", program->n_insns);
  x86_emit_push (program, 4, X86_EBP);
  x86_emit_mov_memoffset_reg (program, 4, 8, X86_ESP, X86_EBP);
  x86_emit_push (program, 4, X86_EDI);
  x86_emit_push (program, 4, X86_ESI);
  x86_emit_push (program, 4, X86_EBX);

  //g_print("  movl 0x%02x(%%ebp), %%ecx\n", (int)G_STRUCT_OFFSET(OJExecutor,n));
  x86_emit_mov_memoffset_reg (program, 4, (int)G_STRUCT_OFFSET(OJExecutor,n),
      X86_EBP, X86_ECX);

  //g_print("  movl %%ecx, 0x%02x(%%ebp)\n", (int)G_STRUCT_OFFSET(OJExecutor,counter));
  x86_emit_mov_reg_memoffset (program, 4, X86_ECX,
      (int)G_STRUCT_OFFSET(OJExecutor,counter), X86_EBP);

  //g_print("  testl %%ecx, %%ecx\n");
  x86_emit_test_reg_reg (program, 4, X86_ECX, X86_ECX);

  //g_print("  je 2f\n");
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
          //g_print("  movl 0x%02x(%%ebp), %%ecx\n",
          //    (int)G_STRUCT_OFFSET(OJExecutor, arrays[k]));
          x86_emit_mov_memoffset_reg (program, 4,
              (int)G_STRUCT_OFFSET(OJExecutor, arrays[k]),
              X86_EBP, X86_ECX);
          //g_print("  movw 0(%%ecx), %%%s\n",
          //    x86_get_regname_16(args[k]->alloc));
          x86_emit_mov_memoffset_reg (program, 2, 0, X86_ECX, args[k]->alloc);
          break;
        case OJ_VAR_TYPE_CONST:
          //g_print("  movl $%d, %%%s\n", args[k]->s16,
          //    x86_get_regname(args[k]->alloc));
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
      if (rule->flags & OJ_RULE_MUST_CHAIN_SRC1 &&
          insn->args[0] != insn->args[1]) {
        //g_print ("  movw %%%s, %%%s\n",
        //    x86_get_regname_16(args[1]->alloc),
        //    x86_get_regname_16(args[0]->alloc));
        x86_emit_mov_reg_reg (program, 2, args[1]->alloc, args[0]->alloc);
      }
      rule->emit (program, rule->emit_user, insn);
    } else {
      printf("No rule for: %s\n", opcode->name);
    }

    for(k=0;k<opcode->n_dest;k++){
      switch (args[k]->vartype) {
        case OJ_VAR_TYPE_DEST:
          //g_print("  movl 0x%02x(%%ebp), %%ecx\n",
          //    (int)G_STRUCT_OFFSET(OJExecutor, arrays[k]));
          x86_emit_mov_memoffset_reg (program, 4,
              (int)G_STRUCT_OFFSET(OJExecutor, arrays[k]),
              X86_EBP, X86_ECX);
          //g_print("  movw %%%s, 0(%%ecx)\n",
          //    x86_get_regname_16(args[k]->alloc));
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
      //g_print("  addl $2, 0x%02x(%%ebp)\n",
      //    (int)G_STRUCT_OFFSET(OJExecutor, arrays[k]));
      x86_emit_add_imm_memoffset (program, 4, 2, 
          (int)G_STRUCT_OFFSET(OJExecutor, arrays[k]),
          X86_EBP);
    }
  }
  //g_print("  decl 0x%02x(%%ebp)\n", (int)G_STRUCT_OFFSET(OJExecutor,counter));
  x86_emit_dec_memoffset (program, 4, (int)G_STRUCT_OFFSET(OJExecutor,counter),
      X86_EBP);
  //g_print("  jne 1b\n");
  x86_emit_jne (program, 0);
  x86_emit_label (program, 1);

  x86_emit_pop (program, 4, X86_EBX);
  x86_emit_pop (program, 4, X86_ESI);
  x86_emit_pop (program, 4, X86_EDI);
  x86_emit_pop (program, 4, X86_EBP);
  x86_emit_ret (program);

  x86_test (program);


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

  oj_program_dump_code (program);
}


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


void
oj_program_allocate_codemem (OJProgram *program)
{
  char *filename;
  int fd;
  GError *error = NULL;

  fd = g_file_open_tmp ("liboilexecXXXXXX", &filename, &error);
  if (fd == -1) {
    /* FIXME oh crap */
    g_print("failed to create temp file\n");
    return;
  }
  unlink (filename);
  g_free (filename);

  ftruncate (fd, SIZE);

  program->code = mmap (NULL, SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
  if (program->code == MAP_FAILED) {
    /* FIXME oh crap */
    g_print("failed to create write map\n");
    return;
  }
  program->code_exec = mmap (NULL, SIZE, PROT_READ|PROT_EXEC, MAP_SHARED, fd, 0);
  if (program->code_exec == MAP_FAILED) {
    /* FIXME oh crap */
    g_print("failed to create exec map\n");
    return;
  }

  close (fd);

  program->codeptr = program->code;
}


/* code generation */

void
oj_program_dump_code (OJProgram *program)
{
  //unsigned char *ptr;
  FILE *file;

  file = fopen("dump","w");

  fwrite (program->code, 1, program->codeptr - program->code, file);
  fclose (file);

#if 0
  ptr = program->code;
  while(ptr < program->codeptr) {
    g_print("# %02x\n", ptr[0]);
    ptr++;
  }
#endif
}

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
      reg = X86_REG_BASE + i;
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
        int reg2 = X86_REG_BASE + j;
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


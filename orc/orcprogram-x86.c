
#include "config.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>

#include <orc/orcprogram.h>

#define SIZE 65536

void x86_emit_push (OrcProgram *program, int size, int reg);
void x86_emit_pop (OrcProgram *program, int size, int reg);
void x86_emit_mov_memoffset_reg (OrcProgram *program, int size, int offset, int reg1, int reg2);
void x86_emit_mov_memoffset_mmx (OrcProgram *program, int size, int offset,
    int reg1, int reg2);
void x86_emit_mov_reg_memoffset (OrcProgram *program, int size, int reg1, int offset, int reg2);
void x86_emit_mov_mmx_memoffset (OrcProgram *program, int size, int reg1, int offset,
    int reg2);
void x86_emit_mov_imm_reg (OrcProgram *program, int size, int value, int reg1);
void x86_emit_mov_reg_reg (OrcProgram *program, int size, int reg1, int reg2);
void x86_emit_mov_reg_mmx (OrcProgram *program, int reg1, int reg2);
void x86_emit_mov_mmx_reg (OrcProgram *program, int reg1, int reg2);
void x86_emit_test_reg_reg (OrcProgram *program, int size, int reg1, int reg2);
void x86_emit_sar_imm_reg (OrcProgram *program, int size, int value, int reg);
void x86_emit_dec_memoffset (OrcProgram *program, int size, int offset, int reg);
void x86_emit_add_imm_memoffset (OrcProgram *program, int size, int value, int offset, int reg);
void x86_emit_add_imm_reg (OrcProgram *program, int size, int value, int reg);
void x86_emit_emms (OrcProgram *program);
void x86_emit_ret (OrcProgram *program);
void x86_emit_je (OrcProgram *program, int label);
void x86_emit_jne (OrcProgram *program, int label);
void x86_emit_label (OrcProgram *program, int label);

static void mmx_emit_loadi_s16 (OrcProgram *p, int reg, int value);

void x86_emit_modrm_memoffset (OrcProgram *program, int reg1, int offset, int reg2);
void x86_emit_modrm_reg (OrcProgram *program, int reg1, int reg2);
void x86_test (OrcProgram *program);

void orc_program_x86_register_rules (void);
void orc_program_mmx_register_rules (void);

enum {
  X86_EAX = ORC_GP_REG_BASE,
  X86_ECX,
  X86_EDX,
  X86_EBX,
  X86_ESP,
  X86_EBP,
  X86_ESI,
  X86_EDI,
  X86_R8,
  X86_R9,
  X86_R10,
  X86_R11,
  X86_R12,
  X86_R13,
  X86_R14,
  X86_R15,
  X86_MM0,
  X86_MM1,
  X86_MM2,
  X86_MM3,
  X86_MM4,
  X86_MM5,
  X86_MM6,
  X86_MM7
};

static int x86_64 = 1;
static int x86_ptr_size = 8;
static int x86_exec_ptr = X86_EDI;


static const char *
x86_get_regname(int i)
{
  static const char *x86_regs[] = {
    "eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi",
    "r8d", "r9d", "r10d", "r11d", "r12d", "r13d", "r14d", "r15d" };

  if (i>=ORC_GP_REG_BASE && i<ORC_GP_REG_BASE + 16) return x86_regs[i - ORC_GP_REG_BASE];
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
  return (i&0xf);
}

static const char *
x86_get_regname_16(int i)
{
  static const char *x86_regs[] = { "ax", "cx", "dx", "bx",
    "sp", "bp", "si", "di" };

  if (i>=ORC_GP_REG_BASE && i<ORC_GP_REG_BASE + 8) return x86_regs[i - ORC_GP_REG_BASE];
  switch (i) {
    case 0:
      return "UNALLOCATED";
    case 1:
      return "direct";
    default:
      return "ERROR";
  }
}

const char *
x86_get_regname_64(int i)
{
  static const char *x86_regs[] = {
    "rax", "rcx", "rdx", "rbx", "rsp", "rbp", "rsi", "rdi",
    "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15" };

  if (i>=ORC_GP_REG_BASE && i<ORC_GP_REG_BASE + 16) return x86_regs[i - ORC_GP_REG_BASE];
  switch (i) {
    case 0:
      return "UNALLOCATED";
    case 1:
      return "direct";
    default:
      return "ERROR";
  }
}

const char *
x86_get_regname_ptr(int i)
{
  if (x86_64) {
    return x86_get_regname_64 (i);
  } else {
    return x86_get_regname (i);
  }
}

static const char *
x86_get_regname_mmx(int i)
{
  static const char *x86_regs[] = { "mm0", "mm1", "mm2", "mm3",
    "mm4", "mm5", "mm6", "mm7" };

  if (i>=X86_MM0 && i<X86_MM0 + 8) return x86_regs[i - X86_MM0];
  switch (i) {
    case 0:
      return "UNALLOCATED";
    case 1:
      return "direct";
    default:
      return "ERROR";
  }
}

int
orc_program_x86_allocate_register (OrcProgram *program, int data_reg)
{
  int i;

  if (program->rule_set == ORC_RULE_SCALAR_1) {
    data_reg = FALSE;
  }

  if (!data_reg) {
    int n_regs = 8;
    if (x86_64) n_regs = 16;
    for(i=ORC_GP_REG_BASE;i<ORC_GP_REG_BASE+n_regs;i++){
      if (program->alloc_regs[i] == 0) {
        program->alloc_regs[i]++;
        program->used_regs[i] = 1;
        return i;
      }
    }
  } else {
    for(i=X86_MM0;i<X86_MM0+8;i++){
      if (program->alloc_regs[i] == 0) {
        program->alloc_regs[i]++;
        program->used_regs[i] = 1;
        return i;
      }
    }
  }
  printf("register overflow\n");
  return 0;
}


void orc_program_rewrite_vars (OrcProgram *program);
void orc_program_dump (OrcProgram *program);

void
x86_emit_prologue (OrcProgram *program)
{
  printf(".global test\n");
  printf("test:\n");
  if (x86_64) {

  } else {
    x86_emit_push (program, 4, X86_EBP);
    x86_emit_mov_memoffset_reg (program, 4, 8, X86_ESP, X86_EBP);
    if (program->used_regs[X86_EDI]) {
      x86_emit_push (program, 4, X86_EDI);
    }
    if (program->used_regs[X86_ESI]) {
      x86_emit_push (program, 4, X86_ESI);
    }
    if (program->used_regs[X86_EBX]) {
      x86_emit_push (program, 4, X86_EBX);
    }
  }
}

void
x86_emit_epilogue (OrcProgram *program)
{
  if (program->rule_set == ORC_RULE_MMX_1 ||
      program->rule_set == ORC_RULE_MMX_2 ||
      program->rule_set == ORC_RULE_MMX_4 ||
      program->rule_set == ORC_RULE_MMX_8) {
    x86_emit_emms (program);
  }
  if (x86_64) {

  } else {
    if (program->used_regs[X86_EBX]) {
      x86_emit_pop (program, 4, X86_EBX);
    }
    if (program->used_regs[X86_ESI]) {
      x86_emit_pop (program, 4, X86_ESI);
    }
    if (program->used_regs[X86_EDI]) {
      x86_emit_pop (program, 4, X86_EDI);
    }
    x86_emit_pop (program, 4, X86_EBP);
  }
  x86_emit_ret (program);
}

void
x86_do_fixups (OrcProgram *program)
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
orc_x86_init (void)
{
  orc_program_x86_register_rules ();
  orc_program_mmx_register_rules ();
}

void
orc_program_x86_reset_alloc (OrcProgram *program)
{
  int i;

  for(i=ORC_GP_REG_BASE;i<ORC_GP_REG_BASE+16;i++){
    program->alloc_regs[i] = 0;
  }
  program->alloc_regs[X86_ECX] = 1;
  program->alloc_regs[X86_ESP] = 1;
  program->alloc_regs[X86_EBP] = 1;

  if (x86_64) {
    /* used for passing parameters */
    program->alloc_regs[X86_EDI] = 1;
    /* don't feel like saving */
    program->alloc_regs[X86_ESI] = 1;
    program->alloc_regs[X86_EBX] = 1;
  }
}

void
x86_load_constants (OrcProgram *program)
{
  int i;
  for(i=0;i<program->n_vars;i++){
    switch (program->vars[i].vartype) {
      case ORC_VAR_TYPE_CONST:
        mmx_emit_loadi_s16 (program, program->vars[i].alloc,
            program->vars[i].s16);
        break;
      case ORC_VAR_TYPE_SRC:
      case ORC_VAR_TYPE_DEST:
        if (program->vars[i].ptr_register) {
          x86_emit_mov_memoffset_reg (program, x86_ptr_size,
              (int)ORC_STRUCT_OFFSET(OrcExecutor, arrays[i]), x86_exec_ptr,
              program->vars[i].ptr_register);
        } else {
          /* FIXME */
          printf("ERROR");
        }
        break;
      default:
        break;
    }
  }
}

void
x86_emit_load_src (OrcProgram *program, OrcVariable *var)
{
  int ptr_reg;
  if (var->ptr_register == 0) {
    x86_emit_mov_memoffset_reg (program, x86_ptr_size,
        var->ptr_offset, x86_exec_ptr, X86_ECX);
    ptr_reg = X86_ECX;
  } else {
    ptr_reg = var->ptr_register;
  }
  switch (program->rule_set) {
    case ORC_RULE_SCALAR_1:
      x86_emit_mov_memoffset_reg (program, 2, 0, ptr_reg, var->alloc);
      break;
    case ORC_RULE_MMX_1:
      x86_emit_mov_memoffset_reg (program, 2, 0, ptr_reg, X86_ECX);
      x86_emit_mov_reg_mmx (program, X86_ECX, var->alloc);
      break;
    case ORC_RULE_MMX_2:
      x86_emit_mov_memoffset_mmx (program, 4, 0, ptr_reg, var->alloc);
      break;
    case ORC_RULE_MMX_4:
      x86_emit_mov_memoffset_mmx (program, 8, 0, ptr_reg, var->alloc);
      break;
    default:
      printf("ERROR\n");
  }
}

void
x86_emit_store_dest (OrcProgram *program, OrcVariable *var)
{
  int ptr_reg;
  if (var->ptr_register == 0) {
    x86_emit_mov_memoffset_reg (program, x86_ptr_size,
        var->ptr_offset, x86_exec_ptr, X86_ECX);
    ptr_reg = X86_ECX;
  } else {
    ptr_reg = var->ptr_register;
  }
  switch (program->rule_set) {
    case ORC_RULE_SCALAR_1:
      x86_emit_mov_reg_memoffset (program, 2, var->alloc, 0, ptr_reg);
      break;
    case ORC_RULE_MMX_1:
      /* FIXME we might be using ecx twice here */
      if (ptr_reg == X86_ECX) {
        printf("ERROR\n");
      }
      x86_emit_mov_mmx_reg (program, var->alloc, X86_ECX);
      x86_emit_mov_reg_memoffset (program, 2, X86_ECX, 0, ptr_reg);
      break;
    case ORC_RULE_MMX_2:
      x86_emit_mov_mmx_memoffset (program, 4, var->alloc, 0, ptr_reg);
      break;
    case ORC_RULE_MMX_4:
      x86_emit_mov_mmx_memoffset (program, 8, var->alloc, 0, ptr_reg);
      break;
    default:
      printf("ERROR\n");
  }
}

void
orc_program_assemble_x86 (OrcProgram *program)
{
  int j;
  int k;
  OrcInstruction *insn;
  OrcOpcode *opcode;
  OrcVariable *args[10];
  OrcRule *rule;

  x86_emit_prologue (program);

  x86_emit_mov_memoffset_reg (program, 4, (int)ORC_STRUCT_OFFSET(OrcExecutor,n),
      x86_exec_ptr, X86_ECX);

  x86_emit_sar_imm_reg (program, 4, program->loop_shift, X86_ECX);
  x86_emit_mov_reg_memoffset (program, 4, X86_ECX,
      (int)ORC_STRUCT_OFFSET(OrcExecutor,counter), x86_exec_ptr);

  x86_emit_test_reg_reg (program, 4, X86_ECX, X86_ECX);

  x86_emit_je (program, 1);

  x86_load_constants (program);

  x86_emit_label (program, 0);

  for(j=0;j<program->n_insns;j++){
    insn = program->insns + j;
    opcode = insn->opcode;

    printf("# %d: %s", j, insn->opcode->name);

    /* set up args */
    for(k=0;k<opcode->n_src + opcode->n_dest;k++){
      args[k] = program->vars + insn->args[k];
      printf(" %d", args[k]->alloc);
      if (args[k]->is_chained) {
        printf(" (chained)");
      }
    }
    printf(" rule_flag=%d", insn->rule_flag);
    printf("\n");

    for(k=opcode->n_dest;k<opcode->n_src + opcode->n_dest;k++){
      switch (args[k]->vartype) {
        case ORC_VAR_TYPE_SRC:
          x86_emit_load_src (program, args[k]);
          break;
        case ORC_VAR_TYPE_CONST:
          break;
        case ORC_VAR_TYPE_TEMP:
          break;
        default:
          break;
      }
    }

    rule = insn->rule;
    if (rule) {
      if (!(rule->flags & ORC_RULE_3REG) && args[0]->alloc != args[1]->alloc) {
        x86_emit_mov_reg_reg (program, 2, args[1]->alloc, args[0]->alloc);
      }
      rule->emit (program, rule->emit_user, insn);
    } else {
      printf("No rule for: %s\n", opcode->name);
    }

    for(k=0;k<opcode->n_dest;k++){
      switch (args[k]->vartype) {
        case ORC_VAR_TYPE_DEST:
          x86_emit_store_dest (program, args[k]);
          break;
        case ORC_VAR_TYPE_TEMP:
          break;
        default:
          break;
      }
    }
  }

  for(k=0;k<program->n_vars;k++){
    if (program->vars[k].vartype == ORC_VAR_TYPE_SRC ||
        program->vars[k].vartype == ORC_VAR_TYPE_DEST) {
      if (program->vars[k].ptr_register) {
        x86_emit_add_imm_reg (program, x86_ptr_size,
            orc_variable_get_size(program->vars + k) * program->n_per_loop,
            program->vars[k].ptr_register);
      } else {
        x86_emit_add_imm_memoffset (program, x86_ptr_size,
            orc_variable_get_size(program->vars + k) * program->n_per_loop,
            (int)ORC_STRUCT_OFFSET(OrcExecutor, arrays[k]),
            x86_exec_ptr);
      }
    }
  }

  x86_emit_dec_memoffset (program, 4,
      (int)ORC_STRUCT_OFFSET(OrcExecutor,counter),
      x86_exec_ptr);
  x86_emit_jne (program, 0);
  x86_emit_label (program, 1);

  x86_emit_epilogue (program);

  //x86_test (program);


  x86_do_fixups (program);
}


/* rules */

static void
x86_rule_loadi_s16 (OrcProgram *p, void *user, OrcInstruction *insn)
{
  x86_emit_mov_imm_reg (p, 2,  p->vars[insn->args[2]].s16,
      p->vars[insn->args[0]].alloc);
}

static void
x86_rule_add_s16 (OrcProgram *p, void *user, OrcInstruction *insn)
{
  if (insn->rule_flag == ORC_RULE_REG_IMM) {
    int value = p->vars[insn->args[2]].s16;
    printf("  addw $%d, %%%s\n", value,
        x86_get_regname_16(p->vars[insn->args[0]].alloc));

    if (value >= -128 && value < 128) {
      *p->codeptr++ = 0x66;
      *p->codeptr++ = 0x83;
      x86_emit_modrm_reg (p, p->vars[insn->args[0]].alloc, 0);
      *p->codeptr++ = value;
    } else {
      *p->codeptr++ = 0x66;
      *p->codeptr++ = 0x81;
      x86_emit_modrm_reg (p, p->vars[insn->args[0]].alloc, 0);
      *p->codeptr++ = value & 0xff;
      *p->codeptr++ = value >> 8;
    }
  } else {
    printf("  addw %%%s, %%%s\n",
        x86_get_regname_16(p->vars[insn->args[2]].alloc),
        x86_get_regname_16(p->vars[insn->args[0]].alloc));

    *p->codeptr++ = 0x66;
    *p->codeptr++ = 0x03;
    x86_emit_modrm_reg (p, p->vars[insn->args[2]].alloc,
        p->vars[insn->args[0]].alloc);
  }
}

static void
x86_rule_sub_s16 (OrcProgram *p, void *user, OrcInstruction *insn)
{
  printf("  subw %%%s, %%%s\n",
      x86_get_regname_16(p->vars[insn->args[2]].alloc),
      x86_get_regname_16(p->vars[insn->args[0]].alloc));

  *p->codeptr++ = 0x66;
  *p->codeptr++ = 0x2b;
  x86_emit_modrm_reg (p, p->vars[insn->args[2]].alloc,
      p->vars[insn->args[0]].alloc);
}

static void
x86_rule_mul_s16 (OrcProgram *p, void *user, OrcInstruction *insn)
{
  printf("  imulw %%%s, %%%s\n",
      x86_get_regname_16(p->vars[insn->args[2]].alloc),
      x86_get_regname_16(p->vars[insn->args[0]].alloc));

  *p->codeptr++ = 0x66;
  *p->codeptr++ = 0x0f;
  *p->codeptr++ = 0xaf;
  x86_emit_modrm_reg (p, p->vars[insn->args[2]].alloc,
      p->vars[insn->args[0]].alloc);
}

static void
x86_rule_lshift_s16 (OrcProgram *p, void *user, OrcInstruction *insn)
{
  x86_emit_mov_reg_reg(p, 4, p->vars[insn->args[2]].alloc, X86_ECX);

  printf("  shlw %%cl, %%%s\n",
      x86_get_regname_16(p->vars[insn->args[0]].alloc));

  *p->codeptr++ = 0x66;
  *p->codeptr++ = 0xd3;
  x86_emit_modrm_reg (p, p->vars[insn->args[0]].alloc, 4);
}

static void
x86_rule_rshift_s16 (OrcProgram *p, void *user, OrcInstruction *insn)
{
  if (insn->rule_flag == ORC_RULE_REG_IMM) {
    printf("  sarw $%d, %%%s\n",
        p->vars[insn->args[2]].s16,
        x86_get_regname_16(p->vars[insn->args[0]].alloc));

    if (p->vars[insn->args[2]].s16 == 1) {
      *p->codeptr++ = 0x66;
      *p->codeptr++ = 0xd1;
      x86_emit_modrm_reg (p, p->vars[insn->args[0]].alloc, 7);
    } else {
      *p->codeptr++ = 0x66;
      *p->codeptr++ = 0xc1;
      x86_emit_modrm_reg (p, p->vars[insn->args[0]].alloc, 7);
      *p->codeptr++ = p->vars[insn->args[2]].s16;
    }
  } else {
    x86_emit_mov_reg_reg(p, 4, p->vars[insn->args[2]].alloc, X86_ECX);

    printf("  sarw %%cl, %%%s\n",
        x86_get_regname_16(p->vars[insn->args[0]].alloc));

    *p->codeptr++ = 0x66;
    *p->codeptr++ = 0xd3;
    x86_emit_modrm_reg (p, p->vars[insn->args[0]].alloc, 7);
  }
}


void
orc_program_x86_register_rules (void)
{
  orc_rule_register ("_loadi_s16", ORC_RULE_SCALAR_1, x86_rule_loadi_s16, NULL,
      ORC_RULE_REG_IMM);

  orc_rule_register ("add_s16", ORC_RULE_SCALAR_1, x86_rule_add_s16, NULL,
      ORC_RULE_REG_REG | ORC_RULE_REG_IMM);
  orc_rule_register ("sub_s16", ORC_RULE_SCALAR_1, x86_rule_sub_s16, NULL,
      ORC_RULE_REG_REG);
  orc_rule_register ("mul_s16", ORC_RULE_SCALAR_1, x86_rule_mul_s16, NULL,
      ORC_RULE_REG_REG);
  orc_rule_register ("lshift_s16", ORC_RULE_SCALAR_1, x86_rule_lshift_s16, NULL,
      ORC_RULE_REG_REG);
  orc_rule_register ("rshift_s16", ORC_RULE_SCALAR_1, x86_rule_rshift_s16, NULL,
      ORC_RULE_REG_REG | ORC_RULE_REG_IMM);
}

/* mmx rules */

static void
mmx_emit_loadi_s16 (OrcProgram *p, int reg, int value)
{
  if (value == 0) {
    printf("  pxor %%%s, %%%s\n", x86_get_regname_mmx(reg),
        x86_get_regname_mmx(reg));
    *p->codeptr++ = 0x0f;
    *p->codeptr++ = 0xef;
    x86_emit_modrm_reg (p, reg, reg);
  } else {
    x86_emit_mov_imm_reg (p, 4, value, X86_ECX);

    printf("  movd %%ecx, %%%s\n", x86_get_regname_mmx(reg));
    *p->codeptr++ = 0x0f;
    *p->codeptr++ = 0x6e;
    x86_emit_modrm_reg (p, X86_ECX, reg);

    printf("  pshufw $0, %%%s, %%%s\n", x86_get_regname_mmx(reg),
        x86_get_regname_mmx(reg));

    *p->codeptr++ = 0x0f;
    *p->codeptr++ = 0x70;
    x86_emit_modrm_reg (p, reg, reg);
    *p->codeptr++ = 0x00;
  }
}

static void
mmx_rule_loadi_s16 (OrcProgram *p, void *user, OrcInstruction *insn)
{
  mmx_emit_loadi_s16 (p, p->vars[insn->args[0]].alloc,
      p->vars[insn->args[2]].s16);
}

static void
mmx_rule_add_s16 (OrcProgram *p, void *user, OrcInstruction *insn)
{
  printf("  paddw %%%s, %%%s\n",
      x86_get_regname_mmx(p->vars[insn->args[2]].alloc),
      x86_get_regname_mmx(p->vars[insn->args[0]].alloc));

  *p->codeptr++ = 0x0f;
  *p->codeptr++ = 0xfd;
  x86_emit_modrm_reg (p, p->vars[insn->args[2]].alloc,
      p->vars[insn->args[0]].alloc);
}

static void
mmx_rule_sub_s16 (OrcProgram *p, void *user, OrcInstruction *insn)
{
  printf("  psubw %%%s, %%%s\n",
      x86_get_regname_mmx(p->vars[insn->args[2]].alloc),
      x86_get_regname_mmx(p->vars[insn->args[0]].alloc));

  *p->codeptr++ = 0x0f;
  *p->codeptr++ = 0xf9;
  x86_emit_modrm_reg (p, p->vars[insn->args[2]].alloc,
      p->vars[insn->args[0]].alloc);
}

static void
mmx_rule_mul_s16 (OrcProgram *p, void *user, OrcInstruction *insn)
{
  printf("  pmullw %%%s, %%%s\n",
      x86_get_regname_mmx(p->vars[insn->args[2]].alloc),
      x86_get_regname_mmx(p->vars[insn->args[0]].alloc));

  *p->codeptr++ = 0x0f;
  *p->codeptr++ = 0xd5;
  x86_emit_modrm_reg (p, p->vars[insn->args[2]].alloc,
      p->vars[insn->args[0]].alloc);
}

static void
mmx_rule_lshift_s16 (OrcProgram *p, void *user, OrcInstruction *insn)
{
  if (p->vars[insn->args[2]].vartype == ORC_VAR_TYPE_CONST) {
    printf("  psllw $%d, %%%s\n",
        p->vars[insn->args[2]].s16,
        x86_get_regname_mmx(p->vars[insn->args[0]].alloc));

    *p->codeptr++ = 0x0f;
    *p->codeptr++ = 0x71;
    x86_emit_modrm_reg (p, p->vars[insn->args[0]].alloc, 6);
    *p->codeptr++ = p->vars[insn->args[2]].s16;
  } else {
    /* FIXME this doesn't work quite right */
    printf("  psllw %%%s, %%%s\n",
        x86_get_regname_mmx(p->vars[insn->args[2]].alloc),
        x86_get_regname_mmx(p->vars[insn->args[0]].alloc));

    *p->codeptr++ = 0x0f;
    *p->codeptr++ = 0xf1;
    x86_emit_modrm_reg (p, p->vars[insn->args[0]].alloc,
        p->vars[insn->args[2]].alloc);
  }
}

static void
mmx_rule_rshift_s16 (OrcProgram *p, void *user, OrcInstruction *insn)
{
  if (p->vars[insn->args[2]].vartype == ORC_VAR_TYPE_CONST) {
    printf("  psraw $%d, %%%s\n",
        p->vars[insn->args[2]].s16,
        x86_get_regname_mmx(p->vars[insn->args[0]].alloc));

    *p->codeptr++ = 0x0f;
    *p->codeptr++ = 0x71;
    x86_emit_modrm_reg (p, p->vars[insn->args[0]].alloc, 4);
    *p->codeptr++ = p->vars[insn->args[2]].s16;
  } else {
    /* FIXME this doesn't work quite right */
    printf("  psraw %%%s, %%%s\n",
        x86_get_regname_mmx(p->vars[insn->args[2]].alloc),
        x86_get_regname_mmx(p->vars[insn->args[0]].alloc));

    *p->codeptr++ = 0x0f;
    *p->codeptr++ = 0xe1;
    x86_emit_modrm_reg (p, p->vars[insn->args[0]].alloc,
        p->vars[insn->args[2]].alloc);
  }
}

void
orc_program_mmx_register_rules (void)
{
  int i;

  orc_rule_register ("_loadi_s16", ORC_RULE_MMX_4, mmx_rule_loadi_s16, NULL,
      ORC_RULE_REG_IMM);

  for(i=ORC_RULE_MMX_1; i <= ORC_RULE_MMX_4; i++) {
    orc_rule_register ("add_s16", i, mmx_rule_add_s16, NULL,
        ORC_RULE_REG_REG);
    orc_rule_register ("sub_s16", i, mmx_rule_sub_s16, NULL,
        ORC_RULE_REG_REG);
    orc_rule_register ("mul_s16", i, mmx_rule_mul_s16, NULL,
        ORC_RULE_REG_REG);
    orc_rule_register ("lshift_s16", i, mmx_rule_lshift_s16, NULL,
        ORC_RULE_REG_REG);
    orc_rule_register ("rshift_s16", i, mmx_rule_rshift_s16, NULL,
        ORC_RULE_REG_REG);
  }
}

/* code generation */

void
x86_emit_push (OrcProgram *program, int size, int reg)
{

  if (size == 1) {
    program->error = 1;
  } else if (size == 2) {
    printf("  pushw %%%s\n", x86_get_regname_16(reg));
    *program->codeptr++ = 0x66;
    *program->codeptr++ = 0x50 + x86_get_regnum(reg);
  } else {
    printf("  pushl %%%s\n", x86_get_regname(reg));
    *program->codeptr++ = 0x50 + x86_get_regnum(reg);
  }
}

void
x86_emit_pop (OrcProgram *program, int size, int reg)
{

  if (size == 1) {
    program->error = 1;
  } else if (size == 2) {
    printf("  popw %%%s\n", x86_get_regname_16(reg));
    *program->codeptr++ = 0x66;
    *program->codeptr++ = 0x58 + x86_get_regnum(reg);
  } else {
    printf("  popl %%%s\n", x86_get_regname(reg));
    *program->codeptr++ = 0x58 + x86_get_regnum(reg);
  }
}

#define X86_MODRM(mod, rm, reg) ((((mod)&3)<<6)|(((rm)&7)<<0)|(((reg)&7)<<3))
#define X86_SIB(ss, ind, reg) ((((ss)&3)<<6)|(((ind)&7)<<3)|((reg)&7))

void
x86_emit_modrm_memoffset (OrcProgram *program, int reg1, int offset, int reg2)
{
  if (offset == 0 && reg2 != x86_exec_ptr) {
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
x86_emit_modrm_reg (OrcProgram *program, int reg1, int reg2)
{
  *program->codeptr++ = X86_MODRM(3, reg1, reg2);
}

void
x86_emit_rex (OrcProgram *program, int size, int reg1, int reg2, int reg3)
{
  int rex = 0x40;

  if (x86_64) {
    if (size >= 8) rex |= 0x08;
    if (reg1 == 1 || (x86_get_regnum(reg1)>=8)) rex |= 0x4;
    if (reg2 == 1 || (x86_get_regnum(reg2)>=8)) rex |= 0x2;
    if (reg3 == 1 || (x86_get_regnum(reg3)>=8)) rex |= 0x1;

    if (rex != 0x40) *program->codeptr++ = rex;
    //*program->codeptr++ = rex;
  }
}

void
x86_emit_mov_memoffset_reg (OrcProgram *program, int size, int offset,
    int reg1, int reg2)
{

  if (size == 2) {
    printf("  movw %d(%%%s), %%%s\n", offset, x86_get_regname_ptr(reg1),
        x86_get_regname_16(reg2));
    *program->codeptr++ = 0x66;
  } else if (size == 4) {
    printf("  movl %d(%%%s), %%%s\n", offset, x86_get_regname_ptr(reg1),
        x86_get_regname(reg2));
  } else {
    printf("  mov %d(%%%s), %%%s\n", offset, x86_get_regname_ptr(reg1),
        x86_get_regname_64(reg2));
  }

  x86_emit_rex(program, size, reg2, 0, reg1);
  *program->codeptr++ = 0x8b;
  x86_emit_modrm_memoffset (program, reg2, offset, reg1);
}

void
x86_emit_mov_memoffset_mmx (OrcProgram *program, int size, int offset,
    int reg1, int reg2)
{
  if (size == 4) {
    printf("  movd %d(%%%s), %%%s\n", offset, x86_get_regname_ptr(reg1),
        x86_get_regname_mmx(reg2));
    x86_emit_rex(program, 0, reg2, 0, reg1);
    *program->codeptr++ = 0x0f;
    *program->codeptr++ = 0x6e;
  } else {
    printf("  movq %d(%%%s), %%%s\n", offset, x86_get_regname_ptr(reg1),
        x86_get_regname_mmx(reg2));
    x86_emit_rex(program, 0, reg2, 0, reg1);
    *program->codeptr++ = 0x0f;
    *program->codeptr++ = 0x6f;
  }
  x86_emit_modrm_memoffset (program, reg2, offset, reg1);
}

void
x86_emit_mov_reg_memoffset (OrcProgram *program, int size, int reg1, int offset,
    int reg2)
{
  if (size == 2) {
    printf("  movw %%%s, %d(%%%s)\n", x86_get_regname_16(reg1), offset,
        x86_get_regname_ptr(reg2));
    *program->codeptr++ = 0x66;
  } else if (size == 4) {
    printf("  movl %%%s, %d(%%%s)\n", x86_get_regname(reg1), offset,
        x86_get_regname_ptr(reg2));
  } else {
    printf("  mov %%%s, %d(%%%s)\n", x86_get_regname(reg1), offset,
        x86_get_regname_ptr(reg2));
  }

  x86_emit_rex(program, size, reg1, 0, reg2);
  *program->codeptr++ = 0x89;
  x86_emit_modrm_memoffset (program, reg1, offset, reg2);
}

void
x86_emit_mov_mmx_memoffset (OrcProgram *program, int size, int reg1, int offset,
    int reg2)
{
  x86_emit_rex(program, 0, reg1, 0, reg2);
  if (size == 4) {
    printf("  movd %%%s, %d(%%%s)\n", x86_get_regname_mmx(reg1), offset,
        x86_get_regname_ptr(reg2));
    *program->codeptr++ = 0x0f;
    *program->codeptr++ = 0x7e;
  } else if (size == 8) {
    printf("  movq %%%s, %d(%%%s)\n", x86_get_regname_mmx(reg1), offset,
        x86_get_regname_ptr(reg2));
    *program->codeptr++ = 0x0f;
    *program->codeptr++ = 0x7f;
  }

  x86_emit_modrm_memoffset (program, reg1, offset, reg2);
}

void
x86_emit_mov_imm_reg (OrcProgram *program, int size, int value, int reg1)
{
  if (size == 2) {
    printf("  movw $%d, %%%s\n", value, x86_get_regname_16(reg1));
    x86_emit_rex(program, size, reg1, 0, 0);
    *program->codeptr++ = 0x66;
    *program->codeptr++ = 0xb8 + x86_get_regnum(reg1);
    *program->codeptr++ = (value & 0xff);
    *program->codeptr++ = ((value>>8) & 0xff);
  } else if (size == 4) {
    printf("  movl $%d, %%%s\n", value, x86_get_regname(reg1));
    x86_emit_rex(program, size, reg1, 0, 0);
    *program->codeptr++ = 0xb8 + x86_get_regnum(reg1);
    *program->codeptr++ = (value & 0xff);
    *program->codeptr++ = ((value>>8) & 0xff);
    *program->codeptr++ = ((value>>16) & 0xff);
    *program->codeptr++ = ((value>>24) & 0xff);
  } else {
    /* FIXME */
  }

}

void x86_emit_mov_reg_reg (OrcProgram *program, int size, int reg1, int reg2)
{
  if (size == 2) {
    printf("  movw %%%s, %%%s\n", x86_get_regname_16(reg1),
        x86_get_regname_16(reg2));
    *program->codeptr++ = 0x66;
  } else if (size == 4) {
    printf("  movl %%%s, %%%s\n", x86_get_regname(reg1),
        x86_get_regname(reg2));
  } else {
    printf("  mov %%%s, %%%s\n", x86_get_regname_64(reg1),
        x86_get_regname_64(reg2));
  }

  x86_emit_rex(program, size, reg2, 0, reg1);
  *program->codeptr++ = 0x89;
  x86_emit_modrm_reg (program, reg2, reg1);
}

void x86_emit_mov_reg_mmx (OrcProgram *program, int reg1, int reg2)
{
  /* FIXME */
  printf("  movd %%%s, %%%s\n", x86_get_regname(reg1),
      x86_get_regname_mmx(reg2));
  x86_emit_rex(program, 0, reg1, 0, reg2);
  *program->codeptr++ = 0x0f;
  *program->codeptr++ = 0x6e;
  x86_emit_modrm_reg (program, reg1, reg2);
}

void x86_emit_mov_mmx_reg (OrcProgram *program, int reg1, int reg2)
{
  /* FIXME */
  printf("  movd %%%s, %%%s\n", x86_get_regname_mmx(reg1),
      x86_get_regname(reg2));
  x86_emit_rex(program, 0, reg2, 0, reg1);
  *program->codeptr++ = 0x0f;
  *program->codeptr++ = 0x7e;
  x86_emit_modrm_reg (program, reg2, reg1);
}

void
x86_emit_test_reg_reg (OrcProgram *program, int size, int reg1, int reg2)
{
  if (size == 2) {
    printf("  testw %%%s, %%%s\n", x86_get_regname_16(reg1),
        x86_get_regname_16(reg2));
    *program->codeptr++ = 0x66;
  } else if (size == 4) {
    printf("  testl %%%s, %%%s\n", x86_get_regname(reg1),
        x86_get_regname(reg2));
  } else {
    printf("  test %%%s, %%%s\n", x86_get_regname(reg1),
        x86_get_regname(reg2));
  }

  x86_emit_rex(program, size, reg2, 0, reg1);
  *program->codeptr++ = 0x85;
  x86_emit_modrm_reg (program, reg2, reg1);
}

void
x86_emit_sar_imm_reg (OrcProgram *program, int size, int value, int reg)
{
  if (size == 2) {
    printf("  sarw $%d, %%%s\n", value, x86_get_regname_16(reg));
  } else if (size == 4) {
    printf("  sarl $%d, %%%s\n", value, x86_get_regname(reg));
  } else {
    printf("  sar $%d, %%%s\n", value, x86_get_regname_64(reg));
  }

  x86_emit_rex(program, size, reg, 0, 0);
  if (value == 1) {
    *program->codeptr++ = 0xd1;
    x86_emit_modrm_reg (program, reg, 7);
  } else {
    *program->codeptr++ = 0xc1;
    x86_emit_modrm_reg (program, reg, 7);
    *program->codeptr++ = value;
  }
}

void
x86_emit_add_imm_memoffset (OrcProgram *program, int size, int value,
    int offset, int reg)
{
  if (size == 2) {
    printf("  addw $%d, %d(%%%s)\n", value, offset,
        x86_get_regname_ptr(reg));
    *program->codeptr++ = 0x66;
  } else if (size == 4) {
    printf("  addl $%d, %d(%%%s)\n", value, offset,
        x86_get_regname_ptr(reg));
  } else {
    printf("  add $%d, %d(%%%s)\n", value, offset,
        x86_get_regname_ptr(reg));
  }

  x86_emit_rex(program, size, 0, 0, reg);
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
x86_emit_add_imm_reg (OrcProgram *program, int size, int value, int reg)
{
  if (size == 2) {
    printf("  addw $%d, %%%s\n", value, x86_get_regname_16(reg));
    *program->codeptr++ = 0x66;
  } else if (size == 4) {
    printf("  addl $%d, %%%s\n", value, x86_get_regname(reg));
  } else {
    printf("  add $%d, %%%s\n", value, x86_get_regname_64(reg));
  }

  x86_emit_rex(program, size, 0, 0, reg);
  if (value >= -128 && value < 128) {
    *program->codeptr++ = 0x83;
    x86_emit_modrm_reg (program, reg, 0);
    *program->codeptr++ = (value & 0xff);
  } else {
    *program->codeptr++ = 0x81;
    x86_emit_modrm_reg (program, reg, 0);
    *program->codeptr++ = (value & 0xff);
    *program->codeptr++ = ((value>>8) & 0xff);
    if (size == 4) {
      *program->codeptr++ = ((value>>16) & 0xff);
      *program->codeptr++ = ((value>>24) & 0xff);
    }
  }
}

void
x86_emit_dec_memoffset (OrcProgram *program, int size,
    int offset, int reg)
{
  if (size == 2) {
    printf("  decw %d(%%%s)\n", offset, x86_get_regname_ptr(reg));
    *program->codeptr++ = 0x66;
  } else if (size == 4) {
    printf("  decl %d(%%%s)\n", offset, x86_get_regname_ptr(reg));
  } else {
    printf("  dec %d(%%%s)\n", offset, x86_get_regname_ptr(reg));
  }

  x86_emit_rex(program, size, 0, 0, reg);
  *program->codeptr++ = 0xff;
  x86_emit_modrm_memoffset (program, 1, offset, reg);
}

void x86_emit_ret (OrcProgram *program)
{
  if (x86_64) {
    printf("  retq\n");
  } else {
    printf("  ret\n");
  }
  *program->codeptr++ = 0xc3;
}

void x86_emit_emms (OrcProgram *program)
{
  printf("  emms\n");
  *program->codeptr++ = 0x0f;
  *program->codeptr++ = 0x77;
}

void
x86_add_fixup (OrcProgram *program, unsigned char *ptr, int label)
{
  program->fixups[program->n_fixups].ptr = ptr;
  program->fixups[program->n_fixups].label = label;
  program->fixups[program->n_fixups].type = 0;
  program->n_fixups++;
}

void
x86_add_label (OrcProgram *program, unsigned char *ptr, int label)
{
  program->labels[label] = ptr;
}

void x86_emit_je (OrcProgram *program, int label)
{
  printf("  je .L%d\n", label);

  *program->codeptr++ = 0x74;
  x86_add_fixup (program, program->codeptr, label);
  *program->codeptr++ = -1;
}

void x86_emit_jne (OrcProgram *program, int label)
{
  printf("  jne .L%d\n", label);
  *program->codeptr++ = 0x75;
  x86_add_fixup (program, program->codeptr, label);
  *program->codeptr++ = -1;
}

void x86_emit_label (OrcProgram *program, int label)
{
  printf(".L%d:\n", label);

  x86_add_label (program, program->codeptr, label);
}

void
x86_test (OrcProgram *program)
{
  int size;
  int i;
  int j;
  int reg;

  for(size=2;size<=4;size+=2) {
    for(i=0;i<8;i++){
      reg = ORC_GP_REG_BASE + i;
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
        int reg2 = ORC_GP_REG_BASE + j;
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



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

#if 0
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
#endif
void powerpc_emit_ret (OrcProgram *program);
void powerpc_emit_beq (OrcProgram *program, int label);
void powerpc_emit_bne (OrcProgram *program, int label);
void powerpc_emit_label (OrcProgram *program, int label);

#if 0
static void mmx_emit_loadi_s16 (OrcProgram *p, int reg, int value);

void x86_emit_modrm_memoffset (OrcProgram *program, int reg1, int offset, int reg2);
void x86_emit_modrm_reg (OrcProgram *program, int reg1, int reg2);
void x86_test (OrcProgram *program);
#endif

void orc_program_powerpc_register_rules (void);

enum {
  POWERPC_R0 = ORC_GP_REG_BASE,
  POWERPC_R1,
  POWERPC_R2,
  POWERPC_R3,
  POWERPC_R4,
  POWERPC_R5,
  POWERPC_R6,
  POWERPC_R7,
  POWERPC_R8,
  POWERPC_R9,
  POWERPC_R10,
  POWERPC_R11,
  POWERPC_R12,
  POWERPC_R13,
  POWERPC_R14,
  POWERPC_R15,
  POWERPC_R16,
  POWERPC_R17,
  POWERPC_R18,
  POWERPC_R19,
  POWERPC_R20,
  POWERPC_R21,
  POWERPC_R22,
  POWERPC_R23,
  POWERPC_R24,
  POWERPC_R25,
  POWERPC_R26,
  POWERPC_R27,
  POWERPC_R28,
  POWERPC_R29,
  POWERPC_R30,
  POWERPC_R31
};

const char *
powerpc_get_regname(int i)
{
  static const char *powerpc_regs[] = {
    "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9",
    "r10", "r11", "r12", "r13", "r14", "r15", "r16", "r17", "r18", "r19",
    "r20", "r21", "r22", "r23", "r24", "r25", "r26", "r27", "r28", "r29",
    "r30", "r31",
    "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7", "v8", "v9",
    "v10", "v11", "v12", "v13", "v14", "v15", "v16", "v17", "v18", "v19",
    "v20", "v21", "v22", "v23", "v24", "v25", "v26", "v27", "v28", "v29",
    "v30", "v31",
  };

  if (i>=ORC_GP_REG_BASE && i<ORC_GP_REG_BASE + 64) {
    return powerpc_regs[i - ORC_GP_REG_BASE];
  }
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
orc_program_powerpc_allocate_register (OrcProgram *program, int data_reg)
{
  int i;

  if (!data_reg) {
    for(i=ORC_GP_REG_BASE;i<ORC_GP_REG_BASE+32;i++){
      if (program->alloc_regs[i] == 0) {
        program->alloc_regs[i]++;
        program->used_regs[i] = 1;
        return i;
      }
    }
  } else {
    for(i=ORC_GP_REG_BASE+32;i<ORC_GP_REG_BASE+64;i++){
      if (program->alloc_regs[i] == 0) {
        program->alloc_regs[i]++;
        program->used_regs[i] = 1;
        return i;
      }
    }
  }
  g_print("register overflow\n");
  return 0;
}


void orc_program_rewrite_vars (OrcProgram *program);
void orc_program_dump (OrcProgram *program);

void
powerpc_emit_prologue (OrcProgram *program)
{
  int i;

  printf (".global test\n");
  printf ("test:\n");

  printf ("  stwu %s,-16(%s)\n", 
      powerpc_get_regname(POWERPC_R1),
      powerpc_get_regname(POWERPC_R1));

  for(i=POWERPC_R13;i<=POWERPC_R31;i++){
    if (program->used_regs[i]) {
      //powerpc_emit_push (program, 4, i);
    }
  }
}

void
powerpc_emit_epilogue (OrcProgram *program)
{
  int i;

  for(i=POWERPC_R31;i>=POWERPC_R31;i--){
    if (program->used_regs[i]) {
      //powerpc_emit_pop (program, 4, i);
    }
  }

  printf("  addi %s, %s, 16\n",
      powerpc_get_regname(POWERPC_R1),
      powerpc_get_regname(POWERPC_R1));
  printf("  blr\n");
}

void
powerpc_do_fixups (OrcProgram *program)
{
#if 0
  int i;
  for(i=0;i<program->n_fixups;i++){
    if (program->fixups[i].type == 0) {
      unsigned char *label = program->labels[program->fixups[i].label];
      unsigned char *ptr = program->fixups[i].ptr;

      ptr[0] += label - ptr;
    }
  }
#endif
}

void
orc_powerpc_init (void)
{
  orc_program_powerpc_register_rules ();
}

void
orc_program_powerpc_reset_alloc (OrcProgram *program)
{
  int i;

  for(i=ORC_GP_REG_BASE;i<ORC_GP_REG_BASE+64;i++){
    program->alloc_regs[i] = 0;
  }
  program->alloc_regs[POWERPC_R0] = 1;
  program->alloc_regs[POWERPC_R1] = 1;
  program->alloc_regs[POWERPC_R3] = 1;
}

void
powerpc_load_constants (OrcProgram *program)
{
  int i;
  for(i=0;i<program->n_vars;i++){
    switch (program->vars[i].vartype) {
      case ORC_VAR_TYPE_CONST:
        printf("  vspltish %s, %d\n",
            powerpc_get_regname(program->vars[i].alloc),
            program->vars[i].s16);
        break;
      case ORC_VAR_TYPE_SRC:
      case ORC_VAR_TYPE_DEST:
        if (program->vars[i].ptr_register) {
          printf("  ldw %s, %d(%s)\n",
              powerpc_get_regname(program->vars[i].ptr_register),
              (int)G_STRUCT_OFFSET(OrcExecutor, arrays[i]),
              powerpc_get_regname(POWERPC_R3));
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
powerpc_emit_load_src (OrcProgram *program, OrcVariable *var)
{
  int ptr_reg;
  ptr_reg = var->ptr_register;

  switch (program->rule_set) {
    case ORC_RULE_ALTIVEC_1:
      printf("  lvehx %s, 0(%s)\n", 
          powerpc_get_regname (var->alloc),
          powerpc_get_regname (ptr_reg));
      break;
    default:
      printf("ERROR\n");
  }
}

void
powerpc_emit_store_dest (OrcProgram *program, OrcVariable *var)
{
  int ptr_reg;
  ptr_reg = var->ptr_register;

  switch (program->rule_set) {
    case ORC_RULE_ALTIVEC_1:
      printf("  stvehx %s, 0(%s)\n", 
          powerpc_get_regname (var->alloc),
          powerpc_get_regname (ptr_reg));
      break;
    default:
      printf("ERROR\n");
  }
}

void
orc_program_assemble_powerpc (OrcProgram *program)
{
  int j;
  int k;
  OrcInstruction *insn;
  OrcOpcode *opcode;
  OrcVariable *args[10];
  OrcRule *rule;

  powerpc_emit_prologue (program);

  printf("  ldw. %s, %d(%s)\n",
      powerpc_get_regname(POWERPC_R0),
      (int)G_STRUCT_OFFSET(OrcExecutor, n),
      powerpc_get_regname(POWERPC_R3));
  if (program->loop_shift != 0) {
    printf("  srawi. %s, %s, %d\n",
        powerpc_get_regname(POWERPC_R0),
        powerpc_get_regname(POWERPC_R0),
        program->loop_shift);
  }

  powerpc_emit_beq (program, 1);
  printf ("  mtctr %s\n", powerpc_get_regname(POWERPC_R0));

  powerpc_load_constants (program);

  powerpc_emit_label (program, 0);

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
          powerpc_emit_load_src (program, args[k]);
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
      rule->emit (program, rule->emit_user, insn);
    } else {
      printf("No rule for: %s\n", opcode->name);
    }

    for(k=0;k<opcode->n_dest;k++){
      switch (args[k]->vartype) {
        case ORC_VAR_TYPE_DEST:
          powerpc_emit_store_dest (program, args[k]);
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
        printf("  addi %s, %s, %d\n",
            powerpc_get_regname(program->vars[k].ptr_register),
            powerpc_get_regname(program->vars[k].ptr_register),
            orc_variable_get_size(program->vars + k) << program->loop_shift);
      } else {
        printf("ERROR\n");
      }
    }
  }

  powerpc_emit_bne (program, 0);
  powerpc_emit_label (program, 1);

  powerpc_emit_epilogue (program);

  powerpc_do_fixups (program);
}


/* rules */

static void
powerpc_rule_add_s16 (OrcProgram *p, void *user, OrcInstruction *insn)
{
  printf("  vadduhm %s, %s, %s\n",
      powerpc_get_regname(p->vars[insn->args[0]].alloc),
      powerpc_get_regname(p->vars[insn->args[1]].alloc),
      powerpc_get_regname(p->vars[insn->args[2]].alloc));
}

static void
powerpc_rule_sub_s16 (OrcProgram *p, void *user, OrcInstruction *insn)
{
  printf("  vsubuhm %s, %s, %s\n",
      powerpc_get_regname(p->vars[insn->args[0]].alloc),
      powerpc_get_regname(p->vars[insn->args[1]].alloc),
      powerpc_get_regname(p->vars[insn->args[2]].alloc));
}

static void
powerpc_rule_mul_s16 (OrcProgram *p, void *user, OrcInstruction *insn)
{
  printf("  vmladduhm %s, %s, %s, vzero\n",
      powerpc_get_regname(p->vars[insn->args[0]].alloc),
      powerpc_get_regname(p->vars[insn->args[1]].alloc),
      powerpc_get_regname(p->vars[insn->args[2]].alloc));
}

static void
powerpc_rule_lshift_s16 (OrcProgram *p, void *user, OrcInstruction *insn)
{
  printf("  vrlh %s, %s, %s\n",
      powerpc_get_regname(p->vars[insn->args[0]].alloc),
      powerpc_get_regname(p->vars[insn->args[1]].alloc),
      powerpc_get_regname(p->vars[insn->args[2]].alloc));
}

static void
powerpc_rule_rshift_s16 (OrcProgram *p, void *user, OrcInstruction *insn)
{
  printf("  vsrah %s, %s, %s\n",
      powerpc_get_regname(p->vars[insn->args[0]].alloc),
      powerpc_get_regname(p->vars[insn->args[1]].alloc),
      powerpc_get_regname(p->vars[insn->args[2]].alloc));
}


void
orc_program_powerpc_register_rules (void)
{
  orc_rule_register ("add_s16", ORC_RULE_ALTIVEC_1, powerpc_rule_add_s16, NULL,
      ORC_RULE_REG_REG);
  orc_rule_register ("sub_s16", ORC_RULE_ALTIVEC_1, powerpc_rule_sub_s16, NULL,
      ORC_RULE_REG_REG);
  orc_rule_register ("mul_s16", ORC_RULE_ALTIVEC_1, powerpc_rule_mul_s16, NULL,
      ORC_RULE_REG_REG);
  orc_rule_register ("lshift_s16", ORC_RULE_ALTIVEC_1, powerpc_rule_lshift_s16, NULL,
      ORC_RULE_REG_REG);
  orc_rule_register ("rshift_s16", ORC_RULE_ALTIVEC_1, powerpc_rule_rshift_s16, NULL,
      ORC_RULE_REG_REG);
}

/* code generation */

#if 0
void
x86_emit_push (OrcProgram *program, int size, int reg)
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
x86_emit_pop (OrcProgram *program, int size, int reg)
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
x86_emit_modrm_memoffset (OrcProgram *program, int reg1, int offset, int reg2)
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
x86_emit_modrm_reg (OrcProgram *program, int reg1, int reg2)
{
  *program->codeptr++ = X86_MODRM(3, reg1, reg2);
}

void
x86_emit_mov_memoffset_reg (OrcProgram *program, int size, int offset,
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
x86_emit_mov_memoffset_mmx (OrcProgram *program, int size, int offset,
    int reg1, int reg2)
{
  if (size == 4) {
    g_print("  movd %d(%%%s), %%%s\n", offset, x86_get_regname(reg1),
        x86_get_regname_mmx(reg2));
    *program->codeptr++ = 0x0f;
    *program->codeptr++ = 0x6e;
  } else {
    g_print("  movq %d(%%%s), %%%s\n", offset, x86_get_regname(reg1),
        x86_get_regname_mmx(reg2));
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
x86_emit_mov_mmx_memoffset (OrcProgram *program, int size, int reg1, int offset,
    int reg2)
{
  if (size == 4) {
    g_print("  movd %%%s, %d(%%%s)\n", x86_get_regname_mmx(reg1), offset,
        x86_get_regname(reg2));
    *program->codeptr++ = 0x0f;
    *program->codeptr++ = 0x7e;
  } else {
    g_print("  movq %%%s, %d(%%%s)\n", x86_get_regname_mmx(reg1), offset,
        x86_get_regname(reg2));
    *program->codeptr++ = 0x0f;
    *program->codeptr++ = 0x7f;
  }

  x86_emit_modrm_memoffset (program, reg1, offset, reg2);
}

void
x86_emit_mov_imm_reg (OrcProgram *program, int size, int value, int reg1)
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

void x86_emit_mov_reg_reg (OrcProgram *program, int size, int reg1, int reg2)
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

void x86_emit_mov_reg_mmx (OrcProgram *program, int reg1, int reg2)
{
  /* FIXME */
  g_print("  movd %%%s, %%%s\n", x86_get_regname(reg1),
      x86_get_regname_mmx(reg2));
  *program->codeptr++ = 0x0f;
  *program->codeptr++ = 0x6e;
  x86_emit_modrm_reg (program, reg1, reg2);
}

void x86_emit_mov_mmx_reg (OrcProgram *program, int reg1, int reg2)
{
  /* FIXME */
  g_print("  movd %%%s, %%%s\n", x86_get_regname_mmx(reg1),
      x86_get_regname(reg2));
  *program->codeptr++ = 0x0f;
  *program->codeptr++ = 0x7e;
  x86_emit_modrm_reg (program, reg2, reg1);
}

void
x86_emit_test_reg_reg (OrcProgram *program, int size, int reg1, int reg2)
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
x86_emit_sar_imm_reg (OrcProgram *program, int size, int value, int reg)
{
  g_print("  sarl $%d, %%%s\n", value, x86_get_regname(reg));

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
x86_emit_add_imm_reg (OrcProgram *program, int size, int value, int reg)
{
  if (size == 2) {
    g_print("  addw $%d, %%%s\n", value, x86_get_regname_16(reg));
    *program->codeptr++ = 0x66;
  } else {
    g_print("  addl $%d, %%%s\n", value, x86_get_regname(reg));
  }

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
    g_print("  decw %d(%%%s)\n", offset, x86_get_regname(reg));
    *program->codeptr++ = 0x66;
  } else {
    g_print("  decl %d(%%%s)\n", offset, x86_get_regname(reg));
  }

  *program->codeptr++ = 0xff;
  x86_emit_modrm_memoffset (program, 1, offset, reg);
}
#endif

void powerpc_emit_ret (OrcProgram *program)
{
  g_print("  ret\n");
  //*program->codeptr++ = 0xc3;
}

#if 0
void
x86_add_fixup (OrcProgram *program, unsigned char *ptr, int label)
{
  program->fixups[program->n_fixups].ptr = ptr;
  program->fixups[program->n_fixups].label = label;
  program->fixups[program->n_fixups].type = 0;
  program->n_fixups++;
}
#endif

void
powerpc_add_label (OrcProgram *program, unsigned char *ptr, int label)
{
  program->labels[label] = ptr;
}

void powerpc_emit_beq (OrcProgram *program, int label)
{
  g_print("  ble- .L%d\n", label);

#if 0
  *program->codeptr++ = 0x74;
  x86_add_fixup (program, program->codeptr, label);
  *program->codeptr++ = -1;
#endif
}

void powerpc_emit_bne (OrcProgram *program, int label)
{
  g_print("  bdnz+ .L%d\n", label);
#if 0
  *program->codeptr++ = 0x75;
  x86_add_fixup (program, program->codeptr, label);
  *program->codeptr++ = -1;
#endif
}

void powerpc_emit_label (OrcProgram *program, int label)
{
  g_print(".L%d:\n", label);

  powerpc_add_label (program, program->codeptr, label);
}


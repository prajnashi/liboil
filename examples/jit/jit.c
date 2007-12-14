
#include "config.h"

#include <glib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


void
stuff (void *d, void *s1, void *s2)
{

  __asm__ __volatile__ (
      "  movq 0(%[s1]), %%mm0 \n"
      "  movq 0(%[s2]), %%mm1 \n"
      "  paddw %%mm1, %%mm0 \n"
      "  movq %%mm0, 0(%[d]) \n"
      "  emms \n"
      : [d] "+r" (d), [s1] "+r" (s1), [s2] "+r" (s2));
}


typedef struct _OJArgument {
  int type;
  int index;
  int offset;
} OJArgument;

typedef struct _OJInstruction {
  int opcode;

  OJArgument args[3];
} OJInstruction;

typedef struct _OJRegister {
  int type;

  int index;
}OJRegister;

typedef struct _OJProgram {
  OJInstruction insns[100];
  int n_insns;

  /* parsing state */
  char *s;
  char *error;

  OJInstruction *insn;

}OJProgram;

typedef struct _OJOpcode {
  char *name;
  int n_args;
} OJOpcode;


const OJOpcode opcode_list[] = {
  { "load", 2 },
  { "store", 2 },
  { "add", 3 }
};
#define N_OPCODES 3

#define ARG_REG 0
#define ARG_SRC 1
#define ARG_DEST 2


OJProgram *
oj_program_new (void)
{
  OJProgram *p;
  p = g_malloc0(sizeof(OJProgram));
  return p;
}

int
oj_opcode_lookup (const char *s, int len)
{
  int i;
  for(i=0;i<N_OPCODES;i++){
    if (strlen (opcode_list[i].name) != len) continue;
    if (strncmp (opcode_list[i].name, s, len) == 0) {
      return i;
    }
  }

  return -1;
}

static gboolean
get_opcode (OJProgram *p)
{
  char *s;
  char *opcode;
  int opcode_len;
  int i;

  if (p->error) return FALSE;

  g_print("looking for opcode at \"%s\"\n", p->s);

  s = p->s;
  while (g_ascii_isspace (s[0])) s++;
  opcode = s;
  while (g_ascii_isalnum (s[0])) s++;
  opcode_len = s - opcode;

  if (opcode_len == 0) {
    p->error = g_strdup ("expected opcode");
    return FALSE;
  }

  p->insn->opcode = oj_opcode_lookup (opcode, opcode_len);

  for(i=0;i<N_OPCODES;i++){
    if (strlen (opcode_list[i].name) != opcode_len) continue;
    if (strncmp (opcode_list[i].name, opcode, opcode_len) == 0) {
      break;
    }
  }
  if (i == N_OPCODES) {
    p->error = g_strdup ("unknown opcode");
    return FALSE;
  }

  p->insn->opcode = i;
  p->s = s;
  return TRUE;
}

static gboolean
get_arg (OJProgram *p, int i)
{
  char *s;
  char *end;

  if (p->error) return FALSE;

  g_print("looking for arg at \"%s\"\n", p->s);

  s = p->s;
  while (g_ascii_isspace (s[0])) s++;

  switch (s[0]) {
    case 'r':
      p->insn->args[i].type = ARG_REG;
      break;
    case 's':
      p->insn->args[i].type = ARG_SRC;
      break;
    case 'd':
      p->insn->args[i].type = ARG_DEST;
      break;
    default:
      p->s = s;
      p->error = g_strdup ("expected argument");
      return FALSE;
  }
  s++;

  p->insn->args[i].index = strtoul (s, &end, 10);
  if (s == end) {
    p->s = s;
    p->error = strdup ("expected number");
    return FALSE;
  }

  s = end;

  p->s = s;
  return TRUE;
}

static gboolean
skip_comma (OJProgram *p)
{
  char *s;

  if (p->error) return FALSE;
  
  g_print("looking for comma at \"%s\"\n", p->s);

  s = p->s;
  while (g_ascii_isspace (s[0])) s++;
  if (s[0] != ',') {
    p->error = g_strdup ("expected comma");
    return FALSE;
  }
  s++;
  while (g_ascii_isspace (s[0])) s++;

  p->s = s;
  return TRUE;
}

void
oj_program_parse (OJProgram *p, const char *program)
{
  char **lines;
  char *line;
  char *s;
  int i;

  lines = g_strsplit(program, "\n", 0);

  for(i=0;lines[i];i++){
    p->insn = p->insns + p->n_insns;
    line = lines[i];

    s = line;

    g_print("looking at \"%s\"\n", s);

    while (g_ascii_isspace (s[0])) s++;
    if (s[0] == 0 || s[0] == '#') continue;

    p->s = s;
    get_opcode (p);

    get_arg (p, 0);
    if (opcode_list[p->insn->opcode].n_args >= 2) {
      skip_comma (p);
      get_arg (p, 1);
      if (opcode_list[p->insn->opcode].n_args >= 3) {
        skip_comma (p);
        get_arg (p, 2);
      }
    }

    if (p->error) {
      g_print("error on line %d: %s at \"%s\"\n", i, p->error, p->s);
      g_free(p->error);
      p->error = NULL;
    }

    p->n_insns++;
  }
  g_strfreev (lines);
}

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

  if (regmem->type != ARG_REG) {
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


const char *program =
"load r0, s1\n"
"load r1, s2\n"
"add r2, r0, r1\n"
"store d1, r2\n";

int
main (int argc, char *argv[])
{
  OJProgram *p;

  p = oj_program_new ();
  oj_program_parse (p, program);
  oj_program_output_mmx (p);

  return 0;
}



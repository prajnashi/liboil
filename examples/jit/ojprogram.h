
#ifndef _OJ_PROGRAM_H_
#define _OJ_PROGRAM_H_

#include <glib.h>


typedef struct _OJVariable {
  char *name;
} OJVariable;

typedef struct _OJType {
  char *name;
  int size;
} OJType;

typedef struct _OJArgument {
  OJVariable *var;
  int is_indirect;
  int is_indexed;
  OJVariable *index_var;
  int index_scale;
  int type; // remove
  int index; // remove
  int offset;
} OJArgument;

typedef struct _OJInstruction {
  int opcode;

  OJArgument args[3];
} OJInstruction;

typedef struct _OJProgram {
  OJInstruction insns[100];
  int n_insns;

  OJVariable vars[100];
  int n_vars;

  /* parsing state */
  char *s;
  char *error;

  OJInstruction *insn;

}OJProgram;

typedef struct _OJState {
  int index;

  int args[4];

}OJState;

typedef void (*OJOpcodeEmulateFunc)(OJState *state, void *user);

typedef struct _OJOpcode {
  char *name;
  int n_src;
  int n_dest;
  OJType *arg_types[10];

  OJOpcodeEmulateFunc emulate;
  void *emulate_user;
} OJOpcode;


OJProgram * oj_program_new (void);
int oj_opcode_lookup (const char *s, int len);

void oj_program_append (OJProgram *p, const char *opcode, int arg0, int arg1, int arg2);

void oj_program_output_mmx (OJProgram *p);
void oj_program_free (OJProgram *program);

int oj_program_add_temporary (OJProgram *program, OJType *type);
int oj_program_add_source (OJProgram *program, const char *type);
int oj_program_add_destination (OJProgram *program, const char *type);
int oj_program_add_constant (OJProgram *program, OJType *type, int value);
int oj_program_add_parameter (OJProgram *program, OJType *type, int value);
void oj_program_append (OJProgram *program, const char *opcode, int arg0,
    int arg1, int arg2);



OJType * oj_type_get (const char *name);
void oj_type_register (const char *name, int size);


#endif


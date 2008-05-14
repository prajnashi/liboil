
#ifndef _OJ_PROGRAM_H_
#define _OJ_PROGRAM_H_

#include <glib.h>

typedef struct _OJType OJType;
typedef struct _OJExecutor OJExecutor;
typedef struct _OJVariable OJVariable;
typedef struct _OJOpcode OJOpcode;
typedef struct _OJArgument OJArgument;
typedef struct _OJInstruction OJInstruction;
typedef struct _OJProgram OJProgram;
typedef struct _OJRegister OJRegister;

typedef void (*OJOpcodeEmulateFunc)(OJExecutor *ex, void *user);

struct _OJType {
  char *name;
  int size;
};

typedef enum {
  OJ_VAR_TYPE_TEMP,
  OJ_VAR_TYPE_SRC,
  OJ_VAR_TYPE_DEST,
  OJ_VAR_TYPE_CONST,
  OJ_VAR_TYPE_PARAM
} OJVarType;

struct _OJVariable {
  char *name;

  OJType *type;
  OJVarType vartype;

  int used;
  int first_use;
  int last_use;
  int replaced;
  int replacement;

  int16_t s16;
};


struct _OJOpcode {
  char *name;
  int n_src;
  int n_dest;
  OJType *arg_types[10];

  OJOpcodeEmulateFunc emulate;
  void *emulate_user;
};

struct _OJArgument {
  OJVariable *var;
  int is_indirect;
  int is_indexed;
  OJVariable *index_var;
  int index_scale;
  int type; // remove
  int index; // remove
  int offset;
};

struct _OJInstruction {
  OJOpcode *opcode;
  int args[3];
};

struct _OJRegister {
  int var;

  int first_use;
  int last_use;
  int retired;

  int alloc;
};

struct _OJProgram {
  OJInstruction insns[100];
  int n_insns;

  OJVariable vars[100];
  int n_vars;

  OJInstruction *insn;

  OJRegister regs[100];
  int n_regs;
};

struct _OJExecutor {
  OJProgram *program;
  int n;
  int counter;

  void *arrays[10];

  OJVariable vars[10];
  OJVariable *args[4];

};

OJProgram * oj_program_new (void);
OJOpcode * oj_opcode_find_by_name (const char *name);
void oj_opcode_init (void);

void oj_program_append (OJProgram *p, const char *opcode, int arg0, int arg1, int arg2);

void oj_program_compile_x86 (OJProgram *p);
void oj_program_free (OJProgram *program);

int oj_program_add_temporary (OJProgram *program, const char *type, const char *name);
int oj_program_dup_temporary (OJProgram *program, int i, int j);
int oj_program_add_source (OJProgram *program, const char *type, const char *name);
int oj_program_add_destination (OJProgram *program, const char *type, const char *name);
int oj_program_add_constant (OJProgram *program, const char *type, int value, const char *name);
void oj_program_append (OJProgram *program, const char *opcode, int arg0,
    int arg1, int arg2);


OJType * oj_type_get (const char *name);
void oj_type_register (const char *name, int size);

OJExecutor * oj_executor_new (OJProgram *program);
void oj_executor_free (OJExecutor *ex);
void oj_executor_set_array (OJExecutor *ex, int var, void *ptr);
void oj_executor_set_n (OJExecutor *ex, int n);
void oj_executor_emulate (OJExecutor *ex);



#endif


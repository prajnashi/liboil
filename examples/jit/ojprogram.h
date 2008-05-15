
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
typedef struct _OJRule OJRule;
typedef struct _OJRuleList OJRuleList;

typedef void (*OJOpcodeEmulateFunc)(OJExecutor *ex, void *user);
typedef void (*OJRuleEmitFunc)(OJProgram *p, void *user, OJInstruction *insn);

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

  int alloc;

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

  OJRule *rule;
};

#if 0
struct _OJRegister {
  int var;

  int first_use;
  int last_use;
  int retired;

  int alloc;
};
#endif

struct _OJProgram {
  OJInstruction insns[100];
  int n_insns;

  OJVariable vars[100];
  int n_vars;

  OJInstruction *insn;
};

struct _OJExecutor {
  OJProgram *program;
  int n;
  int counter;

  void *arrays[10];

  OJVariable vars[10];
  OJVariable *args[4];

};

#define OJ_RULE_MUST_CHAIN_SRC1   0x0001
#define OJ_RULE_MAY_CHAIN_SRC1    0x0002
#define OJ_RULE_SYMMETRIC_SRC     0x0004
#define OJ_RULE_SRC2_IS_CL        0x0008

struct _OJRule {
  OJOpcode *opcode;

  unsigned int flags;

  OJRuleEmitFunc emit;
  void *emit_user;
};

struct _OJRuleList {
  int n_alloc;
  int n_rules;
  OJRule *rules;
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


OJRuleList *oj_rule_list_new(void);
void oj_rule_list_free (OJRuleList *rule_list);
void oj_rule_list_register (OJRuleList *rule_list, const char *op_name,
    OJRuleEmitFunc emit, void *emit_user, unsigned int flags);
OJRule * oj_rule_list_get (OJRuleList *rule_list, OJOpcode *opcode);
void oj_program_x86_register_rules (OJRuleList *rule_list);

#endif


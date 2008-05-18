
#ifndef _ORC_PROGRAM_H_
#define _ORC_PROGRAM_H_

#include <glib.h>

typedef struct _OrcType OrcType;
typedef struct _OrcExecutor OrcExecutor;
typedef struct _OrcVariable OrcVariable;
typedef struct _OrcOpcode OrcOpcode;
typedef struct _OrcArgument OrcArgument;
typedef struct _OrcInstruction OrcInstruction;
typedef struct _OrcProgram OrcProgram;
typedef struct _OrcRule OrcRule;
typedef struct _OrcRuleList OrcRuleList;
typedef struct _OrcFixup OrcFixup;

typedef void (*OrcOpcodeEmulateFunc)(OrcExecutor *ex, void *user);
typedef void (*OrcRuleEmitFunc)(OrcProgram *p, void *user, OrcInstruction *insn);

#define ORC_N_REGS 64

struct _OrcType {
  char *name;
  int size;
};

typedef enum {
  ORC_VAR_TYPE_TEMP,
  ORC_VAR_TYPE_SRC,
  ORC_VAR_TYPE_DEST,
  ORC_VAR_TYPE_CONST,
  ORC_VAR_TYPE_PARAM
} OrcVarType;

struct _OrcVariable {
  char *name;

  OrcType *type;
  OrcVarType vartype;

  int used;
  int first_use;
  int last_use;
  int replaced;
  int replacement;

  int alloc;
  int is_chained;

  int16_t s16;
};


struct _OrcOpcode {
  char *name;
  int n_src;
  int n_dest;
  OrcType *arg_types[10];

  OrcOpcodeEmulateFunc emulate;
  void *emulate_user;
};

struct _OrcArgument {
  OrcVariable *var;
  int is_indirect;
  int is_indexed;
  OrcVariable *index_var;
  int index_scale;
  int type; // remove
  int index; // remove
  int offset;
};

struct _OrcInstruction {
  OrcOpcode *opcode;
  int args[3];

  OrcRule *rule;
  unsigned int rule_flag;
};

struct _OrcFixup {
  unsigned char *ptr;
  int type;
  int label;
};

struct _OrcProgram {
  OrcInstruction insns[100];
  int n_insns;

  OrcVariable vars[100];
  int n_vars;

  OrcInstruction *insn;

  unsigned char *code;
  void *code_exec;
  unsigned char *codeptr;
  int code_size;
  
  OrcFixup fixups[100];
  int n_fixups;
  unsigned char *labels[100];

  int error;

  int used_regs[ORC_N_REGS];
  int alloc_regs[ORC_N_REGS];
};

struct _OrcExecutor {
  OrcProgram *program;
  int n;
  int counter;

  void *arrays[10];

  OrcVariable vars[10];
  OrcVariable *args[4];

};

enum {
  ORC_RULE_3REG = (1<<0),
  ORC_RULE_REG_REG = (1<<1),
  ORC_RULE_MEM_REG = (1<<2),
  ORC_RULE_REG_MEM = (1<<3),
  ORC_RULE_REG_IMM = (1<<4),
  ORC_RULE_MEM_IMM = (1<<5),
  ORC_RULE_REG_CL = (1<<6)
};

struct _OrcRule {
  OrcOpcode *opcode;

  unsigned int flags;

  OrcRuleEmitFunc emit;
  void *emit_user;
};

struct _OrcRuleList {
  int n_alloc;
  int n_rules;
  OrcRule *rules;
};

#define ORC_GP_REG_BASE 8

void orc_init (void);

OrcProgram * orc_program_new (void);
OrcOpcode * orc_opcode_find_by_name (const char *name);
void orc_opcode_init (void);

void orc_program_append (OrcProgram *p, const char *opcode, int arg0, int arg1, int arg2);

void orc_x86_init (void);

void orc_program_compile_x86 (OrcProgram *p);
void orc_program_free (OrcProgram *program);

int orc_program_add_temporary (OrcProgram *program, const char *type, const char *name);
int orc_program_dup_temporary (OrcProgram *program, int i, int j);
int orc_program_add_source (OrcProgram *program, const char *type, const char *name);
int orc_program_add_destination (OrcProgram *program, const char *type, const char *name);
int orc_program_add_constant (OrcProgram *program, const char *type, int value, const char *name);
void orc_program_append (OrcProgram *program, const char *opcode, int arg0,
    int arg1, int arg2);

void orc_program_reset_alloc (OrcProgram *program);


OrcType * orc_type_get (const char *name);
void orc_type_register (const char *name, int size);

OrcExecutor * orc_executor_new (OrcProgram *program);
void orc_executor_free (OrcExecutor *ex);
void orc_executor_set_array (OrcExecutor *ex, int var, void *ptr);
void orc_executor_set_n (OrcExecutor *ex, int n);
void orc_executor_emulate (OrcExecutor *ex);
void orc_executor_run (OrcExecutor *ex);


OrcRuleList *orc_rule_list_new(void);
void orc_rule_list_free (OrcRuleList *rule_list);
void orc_rule_list_register (OrcRuleList *rule_list, const char *op_name,
    OrcRuleEmitFunc emit, void *emit_user, unsigned int flags);
OrcRule * orc_rule_list_get (OrcRuleList *rule_list, OrcOpcode *opcode);
void orc_program_x86_register_rules (OrcRuleList *rule_list);
void orc_program_allocate_codemem (OrcProgram *program);
void orc_program_dump_code (OrcProgram *program);
 
extern OrcRuleList *orc_x86_list;

#endif



#include "config.h"

#include <glib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ojprogram.h"


OJProgram *
oj_program_new (void)
{
  OJProgram *p;
  p = g_malloc0(sizeof(OJProgram));
  return p;
}

int
oj_program_add_temporary (OJProgram *program, OJType *type)
{

  return 0;
}

int
oj_program_add_source (OJProgram *program, const char *type)
{

  return 0;
}

int
oj_program_add_destination (OJProgram *program, const char *type)
{

  return 0;
}

int
oj_program_add_constant (OJProgram *program, OJType *type, int value)
{

  return 0;
}

int
oj_program_add_parameter (OJProgram *program, OJType *type, int value)
{

  return 0;
}

void
oj_program_append (OJProgram *program, const char *opcode, int arg0,
    int arg1, int arg2)
{


}



OJType *types;
static int n_types;
static int n_alloc_types;

OJType *
oj_type_get (const char *name)
{
  int i;
  for(i=0;i<n_types;i++){
    if (!strcmp (types[i].name, name)) {
      return types + i;
    }
  }
  return NULL;
}

void
oj_type_register (const char *name, int size)
{
  OJType *type;

  if (n_types == n_alloc_types) {
    n_alloc_types += 100;
    types = realloc (types, sizeof(OJType) * n_alloc_types);
  }

  type = types + n_types;
  type->name = strdup (name);
  type->size = size;

  n_types++;
}



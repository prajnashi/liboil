
#include <glib.h>
#include <stdio.h>
#include <liboil/liboil.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

typedef struct _Parameter Parameter;
struct _Parameter {
  char *type;
  gboolean ptr;
  char *name;
};

enum{
  OIL_TYPE_UNKNOWN,
  OIL_TYPE_s8,
  OIL_TYPE_u8,
  OIL_TYPE_s16,
  OIL_TYPE_u16,
  OIL_TYPE_s32,
  OIL_TYPE_u32,
  OIL_TYPE_f32,
  OIL_TYPE_f64,
};

GArray * parse_prototype (const char *s);
char *param_to_string (GArray *array);
void param_free (GArray *array);

int main (int argc, char *argv[])
{
  OilFunctionClass *klass;
  GArray *array;
  int i;
  char *string;

  oil_init ();

  for (i=0;i<oil_n_function_classes; i++ ){
    klass = oil_function_classes + i;

    if(klass->prototype) {
#if 0
      printf ("extern OilFunctionClass _oil_function_%s_class;\n",
          klass->name);
      printf ("#define %s ((void (*)(%s)) \\\n\t_oil_function_%s_class.func)\n",
          klass->name, klass->prototype, klass->name);
#endif
      //printf("void %s (%s);\n", klass->name, klass->prototype);
      array = parse_prototype(klass->prototype);

      string = param_to_string (array);

      printf ("extern OilFunctionClass _oil_function_%s_class;\n",
          klass->name);
      printf ("#define %s ((void (*)(%s)) \\\n\t_oil_function_%s_class.func)\n",
          klass->name, string, klass->name);

      param_free (array);
    }
  }

  return 0;
}

static char *typenames[][2] = {
  { "type_s8", "int8_t" },
  { "type_u8", "uint8_t" },
  { "type_s16", "int16_t" },
  { "type_u16", "uint16_t" },
  { "type_s32", "int32_t" },
  { "type_u32", "uint32_t" },
  { "type_f32", "float" },
  { "type_f64", "double" },
};

#ifdef unused
static int parse_type (char *s, char **endptr)
{
  int i;

  while(isspace(*s))s++;

  for(i=0;i<8;i++){
    if(strncmp(typenames[i],s,strlen(typenames[i]))==0){
      *endptr = s + strlen(typenames[i]);
      return OIL_TYPE_s8 + i;
    }
  }

  return OIL_TYPE_UNKNOWN;
}

static int parse_size (const char *s, const char **endptr)
{
  while(isspace(*s))s++;

  if(s[0] == 'n'){
    *endptr = s + 1;
    return 0;
  }
  if(isdigit(s[0])){
    return strtol(s,endptr,0);
  }

  return -1;
}
#endif

static char * parse_string (const char *s, const char **endptr)
{
  const char *s0;

  s0 = s;
  while(isalnum(*s) || *s=='_') {
    s++;
  }
  *endptr = s;

  return strndup(s0, s - s0);
}

void
param_append (GArray **array, char *type, gboolean ptr, char *name)
{
  Parameter param;

  param.type = type;
  param.ptr = ptr;
  param.name = name;

  *array = g_array_append_val (*array, param);
}

GArray *
parse_prototype (const char *s)
{
  char *type;
  char *name;
  gboolean ptr;
  int i;
  GArray *array;

  array = g_array_new (FALSE, FALSE, sizeof(Parameter));

  while (isspace(*s))s++;
  while (*s) {
    type = parse_string (s, &s);
    for(i=0;i<G_N_ELEMENTS(typenames);i++) {
      if (strcmp(type,typenames[i][0])==0) {
        free(type);
        type = strdup(typenames[i][1]);
        break;
      }
    }

    while (isspace(*s))s++;

    ptr = FALSE;
    if(s[0] == '*'){
      ptr = TRUE;
      s++;
    }
    while (isspace(*s))s++;
    name = parse_string (s, &s);

    while (isspace(*s))s++;

    if(s[0] == ','){
      s++;
    }
    while (isspace(*s))s++;

    param_append (&array, type, ptr, name);
  }

  return array;
}

char *param_to_string (GArray *array)
{
  GString *string;
  Parameter *param;
  int i;

  string = g_string_new ("");

  for(i=0;i<array->len;i++){
    param = &g_array_index (array, Parameter, i);

    g_string_append (string, param->type);
    g_string_append (string, " ");
    if (param->ptr) g_string_append (string, "*");
    g_string_append (string, param->name);

    if(i<array->len-1){
      g_string_append (string, ", ");
    }
  }

  return g_string_free (string, FALSE);
}

void param_free (GArray *array)
{
  Parameter *param;
  int i;

  for(i=0;i<array->len;i++){
    param = &g_array_index (array, Parameter, i);

    g_free(param->name);
    g_free(param->type);
  }
  g_array_free(array, TRUE);
}


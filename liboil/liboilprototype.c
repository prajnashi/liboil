
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include <liboil/liboil.h>
#include <liboil/liboilprototype.h>

#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif

typedef struct _OilString OilString;
struct _OilString {
  int len;
  int alloc_len;
  char *data;
};

static char * _strndup (const char *s, int n);
static const char * oil_type_name (OilType type);
static OilType oil_type_from_string (const char *s, int ptr);
static OilArgType oil_arg_type_from_string (const char *s);

static OilString *oil_string_new (const char *a);
static void oil_string_append (OilString *s, const char *a);
static char * oil_string_free (OilString *s, int free_seg);

static char * parse_string (const char *s, const char **endptr);

void
oil_prototype_append_param (OilPrototype *proto, OilParameter *param)
{
  proto->n_params++;
  proto->params = realloc(proto->params,
      sizeof(OilParameter) * proto->n_params);
  proto->params[proto->n_params - 1] = *param;
}

char *oil_prototype_to_string (OilPrototype *proto)
{
  OilString *string;
  OilParameter *param;
  int i;

  string = oil_string_new ("");

  for(i=0;i<proto->n_params;i++){
    param = proto->params + i;

    if (param->type != OIL_TYPE_UNKNOWN) {
      oil_string_append (string, oil_type_name (param->type));
    } else {
      oil_string_append (string, param->type_name);
    }
    oil_string_append (string, " ");
    oil_string_append (string, param->parameter_name);

    if(i<proto->n_params-1){
      oil_string_append (string, ", ");
    }
  }

  return oil_string_free (string, FALSE);
}

OilPrototype *oil_prototype_from_string (const char *s)
{
  OilPrototype *proto;
  OilParameter param = { 0 };
  char *type_name;
  char *parameter_name;
  int ptr;
  int order = 0;

  proto = malloc (sizeof(OilPrototype));
  memset (proto, 0, sizeof(OilPrototype));

  while (isspace(*s))s++;
  while (*s) {
    type_name = parse_string (s, &s);

    while (isspace(*s))s++;

    ptr = FALSE;
    if(s[0] == '*'){
      ptr = TRUE;
      s++;
    }
    while (isspace(*s))s++;
    parameter_name = parse_string (s, &s);

    while (isspace(*s))s++;

    if(s[0] == ','){
      s++;
    }
    while (isspace(*s))s++;

    param.type = oil_type_from_string (type_name, ptr);
    param.type_name = type_name;
    param.parameter_name = parameter_name;
    param.parameter_type = oil_arg_type_from_string (parameter_name);
    param.order = order;
    oil_prototype_append_param (proto, &param);

    order++;
  }

  return proto;
}

static char * parse_string (const char *s, const char **endptr)
{
  const char *s0;

  s0 = s;
  while(isalnum(*s) || *s=='_') {
    s++;
  }
  *endptr = s;

  return _strndup(s0, s - s0);
}

void
oil_prototype_free (OilPrototype *proto)
{
  int i;

  if (proto->params) {
    for(i=0;i<proto->n_params;i++) {
      if (proto->params[i].parameter_name) {
        free (proto->params[i].parameter_name);
      }
      if (proto->params[i].type_name) {
        free (proto->params[i].type_name);
      }
    }
    free (proto->params);
  }
  free (proto);
}



/* OilString */

static OilString *
oil_string_new (const char *a)
{
  OilString *s;

  s = malloc (sizeof(OilString));
  memset (s, 0, sizeof(OilString));

  if (a) {
    oil_string_append (s, a);
  }

  return s;

}

static void
oil_string_append (OilString *s, const char *a)
{
  int len = strlen (a);

  if (s->len + len + 1 > s->alloc_len) {
    s->alloc_len = s->len + len + 1;
    s->data = realloc (s->data, s->alloc_len);
  }
  memcpy (s->data + s->len, a, len + 1);
  s->len += len;
}

static char *
oil_string_free (OilString *s, int free_seg)
{
  char *data = s->data;
  free(s);
  if (free_seg) {
    free (data);
    return NULL;
  }
  return data;
}


static char *
_strndup (const char *s, int n)
{
  char *r;
  r = malloc (n+1);
  memcpy(r,s,n);
  r[n]=0;

  return r;
}


int oil_type_sizeof (OilType type)
{
  switch (type) {
    case OIL_TYPE_UNKNOWN:
    case OIL_TYPE_INT:
      return 0;
    case OIL_TYPE_s8:
    case OIL_TYPE_u8:
    case OIL_TYPE_s8p:
    case OIL_TYPE_u8p:
      return 1;
    case OIL_TYPE_s16:
    case OIL_TYPE_u16:
    case OIL_TYPE_s16p:
    case OIL_TYPE_u16p:
      return 2;
    case OIL_TYPE_s32:
    case OIL_TYPE_u32:
    case OIL_TYPE_f32:
    case OIL_TYPE_s32p:
    case OIL_TYPE_u32p:
    case OIL_TYPE_f32p:
      return 4;
    case OIL_TYPE_f64:
    case OIL_TYPE_f64p:
      return 8;
  }
  return 0;
}

static const char *oil_type_names [] = {
  "unknown",
  "int",
  "int8_t",
  "uint8_t",
  "int16_t",
  "uint16_t",
  "int32_t",
  "uint32_t",
  "float",
  "double",
  "int8_t *",
  "uint8_t *",
  "int16_t *",
  "uint16_t *",
  "int32_t *",
  "uint32_t *",
  "float *",
  "double *"
};
static const char *oil_type_names_2 [] = {
  "int8_t",
  "uint8_t",
  "int16_t",
  "uint16_t",
  "int32_t",
  "uint32_t",
  "float",
  "double"
};
static const char *oil_type_names_3 [] = {
  "type_s8",
  "type_u8",
  "type_s16",
  "type_u16",
  "type_s32",
  "type_u32",
  "type_f32",
  "type_f64"
};
#define ARRAY_LENGTH(x) (sizeof(x)/sizeof(x[0]))

static const char * oil_type_name (OilType type)
{

  if (type < 0 || type >= ARRAY_LENGTH(oil_type_names))
    return "unknown";

  return oil_type_names[type];
}

static OilType oil_type_from_string (const char *s, int ptr)
{
  int i;

  if (ptr) {
    for(i=0;i<ARRAY_LENGTH(oil_type_names_2);i++){
      if (strcmp(s,oil_type_names_2[i]) == 0){
        return OIL_TYPE_s8p + i;
      }
    }
    for(i=0;i<ARRAY_LENGTH(oil_type_names_3);i++){
      if (strcmp(s,oil_type_names_3[i]) == 0){
        return OIL_TYPE_s8p + i;
      }
    }
  } else {
    if (strcmp (s,"int") == 0) {
      return OIL_TYPE_INT;
    }
    for(i=0;i<ARRAY_LENGTH(oil_type_names_2);i++){
      if (strcmp(s,oil_type_names_2[i]) == 0){
        return OIL_TYPE_s8 + i;
      }
    }
    for(i=0;i<ARRAY_LENGTH(oil_type_names_3);i++){
      if (strcmp(s,oil_type_names_3[i]) == 0){
        return OIL_TYPE_s8 + i;
      }
    }
  }
  return OIL_TYPE_UNKNOWN;
}

static char *arg_names[] = {
  "unknown",
  "n",
  "dest1",
  "dstr1",
  "dest2",
  "dstr2",
  "src1",
  "sstr1",
  "src2",
  "sstr2",
  "src3",
  "sstr3",
  "state",
  "arg1",
  "arg2",
  "arg3",
  NULL
};

static OilArgType
oil_arg_type_from_string (const char *s)
{

  return OIL_ARG_TYPE_UNKNOWN;
}


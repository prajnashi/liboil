
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

static OilString *oil_string_new (const char *a);
static void oil_string_append (OilString *s, const char *a);
static char * oil_string_free (OilString *s, int free_seg);

static char * parse_string (const char *s, const char **endptr);

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


void
oil_prototype_append_param (OilPrototype *proto, OilParameter *param)
{
  proto->n_params++;
  proto->params = realloc(proto->params,
      sizeof(OilParameter) * proto->n_params);
  proto->params[proto->n_params] = *param;
}

char *oil_prototype_to_string (OilPrototype *proto)
{
  OilString *string;
  OilParameter *param;
  int i;

  string = oil_string_new ("");

  for(i=0;i<proto->n_params;i++){
    param = proto->params + i;

    oil_string_append (string, param->type);
    oil_string_append (string, " ");
    if (param->ptr) oil_string_append (string, "*");
    oil_string_append (string, param->name);

    if(i<proto->n_params-1){
      oil_string_append (string, ", ");
    }
  }

  return oil_string_free (string, FALSE);
}

OilPrototype *oil_prototype_from_string (const char *s)
{
  OilPrototype *proto;
  OilParameter param;
  char *type;
  char *name;
  int ptr;
  int i;

  proto = malloc (sizeof(OilPrototype));
  memset (proto, 0, sizeof(OilPrototype));

  while (isspace(*s))s++;
  while (*s) {
    type = parse_string (s, &s);
    for(i=0;i<sizeof(typenames)/sizeof(typenames[0]);i++) {
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

    param.type = type;
    param.ptr = ptr;
    param.name = name;
    oil_prototype_append_param (proto, &param);
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

  return strndup(s0, s - s0);
}

void
oil_prototype_free (OilPrototype *proto)
{
  if (proto->params) free (proto->params);
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


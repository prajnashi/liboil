
#include <stdio.h>
#include <liboil/liboil.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

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

void parse_prototype (const char *s);

int main (int argc, char *argv[])
{
  OilFunctionClass *klass;
  OilFunctionImpl *impl;
  int i;

  oil_init ();

  for (i=0;i<oil_n_function_classes; i++ ){
    klass = oil_function_classes + i;

    printf ("class: %s\n", klass->name);
    for(impl = klass->first_impl; impl; impl=impl->next) {
      printf ("  %s\n", impl->name);
    }
#if 0
      printf ("#define %s ((void (*)(%s)) \\\n\t_oil_function_%s_class.func)\n",
          klass->name, klass->prototype, klass->name);
      //printf("void %s (%s);\n", klass->name, klass->prototype);
      parse_prototype(klass->prototype);
    }
#endif
  }

  return 0;
}

#ifdef unused
static char *typenames[] = {
  "type_s8",
  "type_u8",
  "type_s16",
  "type_u16",
  "type_s32",
  "type_u32",
  "type_f32",
  "type_f64",
  "int8_t",
  "uint8_t",
  "int16_t",
  "uint16_t",
  "int32_t",
  "uint32_t",
  "float",
  "double",
  "int",
};

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

void parse_prototype (const char *s)
{
  char *type;
  char *name;
  int ptr = 0;

  while (isspace(*s))s++;
  while (*s) {
    type = parse_string (s, &s);

    while (isspace(*s))s++;

    if(s[0] == '*'){
      ptr = 1;
      s++;
    }
    while (isspace(*s))s++;
    name = parse_string (s, &s);

    while (isspace(*s))s++;

    if(s[0] == ','){
      s++;
    }
    while (isspace(*s))s++;

    printf("%s %s\n", type, name);
  }
}



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

void parse_description(char *s);

int main (int argc, char *argv[])
{
  OilFunctionClass *klass;
  int i;

  oil_init ();

  for (i=0;i<oil_n_function_classes; i++ ){
    klass = oil_function_classes + i;

    printf("class %s\n", klass->name);
    if(klass->test_description) {
      printf("test: %s\n",
	  klass->test_description);
      parse_description(klass->test_description);
    }
  }

  return 0;
}

static int parse_type (char *s, char **endptr)
{
  char *typenames[] = { "s8","u8","s16","u16","s32","u32","f32","f64" };
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

static int parse_size (char *s, char **endptr)
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

void parse_description (char *s)
{
  int type;
  int size;

  while(isspace(*s))s++;

  type = parse_type (s,&s);
  if (type == OIL_TYPE_UNKNOWN){
    return;
  }
  while(isspace(*s))s++;

  if(s[0] == '['){
    s++;
    size = parse_size(s,&s);
    if (size < 0) return;
  }
}


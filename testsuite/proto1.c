
#include <stdio.h>
#include <liboil/liboil.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include <liboil/liboilprototype.h>

static char *names[] = {
  "n",
  "dest",
  "dstr",
  "dest1",
  "dest2",
  "src",
  "src1",
  "src2",
  "src3",
  "sstr",
  "sstr1",
  "sstr2",
  "sstr3",
  "param",
  "param1",
  "param2",
  "state",
  NULL
};

static int get_name_index (const char *s);


int main (int argc, char *argv[])
{
  OilFunctionClass *klass;
  int i;
  int j;

  oil_init ();

  for (i=0;i<oil_n_function_classes; i++ ){
    klass = oil_class_get_by_index(i);

    if(klass->prototype) {
      int x = 0;
      OilPrototype *proto;

      proto = oil_prototype_from_string (klass->prototype);

      for(j=0;j<proto->n_params;j++){
        x = get_name_index (proto->params[j].parameter_name);
        if (x<0) {
          printf("%s(%s) [%s]\n", klass->name, klass->prototype,
              proto->params[j].parameter_name);
          break;
        }
      }
      oil_prototype_free (proto);
    }
  }

  return 0;
}

static int get_name_index (const char *s)
{
  int i;
  for(i=0;names[i];i++){
    if (strcmp (names[i], s) == 0) {
      return i;
    }
  }
  return -1;
}


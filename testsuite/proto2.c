
#include <stdio.h>
#include <liboil/liboil.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include <liboil/liboilprototype.h>


int main (int argc, char *argv[])
{
  OilFunctionClass *klass;
  int i;
  int j;

  oil_init ();

  for (i=0;i<oil_n_function_classes; i++ ){
    klass = oil_class_get_by_index(i);

    if(klass->prototype) {
      OilPrototype *proto;

      proto = oil_prototype_from_string (klass->prototype);

      printf("%s\n", klass->name);
      for(j=0;j<proto->n_params;j++){
        printf("  (%s) (%s) %d %d\n",
            proto->params[j].type_name,
            proto->params[j].parameter_name,
            proto->params[j].type,
            proto->params[j].arg_type);
      }
      oil_prototype_free (proto);
    }
  }

  return 0;
}




#include <stdio.h>
#include <liboil/liboil.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include <liboil/liboilprototype.h>

static OilParameter * get_param_by_type (OilPrototype *proto,
    OilArgType ptype);

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
      OilParameter params[OIL_ARG_LAST];
      OilParameter *p;

      memset (params, 0, sizeof(params));
      proto = oil_prototype_from_string (klass->prototype);

      printf("%s\n", klass->name);
      if (proto) {
        for(j=0;j<proto->n_params;j++){

          printf("  (%s) (%s) %d %d [%s] [%s]\n",
              proto->params[j].type_name,
              proto->params[j].parameter_name,
              proto->params[j].type,
              proto->params[j].parameter_type,
              oil_type_name (proto->params[j].type),
              oil_arg_type_name (proto->params[j].parameter_type));
          if (proto->params[j].parameter_type == OIL_ARG_UNKNOWN) {
            printf("unknown parameter type\n");
          }
        }

        for(j=0;j<proto->n_params;j++){
          memcpy (&params[proto->params[j].parameter_type], &proto->params[j],
              sizeof (OilParameter));
        }
        /* n */
        p = get_param_by_type (proto, OIL_ARG_N);
        if (p) {
          printf("  n: yes\n");
        } else {
          printf("  n: no\n");
        }

        oil_prototype_free (proto);
      } else {
        printf("  bad prototype\n");
      }

    }
  }

  return 0;
}


static OilParameter *
get_param_by_type (OilPrototype *proto, OilArgType ptype)
{
  int i;
  for(i=0;i<proto->n_params;i++){
    if (proto->params[i].parameter_type == ptype) {
      return &proto->params[i];
    }
  }
  return NULL;
}


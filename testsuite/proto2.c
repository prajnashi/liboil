
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

      /* standard cleanup */
      
      //oil_prototype_cleanup (params);

#if 0
      /* src1 */
      if(params[OIL_ARG_SRC1].type) {
        printf("  src1: yes\n");
        if (params[OIL_ARG_SSTR1].type) {
          printf("  sstr1: yes\n");
        } else {
          params[OIL_ARG_SSTR1].value = oil_type_sizeof (params[OIL_ARG_SRC1].type);
          printf("  sstr1: natural %ld\n", params[OIL_ARG_SSTR1].value);
        }
      }

      /* src2 */
      if(params[OIL_ARG_SRC2].type) {
        printf("  src2: yes\n");
        if (params[OIL_ARG_SSTR2].type) {
          printf("  sstr2: yes\n");
        } else {
          params[OIL_ARG_SSTR2].value = oil_type_sizeof (params[OIL_ARG_SRC2].type);
          printf("  sstr2: natural %ld\n", params[OIL_ARG_SSTR2].value);
        }
      }

      /* src3 */
      if(params[OIL_ARG_SRC3].type) {
        printf("  src3: yes\n");
        if (params[OIL_ARG_SSTR3].type) {
          printf("  sstr3: yes\n");
        } else {
          params[OIL_ARG_SSTR3].value = oil_type_sizeof (params[OIL_ARG_SRC3].type);
          printf("  sstr3: natural %ld\n", params[OIL_ARG_SSTR3].value);
        }
      }

      /* dest1 */
      if(params[OIL_ARG_DEST1].type) {
        printf("  dest1: yes\n");
        if (params[OIL_ARG_DSTR1].type) {
          printf("  dstr1: yes\n");
        } else {
          params[OIL_ARG_DSTR1].value = oil_type_sizeof (params[OIL_ARG_DEST1].type);
          printf("  dstr1: natural %ld\n", params[OIL_ARG_DSTR1].value);
        }
      }

      /* dest2 */
      if(params[OIL_ARG_DEST2].type) {
        printf("  dest2: yes\n");
        if (params[OIL_ARG_DSTR2].type) {
          printf("  dstr2: yes\n");
        } else {
          params[OIL_ARG_DSTR2].value = oil_type_sizeof (params[OIL_ARG_DEST1].type);
          printf("  dstr2: natural %ld\n", params[OIL_ARG_DSTR2].value);
        }
      }
#endif


      /* misc */

      /* n */
      p = get_param_by_type (proto, OIL_ARG_N);
      if (p) {
        printf("  n: yes\n");
      } else {
        printf("  n: no\n");
      }


      oil_prototype_free (proto);
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


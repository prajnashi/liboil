
#include <liboil/liboil.h>
#include <stdio.h>
#include <stdlib.h>



int main(int argc, char *argv[])
{
  double *dest;
  int32_t *src;
  int i;

  oil_init();
  oil_optimize_all();

  src = malloc(100*sizeof(int32_t));
  dest = malloc(100*sizeof(double));

  for(i=0;i<100;i++){
    src[i] = i;
  }

  oil_conv_f64_s32 (dest, sizeof(double), src, sizeof(int32_t), 100);

  for(i=0;i<100;i++){
    printf("%d %g\n",src[i],dest[i]);
  }

  return 0;
}


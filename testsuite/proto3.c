/*
 * LIBOIL - Library of Optimized Inner Loops
 * Copyright (c) 2004 David A. Schleef <ds@schleef.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */


#include <stdio.h>
#include <liboil/liboil.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include <liboil/liboilprototype.h>

typedef struct _Param Param;
struct _Param {
  int direction;
  int is_stride;
  int index;
  int prestride_length;
  int prestride_var;
  int poststride_length;
  int poststride_var;
};

int check_param (Param *p, char *s);
void print_param (Param *param);

/* format:
 * <isd>[s][0-9*][_[<0-9*,nm>x]<0-9*,nm>] */

char *good_params[] = {
  "d",
  "s",
  "i",
  "d1",
  "d2",
  "ds",
  "ss",
  "is",
  "ds1",
  "ds2",
  "d_1",
  "d_2",
  "d_4",
  "d_n",
  "d_1xn",
  "d_4xn",
  "d_nxm",
  "d_8x8",
  NULL
};

char *bad_params[] = {
  "e",
  NULL
};

int main (int argc, char *argv[])
{
  int i;
  int ret;
  int failed = 0;
  Param param;

  for(i=0;good_params[i];i++){
    ret = check_param (&param, good_params[i]);
    if (!ret) {
      printf("***ERROR***\n");
      failed = 1;
    }
    print_param (&param);
  }

  for(i=0;bad_params[i];i++){
    ret = check_param (&param, bad_params[i]);
    if (ret) {
      printf("***ERROR***\n");
      failed = 1;
    }
  }

  return failed;
}

int check_param (Param *p, char *s)
{
  printf("trying %s:\n", s);
  p->direction = *s;
  switch (*s) {
    case 'i':
      break;
    case 's':
      break;
    case 'd':
      break;
    default:
      printf ("  parse error at %s\n", s);
      return 0;
  }
  s++;

  if (*s == 's') {
    p->is_stride = 1;
    s++;
  } else {
    p->is_stride = 0;
  }

  if (isdigit ((int)*s)) {
    p->index = *s - '0';
    s++;
  } else {
    p->index = 1;
  }

  if (!p->is_stride && *s == '_') {
    int length;
    int var;

    s++;

    if (isdigit ((int)*s)) {
      length = strtoul (s, &s, 10);
      var = 0;
    } else if (*s == 'n') {
      length = 0;
      var = 1;
      s++;
    } else if (*s == 'm') {
      length = 0;
      var = 2;
      s++;
    } else {
      printf ("  parse error at %s\n", s);
      return 0;
    }

    if (*s == 'x') {
      s++;
      p->prestride_length = length;
      p->prestride_var = var;

      if (isdigit ((int)*s)) {
        p->poststride_length = strtoul (s, &s, 10);
        p->poststride_var = 0;
      } else if (*s == 'n') {
        p->poststride_length = 0;
        p->poststride_var = 1;
        s++;
      } else if (*s == 'm') {
        p->poststride_length = 0;
        p->poststride_var = 2;
        s++;
      } else {
        printf ("  parse error at %s\n", s);
        return 0;
      }

    } else {
      p->poststride_length = length;
      p->poststride_var = var;
      p->prestride_length = 1;
      p->prestride_var = 0;
    }

  } else {
    p->poststride_length = 0;
    p->poststride_var = 1;
    p->prestride_length = 1;
    p->prestride_var = 0;
  }
  if (*s != 0) {
    printf ("  parse error at %s\n", s);
    return 0;
  }

  return 1;
}

void print_param (Param *param)
{
  if (param->is_stride) {
    printf ("  %cs%d\n", param->direction, param->index);
  } else {
    printf ("  %c%d_", param->direction, param->index);
    if (param->prestride_length) {
      printf("%d", param->prestride_length);
    } else {
      printf("%c", (param->prestride_var==1) ? 'n' : 'm');
    }
    printf("x");
    if (param->poststride_length) {
      printf("%d", param->poststride_length);
    } else {
      printf("%c", (param->poststride_var==1) ? 'n' : 'm');
    }
    printf("\n");
  }

}


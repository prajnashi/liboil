
#include <stdio.h>
#include <liboil/liboil.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include <liboil/liboilprototype.h>

void print_header (void);
void print_footer (void);

int main (int argc, char *argv[])
{
  OilFunctionClass *klass;
  OilPrototype *proto;
  int i;
  char *string;

  oil_init ();

  print_header ();

  for (i=0;i<oil_n_function_classes; i++ ){
    klass = oil_function_classes + i;

    if(klass->prototype) {
      proto = oil_prototype_from_string (klass->prototype);

      string = oil_prototype_to_string (proto);

      printf ("extern OilFunctionClass *oil_function_%s_class_ptr;\n",
          klass->name);
      printf ("#define %s ((void (*)(%s)) \\\n\toil_function_%s_class_ptr->func)\n",
          klass->name, string, klass->name);

      oil_prototype_free (proto);
      free (string);
    }
  }

  print_footer ();

  return 0;
}

void print_header (void)
{
  printf ("/* liboil - Library of Optimized Inner Loops\n");
  printf (" * Copyright (C) 2003  David A. Schleef <ds@schleef.org>\n");
  printf (" *\n");
  printf (" * This library is free software; you can redistribute it and/or\n");
  printf (" * modify it under the terms of version 2.1 of the GNU Lesser General\n");
  printf (" * Public License as published by the Free Software Foundation.\n");
  printf (" *\n");
  printf (" * This library is distributed in the hope that it will be useful,\n");
  printf (" * but WITHOUT ANY WARRANTY; without even the implied warranty of\n");
  printf (" * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU\n");
  printf (" * Library General Public License for more details.\n");
  printf (" *\n");
  printf (" * You should have received a copy of the GNU Lesser General Public\n");
  printf (" * License along with this library; if not, write to the\n");
  printf (" * Free Software Foundation, Inc., 59 Temple Place, Suite 330,\n");
  printf (" * Boston, MA 02111-1307 USA.\n");
  printf (" */\n");
  printf ("\n");
  printf ("#ifndef _LIBOIL_FUNCS_H_\n");
  printf ("#define _LIBOIL_FUNCS_H_\n");
  printf ("\n");
}

void print_footer (void)
{
  printf("\n");
  printf("#endif\n");
  printf("\n");
}


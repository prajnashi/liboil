/* liboil - Library of Optimized Inner Loops
 * Copyright (C) 2003  David A. Schleef <ds@schleef.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of version 2.1 of the GNU Lesser General
 * Public License as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307 USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <liboil/liboildebug.h>
#include <liboil/liboilfunction.h>

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>

int _oil_debug_enabled = 1;
int _oil_debug_level = OIL_DEBUG_ERROR;


void
_oil_debug_init(void)
{
  const char *envvar;

  envvar = getenv ("OIL_DEBUG");
  if (envvar != NULL) {
    _oil_debug_level = strtol (envvar + 10, NULL, 0);
  }

  //OIL_INFO ("debug init");
  OIL_DEBUG_PRINT(OIL_DEBUG_INFO, "debug init");

}

void
oil_spill (void)
{
  OilFunctionClass *klass;
  OilFunctionImpl *impl;
  int i;

  for(i=0;i<oil_n_function_classes;i++){
    klass = oil_function_classes + i;

    printf("%s\n",klass->name);
    for(impl = klass->first_impl; impl; impl=impl->next){
      printf("  %p %08x %5d %-10s %s\n",impl->func,
	  impl->flags,
	  impl->prof,
	  (impl==klass->reference_impl)?"(ref)":"",
	  impl->name);
    }
  }
}

static void
oil_debug_print_valist (int level, const char *file, const char *func,
        int line, const char *format, va_list args)
{
  static char *level_names[] = { "NONE", "ERROR", "WARNING", "INFO",
    "DEBUG", "LOG" };
  char *level_name = "unknown";

  if (level > _oil_debug_level) return;

  if(level>=OIL_DEBUG_NONE && level<OIL_DEBUG_LOG){
    level_name = level_names[level];
  }
  
  fprintf (stdout, "OIL: %s %s %d: %s(): ", level_name, file, line, func);
  vfprintf (stdout, format, args);
  fprintf (stdout, "\n");
}

void
oil_debug_print (int level, const char *file, const char *func,
        int line, const char *format, ...)
{
  va_list var_args;

  va_start (var_args, format);
  oil_debug_print_valist (level, file, func, line, format, var_args);
  va_end (var_args);
}



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
#include <liboil/liboilfunction.h>
#include <liboil/liboildebug.h>

#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static char * get_cpuinfo_flags_string (char *cpuinfo);
static char ** strsplit (char *s);
static char * _strndup (const char *s, int n);

unsigned long oil_cpu_flags;

static char *
get_cpuinfo (void)
{
  char *cpuinfo = malloc(4096);
  int fd;
  int n;

  fd = open("/proc/cpuinfo", O_RDONLY);
  if (fd < 0) return NULL;

  n = read(fd, cpuinfo, 4095);
  cpuinfo[n] = 0;

  return cpuinfo;
}

#ifdef __i386__
static void
oil_cpu_i386_getflags(void)
{
  char *cpuinfo;
  char *cpuinfo_flags;
  char **flags;
  char **f;

  cpuinfo = get_cpuinfo();

  cpuinfo_flags = get_cpuinfo_flags_string(cpuinfo);
  if (cpuinfo_flags == NULL) return;

  flags = strsplit(cpuinfo_flags);
  for (f = flags; *f; f++) {
    if (strcmp (*f, "cmov") == 0) {
      OIL_DEBUG ("cpu flag %s\n", *f);
      oil_cpu_flags |= OIL_IMPL_REQUIRES_CMOV;
    }
    if (strcmp (*f, "mmx") == 0) {
      OIL_DEBUG ("cpu flag %s\n", *f);
      oil_cpu_flags |= OIL_IMPL_REQUIRES_MMX;
    }
    if (strcmp (*f, "sse") == 0) {
      OIL_DEBUG ("cpu flag %s\n", *f);
      oil_cpu_flags |= OIL_IMPL_REQUIRES_SSE;
    }
    if (strcmp (*f, "mmxext") == 0) {
      OIL_DEBUG ("cpu flag %s\n", *f);
      oil_cpu_flags |= OIL_IMPL_REQUIRES_MMXEXT;
    }
    if (strcmp (*f, "sse2") == 0) {
      OIL_DEBUG ("cpu flag %s\n", *f);
      oil_cpu_flags |= OIL_IMPL_REQUIRES_SSE2;
    }
    if (strcmp (*f, "3dnow") == 0) {
      OIL_DEBUG ("cpu flag %s\n", *f);
      oil_cpu_flags |= OIL_IMPL_REQUIRES_3DNOW;
    }
    if (strcmp (*f, "3dnowext") == 0) {
      OIL_DEBUG ("cpu flag %s\n", *f);
      oil_cpu_flags |= OIL_IMPL_REQUIRES_3DNOWEXT;
    }

    free (*f);
  }
  free (flags);
  free (cpuinfo);
  free (cpuinfo_flags);

  OIL_INFO ("cpu flags %08lx\n", oil_cpu_flags);
}
#endif

void
_oil_cpu_init (void)
{
#ifdef __i386__
  oil_cpu_i386_getflags();
#endif

}

static char *
get_cpuinfo_flags_string (char *cpuinfo)
{
  char *flags;
  char *end;
  char *colon;

  flags = strstr(cpuinfo,"flags");
  if (flags == NULL) return NULL;

  end = strchr(flags, '\n');
  if (end == NULL) return NULL;
  colon = strchr (flags, ':');
  if (colon == NULL) return NULL;
  colon++;
  if(colon >= end) return NULL;

  return _strndup (colon, end-colon);
}


static char **
strsplit (char *s)
{
  char **list = NULL;
  char *tok;
  int n = 0;

  while (*s == ' ') s++;

  list = malloc (1 * sizeof(char *));
  while (*s) {
    tok = s;
    while (*s && *s != ' ') s++;

    list[n] = _strndup (tok, s - tok);
    while (*s && *s == ' ') s++;
    list = realloc (list, (n + 2) * sizeof(char *));
    n++;
  }

  list[n] = NULL;
  return list;
}

static char *
_strndup (const char *s, int n)
{
  char *r;
  r = malloc (n+1);
  memcpy(r,s,n);
  r[n]=0;

  return r;
}


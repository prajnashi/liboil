/*
 * LIBOIL - Library of Optimized Inner Loops
 * Copyright (c) 2003,2004 David A. Schleef <ds@schleef.org>
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <liboil/liboilfunction.h>
#include <liboil/liboildebug.h>
#include <liboil/liboilcpu.h>

#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <setjmp.h>
#include <signal.h>

#if defined(__i386__)
static char * get_cpuinfo_flags_string (char *cpuinfo);
static char ** strsplit (char *s);
static char * _strndup (const char *s, int n);
#endif

static unsigned long oil_cpu_flags;

#if defined(__i386__)
static char *
get_cpuinfo (void)
{
  char *cpuinfo;
  int fd;
  int n;

  cpuinfo = malloc(4096);
  if (cpuinfo == NULL) return NULL;

  fd = open("/proc/cpuinfo", O_RDONLY);
  if (fd < 0) return NULL;

  n = read(fd, cpuinfo, 4095);
  cpuinfo[n] = 0;

  return cpuinfo;
}
#endif

#ifdef __i386__
static void
oil_cpu_i386_getflags(void)
{
  char *cpuinfo;
  char *cpuinfo_flags;
  char **flags;
  char **f;

  cpuinfo = get_cpuinfo();
  if (cpuinfo == NULL) return;

  cpuinfo_flags = get_cpuinfo_flags_string(cpuinfo);
  if (cpuinfo_flags == NULL) {
    free (cpuinfo);
    return;
  }

  flags = strsplit(cpuinfo_flags);
  for (f = flags; *f; f++) {
    if (strcmp (*f, "cmov") == 0) {
      OIL_DEBUG ("cpu flag %s", *f);
      oil_cpu_flags |= OIL_IMPL_FLAG_CMOV;
    }
    if (strcmp (*f, "mmx") == 0) {
      OIL_DEBUG ("cpu flag %s", *f);
      oil_cpu_flags |= OIL_IMPL_FLAG_MMX;
    }
    if (strcmp (*f, "sse") == 0) {
      OIL_DEBUG ("cpu flag %s", *f);
      oil_cpu_flags |= OIL_IMPL_FLAG_SSE;
    }
    if (strcmp (*f, "mmxext") == 0) {
      OIL_DEBUG ("cpu flag %s", *f);
      oil_cpu_flags |= OIL_IMPL_FLAG_MMXEXT;
    }
    if (strcmp (*f, "sse2") == 0) {
      OIL_DEBUG ("cpu flag %s", *f);
      oil_cpu_flags |= OIL_IMPL_FLAG_SSE2;
    }
    if (strcmp (*f, "3dnow") == 0) {
      OIL_DEBUG ("cpu flag %s", *f);
      oil_cpu_flags |= OIL_IMPL_FLAG_3DNOW;
    }
    if (strcmp (*f, "3dnowext") == 0) {
      OIL_DEBUG ("cpu flag %s", *f);
      oil_cpu_flags |= OIL_IMPL_FLAG_3DNOWEXT;
    }

    free (*f);
  }
  free (flags);
  free (cpuinfo);
  free (cpuinfo_flags);
}
#endif

#ifdef __powerpc__
static void
test_altivec (void * ignored)
{
  char x[16] = { 0, };

  asm volatile (
      "  lvx %%v0, %0, %%r0  \n"
      :: "r" (x));
}

static void
oil_cpu_powerpc_getflags(void)
{

  oil_cpu_fault_check_enable ();
  if (oil_cpu_fault_check_try(test_altivec, NULL)) {
    OIL_DEBUG ("cpu flag altivec");
    oil_cpu_flags |= OIL_IMPL_FLAG_ALTIVEC;
  }
  oil_cpu_fault_check_disable ();
}
#endif

void
_oil_cpu_init (void)
{
  const char *envvar;

  envvar = getenv ("OIL_CPU_FLAGS");
  if (envvar != NULL) {
    char *end = NULL;
    unsigned long flags;

    flags = strtoul (envvar, &end, 0);
    if (end > envvar) {
      oil_cpu_flags = flags;
    }
    OIL_INFO ("cpu flags from environment %08lx", oil_cpu_flags);
    return;
  }

#ifdef __i386__
  oil_cpu_i386_getflags();
#endif
#ifdef __powerpc__
  oil_cpu_powerpc_getflags();
#endif

  OIL_INFO ("cpu flags %08lx", oil_cpu_flags);
}

unsigned int
oil_cpu_get_flags (void)
{
  return oil_cpu_flags;
}

#if defined(__i386__)
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
#endif

static jmp_buf jump_env;
static struct sigaction action;
static struct sigaction oldaction;
static int in_try_block;

static void
illegal_instruction_handler (int num)
{
  if (in_try_block) {
    longjmp (jump_env, 1);
  } else {
    abort ();
  }
}

void
oil_cpu_fault_check_enable (void)
{
  memset (&action, 0, sizeof(action));
  action.sa_handler = &illegal_instruction_handler;
  sigaction (SIGILL, &action, &oldaction);
  in_try_block = 0;
}

int
oil_cpu_fault_check_try (void (*func) (void *), void *priv)
{
  int ret;

  in_try_block = 1;
  ret = setjmp (jump_env);
  if (!ret) {
    func (priv);
  }
  in_try_block = 0;

  return (ret == 0);
}

void
oil_cpu_fault_check_disable (void)
{
  sigaction (SIGILL, &oldaction, NULL);
}


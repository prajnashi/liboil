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

#if defined(__FreeBSD__)
#include <sys/types.h>
#include <sys/sysctl.h>
#endif

#if defined(__i386__)
static char * get_cpuinfo_flags_string (char *cpuinfo);
static char ** strsplit (char *s);
static char * _strndup (const char *s, int n);
#endif

static unsigned long oil_cpu_flags;

#if defined(__i386__)
static char *
get_proc_cpuinfo (void)
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
oil_cpu_i386_getflags_cpuinfo (char *cpuinfo)
{
  char *cpuinfo_flags;
  char **flags;
  char **f;

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
      oil_cpu_flags |= OIL_IMPL_FLAG_MMXEXT;
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

static void
get_cpuid (uint32_t op, uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d)
{
  __asm__ (
      "  pushl %%ebx\n"
      "  cpuid\n"
      "  mov %%ebx, %%esi\n"
      "  popl %%ebx\n"
      : "=a" (*a), "=S" (*b), "=c" (*c), "=d" (*d)
      : "0" (op));
}

static void
test_cpuid (void *ignored)
{
  uint32_t eax, ebx, ecx, edx;

  get_cpuid (0x00000000, &eax, &ebx, &ecx, &edx);
}

static void
oil_cpu_i386_getflags_cpuid (void)
{
  uint32_t eax, ebx, ecx, edx;
  uint32_t level;
  char vendor[13] = { 0 };
  int ret;

  oil_cpu_fault_check_enable ();
  ret = oil_cpu_fault_check_try(test_cpuid, NULL);
  oil_cpu_fault_check_disable ();
  if (!ret) {
    /* CPU thinks cpuid is an illegal instruction. */
    return;
  }

  get_cpuid (0x00000000, &level, (uint32_t *)(vendor+0),
      (uint32_t *)(vendor+8), (uint32_t *)(vendor+4));

  OIL_DEBUG("cpuid %d %s", level, vendor);

  if (level < 1) {
    return;
  }

  get_cpuid (0x00000001, &eax, &ebx, &ecx, &edx);

  /* Intel flags */
  if (edx & (1<<15)) {
    oil_cpu_flags |= OIL_IMPL_FLAG_CMOV;
  }
  if (edx & (1<<23)) {
    oil_cpu_flags |= OIL_IMPL_FLAG_MMX;
  }
  if (edx & (1<<25)) {
    oil_cpu_flags |= OIL_IMPL_FLAG_SSE;
  }
  if (edx & (1<<26)) {
    oil_cpu_flags |= OIL_IMPL_FLAG_SSE2;
    oil_cpu_flags |= OIL_IMPL_FLAG_MMXEXT;
  }
  if (ecx & (1<<0)) {
    oil_cpu_flags |= OIL_IMPL_FLAG_SSE3;
  }
  
  if (memcmp (vendor, "AuthenticAMD", 12) == 0) {
    get_cpuid (0x80000001, &eax, &ebx, &ecx, &edx);

    /* AMD flags */
    if (edx & (1<<22)) {
      oil_cpu_flags |= OIL_IMPL_FLAG_MMXEXT;
    }
    if (edx & (1<<31)) {
      oil_cpu_flags |= OIL_IMPL_FLAG_3DNOW;
    }
    if (edx & (1<<30)) {
      oil_cpu_flags |= OIL_IMPL_FLAG_3DNOWEXT;
    }
  }
}

/* Reduce the set of CPU capabilities detected by whatever detection mechanism
 * was chosen, according to kernel limitations.  SSE requires kernel support for
 * use.
 *
 * This function might also want to grow a check for the old RedHat + Linux 2.2
 * unmasked SSE FPU exception bug.  Other than that, if /proc/cpuinfo reported
 * SSE, then it's safe.
 */
static void
oil_cpu_i386_kernel_restrict_flags(void)
{
#ifdef __FreeBSD__
  int ret, enabled;
  unsigned int len;

  len = sizeof(enabled);
  ret = sysctlbyname("hw.instruction_sse", &enabled, &len, NULL, 0);
  if (ret || !enabled) {
    oil_cpu_flags &= ~(OIL_IMPL_FLAG_SSE | OIL_IMPL_FLAG_SSE2 |
		       OIL_IMPL_FLAG_MMXEXT | OIL_IMPL_FLAG_SSE3);
  }
#endif
#if !defined(__linux__) && !defined(__FreeBSD__)
  /* If we don't know that the operating system supports SSE, don't trust that
   * it will properly support it.
   */
  oil_cpu_flags &= ~(OIL_IMPL_FLAG_SSE | OIL_IMPL_FLAG_SSE2 |
		     OIL_IMPL_FLAG_MMXEXT | OIL_IMPL_FLAG_SSE3);
#endif
}

static void
oil_cpu_i386_getflags(void)
{
  char *cpuinfo;

  cpuinfo = get_proc_cpuinfo();
  if (cpuinfo) {
    oil_cpu_i386_getflags_cpuinfo(cpuinfo);
  } else {
    oil_cpu_i386_getflags_cpuid();
  }

  oil_cpu_i386_kernel_restrict_flags();
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
#ifdef HAVE_SIGACTION
static struct sigaction action;
static struct sigaction oldaction;
#else
static void * oldhandler;
#endif
static int in_try_block;
static int enable_level;

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
  if (enable_level == 0) {
#ifdef HAVE_SIGACTION
    memset (&action, 0, sizeof(action));
    action.sa_handler = &illegal_instruction_handler;
    sigaction (SIGILL, &action, &oldaction);
#else
    signal (SIGILL, illegal_instruction_handler);
#endif
    in_try_block = 0;
  }
  enable_level++;
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
  enable_level--;
  if (enable_level == 0) {
#ifdef HAVE_SIGACTION
    sigaction (SIGILL, &oldaction, NULL);
#else
    signal (SIGILL, oldhandler);
#endif
  }
}

#if 0
void oil_memory_prefetch_local (void *ptr, int n_bytes)
{
#ifdef __i386__
  if (oil_cpu_flags & OIL_IMPL_FLAG_MMX) {
    asm volatile (
        "prefetcht0 %0\n"
        :: "m" (ptr));
  }
#endif

}

void oil_memory_prefetch_transient (void *ptr, int n_bytes)
{
#ifdef __i386__
  if (oil_cpu_flags & OIL_IMPL_FLAG_MMX) {
    asm volatile (
        "prefetchnta %0\n"
        :: "m" (ptr));
  }
#endif

}
#endif

#if 0
double
oil_cpu_get_ticks_per_second (void)
{
  return _oil_ticks_per_second;
}
#endif


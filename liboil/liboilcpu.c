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
#include <sys/time.h>
#include <time.h>

#if defined(__FreeBSD__)
#include <sys/types.h>
#include <sys/sysctl.h>
#endif

/**
 * SECTION:liboilcpu
 * @title: CPU
 * @short_description: Check the capabilities of the current CPU
 *
 */

static void oil_cpu_detect_arch(void);

static unsigned long oil_cpu_flags;

extern unsigned long (*_oil_profile_stamp)(void);

#if defined(__linux__) && defined(__arm__)
#define USE_CPUINFO
#endif


#ifdef HAVE_GETTIMEOFDAY
static unsigned long
oil_profile_stamp_gtod (void)
{
  struct timeval tv;
  gettimeofday(&tv,NULL);
  return 1000000*(unsigned long)tv.tv_sec + (unsigned long)tv.tv_usec;
}
#endif

static unsigned long
oil_profile_stamp_zero (void)
{
  return 0;
}

void
_oil_cpu_init (void)
{
  const char *envvar;

  oil_cpu_detect_arch();

  envvar = getenv ("OIL_CPU_FLAGS");
  if (envvar != NULL) {
    char *end = NULL;
    unsigned long flags;

    flags = strtoul (envvar, &end, 0);
    if (end > envvar) {
      oil_cpu_flags = flags;
    }
    OIL_INFO ("cpu flags from environment %08lx", oil_cpu_flags);
  }

  OIL_INFO ("cpu flags %08lx", oil_cpu_flags);

#ifdef HAVE_GETTIMEOFDAY
  if (_oil_profile_stamp == NULL) {
    _oil_profile_stamp = oil_profile_stamp_gtod;
    OIL_WARNING("Using gettimeofday() as a timestamp function.");
  }
#endif
  if (_oil_profile_stamp == NULL) {
    _oil_profile_stamp = oil_profile_stamp_zero;
    OIL_ERROR("No timestamping function.  This is kinda bad.");
  }
}

/**
 * oil_cpu_get_flags:
 *
 * Returns a bitmask containing the available CPU features.
 *
 * Returns: the CPU features.
 */
unsigned int
oil_cpu_get_flags (void)
{
  return oil_cpu_flags;
}


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

/**
 * oil_cpu_fault_check_enable:
 *
 * Enables fault checking mode.  This function may be called multiple times.
 * Each call to this function must be paired with a corresponding call
 * to oil_cpu_fault_check_disable().
 *
 * This function sets a signal handler for SIGILL.
 */
void
oil_cpu_fault_check_enable (void)
{
  if (enable_level == 0) {
#ifdef HAVE_SIGACTION
    memset (&action, 0, sizeof(action));
    action.sa_handler = &illegal_instruction_handler;
    sigaction (SIGILL, &action, &oldaction);
#else
    oldhandler = signal (SIGILL, illegal_instruction_handler);
#endif
    in_try_block = 0;
    OIL_INFO("enabling SIGILL handler.  Make sure to continue past "
        "any SIGILL signals caught by gdb.");
  }
  enable_level++;
}

/**
 * oil_cpu_fault_check_try:
 * @func: the function to attempt
 * @priv: a value to pass to the function
 *
 * Calls to this
 * function must be preceded by a call to oil_cpu_fault_check_enable()
 * to enable fault checking mode.  This function sets up recovery
 * information and then calls the function @func with the parameter
 * @priv.  If @func or any other functions it calls attempt to execute
 * an illegal instruction, the exception will be caught and recovered from.
 *
 * Returns: 1 if the function was executed sucessfully, 0 if the
 * function attempted to execute an illegal instruction.
 */
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

/**
 * oil_cpu_fault_check_disable:
 *
 * Disables fault checking mode.  See oil_cpu_fault_check_enable()
 * for details.
 */
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
    OIL_INFO("disabling SIGILL handler");
  }
}

#if 0
/**
 * oil_cpu_get_ticks_per_second:
 *
 * Returns the estimated number of ticks per second.  This feature
 * is currently unimplemented.
 *
 * This function may take several milliseconds or more to execute
 * in order to calculate a good estimate of the number of ticks (as
 * measured by the profiling functions) per second.  Note that the
 * number of ticks per second is often dependent on the CPU frequency,
 * which can change dynamically.  Thus the value returned by this
 * function may be incorrect as soon as it is returned.
 *
 * Returns: a double
 */
double
oil_cpu_get_ticks_per_second (void)
{
  return _oil_ticks_per_second;
}
#endif


#ifdef USE_CPUINFO
static char * get_cpuinfo_line (char *cpuinfo, const char *tag);
static char * _strndup (const char *s, int n);

char *
get_proc_cpuinfo (void)
{
  char *cpuinfo;
  int fd;
  int n;

  cpuinfo = malloc(4096);
  if (cpuinfo == NULL) return NULL;

  fd = open("/proc/cpuinfo", O_RDONLY);
  if (fd < 0) {
    free (cpuinfo);
    return NULL;
  }

  n = read(fd, cpuinfo, 4095);
  if (n < 0) {
    free (cpuinfo);
    close (fd);
    return NULL;
  }
  cpuinfo[n] = 0;

  close (fd);

  return cpuinfo;
}

static char *
get_cpuinfo_line (char *cpuinfo, const char *tag)
{
  char *flags;
  char *end;
  char *colon;

  flags = strstr(cpuinfo,tag);
  if (flags == NULL) return NULL;

  end = strchr(flags, '\n');
  if (end == NULL) return NULL;
  colon = strchr (flags, ':');
  if (colon == NULL) return NULL;
  colon++;
  if(colon >= end) return NULL;

  return _strndup (colon, end-colon);
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



/***** i386, amd64 *****/

#if defined(__i386__) || defined(__amd64__)

#if 0
#if defined(__linux__)
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

static void
oil_cpu_i386_getflags_cpuinfo (char *cpuinfo)
{
  char *cpuinfo_flags;
  char **flags;
  char **f;

  cpuinfo_flags = get_cpuinfo_line(cpuinfo, "flags");
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
#endif
#endif

static unsigned long
oil_profile_stamp_rdtsc(void)
{
	unsigned long ts;
	__asm__ __volatile__("rdtsc\n" : "=a" (ts) : : "edx");
	return ts;
}

#ifdef __i386__
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
#endif

#ifdef __amd64__
static void
get_cpuid (uint32_t op, uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d)
{
  __asm__ (
      "  pushq %%rbx\n"
      "  cpuid\n"
      "  mov %%ebx, %%esi\n"
      "  popq %%rbx\n"
      : "=a" (*a), "=S" (*b), "=c" (*c), "=d" (*d)
      : "0" (op));
}
#endif

static void
test_cpuid (void *ignored)
{
  uint32_t eax, ebx, ecx, edx;

  get_cpuid (0x00000000, &eax, &ebx, &ecx, &edx);
}

static void
oil_cpu_detect_cpuid (void)
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

  if (edx & (1<<4)) {
    _oil_profile_stamp = oil_profile_stamp_rdtsc;
  }

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

    get_cpuid (0x80000005, &eax, &ebx, &ecx, &edx);

    OIL_INFO("L1 D-cache: %d kbytes, %d-way, %d lines/tag, %d line size",
        (ecx>>24)&0xff, (ecx>>16)&0xff, (ecx>>8)&0xff, ecx&0xff);
    OIL_INFO("L1 I-cache: %d kbytes, %d-way, %d lines/tag, %d line size",
        (edx>>24)&0xff, (edx>>16)&0xff, (edx>>8)&0xff, edx&0xff);

    get_cpuid (0x80000006, &eax, &ebx, &ecx, &edx);
    OIL_INFO("L2 cache: %d kbytes, %d assoc, %d lines/tag, %d line size",
        (ecx>>16)&0xffff, (ecx>>12)&0xf, (ecx>>8)&0xf, ecx&0xff);
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
oil_cpu_detect_kernel_support (void)
{
#ifdef __FreeBSD__
  int ret, enabled;
  size_t len;

  len = sizeof(enabled);
  ret = sysctlbyname("hw.instruction_sse", &enabled, &len, NULL, 0);
  if (ret || !enabled) {
    oil_cpu_flags &= ~(OIL_IMPL_FLAG_SSE | OIL_IMPL_FLAG_SSE2 |
		       OIL_IMPL_FLAG_MMXEXT | OIL_IMPL_FLAG_SSE3);
  }
#endif
#if !defined(__linux__) && !defined(__FreeBSD__) && !defined(__NetBSD__)
  /* If we don't know that the operating system supports SSE, don't trust that
   * it will properly support it.
   */
  OIL_ERROR("Operating system is not known to support SSE.  Assuming it does, which might cause problems");
#if 0
  oil_cpu_flags &= ~(OIL_IMPL_FLAG_SSE | OIL_IMPL_FLAG_SSE2 |
		     OIL_IMPL_FLAG_MMXEXT | OIL_IMPL_FLAG_SSE3);
#endif
#endif
}

static void
oil_cpu_detect_i386(void)
{
#if 0
  char *cpuinfo;

  cpuinfo = get_proc_cpuinfo();
  if (cpuinfo) {
    oil_cpu_i386_getflags_cpuinfo(cpuinfo);
  } else {
    oil_cpu_i386_getflags_cpuid();
  }
#endif

  oil_cpu_detect_cpuid ();

  oil_cpu_detect_kernel_support ();
}
#endif


/***** powerpc *****/

#if defined(__powerpc__) || defined(__PPC__) || defined(__ppc__)
static unsigned long
oil_profile_stamp_tb(void)
{
	unsigned long ts;
	__asm__ __volatile__("mftb %0\n" : "=r" (ts));
	return ts;
}

static void
test_altivec (void * ignored)
{
  char x[16] = { 0, };

  asm volatile (
      "  lvx %%v0, %0, %%r0  \n"
      :: "r" (x));
}

static void
oil_cpu_detect_powerpc(void)
{

  oil_cpu_fault_check_enable ();
  if (oil_cpu_fault_check_try(test_altivec, NULL)) {
    OIL_DEBUG ("cpu flag altivec");
    oil_cpu_flags |= OIL_IMPL_FLAG_ALTIVEC;
  }
  oil_cpu_fault_check_disable ();

  _oil_profile_stamp = oil_profile_stamp_tb;
}
#endif


/***** arm *****/

#ifdef __arm__
static unsigned long
oil_profile_stamp_xscale(void)
{
  unsigned int ts;
  __asm__ __volatile__ (
      "  mrc p14, 0, %0, c0, c0, 0 \n"
      : "=r" (ts));
  return ts;
}

static void
oil_cpu_detect_arm(void)
{
#ifdef __linux__
  int arm_implementer;
  char *cpuinfo;
  char *s;

  cpuinfo = get_proc_cpuinfo();

  s = get_cpuinfo_line(cpuinfo, "CPU implementer");
  if (s) {
    arm_implementer = strtoul (s, NULL, 0);
    free(s);
  }

  switch(arm_implementer) {
    case 0x69: /* Intel */
      /* assume that all Intel chips support CP14 timestamp */
      _oil_profile_stamp = oil_profile_stamp_xscale;
      break;
    case 0x41: /* ARM */
      /* ARM chips are known to not have timestamping available from 
       * user space */
      break;
    default:
      break;
  }

  free (cpuinfo);
#endif
}
#endif


/***** alpha *****/

#if defined(__alpha__)
static unsigned long
oil_profile_stamp_alpha(void)
{
	unsigned int ts;
	__asm__ __volatile__ ("rpcc %0\n" : "=r"(ts));
	return ts;
}

static void
oil_cpu_detect_alpha(void)
{
  _oil_profile_stamp = oil_profile_stamp_alpha;
}
#endif

/***** ia64 *****/

#if defined(__ia64__)
static unsigned long
oil_profile_stamp_ia64(void)
{
	unsigned int ts;
	__asm__ __volatile__("mov %0=ar.itc\n" : "=r"(ts) :: "memory");
	return ts;
}

static void
oil_cpu_detect_ia64(void)
{
  _oil_profile_stamp = oil_profile_stamp_ia64;
}
#endif

/***** s390 *****/

#if defined(__s390__)
static unsigned long
oil_profile_stamp_s390(void)
{
	uint64_t ts;
	__asm__ __volatile__ ("STCK %0\n" : : "m" (ts));
	return ts;
}

static void
oil_cpu_detect_s390(void)
{
  _oil_profile_stamp = oil_profile_stamp_s390;
}
#endif

/***** mips *****/

#if defined(__mips__)
#if 0
/* broken */
static unsigned long
oil_profile_stamp_mips(void)
{
	unsigned int ts;
	__asm__ __volatile__ (
		"	.set	push		\n"
		"	.set	reorder		\n"
		"	mfc0	%0,$9		\n"
		"	.set	pop		\n"
	: "=m" (ts));
	return ts;
}
#endif

static void
oil_cpu_detect_mips(void)
{
  //_oil_profile_stamp = oil_profile_stamp_mips;
}
#endif

static void
oil_cpu_detect_arch(void)
{
#if defined(__i386__) || defined(__amd64__)
  oil_cpu_detect_i386();
#endif
#if defined(__powerpc__) || defined(__PPC__) || defined(__ppc__)
  oil_cpu_detect_powerpc();
#endif
#ifdef __arm__
  oil_cpu_detect_arm();
#endif
#ifdef __alpha__
  oil_cpu_detect_alpha();
#endif
#ifdef __ia64__
  oil_cpu_detect_ia64();
#endif
#ifdef __s390__
  oil_cpu_detect_s390();
#endif
#ifdef __mips__
  oil_cpu_detect_mips();
#endif
}


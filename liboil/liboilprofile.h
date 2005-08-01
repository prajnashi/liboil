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

#ifndef _LIBOIL_PROFILE_H_
#define _LIBOIL_PROFILE_H_

#include <stdint.h>

#define OIL_PROFILE_HIST_LENGTH 10

typedef struct _OilProfile OilProfile;
struct _OilProfile {
  unsigned long start;
  unsigned long stop;
  unsigned long min;
  unsigned long last;
  unsigned long total;
  int n;

  int hist_n;
  unsigned long hist_time[OIL_PROFILE_HIST_LENGTH];
  int hist_count[OIL_PROFILE_HIST_LENGTH];
};

#if defined(__i386__)

static inline unsigned long oil_profile_stamp(void)
{
	unsigned long ts;
	__asm__ __volatile__("rdtsc\n" : "=a" (ts) : : "edx");
	return ts;
}

#elif defined(__amd64__)

static inline unsigned long oil_profile_stamp(void)
{
	unsigned long ts;
	__asm__ __volatile__("rdtsc\n" : "=a" (ts) : : "edx");
	return ts;
}

#elif defined(__powerpc__)

static inline unsigned long oil_profile_stamp(void)
{
	unsigned long ts;
	__asm__ __volatile__("mftb %0\n" : "=r" (ts));
	return ts;
}

#elif defined(__alpha__)

static inline unsigned long oil_profile_stamp(void)
{
	unsigned int ts;
	__asm__ __volatile__ ("rpcc %0\n" : "=r"(ts));
	return ts;
}

#elif defined(__ia64__)

static inline unsigned long oil_profile_stamp(void)
{
	unsigned int ts;
	__asm__ __volatile__("mov %0=ar.itc\n" : "=r"(ts) :: "memory");
	return ts;
}

#elif defined(__s390__)

static inline unsigned long oil_profile_stamp(void)
{
	uint64_t ts;
	__asm__ __volatile__ ("STCK %0\n" : "=m" (ts));
	return ts;
}

#elif defined(__mips__) && 0

/* broken */
static inline unsigned long oil_profile_stamp(void)
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

#elif defined(__arm__)

/* untested */
static inline unsigned long oil_profile_stamp(void)
{
  unsigned int ts;
  /* this only works for XScale 255 */
  __asm__ __volatile__ (
      "  mrc p14, 0, %0, c0, c0, 0 \n"
      : "=r" (ts));
  return ts;
}

#else

#define oil_profile_stamp() oil_profile_stamp_gtod()

#endif

unsigned long oil_profile_stamp_gtod (void);
void oil_profile_init(OilProfile *prof);
void oil_profile_stop_handle(OilProfile *prof);
void oil_profile_get_ave_std (OilProfile *prof, double *ave_p, double *std_p);

#define oil_profile_start(x) do{ \
	(x)->start = oil_profile_stamp(); \
}while(0)
#define oil_profile_stop(x) do{ \
	(x)->stop = oil_profile_stamp(); \
        oil_profile_stop_handle(x); \
}while(0)


#endif


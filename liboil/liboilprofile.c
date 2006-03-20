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
#include <config.h>
#endif

#include <liboil/liboilprofile.h>
#include <liboil/liboildebug.h>

#include <sys/time.h>
#include <time.h>
#include <string.h>
#include <math.h>

/**
 * SECTION:liboilprofile
 * @title:OilProfile
 * @short_description:
 * Measuring the length of time needed to execute Liboil functions.
 *
 */

/**
 * oil_profile_init:
 * @prof: the OilProfile structure
 *
 * Initializes a profiling structure.
 */
void
oil_profile_init (OilProfile *prof)
{
  memset(prof, 0, sizeof(OilProfile));

  prof->min = -1;

}

/**
 * oil_profile_stop_handle:
 * @prof: the OilProfile structure
 *
 * Handles post-processing of a single profiling run.
 *
 * FIXME: need more info
 */
void
oil_profile_stop_handle (OilProfile *prof)
{
  int i;

  prof->last = prof->stop - prof->start;

  prof->total += prof->last;
  prof->n++;

  if (prof->last < prof->min) prof->min = prof->last;
  
  for(i=0;i<prof->hist_n;i++) {
    if (prof->last == prof->hist_time[i]) {
      prof->hist_count[i]++;
      break;
    }
  }
  if (i == prof->hist_n && prof->hist_n < OIL_PROFILE_HIST_LENGTH) {
    prof->hist_time[prof->hist_n] = prof->last;
    prof->hist_count[prof->hist_n] = 1;
    prof->hist_n++;
  }
}

/**
 * oil_profile_get_ave_std:
 * @prof: the OilProfile structure
 * @ave_p: pointer to average
 * @std_p: pointer to standard deviation
 *
 * Calculates the average and standard deviation of a number of
 * profiling runs, and places the results in the locations
 * provided by @ave_p and @std_p.  Either @ave_p and @std_p may
 * be NULL, in which case the values will not be written.
 */
void
oil_profile_get_ave_std (OilProfile *prof, double *ave_p, double *std_p)
{
  double ave;
  double std;
  int max_i;
  double off;
  double s;
  double s2;
  int i;
  int n;
  double x;

  do {
    s = s2 = 0;
    n = 0;
    max_i = -1;
    for(i=0;i<10;i++){
      x = prof->hist_time[i];
      s2 += x * x * prof->hist_count[i];
      s += x * prof->hist_count[i];
      n += prof->hist_count[i];
      if (prof->hist_count[i] > 0) {
        if (max_i == -1 || prof->hist_time[i] > prof->hist_time[max_i]) {
          max_i = i;
        }
      }
    }

    ave = s / n;
    std = sqrt (s2 - s * s / n + n*n) / (n-1);
    off = (prof->hist_time[max_i] - ave)/std;

    if (off > 4.0) {
      prof->hist_count[max_i] = 0;
    }
  } while (off > 4.0);

  if (ave_p) *ave_p = ave;
  if (std_p) *std_p = std;
}



#if defined(__i386__) || defined(__amd64__)
static unsigned long
oil_profile_stamp_tsc(void)
{
	unsigned long ts;
	__asm__ __volatile__("rdtsc\n" : "=a" (ts) : : "edx");
	return ts;
}
#endif

#if defined(__powerpc__)
static unsigned long
oil_profile_stamp_tb(void)
{
	unsigned long ts;
	__asm__ __volatile__("mftb %0\n" : "=r" (ts));
	return ts;
}
#endif

#if defined(__alpha__)
static unsigned long
oil_profile_stamp_counter(void)
{
	unsigned int ts;
	__asm__ __volatile__ ("rpcc %0\n" : "=r"(ts));
	return ts;
}
#endif

#if defined(__ia64__)
static unsigned long
oil_profile_stamp_counter(void)
{
	unsigned int ts;
	__asm__ __volatile__("mov %0=ar.itc\n" : "=r"(ts) :: "memory");
	return ts;
}
#endif

#if defined(__s390__)
static unsigned long
oil_profile_stamp_counter(void)
{
	uint64_t ts;
	__asm__ __volatile__ ("STCK %0\n" : : "m" (ts));
	return ts;
}
#endif

#if defined(__mips__) && 0
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

#if defined(__arm__)
/* untested */
static unsigned long
oil_profile_stamp_xscale255(void)
{
  unsigned int ts;
  /* this only works for XScale 255 */
  __asm__ __volatile__ (
      "  mrc p14, 0, %0, c0, c0, 0 \n"
      : "=r" (ts));
  return ts;
}
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

static unsigned long (*_oil_profile_stamp)(void);

/**
 * oil_profile_stamp:
 *
 * Creates a timestamp based on a CPU-specific high-frequency
 * counter, if available.
 *
 * Returns: a timestamp
 */
unsigned long
oil_profile_stamp (void)
{
  return _oil_profile_stamp();
}

void
_oil_profile_init (void)
{
  int gtod_warn = 1;

#if defined(__i386__)
#define have_tsc 1
  if (have_tsc) {
    _oil_profile_stamp = oil_profile_stamp_tsc;
  }
#endif
#if defined(__amd64__)
  _oil_profile_stamp = oil_profile_stamp_tsc;
#endif
#if defined(__powerpc__)
  _oil_profile_stamp = oil_profile_stamp_tb;
#endif
#if defined(__alpha__)
  _oil_profile_stamp = oil_profile_stamp_counter;
#endif
#if defined(__ia64__)
  _oil_profile_stamp = oil_profile_stamp_counter;
#endif
#if defined(__s390__)
  _oil_profile_stamp = oil_profile_stamp_counter;
#endif
#if defined(__mips__) && 0
  _oil_profile_stamp = oil_profile_stamp_mips;
#endif
#if defined(__arm__)
  {
    unsigned int id;
    __asm__ __volatile__ (
        "  mrc p15, 0, %0, c0, c0, 0 \n"
        : "=r" (id));
    switch(id >> 24) {
      case 0x69: /* Intel */
        /* assume that all Intel chips support CP14 timestamp */
        _oil_profile_stamp = oil_profile_stamp_xscale255;
        break;
      case 0x41: /* ARM */
        /* ARM chips are known to not have timestamping available from 
         * user space */
        gtod_warn = 0;
        break;
      default:
        break;
    }
  }
#endif

#ifdef HAVE_GETTIMEOFDAY
  if (_oil_profile_stamp == NULL) {
    _oil_profile_stamp = oil_profile_stamp_gtod;
    if (gtod_warn) {
      OIL_ERROR("Using gettimeofday() as a timestamp function.  Please add a timestamp function for your platform.");
    }
  }
#endif

  if (_oil_profile_stamp == NULL) {
    _oil_profile_stamp = oil_profile_stamp_zero;
    OIL_ERROR("No timestamping function.  This is kinda bad.");
  }
}


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

#ifndef _LIBOIL_PROFILE_H_
#define _LIBOIL_PROFILE_H_

typedef struct _OilProfile OilProfile;
struct _OilProfile {
	unsigned long tsum;
	unsigned long tmin;
	unsigned long t;
	unsigned long count;
};

#if defined(__i386__)

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
	unsigned int ts;
	__asm__ __volatile__ ("STCK %0\n" : "=m" (ts));
	return ts;
}

#elif defined(__mips__)

/* untested */
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

#else

#include <sys/time.h>
#include <time.h>

static inline unsigned long oil_profile_stamp(void)
{
	struct timeval tv;
	gettimeofday(&tv,NULL);
	return 1000000*(unsigned long)tv.tv_sec + (unsigned long)tv.tv_usec;
}

#endif

#define oil_profile_init(x) do{ (x).tsum = 0; (x).tmin=-1; (x).count=0; }while(0)
#define oil_profile_start(x) do{ \
	(x).t = -oil_profile_stamp(); \
	__asm__ __volatile__("\n###### START PROFILE"); \
}while(0)
#define oil_profile_stop(x) do{ \
	__asm__ __volatile__("\n###### STOP PROFILE"); \
	(x).t += oil_profile_stamp(); \
	if((x).t<(x).tmin)(x).tmin=(x).t; \
	(x).tsum += (x).t; \
	(x).count ++; \
}while(0)
#define oil_profile_print(x) do{ \
	printf("P: %lu %lu %lu %lu\n",(x).tsum,(x).tmin,(x).t,(x).count); \
}while(0)


#endif


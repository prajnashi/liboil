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
#include <config.h>
#endif

#include <liboil/liboilprofile.h>

#include <sys/time.h>
#include <time.h>
#include <string.h>

#ifdef OIL_PROFILE_USING_GTOD
unsigned long oil_profile_stamp_gtod (void)
{
	struct timeval tv;
	gettimeofday(&tv,NULL);
	return 1000000*(unsigned long)tv.tv_sec + (unsigned long)tv.tv_usec;
}

#endif

void
oil_profile_init (OilProfile *prof)
{
  memset(prof, 0, sizeof(OilProfile));

  prof->min = -1;

}

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


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
#include <math.h>

unsigned long oil_profile_stamp_gtod (void)
{
	struct timeval tv;
	gettimeofday(&tv,NULL);
	return 1000000*(unsigned long)tv.tv_sec + (unsigned long)tv.tv_usec;
}

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



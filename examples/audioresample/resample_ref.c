/*
 * LIBOIL - Library of Optimized Inner Loops
 * Copyright (c) 2001,2006 David A. Schleef <ds@schleef.org>
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


#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <liboil/liboil.h>

#include <audioresample/resample.h>
#include <audioresample/buffer.h>
#include <audioresample/debug.h>


static double
resample_sinc_window (double x, double halfwidth, double scale)
{
  double y;

  if (x == 0) return 1.0;
  if (x < -halfwidth || x > halfwidth) return 0.0;

  y = sin(x * M_PI * scale)/(x * M_PI * scale) * scale;

  x /= halfwidth;
  y *= (1 - x * x) * (1 - x * x);

  return y;
}

void
resample_scale_ref (ResampleState * r)
{
  if (r->need_reinit) {
    r->sample_size = r->n_channels * resample_format_size(r->format);
    OIL_DEBUG("sample size %d", r->sample_size);

    if (r->buffer) free(r->buffer);
    r->buffer_len = r->sample_size * r->filter_length;
    r->buffer = malloc(r->buffer_len);
    memset (r->buffer, 0, r->buffer_len);

    r->i_inc = r->o_rate / r->i_rate;
    r->o_inc = r->i_rate / r->o_rate;
    OIL_DEBUG("i_inc %g o_inc %g", r->i_inc, r->o_inc);

    r->i_start = - r->i_inc * r->filter_length;

    r->need_reinit = 0;

#if 0
    if (r->i_inc < 1.0) {
      r->sinc_scale = r->i_inc;
      if (r->sinc_scale == 0.5) {
        /* strange things happen at integer multiples */
        r->sinc_scale = 1.0;
      }
    } else {
      r->sinc_scale = 1.0;
    }
#else
    r->sinc_scale = 1.0;
#endif
  }

  while(r->o_size > 0) {
    double midpoint;
    int i;
    int j;

    OIL_DEBUG("i_start %g", r->i_start);
    midpoint = r->i_start + (r->filter_length - 1)*0.5*r->i_inc;
    if (midpoint > 0.5 * r->i_inc) {
      OIL_ERROR("inconsistent state");
    }
    while (midpoint < -0.5 * r->i_inc) {
      AudioresampleBuffer *buffer;

      buffer = audioresample_buffer_queue_pull (r->queue, r->sample_size);
      if (buffer == NULL) {
        OIL_ERROR("buffer_queue_pull returned NULL");
        return;
      }

      r->i_start += r->i_inc;
      OIL_DEBUG("pulling (i_start = %g)", r->i_start);

      midpoint += r->i_inc;
      memmove (r->buffer, r->buffer + r->sample_size,
          r->buffer_len - r->sample_size);

      memcpy (r->buffer + r->buffer_len - r->sample_size, buffer->data,
          r->sample_size);
      audioresample_buffer_unref (buffer);
    }

    switch(r->format) {
      case RESAMPLE_FORMAT_S16:
        for (i=0;i<r->n_channels;i++) {
          double acc = 0;
          double offset;
          double x;

          for(j=0;j<r->filter_length;j++){
            offset = (r->i_start + j * r->i_inc) * r->o_inc;
            x = *(int16_t *)(r->buffer + i * sizeof(int16_t) + j*r->sample_size);
            acc += resample_sinc_window (offset, r->filter_length * 0.5, r->sinc_scale) * x;
          }
          if(acc<-32768.0)acc=-32768.0;
          if(acc>32767.0)acc=32767.0;

          *(int16_t *)(r->o_buf + i * sizeof(int16_t)) = rint(acc);
        }
        break;
      case RESAMPLE_FORMAT_S32:
        for (i=0;i<r->n_channels;i++) {
          double acc = 0;
          double offset;
          double x;

          for(j=0;j<r->filter_length;j++){
            offset = (r->i_start + j * r->i_inc) * r->o_inc;
            x = *(int32_t *)(r->buffer + i * sizeof(int32_t) + j*r->sample_size);
            acc += resample_sinc_window (offset, r->filter_length * 0.5, r->sinc_scale) * x;
          }
          if(acc<-2147483648.0)acc=-2147483648.0;
          if(acc>2147483647.0)acc=2147483647.0;

          *(int32_t *)(r->o_buf + i * sizeof(int32_t)) = rint(acc);
        }
        break;
      case RESAMPLE_FORMAT_F32:
        for (i=0;i<r->n_channels;i++) {
          double acc = 0;
          double offset;
          double x;

          for(j=0;j<r->filter_length;j++){
            offset = (r->i_start + j * r->i_inc) * r->o_inc;
            x = *(float *)(r->buffer + i * sizeof(float) + j*r->sample_size);
            acc += resample_sinc_window (offset, r->filter_length * 0.5, r->sinc_scale) * x;
          }

          *(float *)(r->o_buf + i * sizeof(float)) = acc;
        }
        break;
      case RESAMPLE_FORMAT_F64:
        for (i=0;i<r->n_channels;i++) {
          double acc = 0;
          double offset;
          double x;

          for(j=0;j<r->filter_length;j++){
            offset = (r->i_start + j * r->i_inc) * r->o_inc;
            x = *(double *)(r->buffer + i * sizeof(double) + j*r->sample_size);
            acc += resample_sinc_window (offset, r->filter_length * 0.5, r->sinc_scale) * x;
          }

          *(double *)(r->o_buf + i * sizeof(double)) = acc;
        }
        break;
    }

    r->i_start -= 1.0;
    r->o_buf += r->sample_size;
    r->o_size -= r->sample_size;
  }

}


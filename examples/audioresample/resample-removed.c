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
#include <liboil/liboil.h>

#include <audioresample/resample.h>
#include <audioresample/buffer.h>
#include <audioresample/debug.h>

static void resample_scale (ResampleState * r);
static void resample_scale_ref (ResampleState * r);
//static void resample_scale_2 (ResampleState * r);
//static void resample_sinc (ResampleState * r);
static void resample_sinc_ft (ResampleState * r);

void
resample_init (void)
{
  static int inited = 0;
  if (!inited) {
    oil_init ();
    inited = 1;
  }
}

ResampleState *
resample_new (void)
{
  ResampleState *r;

  r = malloc(sizeof(ResampleState));
  memset(r,0,sizeof(ResampleState));

  r->filter_length = 16;

  r->i_start = 0;
  if (r->filter_length & 1) {
    r->o_start = 0;
  } else {
    r->o_start = r->o_inc * 0.5;
  }

  r->queue = audioresample_buffer_queue_new();
  r->out_tmp = malloc(10000*sizeof(double));

  r->need_reinit = 1;

  return r;
}

void
resample_free (ResampleState * r)
{
  if (r->buffer) {
    free (r->buffer);
  }
  if (r->ft) {
    functable_free (r->ft);
  }
  if (r->queue) {
    audioresample_buffer_queue_free (r->queue);
  }
  if (r->out_tmp) {
    free (r->out_tmp);
  }

  free (r);
}

#if 0
void
resample_setup (ResampleState * r)
{
  /* i_inc is the number of samples that the output increments for
   * each input sample.  o_inc is the opposite. */
  r->i_inc = (double) r->o_rate / r->i_rate;
  r->o_inc = (double) r->i_rate / r->o_rate;

  r->halftaps = (r->filter_length - 1.0) * 0.5;

  if (r->format == RESAMPLE_FORMAT_S16) {
    r->sample_size = 2;
  } else if (r->format == RESAMPLE_FORMAT_F32) {
    r->sample_size = 4;
  } else {
    OIL_ERROR ("Unexpected format \"%d\"", r->format);
  }
}
#endif

static void
resample_buffer_free(AudioresampleBuffer *buffer, void *priv)
{
  ((void (*)(void *))buffer->priv2)(buffer->priv);
}

void
resample_add_input_data (ResampleState * r, void *data, int size,
    void (*free_func)(void *), void *closure)
{
  AudioresampleBuffer *buffer;

  OIL_DEBUG ("data %p size %d", data, size);

  buffer = audioresample_buffer_new_with_data (data, size);
  buffer->free = resample_buffer_free;
  buffer->priv2 = free_func;
  buffer->priv = closure;

  audioresample_buffer_queue_push (r->queue, buffer);
}

void
resample_input_eos (ResampleState *r)
{
  AudioresampleBuffer *buffer;
  int sample_size;

  sample_size = r->n_channels * resample_format_size(r->format);

  buffer = audioresample_buffer_new_and_alloc (sample_size *
      (r->filter_length / 2));
  memset(buffer->data, 0, buffer->length);

  audioresample_buffer_queue_push (r->queue, buffer);

  r->eos = 1;
}

int
resample_get_output_size (ResampleState *r)
{
  return floor (audioresample_buffer_queue_get_depth(r->queue) * r->o_rate / r->i_rate);
}

int
resample_get_output_data (ResampleState *r, void *data, int size)
{
  r->o_buf = data;
  r->o_size = size;

  switch (r->method) {
    case 0:
      resample_scale_ref (r);
      break;
    case 1:
      resample_scale (r);
      break;
    default:
      break;
  }

  return size - r->o_size;
}

void
resample_set_filter_length (ResampleState *r, int length)
{
  r->filter_length = length;
  r->need_reinit = 1;
}

void
resample_set_input_rate (ResampleState *r, double rate)
{
  r->i_rate = rate;
  r->need_reinit = 1;
}

void
resample_set_output_rate (ResampleState *r, double rate)
{
  r->o_rate = rate;
  r->need_reinit = 1;
}

void
resample_set_n_channels (ResampleState *r, int n_channels)
{
  r->n_channels = n_channels;
  r->need_reinit = 1;
}

void
resample_set_format (ResampleState *r, ResampleFormat format)
{
  r->format = format;
  r->need_reinit = 1;
}

int
resample_format_size (ResampleFormat format)
{
  switch(format){
    case RESAMPLE_FORMAT_S16:
      return 2;
    case RESAMPLE_FORMAT_S32:
    case RESAMPLE_FORMAT_F32:
      return 4;
    case RESAMPLE_FORMAT_F64:
      return 8;
  }
  return 0;
}

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

static void
resample_scale_ref (ResampleState * r)
{
  if (r->need_reinit) {
    r->sample_size = r->n_channels * resample_format_size(r->format);
    //OIL_ERROR("sample size %d", r->sample_size);

    if (r->buffer) free(r->buffer);
    r->buffer_len = r->sample_size * r->filter_length;
    r->buffer = malloc(r->buffer_len);
    memset (r->buffer, 0, r->buffer_len);

    r->i_inc = r->o_rate / r->i_rate;
    r->o_inc = r->i_rate / r->o_rate;
    //OIL_ERROR("i_inc %g o_inc %g", r->i_inc, r->o_inc);

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

    //OIL_ERROR("i_start %g", r->i_start);
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
      //OIL_ERROR("pulling (i_start = %g)", r->i_start);

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
          /* FIXME */
          //if(acc<32768.0)acc=-32768.0;
          //if(acc>32767.0)acc=32767.0;

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

          *(double *)(r->o_buf + i * sizeof(double)) = rint(acc);
        }
        break;
    }

    r->i_start -= 1.0;
    r->o_buf += r->sample_size;
    r->o_size -= r->sample_size;
  }

}

#if 0
static void
resample_scale_2 (ResampleState * r)
{
  /* only downsamples by a factor of 2, with no filtering */

  while(r->o_size > 0) {
    AudioresampleBuffer *buffer;

    buffer = audioresample_buffer_queue_pull (r->queue, 2 * r->sample_size);
    if (buffer == NULL) {
      return;
    }

    memcpy(r->o_buf, buffer->data, r->sample_size);

    r->o_buf += r->sample_size;
    r->o_size -= r->sample_size;
  }

}
#endif

/*
 * Prepare to be confused.
 *
 * We keep a "timebase" that is based on output samples.  The zero
 * of the timebase corresponds to the next output sample that will
 * be written.
 *
 * i_start is the "time" that corresponds to the first input sample
 * in an incoming buffer.  Since the output depends on input samples
 * ahead in time, i_start will tend to be around halftaps.
 *
 * i_start_buf is the time of the first sample in the temporary
 * buffer.
 */
static void
resample_scale (ResampleState * r)
{
  int o_size;

  r->i_samples = 100 / r->sample_size / r->n_channels;

  //r->i_start_buf = r->i_start - r->filter_length * r->i_inc;

  /* i_start is the offset (in a given output sample) that is the
   * beginning of the current input buffer */
  r->i_end = r->i_start + r->i_inc * r->i_samples;

  r->o_samples = floor (r->i_end - r->halftaps * r->i_inc);

  o_size = r->o_samples * r->n_channels * r->sample_size;

  OIL_DEBUG ("i_samples=%d o_samples=%d i_inc=%g o_buf=%p",
      r->i_samples, r->o_samples, r->i_inc, r->o_buf);
  OIL_DEBUG ("resample_scale: i_start=%g i_end=%g o_start=%g",
      r->i_start, r->i_end, r->o_start);

  if ((r->filter_length + r->i_samples) * r->sample_size * r->n_channels >
      r->buffer_len) {
    int size = (r->filter_length + r->i_samples) * sizeof (double) * 2;

    OIL_DEBUG ("resample temp buffer size=%d", size);
    if (r->buffer)
      free (r->buffer);
    r->buffer_len = size;
    r->buffer = malloc (size);
    memset (r->buffer, 0, size);
  }

  if (r->format == RESAMPLE_FORMAT_S16) {
    if (r->n_channels == 2) {
      oil_conv_f64_s16 (r->buffer + r->filter_length * sizeof (double) * 2,
	  sizeof (double), r->i_buf, sizeof (short), r->i_samples * 2);
    } else {
      oil_conv_f64_s16 (r->buffer + r->filter_length * sizeof (double) * 2,
	  sizeof (double) * 2, r->i_buf, sizeof (short), r->i_samples);
    }
  } else if (r->format == RESAMPLE_FORMAT_F32) {
    if (r->n_channels == 2) {
      oil_conv_f64_f32 (r->buffer + r->filter_length * sizeof (double) * 2,
          sizeof (double), r->i_buf, sizeof (float), r->i_samples * 2);
    } else {
      oil_conv_f64_f32 (r->buffer + r->filter_length * sizeof (double) * 2,
          sizeof (double) * 2, r->i_buf, sizeof (float), r->i_samples);
    }
  }

  resample_sinc_ft (r);

  if (r->format == RESAMPLE_FORMAT_S16) {
    if (r->n_channels == 2) {
      oil_conv_s16_f64 (r->o_buf, sizeof(short), r->out_tmp, sizeof(double),
          2 * r->o_samples);
    } else {
      oil_conv_s16_f64 (r->o_buf, sizeof(short), r->out_tmp, sizeof(double) * 2,
          r->o_samples);
    }
  } else if (r->format == RESAMPLE_FORMAT_F32) {
    if (r->n_channels == 2) {
      oil_conv_f32_f64 (r->o_buf, sizeof(float), r->out_tmp, sizeof(double),
          2 * r->o_samples);
    } else {
      oil_conv_f32_f64 (r->o_buf, sizeof(float), r->out_tmp, sizeof(double) * 2,
          r->o_samples);
    }
  }

  memcpy (r->buffer,
      r->buffer + r->i_samples * sizeof (double) * 2,
      r->filter_length * sizeof (double) * 2);

  /* updating times */
  r->i_start += r->i_samples * r->i_inc;
  r->o_start += r->o_samples * r->o_inc - r->i_samples;

  /* adjusting timebase zero */
  r->i_start -= r->o_samples;
}

#if 0
static void
resample_sinc_slow (ResampleState * r)
{
  signed short *i_ptr, *o_ptr;
  int i, j;
  double c0, c1;
  double a;
  int start;
  double center;
  double weight;

  if (!r->buffer) {
    int size = r->filter_length * 2 * r->n_channels;

    OIL_DEBUG ("resample temp buffer");
    r->buffer = malloc (size);
    memset (r->buffer, 0, size);
  }

  i_ptr = (signed short *) r->i_buf;
  o_ptr = (signed short *) r->o_buf;

  a = r->i_start;
#define GETBUF(index,chan) (((index)<0) \
			? ((short *)(r->buffer))[((index)+r->filter_length)*2+(chan)] \
			: i_ptr[(index)*2+(chan)])
  {
    double sinx, cosx, sind, cosd;
    double x, d;
    double t;

    for (i = 0; i < r->o_samples; i++) {
      start = floor (a) - r->filter_length;
      center = a - r->halftaps;
      x = M_PI * (start - center) * r->o_inc;
      sinx = sin (M_PI * (start - center) * r->o_inc);
      cosx = cos (M_PI * (start - center) * r->o_inc);
      d = M_PI * r->o_inc;
      sind = sin (M_PI * r->o_inc);
      cosd = cos (M_PI * r->o_inc);
      c0 = 0;
      c1 = 0;
      for (j = 0; j < r->filter_length; j++) {
	weight = (x == 0) ? 1 : (sinx / x);
	c0 += weight * GETBUF ((start + j), 0);
	c1 += weight * GETBUF ((start + j), 1);
	t = cosx * cosd - sinx * sind;
	sinx = cosx * sind + sinx * cosd;
	cosx = t;
	x += d;
      }
      o_ptr[0] = rint (c0);
      o_ptr[1] = rint (c1);
      o_ptr += 2;
      a += r->o_inc;
    }
  }
#undef GETBUF

  memcpy (r->buffer,
      i_ptr + (r->i_samples - r->filter_length) * r->n_channels,
      r->filter_length * 2 * r->n_channels);
}

static signed short
double_to_s16 (double x)
{
  if (x < -32768) {
    return -32768;
  }
  if (x > 32767) {
    return -32767;
  }
  return rint (x);
}

/* only works for channels == 2 ???? */
static void
resample_sinc (ResampleState * r)
{
  double *ptr;
  signed short *o_ptr;
  int i, j;
  double c0, c1;
  double a;
  int start;
  double center;
  double weight;
  double x0, x, d;
  double scale;

  ptr = (double *) r->buffer;
  o_ptr = (signed short *) r->o_buf;

  /* scale provides a cutoff frequency for the low
   * pass filter aspects of sinc().  scale=M_PI
   * will cut off at the input frequency, which is
   * good for up-sampling, but will cause aliasing
   * for downsampling.  Downsampling needs to be
   * cut off at o_rate, thus scale=M_PI*r->i_inc. */
  /* actually, it needs to be M_PI*r->i_inc*r->i_inc.
   * Need to research why. */
  scale = M_PI * r->i_inc;
  for (i = 0; i < r->o_samples; i++) {
    a = r->o_start + i * r->o_inc;
    start = floor (a - r->halftaps);
    center = a;
    /*x = M_PI * (start - center) * r->o_inc; */
    /*d = M_PI * r->o_inc; */
    /*x = (start - center) * r->o_inc; */
    x0 = (start - center) * r->o_inc;
    d = r->o_inc;
    c0 = 0;
    c1 = 0;
    for (j = 0; j < r->filter_length; j++) {
      x = x0 + d * j;
      weight = sinc (x * scale * r->i_inc) * scale / M_PI;
      weight *= window_func (x / r->halftaps * r->i_inc);
      c0 += weight * ptr[(start + j + r->filter_length) * 2 + 0];
      c1 += weight * ptr[(start + j + r->filter_length) * 2 + 1];
    }
    o_ptr[0] = double_to_s16 (c0);
    o_ptr[1] = double_to_s16 (c1);
    o_ptr += 2;
  }
}
#endif

/*
 * Resampling audio is best done using a sinc() filter.
 *
 *
 *  out[t] = Sum( in[t'] * sinc((t-t')/delta_t), all t')
 *
 * The immediate problem with this algorithm is that it involves a
 * sum over an infinite number of input samples, both in the past
 * and future.  Note that even though sinc(x) is bounded by 1/x,
 * and thus decays to 0 for large x, since sum(x,{x=0,1..,n}) diverges
 * as log(n), we need to be careful about convergence.  This is
 * typically done by using a windowing function, which also makes
 * the sum over a finite number of input samples.
 *
 * The next problem is computational:  sinc(), and especially
 * sinc() multiplied by a non-trivial windowing function is expensive
 * to calculate, and also difficult to find SIMD optimizations.  Since
 * the time increment on input and output is different, it is not
 * possible to use a FIR filter, because the taps would have to be
 * recalculated for every t.
 *
 * To get around the expense of calculating sinc() for every point,
 * we pre-calculate sinc() at a number of points, and then interpolate
 * for the values we want in calculations.  The interpolation method
 * chosen is bi-cubic, which requires both the evalated function and
 * its derivative at every pre-sampled point.  Also, if the sampled
 * points are spaced commensurate with the input delta_t, we notice
 * that the interpolating weights are the same for every input point.
 * This decreases the number of operations to 4 multiplies and 4 adds
 * for each tap, regardless of the complexity of the filtering function.
 * 
 * At this point, it is possible to rearrange the problem as the sum
 * of 4 properly weghted FIR filters.  Typical SIMD computation units
 * are highly optimized for FIR filters, making long filter lengths
 * reasonable.
 */

static void
resample_sinc_ft (ResampleState * r)
{
  double *ptr;
  signed short *o_ptr;
  int i;

  /*int j; */
  double c0, c1;

  /*double a; */
  double start_f, start_x;
  int start;
  double center;

  /*double weight; */
  double x, d;
  double scale;
  int n = 4;

  scale = r->i_inc;		/* cutoff at 22050 */
  /*scale = 1.0;          // cutoff at 24000 */
  /*scale = r->i_inc * 0.5;       // cutoff at 11025 */

  if (!r->ft) {
    r->ft = functable_new ();

    r->ft->len = (r->filter_length + 2) * n;
    r->ft->offset = 1.0 / n;
    r->ft->start = -r->ft->len * 0.5 * r->ft->offset;

    r->ft->func_x = functable_sinc;
    r->ft->func_dx = functable_dsinc;
    r->ft->scale = M_PI * scale;

    r->ft->func2_x = functable_window_std;
    r->ft->func2_dx = functable_window_dstd;
    r->ft->scale2 = 1.0 / r->halftaps;

    functable_setup (r->ft);
  }

  ptr = r->buffer;
  o_ptr = (signed short *) r->o_buf;

  center = r->o_start;
  start_x = center - r->halftaps;
  start_f = floor (start_x);
  start_x -= start_f;
  start = start_f;
  for (i = 0; i < r->o_samples; i++) {
    /*start_f = floor(center - r->halftaps); */
    x = start_f - center;
    d = 1;
    c0 = 0;
    c1 = 0;
/*#define slow */
#ifdef slow
    for (j = 0; j < r->filter_length; j++) {
      weight = functable_eval (r->ft, x) * scale;
      /*weight = sinc(M_PI * scale * x)*scale*r->i_inc; */
      /*weight *= window_func(x / r->halftaps); */
      c0 += weight * ptr[(start + j + r->filter_length) * 2 + 0];
      c1 += weight * ptr[(start + j + r->filter_length) * 2 + 1];
      x += d;
    }
#else
    functable_fir2 (r->ft,
	&c0, &c1, x, n, ptr + (start + r->filter_length) * 2, r->filter_length);
    c0 *= scale;
    c1 *= scale;
#endif

    r->out_tmp[2 * i + 0] = c0;
    r->out_tmp[2 * i + 1] = c1;
    center += r->o_inc;
    start_x += r->o_inc;
    while (start_x >= 1.0) {
      start_f++;
      start_x -= 1.0;
      start++;
    }
  }

}


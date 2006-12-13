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

void resample_scale_ref (ResampleState * r);
void resample_scale_functable (ResampleState * r);



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

static void
resample_buffer_free(AudioresampleBuffer *buffer, void *priv)
{
  if(buffer->priv2) {
    ((void (*)(void *))buffer->priv2)(buffer->priv);
  }
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
      resample_scale_functable (r);
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

void
resample_set_method (ResampleState *r, int method)
{
  r->method = method;
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


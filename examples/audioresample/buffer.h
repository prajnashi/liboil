/*
 * LIBOIL - Library of Optimized Inner Loops
 * Copyright (c) 2006 David A. Schleef <ds@schleef.org>
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

#ifndef __AUDIORESAMPLE_BUFFER_H__
#define __AUDIORESAMPLE_BUFFER_H__

#include <glib.h>

typedef struct _AudioresampleBuffer AudioresampleBuffer;
typedef struct _AudioresampleBufferQueue AudioresampleBufferQueue;

struct _AudioresampleBuffer
{
  unsigned char *data;
  int length;

  int ref_count;

  AudioresampleBuffer *parent;

  void (*free) (AudioresampleBuffer *, void *);
  void *priv;
  void *priv2;
};

struct _AudioresampleBufferQueue
{
  GList *buffers;
  int depth;
  int offset;
};

AudioresampleBuffer *audioresample_buffer_new (void);
AudioresampleBuffer *audioresample_buffer_new_and_alloc (int size);
AudioresampleBuffer *audioresample_buffer_new_with_data (void *data, int size);
AudioresampleBuffer *audioresample_buffer_new_subbuffer (AudioresampleBuffer * buffer, int offset,
    int length);
void audioresample_buffer_ref (AudioresampleBuffer * buffer);
void audioresample_buffer_unref (AudioresampleBuffer * buffer);

AudioresampleBufferQueue *audioresample_buffer_queue_new (void);
void audioresample_buffer_queue_free (AudioresampleBufferQueue * queue);
int audioresample_buffer_queue_get_depth (AudioresampleBufferQueue * queue);
int audioresample_buffer_queue_get_offset (AudioresampleBufferQueue * queue);
void audioresample_buffer_queue_push (AudioresampleBufferQueue * queue,
    AudioresampleBuffer * buffer);
AudioresampleBuffer *audioresample_buffer_queue_pull (AudioresampleBufferQueue * queue, int len);
AudioresampleBuffer *audioresample_buffer_queue_peek (AudioresampleBufferQueue * queue, int len);

#endif

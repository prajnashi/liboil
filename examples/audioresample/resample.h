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

#ifndef __RESAMPLE_H__
#define __RESAMPLE_H__

#include <audioresample/functable.h>
#include <audioresample/buffer.h>

typedef enum {
	RESAMPLE_FORMAT_S16 = 0,
	RESAMPLE_FORMAT_S32,
	RESAMPLE_FORMAT_F32,
        RESAMPLE_FORMAT_F64
} ResampleFormat;

typedef void (*ResampleCallback) (void *);

typedef struct _ResampleState ResampleState;

struct _ResampleState {
	/* parameters */

	int n_channels;
	ResampleFormat format;

	int filter_length;

	double i_rate;
	double o_rate;

        int method;

	/* internal parameters */

        int need_reinit;

	double halftaps;

	/* filter state */

        void *o_buf;
        int o_size;

        AudioresampleBufferQueue *queue;
        int eos;
        int started;

	int sample_size;

	void *buffer;
	int buffer_len;

	double i_start;
	double o_start;

	double i_inc;
	double o_inc;

        double sinc_scale;

	double i_end;
	double o_end;

	int i_samples;
	int o_samples;

	//void *i_buf;

        Functable *ft;

        double *out_tmp;
};

void resample_init(void);
void resample_cleanup(void);

ResampleState *resample_new (void);
void resample_free (ResampleState *state);

void resample_add_input_data (ResampleState * r, void *data, int size,
    ResampleCallback free_func, void *closure);
void resample_input_eos (ResampleState *r);
int resample_get_output_size (ResampleState *r);
int resample_get_output_data (ResampleState *r, void *data, int size);

void resample_set_filter_length (ResampleState *r, int length);
void resample_set_input_rate (ResampleState *r, double rate);
void resample_set_output_rate (ResampleState *r, double rate);
void resample_set_n_channels (ResampleState *r, int n_channels);
void resample_set_format (ResampleState *r, ResampleFormat format);
void resample_set_method (ResampleState *r, int method);
int resample_format_size (ResampleFormat format);


#endif /* __RESAMPLE_H__ */


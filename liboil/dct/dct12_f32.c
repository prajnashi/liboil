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
#include "config.h"
#endif

#include <liboil/liboilfunction.h>
#include <liboil/dct/dct.h>
#include <math.h>


OIL_DEFINE_CLASS (dct12_f32, "float *d_18, int dstr, float *s_18, int sstr");

/* FIXME this appears to be derived from mpglib */
static void
dct12_f32_ref (float *dest, int dstr, float *src, int sstr)
{
     float in0,in1,in2,in3,in4,in5;
     float *out2 = dest;
     float *in = src;
     float COS6_2 = cos(M_PI / 6.0 * 2.0);
     float COS6_1 = cos(M_PI / 6.0 * 1.0);
     float tfcos12[3];
     float wi[12];
     int i;

     for(i=0;i<3;i++){
	     tfcos12[i] = 0.5 / cos ( M_PI * (i*2.0+1.0) / 12.0 );
     }
     for(i=0;i<12;i++){
	     wi[i] = 0.5 * sin( M_PI / 24.0 * (double) (2*i+1) ) / cos ( M_PI * (double) (2*i+7) / 24.0 );
     }

     out2[12]=out2[13]=out2[14]=out2[15]=out2[16]=out2[17]=0.0;

             in5 = in[5*3];
     in5 += (in4 = in[4*3]);
     in4 += (in3 = in[3*3]);
     in3 += (in2 = in[2*3]);
     in2 += (in1 = in[1*3]);
     in1 += (in0 = in[0*3]);
                            
     in5 += in3; in3 += in1;
                            
     in2 *= COS6_1;
     in3 *= COS6_1;

     {
       float tmp0,tmp1 = (in0 - in4);
       {
         float tmp2 = (in1 - in5) * tfcos12[1];
         tmp0 = tmp1 + tmp2;
         tmp1 -= tmp2;
       }
       out2[11-1] = tmp0 * wi[11-1];
       out2[6 +1] = tmp0 * wi[6+1];
       out2[0+1] += tmp1 * wi[1];
       out2[5-1] += tmp1 * wi[5-1];
     }

     in0 += in4 * COS6_2;
                         
     in4 = in0 + in2;    
     in0 -= in2;         
                         
     in1 += in5 * COS6_2;
                         
     in5 = (in1 + in3) * tfcos12[0];
     in1 = (in1 - in3) * tfcos12[2];
                        
     in3 = in4 + in5;    
     in4 -= in5;         
                         
     in2 = in0 + in1;    
     in0 -= in1;

     out2[11-0] = in2 * wi[11-0];
     out2[6 +0] = in2 * wi[6+0];
     out2[6 +2] = in3 * wi[6+2];
     out2[11-2] = in3 * wi[11-2];

     out2[0+0] += in0 * wi[0];
     out2[5-0] += in0 * wi[5-0];
     out2[0+2] += in4 * wi[2];
     out2[5-2] += in4 * wi[5-2];
}

OIL_DEFINE_IMPL_REF (dct12_f32_ref, dct12_f32);


/* FIXME: broken, reads outside src array */
#if 0
static void
dct12_f32_ref1(float *dest, int dstr, float *src, int sstr)
{
	int l,m,k;
	double x;
	double coeff;
	double cos_s;
	double wi[36];
	int i;

	for(i=0;i<12;i++){
		wi[i] = sin( M_PI / 12.0 * (i+0.5));
	}
	for(;i<36;i++){
		wi[i] = 0;
	}

	for(l=0;l<3;l++){
		for(m=0;m<6;m++){
			x = 0;
			for(k=0;k<12;k++){
				cos_s = cos( (M_PI / 24.0) * (2 * k + 7) *
						(2 * m + 1) ) / 3;
				coeff = wi[k] * cos_s;
				x += coeff * src[k + 6*l + 6];
			}
			dest[3*m + l] = x;
		}
	}
}
OIL_DEFINE_IMPL (dct12_f32_ref1, dct12_f32);
#endif


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
#include "config.h"
#endif

#include <liboil/liboilfunction.h>
#include <liboil/dct/dct.h>
#include <math.h>


OIL_DEFINE_CLASS_X (dct12_f32, "float *dest, int dstr, float *src, int sstr");

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

OIL_DEFINE_IMPL_REF (dct12_f32_ref, dct12_f32_class);


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
OIL_DEFINE_IMPL (dct12_f32_ref1, dct12_f32_class);

/* copyright: from mpglib */
/*
 * new DCT12
 */
static void
dct12_f32_mpglib(float *dest, int dstr, float *src, int sstr)
{
     float in0,in1,in2,in3,in4,in5;
     float *out2 = dest;
     float *in = src;
     float wi[12];
     int i;
     float tmp0, tmp1, tmp2;

     float COS6_2 = cos(M_PI / 6.0 * 2.0);
     float COS6_1 = cos(M_PI / 6.0 * 1.0);
     float tfcos12[3];

     for(i=0;i<3;i++){
	     tfcos12[i] = 0.5 / cos ( M_PI * (i*2.0+1.0) / 12.0 );
     }
     for(i=0;i<12;i++){
	     wi[i] = 0.5 * sin( M_PI / 24.0 * (double) (2*i+1) ) / cos ( M_PI * (double) (2*i+7) / 24.0 );
     }

     out2[12]=out2[13]=out2[14]=out2[15]=out2[16]=out2[17]=0.0;

     in5 = in[5*3] + in[4*3];
     in4 = in[4*3] + in[3*3];
     in3 = in[3*3] + in[2*3];
     in2 = in[2*3] + in[1*3];
     in1 = in[1*3] + in[0*3];
     in0 = in[0*3];
                            
     //in5 += in3;
     in5 = in[5*3] + in[4*3] + in[3*3] + in[2*3];
     //in3 += in1;
     in3 = in[3*3] + in[2*3] + in[1*3] + in[0*3];
                            
     //in2 *= COS6_1;
     in2 = COS6_1 * (in[2*3] + in[1*3]);

     //in3 *= COS6_1;
     in3 = COS6_1 * (in[3*3] + in[2*3] + in[1*3] + in[0*3]);

     //tmp1 = (in0 - in4);
     tmp1 = in[0*3] - in[4*3] - in[3*3];

     //tmp2 = (in1 - in5) * tfcos12[1];
     tmp2 = tfcos12[1] * (in[1*3] + in[0*3] - in[5*3] - in[4*3] - in[3*3] - in[2*3]);

     //tmp0 = tmp1 + tmp2;
     tmp0 = (1 + tfcos12[1]) * (in[0*3] - in[4*3] - in[3*3]) +
             tfcos12[1] * (in[1*3] - in[5*3] - in[2*3]);

     //tmp1 -= tmp2;
     tmp1 = (1 - tfcos12[1]) * (in[0*3] - in[4*3] - in[3*3]) -
             tfcos12[1] * (in[1*3] - in[5*3] - in[2*3]);

     out2[11-1] = tmp0 * wi[11-1];
     out2[6 +1] = tmp0 * wi[6+1];
     out2[0+1] += tmp1 * wi[1];
     out2[5-1] += tmp1 * wi[5-1];

     in0 += in4 * COS6_2;
                         
     in4 = in0 + in2;
     in0 -= in2;
                 
     in1 += in5 * COS6_2;
                        
     in5 = (in1 + in3) * tfcos12[0];
     in1 = (in1 - in3) * tfcos12[2];
                       
     in3 = in4 + in5;
     in4 = in4 - in5;
               
     in2 = in0 + in1;
     in0 = in0 - in1;

     out2[11-0] = in2 * wi[11-0];
     out2[6 +0] = in2 * wi[6+0];
     out2[6 +2] = in3 * wi[6+2];
     out2[11-2] = in3 * wi[11-2];

     out2[0+0] += in0 * wi[0];
     out2[5-0] += in0 * wi[5-0];
     out2[0+2] += in4 * wi[2];
     out2[5-2] += in4 * wi[5-2];
}

OIL_DEFINE_IMPL (dct12_f32_mpglib, dct12_f32_class);



#ifdef TEST_dct12_f32
int TEST_dct12_f32(void)
{
	int i;
	int pass;
	int failures = 0;
	float *src, *dest_ref, *dest_test;
	struct sl_profile_struct t;

	src = sl_malloc_f32(18);
	dest_ref = sl_malloc_f32(18);
	dest_test = sl_malloc_f32(18);

	sl_profile_init(t);
	srand(20021001);

	printf("I: " sl_stringify(dct12_f32_FUNC) "\n");

	for(pass=0;pass<1;pass++){
		for(i=0;i<18;i++){
			src[i]=sl_rand_f32_0_1();
			//src[i]=(i==pass+6);
		}

		dct12_f32_ref(dest_ref,src);
		sl_profile_start(t);
		dct12_f32_FUNC(dest_test,src);
		sl_profile_stop(t);

		for(i=0;i<18;i++){
			if(dest_test[i] != dest_ref[i]){
				printf("%d %g %g %g\n",i,src[i],dest_ref[i],
						dest_test[i]);
			}
		}
	}

	sl_free(src);
	sl_free(dest_ref);
	sl_free(dest_test);

	if(failures){
		printf("E: %d failures\n",failures);
	}

	sl_profile_print(t);

	return failures;
}
#endif


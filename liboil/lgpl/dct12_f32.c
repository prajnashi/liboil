
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <liboil/liboilfunction.h>
#include <liboil/dct/dct.h>
#include <math.h>


/* provenance: from mpglib */
/* author: unknown */
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

OIL_DEFINE_IMPL (dct12_f32_mpglib, dct12_f32);





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

#include <liboil/liboil.h>
#include "jpeg.h"

OIL_DEFINE_CLASS (zigzag8x8_s16,
    "int16_t *d_8x8, int ds, int16_t *s_8x8, int ss");
OIL_DEFINE_CLASS (unzigzag8x8_s16,
    "int16_t *d_8x8, int ds, int16_t *s_8x8, int ss");



#define BLOCK8x8_S16(ptr, stride, row, column) \
	(*((int16_t *)((void *)ptr + stride*row) + column))

static const unsigned char zigzag_order[64] = {
	0,
	8, 1,
	2, 9, 16,
	24, 17, 10, 3, 
	4, 11, 18, 25, 32,
	40, 33, 26, 19, 12, 5, 
	6, 13, 20, 27, 34, 41, 48,
	56, 49, 42, 35, 28, 21, 14, 7,
	15, 22, 29, 36, 43, 50, 57,
	58, 51, 44, 37, 30, 23,
	31, 38, 45, 52, 59,
	60, 53, 46, 39,
	47, 54, 61,
	62, 55,
	63
};

static const unsigned char unzigzag_order[64] = {
	0,  1,  5,  6,  14, 15, 27, 28,
	2,  4,  7,  13, 16, 26, 29, 42,
	3,  8,  12, 17, 25, 30, 41, 43,
	9,  11, 18, 24, 31, 40, 44, 53,
	10, 19, 23, 32, 39, 45, 52, 54,
	20, 22, 33, 38, 46, 51, 55, 60,
	21, 34, 37, 47, 50, 56, 59, 61,
	35, 36, 48, 49, 57, 58, 62, 63,
};


static void
zigzag8x8_s16_ref(int16_t *dest, int dstr, int16_t *src, int sstr)
{
	int i,j;
	unsigned int z;

	for(j=0;j<8;j++){
		for(i=0;i<8;i++){
			z = zigzag_order[j*8+i];
			OIL_GET(dest,j*dstr +i*sizeof(int16_t), int16_t) =
                          OIL_GET(src, sstr*(z>>3)+(z&7)*sizeof(int16_t),
                              int16_t);
		}
	}
}
OIL_DEFINE_IMPL_REF (zigzag8x8_s16_ref, zigzag8x8_s16);

static void
unzigzag8x8_s16_ref (int16_t *dest, int dstr, int16_t *src, int sstr)
{
	int i,j;
	unsigned int z;

	for(i=0;i<8;i++){
		for(j=0;j<8;j++){
			z = unzigzag_order[i*8+j];
			OIL_GET(dest,j*dstr +i*sizeof(int16_t), int16_t) =
                          OIL_GET(src, sstr*(z>>3)+(z&7)*sizeof(int16_t),
                              int16_t);
		}
	}
}
OIL_DEFINE_IMPL_REF (unzigzag8x8_s16_ref, unzigzag8x8_s16);




static void
zigzag8x8_s16_unroll (int16_t *dest, int dstr, int16_t *src, int sstr)
{
#define ACK(x,y) \
        OIL_GET(dest, ((y)>>3) * dstr + ((y)&7) * sizeof(int16_t), int16_t) = \
          OIL_GET(src, ((x)>>3) * sstr + ((x)&7) * sizeof(int16_t), int16_t);

        ACK(0, 0);
        ACK(1, 8);
        ACK(2, 1);
        ACK(3, 2);
        ACK(4, 9);
        ACK(5, 16);
        ACK(6, 24);
        ACK(7, 17);
        ACK(8, 10);
        ACK(9, 3);
        ACK(10, 4);
        ACK(11, 11);
        ACK(12, 18);
        ACK(13, 25);
        ACK(14, 32);
        ACK(15, 40);
        ACK(16, 33);
        ACK(17, 26);
        ACK(18, 19);
        ACK(19, 12);
        ACK(20, 5);
        ACK(21, 6);
        ACK(22, 13);
        ACK(23, 20);
        ACK(24, 27);
        ACK(25, 34);
        ACK(26, 41);
        ACK(27, 48);
        ACK(28, 56);
        ACK(29, 49);
        ACK(30, 42);
        ACK(31, 35);
        ACK(32, 28);
        ACK(33, 21);
        ACK(34, 14);
        ACK(35, 7);
        ACK(36, 15);
        ACK(37, 22);
        ACK(38, 29);
        ACK(39, 36);
        ACK(40, 43);
        ACK(41, 50);
        ACK(42, 57);
        ACK(43, 58);
        ACK(44, 51);
        ACK(45, 44);
        ACK(46, 37);
        ACK(47, 30);
        ACK(48, 23);
        ACK(49, 31);
        ACK(50, 38);
        ACK(51, 45);
        ACK(52, 52);
        ACK(53, 59);
        ACK(54, 60);
        ACK(55, 53);
        ACK(56, 46);
        ACK(57, 39);
        ACK(58, 47);
        ACK(59, 54);
        ACK(60, 61);
        ACK(61, 62);
        ACK(62, 55);
        ACK(63, 63);
}
OIL_DEFINE_IMPL (zigzag8x8_s16_unroll, zigzag8x8_s16);


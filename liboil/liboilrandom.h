/* liboil - Library of Optimized Inner Loops
 * Copyright (C) 2003,2004  David A. Schleef <ds@schleef.org>
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

#ifndef _LIBOIL_RANDOM_H_
#define _LIBOIL_RANDOM_H_

#include <stdlib.h>

/* random number generation */

#define oil_rand_s32() ((rand()&0xffff)<<16 | (rand()&0xffff))
#define oil_rand_s32_l31() (((int32_t)rand())-0x40000000)

#define oil_rand_s16() ((int16_t)(rand()&0xffff))
#define oil_rand_s16_l15() (oil_rand_s16()>>1)
#define oil_rand_s16_l9() (oil_rand_s16()>>7)
#define oil_rand_s16_l8() (oil_rand_s16()>>8)
#define oil_rand_s16_l4() (oil_rand_s16()>>12)

#define oil_rand_s8() ((int8_t)(rand()&0xffff))

#define oil_rand_u32() ((rand()&0xffff)<<16 | (rand()&0xffff))
#define oil_rand_u32_l31() ((uint32_t)rand())

#define oil_rand_u16() ((uint16_t)(rand()&0xffff))

#define oil_rand_u8() ((uint8_t)(rand()&0xffff))


#define oil_rand_f64_0_1() (((rand()/(RAND_MAX+1.0))+rand())/(RAND_MAX+1.0))

#define oil_rand_f64_s32() (oil_rand_f64_0_1()*4294967296.0-2147483648.0)
#define oil_rand_f64_s16() (oil_rand_f64_0_1()*65536.0-32768.0)
#define oil_rand_f64_s8() (oil_rand_f64_0_1()*256.0-128.0)

#define oil_rand_f64_u32() (oil_rand_f64_0_1()*4294967296.0)
#define oil_rand_f64_u16() (oil_rand_f64_0_1()*65536.0)
#define oil_rand_f64_u8() (oil_rand_f64_0_1()*256.0)

#define oil_rand_f32_0_1() (rand()/(RAND_MAX+1.0))

#define oil_rand_f32_s32() (oil_rand_f64_0_1()*4294967296.0-2147483648.0)
#define oil_rand_f32_s16() (oil_rand_f64_0_1()*65536.0-32768.0)
#define oil_rand_f32_s8() (oil_rand_f64_0_1()*256.0-128.0)

#define oil_rand_f32_u32() (oil_rand_f64_0_1()*4294967296.0)
#define oil_rand_f32_u16() (oil_rand_f64_0_1()*65536.0)
#define oil_rand_f32_u8() (oil_rand_f64_0_1()*256.0)

#endif


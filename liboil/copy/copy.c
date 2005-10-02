/*
 * LIBOIL - Library of Optimized Inner Loops
 * Copyright (c) 2004 David A. Schleef <ds@schleef.org>
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

#include <string.h>

#include <liboil/liboilfunction.h>

OIL_DECLARE_CLASS (copy_u8);

static void
copy_u8_libc (uint8_t *dest, uint8_t *src, int n)
{
  memcpy (dest, src, n);
}
OIL_DEFINE_IMPL (copy_u8_libc, copy_u8);

static void
copy_u8_ptr (uint8_t *dest, uint8_t *src, int n)
{
  while(n--) {
    *dest++ = *src++;
  }
}
OIL_DEFINE_IMPL (copy_u8_ptr, copy_u8);

static void
copy_u8_ints (uint8_t *dest, uint8_t *src, int n)
{
  int i;
  for(i=0;i<(n&3);i++){
    *dest++ = *src++;
  }
  n >>= 2;
  for(i=0;i<n;i++){
    *(uint32_t *)dest = *(uint32_t *)src;
    dest += 4;
    src += 4;
  }
}
OIL_DEFINE_IMPL (copy_u8_ints, copy_u8);


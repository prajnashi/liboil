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

#include <liboil/liboil.h>
#include <liboil/liboiltest.h>
#include <liboil/liboilrandom.h>
#include "utf8.h"


static void
utf8_validate_test (OilTest *test)
{
  int i;
  int n = test->n;
  uint8_t *ptr = (uint8_t *)test->params[OIL_ARG_SRC1].src_data +
    OIL_TEST_HEADER;

  for (i=0;i<n;i++){
    OIL_GET(ptr, i, uint8_t) = oil_rand_u8() & 0x7f;
  }

}

OIL_DEFINE_CLASS_FULL (utf8_validate, "int32_t *d_1, uint8_t *s, int n",
    utf8_validate_test);


static void
utf8_validate_ref (int32_t *d_1, uint8_t *s, int n)
{
  int i;
  int extra_bytes;
  int mask;

  for(i=0;i<n;i++){
    if (s[i] < 128) continue;
    if ((s[i] & 0xe0) == 0xc0) {
      extra_bytes = 1;
      mask = 0x7f;
    } else if ((s[i] & 0xf0) == 0xe0) {
      extra_bytes = 2;
      mask = 0x1f;
    } else if ((s[i] & 0xf8) == 0xf0) {
      extra_bytes = 3;
      mask = 0x0f;
    } else {
      goto error;
    }
    if (i + extra_bytes >= n) goto error;
    while(extra_bytes--) {
      if ((s[i] & 0xc0) != 0x80) goto error;
      i++;
    }
  }

error:
  d_1[0] = i;
}

OIL_DEFINE_IMPL_REF (utf8_validate_ref, utf8_validate);


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

#ifndef _LIBOIL_DEBUG_H_
#define _LIBOIL_DEBUG_H_

enum {
  OIL_DEBUG_NONE = 0,
  OIL_DEBUG_ERROR,
  OIL_DEBUG_WARNING,
  OIL_DEBUG_INFO,
  OIL_DEBUG_DEBUG,
  OIL_DEBUG_LOG
};

#define OIL_ERROR(...) OIL_DEBUG_PRINT(OIL_DEBUG_ERROR, __VA_ARGS__)
#define OIL_WARNING(...) OIL_DEBUG_PRINT(OIL_DEBUG_WARNING, __VA_ARGS__)
#define OIL_INFO(...) OIL_DEBUG_PRINT(OIL_DEBUG_INFO, __VA_ARGS__)
#define OIL_DEBUG(...) OIL_DEBUG_PRINT(OIL_DEBUG_DEBUG, __VA_ARGS__)
#define OIL_LOG(...) OIL_DEBUG_PRINT(OIL_DEBUG_LOG, __VA_ARGS__)

#define OIL_DEBUG_PRINT(level, ...) do { \
  if(_oil_debug_enabled) { \
    oil_debug_print((level), __FILE__, __PRETTY_FUNCTION__, __LINE__, \
	__VA_ARGS__); \
  } \
}while(0)

extern int _oil_debug_enabled;
extern int _oil_debug_level;

void _oil_debug_init (void);
void oil_spill (void);

void oil_debug_print (int level, const char *file, const char *func,
    int line, const char *format, ...);

#endif


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

#include <glib.h>
#include <string.h>
#include <stdlib.h>

typedef struct _Insn Insn;
struct _Insn {
  char *line;
  int index;
  int n_deps;
  int deps[10];
};

Insn insns[100];

int state[100];

int max_depth = 8;
int outputting = 1;

int
main (int argc, char *argv[])
{
  gboolean ret;
  char *contents;
  int length;
  char **lines;
  int i;
  int j;
  int start_line = -1;
  int end_line = -1;
  int uber_start_line = -1;
  int uber_end_line = -1;
  int n_insns = 0;
  int depth;
  int n_impls = 0;

  ret = g_file_get_contents ("it.c", &contents, &length, NULL);

  lines = g_strsplit (contents, "\n", 0);
  
  for(i=0;lines[i];i++){
    char *uber;

    if (strcmp(lines[i],"UBER_START")==0) {
      start_line = i;
    }
    if (strcmp(lines[i],"UBER_END")==0) {
      end_line = i;
    }
    uber = strstr(lines[i],"UBER ");
    if (uber) {
      int n_deps = 0;
      char *end;

      if (uber_start_line < 0) uber_start_line = i;
      uber_end_line = i;

      uber+=5;

      insns[n_insns].line = lines[i];
      insns[n_insns].index = strtol(uber, &end, 10);
      uber = end;
      uber++;
      while(1) {
        while(*uber && *uber == ' ')uber++;
        insns[n_insns].deps[n_deps] = strtol(uber, &end, 10);
        if (end == uber) break;
        uber = end;
        n_deps++;
      }
      insns[n_insns].n_deps = n_deps;

      n_insns++;
    }
  }

  if (outputting) {
    for(i=0;i<start_line;i++){
      g_print("%s\n",lines[i]);
    }
  }

  depth = 0;
  while(depth >= 0) {
    int insn;

    insn = state[depth];

    /* check if we've exhausted instructions */
    if (insn >= max_depth) {
      depth--;
      if (depth >= 0) state[depth]++;
      continue;
    }
    /* check if this instruction has been used */
    for(i=0;i<depth;i++) {
      if (state[i] == insn) {
        state[depth]++;
        goto loop;
      }
    }
    /* check if this is a valid instruction to place */
    for(i=0;i<insns[insn].n_deps;i++){
      for(j=0;j<depth;j++){
        if (state[j] == insns[insn].deps[i]){
          break;
        }
      }
      if (j == depth) {
        state[depth]++;
        goto loop;
      }
    }

#if 0
    /* only move instruction a few places */
    if (depth < insn - 1 || depth > insn + 1) {
      state[depth]++;
    }
#endif

    if (depth < max_depth - 1) {
      depth++;
      state[depth]=0;
    } else {
      if (outputting) {
        for(i=start_line + 1;i<uber_start_line;i++){
          char *s;

          s = strstr(lines[i],"UBER_INDEX");
          if (s) {
            g_print("%.*s%d%s\n", (s-lines[i]), lines[i], n_impls, s+10);
          } else {
            g_print("%s\n",lines[i]);
          }
        }

        for(i=0;i<max_depth;i++){
          g_print("%s\n",insns[state[i]].line);
        }
        for(i=max_depth;i<n_insns;i++) {
          g_print("%s\n",insns[i].line);
        }

        for(i=uber_end_line + 1;i<end_line;i++){
          char *s;

          s = strstr(lines[i],"UBER_INDEX");
          if (s) {
            g_print("%.*s%d%s\n", (s-lines[i]), lines[i], n_impls, s+10);
          } else {
            g_print("%s\n",lines[i]);
          }
        }
        g_print("\n");
      }

      n_impls++;
      state[depth]++;
    }
    loop:
    ;
  }

  if (!outputting) {
    g_print("n_impls = %d\n", n_impls);
  }

  return 0;
}


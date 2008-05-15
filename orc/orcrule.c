
#include "config.h"

#include <glib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <orc/orcprogram.h>


OJRuleList *
oj_rule_list_new (void)
{
  OJRuleList *rule_list;

  rule_list = malloc (sizeof(OJRuleList));
  memset (rule_list, 0, sizeof(OJRuleList));

  return rule_list;
}

void
oj_rule_list_free (OJRuleList *rule_list)
{
  free (rule_list->rules);
  free (rule_list);
}

void
oj_rule_list_register (OJRuleList *rule_list, const char *opcode_name,
    OJRuleEmitFunc emit, void *emit_user, unsigned int flags)
{
  int i;

  if (rule_list->n_rules == rule_list->n_alloc) {
    rule_list->n_alloc += 100;
    rule_list->rules = realloc (rule_list, sizeof(OJRule) * rule_list->n_alloc);
  }

  i = rule_list->n_rules;
  rule_list->rules[i].opcode = oj_opcode_find_by_name (opcode_name);
  rule_list->rules[i].emit = emit;
  rule_list->rules[i].emit_user = emit_user;
  rule_list->rules[i].flags = flags;

  rule_list->n_rules++;
}

OJRule *
oj_rule_list_get (OJRuleList *rule_list, OJOpcode *opcode)
{
  int i;
  for (i=0;i<rule_list->n_rules;i++){
    if (rule_list->rules[i].opcode == opcode) {
      return rule_list->rules + i;
    }
  }
  return NULL;
}



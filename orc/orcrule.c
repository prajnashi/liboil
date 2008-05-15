
#include "config.h"

#include <glib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <orc/orcprogram.h>


OrcRuleList *
orc_rule_list_new (void)
{
  OrcRuleList *rule_list;

  rule_list = malloc (sizeof(OrcRuleList));
  memset (rule_list, 0, sizeof(OrcRuleList));

  return rule_list;
}

void
orc_rule_list_free (OrcRuleList *rule_list)
{
  free (rule_list->rules);
  free (rule_list);
}

void
orc_rule_list_register (OrcRuleList *rule_list, const char *opcode_name,
    OrcRuleEmitFunc emit, void *emit_user, unsigned int flags)
{
  int i;

  if (rule_list->n_rules == rule_list->n_alloc) {
    rule_list->n_alloc += 100;
    rule_list->rules = realloc (rule_list, sizeof(OrcRule) * rule_list->n_alloc);
  }

  i = rule_list->n_rules;
  rule_list->rules[i].opcode = orc_opcode_find_by_name (opcode_name);
  rule_list->rules[i].emit = emit;
  rule_list->rules[i].emit_user = emit_user;
  rule_list->rules[i].flags = flags;

  rule_list->n_rules++;
}

OrcRule *
orc_rule_list_get (OrcRuleList *rule_list, OrcOpcode *opcode)
{
  int i;
  for (i=0;i<rule_list->n_rules;i++){
    if (rule_list->rules[i].opcode == opcode) {
      return rule_list->rules + i;
    }
  }
  return NULL;
}



#include "cmd_list.h"

/*  free's a command
    @param p pointer for the command to free */
void free_com(void *p) {
  com *cmd;
  cmd = p;
  if (p == NULL) return;

  if (cmd->oplist != NULL) free_oplist(cmd->oplist);

  if (cmd->label != NULL) free(cmd->label);

  if (cmd->operation != NULL) free(cmd->operation);

  free(cmd);
}

/*  frees a com node and its data
    @param n the com node to free */
void free_com_node(node *n) {
  free_com(get_data(n));

  free(n);
}

/*  free's a command list
    @param p pointer for the command list's head */
void free_com_list(node *list) { free_list(list, &free_com); }

/*  free's a operand list
    @param p pointer for the operand list's head */
void free_oplist(node *list) {
  if (list != NULL) free_list(list, &free);
}

/*  allocate memory to a new command
    @return a pointer to a new command */
com *make_command() {
  com *cmd;
  /* allocating label memory */
  cmd = (com *)(malloc(sizeof(com)));
  if (cmd == NULL) {
    printf("ERROR! - allocating command  memory\n");
    return NULL;
  }

  cmd->oplist = NULL;
  cmd->label = NULL;
  cmd->operation = NULL;

  cmd->op_type = unknown;

  cmd->DC = 0;
  cmd->IC = 0;

  cmd->line_index = -1;
  cmd->opcode = NULL_OPCODE;

  return cmd;
}

/* prints a given macro node */
void print_com_node(node *p) {
  com *m1;
  m1 = p->data;
  printf(
      "line: %d \tIC: %d\tDC: %d\tlabel: %s\toperator: %s     "
      "\toperands: ",
      m1->line_index, m1->IC, m1->DC, m1->label, m1->operation);
  print_oplist(m1->oplist);
}

/* prints a commad macro list */
void print_com(com *m1) {
  printf(
      "line: %d \t\tIC: %d\tDC: %d\tlabel: %s\toperator: %s     "
      "\toperands: ",
      m1->line_index, m1->IC, m1->DC, m1->label, m1->operation);
  print_oplist(m1->oplist);
}

/* prints a given macro list */
void print_com_list(node *head) {
  node *p = head;
  unsigned i = 1;
  if (head == NULL) {
    printf("ERROR! - List dose not exist\n");
    return;
  }
  printf("*Printing command list*\n\n");
  while (p->next_node != NULL) {
    p = p->next_node;
    printf("Command number %d: \t", i++);
    print_com_node(p);
    printf("\n");
  }
}

/* prints a given oplist */
void print_oplist(node *head) {
  node *p = head;
  if (head == NULL) {
    printf("ERROR! - List dose not exist\n");
    return;
  }

  while (p->next_node != NULL) {
    p = p->next_node;
    printf("|%s|->", (char *)p->data);
  }
  printf("\n");
}

/*  adds a com node to the tail of the list
    @param head list's head
    @param cmd_node the node to add
    @return TRUE if secssesful */
bool add_com_node_to_tail(node *head, node *cmd_node) {
  node *p;
  if (head == NULL) {
    printf("ERROR! - List dose not exist\n");
    return FALSE;
  }
  p = head;

  /* find the end of the list */
  while (p->next_node != NULL) {
    p = p->next_node;
  }

  /* add to end of list  */
  return list_push(p, cmd_node);
}

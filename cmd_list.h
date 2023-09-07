#ifndef CMD_H
#define CMD_H

#include "mylist.h"

/*  free's a command
    @param p pointer for the command to free */
void free_com(void *p);

/*  frees a com node and its data
    @param n the com node to free */
void free_com_node(node *n);

/*  free's a command list
    @param p pointer for the command list's head */
void free_com_list(node *list);

/*  free's a operand list
    @param p pointer for the operand list's head */
void free_oplist(node *list);

/*  allocate memory to a new command
    @return a pointer to a new command */
com *make_command();

/* prints a given oplist */
void print_oplist(node *head);

/* prints a given macro list */
void print_macro_list(node *head);

/* prints a given macro list */
void print_com_list(node *head);

/* prints a commad macro list */
void print_com(com *m1);

/*  adds a com node to the tail of the list
    @param head list's head
    @param cmd_node the node to add
    @return TRUE if secssesful */
bool add_com_node_to_tail(node *head, node *cmd_node);

#endif
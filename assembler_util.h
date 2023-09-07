#ifndef A_UTIL_H
#define A_UTIL_H

#include "cmd_list.h"
#include "const.h"
#include "str_util.h"
#include "symbol_table.h"

/*  breaks a string command into:
    label, operation, operands (saved in a operand_list)
    @param reserved_table a hash map with all reserved words
    @param line the string to work on
    @return Command in com struct */
com *break_com(map *reserved_table, char *line);

/*  creates a new stiring that gets the lable from given string string and
    returns it
    @param line the string to work on
    @return a string that holds the lable */
char *get_label(char *line);

/*  creates a new stiring that gets the operation from given string string while
    advencing *i to the next letter after the string

    @param line the string to work on
    @return a string that holds the operation */
char *get_com_operation(char *line, int *i);

/*  creates a new list that holds all the operands from a given string

    @param line the string to work on
    @param *i the index after the operation ended
    @return a list that holds all the operands */
node *get_op_list(char *line, int k);

/*  creates a new stiring that gets the next operand from given string string
    while advencing *i to the next letter after the operand

    @param line the string to work on
    @param *i the index after label ended
    @return a string that holds the operation */
char *get_op(char *line, int *i);

/*  this func returns the machine code legnth of the command

    @param *cmd the command to get the length from
    @return the length in machine code of the command */
int command_length(com *cmd);

/*  checks if command is legal

    @param *reserved_table  the adress to reserve table with reserved words
    @param *cmd the adress to the command to check
    @param *line the current line for error massage

    @return true if command is legal and false if not */
bool is_cmd_llegal(map *reserved_table, com *cmd, char *line);

/*  checks if operands are legal by type

    @param *op_list_head  the head of the op_list
    @param type  the type of command we check
    @param *line the current line for error massage

    @return true if operands are legal and false if not */
bool check_cmd_operands(operation_type type, node *op_list_head, char *line);

/* check if str represents a number

    @param *str string to check

    @return true if data is legal and false if not*/
bool check_data_op(char *str);

/* checks if data oplist operands are legal

  @param *list_head the head of oplist

  @return true if data oplist is legal and false if not*/
bool check_data_op_list(node *list_head);

/* checks if string operand is legal

  @param *str string to check

  @return true if sting is legal and false if not */
bool check_string_op(char *str);

/* checks if operand list of extern is legal */
bool check_extern_op_list(node *op_list_head);

/* checks if operand of extern is legal

    @param *p the node to check

    @return true if extern is legal and false if not*/
bool check_extern_op(node *p);

/* checks if operand list of entry is legal

  @param *op_list_head oplist to check

  @return true if entery oplist is legal and false if not*/
bool check_entry_op_list(node *op_list_head);

/* checks if operand of entry is legal

    @param *p the node to check

    @return true if entry is legal and false if not*/
bool check_entry_op(node *p);

/*  adds an extern symbol to a table

    @param *symb_table symbol table
    @param *reserved_table reserved word table
    @param *line for errors
    @param *cmd the command

    @return TRUE extern was added
    @return FALSE if extern is in the list already or */

bool extern_add_to_symb_table(map *symb_table, map *reserved_table, char *line,
                              com *cmd);

#endif

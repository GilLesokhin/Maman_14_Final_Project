#ifndef STR_H
#define STR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "const.h"

/*  skip spaces from index i
    @param line the string to work on
    @param i index to start at
    return the index to the next char that is not a white space */
int skip_spaces(char *line, int i);

/*  check if line is empty or a comment line
    @param line the string to work on
    return TRUE if line is empty */
bool is_empty(char *line);

/*  checks if line has a label if line has a label skip it
    @param line the string to work on
    return first letter after lable if there is no lable return 0 */
int skip_label(char *line);

/*  check if line is shorter or equall then LINE_LENGTH
    @param line the string to work on
    @return TRUE line shorter or equall to LINE_LENGTH else FALSE */
bool length_test(char *line);

/* convers an opcode int to a string */
char *int_to_opcode_str(int k);

/* convers an reg_num to a string */
char *int_to_reg_num_str(int k);

/* convers an int to a type string */
char *int_to_type_str(int k);

/* removes a file ending from a string */
void remove_file_end(char *f_name);

/* check if file name is legal */
bool check_file_name(char *f_name);

/* calculate an operator bin length for a command

    @param *operator1 first op (null is an option)
    @param *operator2 seconed op (null is an option)
    @return the length of the operation operands in bin code lines */
int get_operator_bin_len(char *operator1, char *operator2);

/* checks if operation is a register */
int is_op_register(char *str);

/* gets an offset from a string */
int get_offset(char *str);

/* checks if operand is absulute value */
bool is_absolute(char *str);

#endif
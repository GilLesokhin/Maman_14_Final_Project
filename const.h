#ifndef CONST_H
#define CONST_H

#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* define boolean variable */
typedef enum { FALSE = 0, TRUE = 1 } bool;

/* used to define command op_type */
typedef enum {
  unknown = 1,
  string_type = 2,
  data_type = 3,
  struct_type = 4,
  extern_type = 5,
  entry_type = 6,
  command_type_1 = 7,
  command_type_2 = 8,
  command_type_3 = 9,
  not_operation = 0
} operation_type;

/* opcodes */
#define CMP_OPCODE 1
#define LEA_OPCODE 6
#define PRN_OPCODE 12

/* consts */
#define COM_TYPE3_AD_TYPE 0
#define EXTERN_ADRESS 0
#define BASE_ADRESS 100
#define NULL_OPCODE -1

/* adressing type for command */
#define absolute_ad_type 0
#define direct_ad_type 1
#define struct_ad_type 2
#define register_ad_type 3
#define BOTH_OP_REG 15

/* A/R/E */
#define absolute_operand 0
#define external_operand 1
#define relocatable_operand 2

/* the minimum of args main can recive*/
#define NUM_OF_ARGS 2

/* hash contsts */
#define HASH_SIZE 97
#define HASH_MULL 31

/* memory size consts */
#define MACRO_DATA_SIZE 512
#define MEMORY_SIZE 256
#define LINE_LENGTH 80
#define TITLE_SIZE 80
#define LONG_WORD 80
#define WORD 20
#define LABLE_LENGTH 30
#define OP_LEGNTH 8
#define OPCODE_LEGNTH 4
#define REG_I_LENGTH 3
#define TYPE_LENGTH 2

/* for reseved table setup */
#define CONV_ARR_SIZE 32
#define COM_ARY_SIZE 26
#define REG_ARY_SIZE 8

/* bit must */
#define BIT_MUSK_10BIT 1023

/* data min and max */
#define DATA_OP_MAX 511
#define DATA_OP_MIN -512
#define ABS_MAX 127
#define ABS_MIN -128

/* used for list */
typedef struct node node;
struct node {
  node *next_node;
  node *prev_node;
  void *data;
};

/* used for hashmap */
typedef struct map map;
struct map {
  node *lists[HASH_SIZE];
};

/* used to save a marco to macro map */
typedef struct macro macro;
struct macro {
  char *title;
  char *data;
};

/* used to represent a symbol in symbol table */
typedef struct symbol symbol;
struct symbol {
  char *label;
  int length;
  int IC;
  int DC;
  operation_type type;
  bool ent_used_flag;
};

/* used to represent a command in command list */
typedef struct com com;
struct com {
  char *label;
  char *operation;
  node *oplist;
  int IC;
  int DC;
  operation_type op_type;
  int line_index;
  int opcode;
};

/* used to save reserved words in reserve table */
typedef struct reserv reserv;
struct reserv {
  char *title;
  int opcode;
  operation_type op_type;
};

#endif
#include "data.h"

/* flags used for printing info when testing */
bool print_database_flag = FALSE;
bool print_adressing_flag = FALSE;
bool print_file_building = FALSE;

/* used for in code */
bool error_flag = FALSE;
bool extern_flag = FALSE;
bool entry_flag = FALSE;

/* global variables */
int line_count = 0;
int IC = 0, DC = 0;
int base_adress = BASE_ADRESS;

/* used to convert to base 32 */
const char *base32_conv_ary[CONV_ARR_SIZE] = {
    "!", "@", "#", "$", "%", "^", "&", "*", "<", ">", "a",
    "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l",
    "m", "n", "o", "p", "q", "r", "s", "t", "u", "v",
};

/* my_assembler takes an assembly file: filename.as and creates a proprietary
 * base 32 machine code in file filename.as if necessary it also creates two
 * other files filename.ext and filename.ent for the linker  */
int main(int argc, char *argv[]) {
  int i;
  /* File pointers */
  FILE *sorce_code = NULL, *sorce_code_am = NULL, *final_code = NULL;
  FILE *ent_file = NULL, *ext_file = NULL;
  /* pointers to map and table data bases */
  map *macro_map = NULL, *reserved_table = NULL, *symb_table = NULL;
  node *cmd_list = NULL;             /* pointer to a command list */
  char sorce_fname[LINE_LENGTH + 2]; /* string for f_name */

  /* create a reserved_table for reserved words */
  if ((reserved_table = make_reserved_table()) == NULL) {
    goto cleanup;
  }

  /* make sure a file name was added in the function call  */
  if (argc < NUM_OF_ARGS) {
    printf(
        "No file added to my_assembler please enter: my_assembler + "
        "\"filename.as\" \n");
    return FALSE;
  }

  for (i = 1; i < argc; i++) { /* loop to go over all the files */

    /* advance base_adress to start at the end of previous file in memory */
    base_adress += IC + DC;
    IC = 0;
    DC = 0;

    /* check if input is llegal */
    if (check_file_name(argv[i]) == FALSE) {
      printf("ERROR! - sorce code file: \"%s\" invalid file name \".as\"\n",
             argv[i]);
      goto cleanup;
    }

    strcpy(sorce_fname, argv[i]); /* copy input to sorce_fname */

    printf("\nFile: %s assembler started.\n\n", sorce_fname);

    /* open file by name given */
    if ((sorce_code = fopen(sorce_fname, "r")) == NULL) {
      printf("ERROR! - sorce code file: \"%s\" not found\n", sorce_fname);
      goto cleanup;
    }

    /* make a string with input file name .am */
    remove_file_end(sorce_fname);
    strcat(sorce_fname, ".am");

    /* open a file with temp file name */
    if ((sorce_code_am = fopen(sorce_fname, "w+")) == NULL) {
      printf("ERROR! - canot open file \"%s\" \n", sorce_fname);
      goto cleanup;
    }

    /* create a macro_map for macros */
    if ((macro_map = make_macro_map()) == NULL) {
      goto cleanup;
    }
    /* pre_prossesor will copy the sorce code to temp file while adding
     * macros to the text and saving them macro_map*/
    pre_prossesor(macro_map, reserved_table, sorce_code, sorce_code_am);
    if (macro_map != NULL) {
      free_macro_map(macro_map);
      macro_map = NULL;
    }
    if (error_flag == TRUE) {
      goto cleanup;
    }
    if (sorce_code != NULL) {
      fclose(sorce_code);
      sorce_code = NULL;
    }

    if (print_database_flag == TRUE) { /* used for testing with database_flag */
      print_macro_map(macro_map);
    }

    /* go to the start of sorce_code */
    fseek(sorce_code_am, 0, SEEK_SET);

    if ((cmd_list = make_list()) == NULL) {
      error_flag = TRUE;
      goto cleanup;
    }

    symb_table = make_map();
    if (symb_table == NULL) {
      error_flag = TRUE;
      goto cleanup;
    }

    /* start seconed run */
    assembler_collect_info(sorce_code_am, reserved_table, symb_table, cmd_list);
    if (print_database_flag == TRUE) { /* used for testing with database_flag */
      print_symbol_table(symb_table);
      print_com_list(cmd_list);
    }

    if (error_flag == TRUE) {
      goto cleanup;
    }

    if (sorce_code_am != NULL) {
      fclose(sorce_code_am);
      sorce_code_am = NULL;
    }

    if (extern_flag == TRUE) { /* make a .ext file */

      /* make a string with input file name .ext */
      remove_file_end(sorce_fname);
      strcat(sorce_fname, ".ext");

      if ((ext_file = fopen(sorce_fname, "w+")) == NULL) {
        printf("ERROR! - canot open file \"%s\" \n", sorce_fname);
        goto cleanup;
      }
    }

    if (entry_flag == TRUE) { /* make a .ent file */

      /* make a string with input file name .ent */
      remove_file_end(sorce_fname);
      strcat(sorce_fname, ".ent");

      if ((ent_file = fopen(sorce_fname, "w+")) == NULL) {
        printf("ERROR! - canot open file \"%s\" \n", sorce_fname);
        goto cleanup;
      }
    }

    /* make a .ob file */
    /* make a string with input file name .ob */
    remove_file_end(sorce_fname);
    strcat(sorce_fname, ".ob");

    if ((final_code = fopen(sorce_fname, "w+")) == NULL) {
      printf("ERROR! - canot open file \"%s\" \n", sorce_fname);
      goto cleanup;
    }

    assembler_build_files(final_code, ent_file, ext_file, symb_table, cmd_list);
    if (error_flag == TRUE) {
      goto cleanup;
    }

    if (final_code != NULL) {
      fclose(final_code);
      final_code = NULL;
    }
    if (ent_file != NULL) {
      fclose(ent_file);
      ent_file = NULL;
    }
    if (ext_file != NULL) {
      fclose(ext_file);
      ext_file = NULL;
    }

    if (cmd_list != NULL) {
      free_com_list(cmd_list);
      cmd_list = NULL;
    }
    if (symb_table != NULL) {
      free_symbol_table(symb_table);
      symb_table = NULL;
    }

    /* make a string with input file name .as */
    remove_file_end(sorce_fname);
    strcat(sorce_fname, ".as");
    printf("\nFile: %s assembler Sucssful!\n\n", sorce_fname);
  }

cleanup:
  if (error_flag == TRUE) {
    printf("\nFile: %s assembler Failed!\n\n", argv[i]);
  }

  /* close files */
  if (sorce_code != NULL) {
    fclose(sorce_code);
  }
  if (sorce_code_am != NULL) {
    fclose(sorce_code_am);
  }
  if (final_code != NULL) {
    fclose(final_code);
  }
  if (ent_file != NULL) {
    fclose(ent_file);
  }
  if (ext_file != NULL) {
    fclose(ext_file);
  }

  /* free databases */
  if (reserved_table != NULL) free_reserv_table(reserved_table);
  if (symb_table != NULL) free_symbol_table(symb_table);
  if (cmd_list != NULL) {
    free_com_list(cmd_list);
  }

  if (error_flag == TRUE) {
    return FALSE;
  }

  return TRUE;
}

/*  after the pre prossesor has done its job the .am file is given to
   assembler_collect_info to collect all the needed info from the file, check
   for errors and build a symbol table and command list all will be done while
   checking for errors in the code (some errors wont be found untill
   assembler_build_files)


    @param *temp_code .am file from pre-procesor
    @param *reserved_table table of all the reserved words
    @param *symb_table symbol table of all the symbols available
    @param *cmd_list a list to save all the commands found in

    @return TRUE if successful and no errors are found else return FALSE */
bool assembler_collect_info(FILE *sorce_code, map *reserved_table,
                            map *symb_table, node *cmd_list) {
  char line[LINE_LENGTH + 2];
  com *cmd;
  symbol *symb;
  reserv temp_res;

  symb = NULL;
  error_flag = FALSE;
  line_count = 0;
  IC = 0;
  DC = 0;

  /* loop to read sorce_code line by line */
  while (fgets(line, LINE_LENGTH + 2, sorce_code)) {
    line_count++;

    if (is_empty(line) == TRUE) { /* check for comments and empty lines */
      continue;
    }

    /* brake line into com */
    /* if break_com returns NULL then there is a problem with the command */
    if ((cmd = break_com(reserved_table, line)) == NULL) {
      continue;
    }
    /* check if operation exits */
    if (cmd->operation != NULL) {
      temp_res.title = cmd->operation;

      /* look in reserved map for a command with the same name*/
      if (find_in_reserv_table(reserved_table, &temp_res) == FALSE) {
        printf("line: %d %s* ^ERROR! - \"%s\" unknown command *\n\n",
               line_count, line, temp_res.title);
        error_flag = TRUE;
      }
    }

    if (cmd->op_type == extern_type) {
      extern_flag = TRUE; /* to know that a .ext file need to be open */
      extern_add_to_symb_table(symb_table, reserved_table, line, cmd);
    }

    else if (cmd->op_type == entry_type) {
      entry_flag = TRUE; /* to know that a .ent file need to be open */
    }

    /* else check if label was found*/
    else if (cmd->label != NULL) {
      /* used for looking in reserved table */
      temp_res.title = cmd->label;

      /* make symbol struct for symbol table */
      symb = make_symbol();
      symb->type = cmd->op_type;
      symb->length = list_len(cmd->oplist);

      strcpy(symb->label, cmd->label);
      if (cmd->op_type == command_type_1 || cmd->op_type == command_type_2 ||
          cmd->op_type == command_type_3) { /* if its a command line */
        symb->IC = IC;
      } else { /* if its an instruction line */
        symb->DC = DC;
      }

      /* look in reserved map for a command with the same name as label*/
      if (find_in_reserv_table(reserved_table, &temp_res) == TRUE) {
        printf("line: %d %s* ^ERROR! - lable :\"%s\" is a reserved word *\n\n",
               line_count, line, temp_res.title);
        error_flag = TRUE;
      }

      /* save to symbol_table */
      else if (add_to_symbol_table(symb_table, symb) == FALSE) {
        printf("line: %d %s* ^ERROR! - lable :\"%s\" is already taken *\n\n",
               line_count, line, symb->label);
        free_symbol(symb);
        error_flag = TRUE;
      }
    }

    /* advance adress based on command type */
    if (cmd->op_type == command_type_1 || cmd->op_type == command_type_2 ||
        cmd->op_type == command_type_3) {
      cmd->IC = IC;
      IC += command_length(cmd);
    } else {
      cmd->DC = DC;
      DC += command_length(cmd);
    }

    /* check for memory overflow */
    if ((base_adress + IC + DC) >= MEMORY_SIZE) {
      printf("ERROR! - Memory overflow, memory is limited to %d \"words\"\n",
             MEMORY_SIZE);
      free_com(cmd);
      error_flag = TRUE;
      return FALSE;
    }
    /* check for errors with list */
    if (add_to_tail(cmd_list, cmd) == FALSE) {
      printf("ERROR! - unable to save Command to cmd_list\n");
      free_com(cmd);
      error_flag = TRUE;
      return FALSE;
    }
  }

  return error_flag;
}

/*  after assembler_collect_info colected all the needed info to build the .ob,
   .ent, .ext files this function will build those files while checking for
   adressing errors and some other errors

    @param *final_code  .ob file to write to
    @param *ent_file    .ent file to write to
    @param *ext_file    .ext file to write to
    @param *symb_table symbol table of all the symbols available
    @param *cmd_list a list of all the commands found

    @return TRUE if successful and no errors are found else return FALSE */
bool assembler_build_files(FILE *final_code, FILE *ent_file, FILE *ext_file,
                           map *symb_table, node *cmd_list) {
  com *cmd;
  node *cmd_node;
  int i, temp_num;
  char c, str1[LONG_WORD], str2[LONG_WORD];
  node *temp_node1, *temp_node2;
  char *temp_str_op1, *temp_str_op2, *base32_cmd_str;
  unsigned int com_ad_type;
  /* a list to save instructions in while finishing setting up all commands */
  node *instructions_cmd_list;

  instructions_cmd_list = make_list(); /* create instructions list */

  /* print IC and DC to final code in base 32 */
  fprintf(final_code, "%s %s\n", base32_conv1(str1, IC),
          base32_conv1(str2, DC));

  /* this part builds the all commands */
  while ((cmd_node = list_pop(cmd_list)) != NULL) {
    if ((cmd = (com *)get_data(cmd_node)) == NULL) {
      printf("cmd_list Error\n");
      error_flag = TRUE;
      free_com_node(cmd_node);
      continue;
    }

    /* a group of commands with 2 operators */
    if (cmd->op_type == command_type_1) {
      temp_node1 = list_pop(cmd->oplist);
      temp_node2 = list_pop(cmd->oplist);
      temp_str_op1 = get_data(temp_node1);
      temp_str_op2 = get_data(temp_node2);
      list_push(cmd->oplist, temp_node2);
      list_push(cmd->oplist, temp_node1);

      /* converts command operands to string and save it to str1
       * com_ad_type gets and adressing type of the operands for command */
      com_ad_type = get_base32_op_str(str1, temp_str_op1, temp_str_op2, cmd,
                                      symb_table, ext_file);
      /* converts command to string with adressing type from com_ad_type */
      base32_cmd_str = get_base32_com_str(str2, cmd, com_ad_type);

      if (print_file_building == TRUE) { /* used for testing with build flag */
        printf("adding to .ob, command type_1:\n%s%s\n\n", base32_cmd_str,
               str1);
      }
      /* print strings to .ob */
      fprintf(final_code, "%s%s", base32_cmd_str, str1);

      free_com_node(cmd_node);
    }

    /* a group of commands with 1 operator */
    else if (cmd->op_type == command_type_2) {
      temp_node1 = list_pop(cmd->oplist);
      temp_str_op1 = get_data(temp_node1);
      list_push(cmd->oplist, temp_node1);

      /* converts command operands to string and save it to str1
       * com_ad_type gets and adressing type of the operands for command */
      com_ad_type = get_base32_op_str(str1, NULL, temp_str_op1, cmd, symb_table,
                                      ext_file);
      /* converts command to string with adressing type from com_ad_type */
      base32_cmd_str = get_base32_com_str(str2, cmd, com_ad_type);

      if (print_file_building == TRUE) { /* used for testing with build flag */
        printf("adding to .ob, command type_2:\n%s%s\n\n", base32_cmd_str,
               str1);
      }
      /* print strings to .ob */
      fprintf(final_code, "%s%s", base32_cmd_str, str1);

      free_com_node(cmd_node);
    }

    /* a group of commands with no operators */
    else if (cmd->op_type == command_type_3) {
      /* used for testing with adressing flag */
      if (print_adressing_flag == TRUE) {
        printf("\n");
        print_com(cmd);
        printf("source_op: (null)\tdest_op: (null)\n\n");
      }

      /* converts command to string  */
      base32_cmd_str = get_base32_com_str(str2, cmd, COM_TYPE3_AD_TYPE);

      if (print_file_building == TRUE) { /* used for testing with build flag */
        printf("\n");
        print_com(cmd);
        printf("\n");
      }
      if (print_file_building == TRUE) { /* used for testing with build flag */
        printf("adding to .ob, command type_3:\n%s\n\n", base32_cmd_str);
      }

      /* print strings to .ob */
      fprintf(final_code, "%s", base32_cmd_str);
      free_com_node(cmd_node);
    }
    /* add to the end of instructions list */
    else {
      add_com_node_to_tail(instructions_cmd_list, cmd_node);
    }
  }
  /* this part builds the instructions part */
  while ((cmd_node = list_pop(instructions_cmd_list)) != NULL) {
    if ((cmd = (com *)get_data(cmd_node)) == NULL) {
      printf("cmd_list Error\n");
      error_flag = TRUE;
      free_com_node(cmd_node);
      continue;
    }
    if (print_file_building == TRUE) { /* used for testing with build flag */
      printf("\n");
      print_com(cmd);
      printf("\n");
    }
    /* string_type */
    if (cmd->op_type == string_type) {
      i = 0;
      temp_node2 = list_pop(cmd->oplist);
      temp_str_op2 = get_data(temp_node2); /* get the string from op_list */
      list_push(cmd->oplist, temp_node2);

      i = 2; /* there are '\"' at the start and end of the string we offset
                the index to ignore them */

      while ((c = temp_str_op2[i]) != '\0') {
        /* used for testing with build flag */
        if (print_file_building == TRUE) {
          printf("string:\tadress: %d\t ascii: %c\n",
                 base_adress + IC + (cmd->DC) + i - 2, temp_str_op2[i - 1]);
          printf("adding to .ob:\n%s\t%s\n\n\n",
                 base32_conv(str1, (base_adress + IC + (cmd->DC)) + i - 2),
                 base32_conv(str2, temp_str_op2[i - 1]));
        }

        /* print to final_code after converting to base 32 */
        fprintf(final_code, "%s\t%s\n",
                base32_conv(str1, (base_adress + IC + (cmd->DC)) + i - 2),
                base32_conv(str2, temp_str_op2[i - 1]));
        i++;
      }

      if (print_file_building == TRUE) { /* used for testing with build flag */
        printf("string:\tadress: %d\t ascii: \\0\n",
               (base_adress + IC + (cmd->DC)) + i - 2);
        printf("adding to .ob:\n%s\t%s\n\n\n",
               base32_conv(str1, (base_adress + IC + (cmd->DC)) + i - 2),
               base32_conv(str2, 0));
      }

      /* print the '\0' at the end of string */
      fprintf(final_code, "%s\t%s\n",
              base32_conv(str1, (base_adress + IC + (cmd->DC)) + i - 2),
              base32_conv(str2, 0));

      free_com_node(cmd_node); /* free command */
    }
    /* data_type */
    else if (cmd->op_type == data_type) {
      i = 0;
      /* get the next number from oplist */
      while ((temp_node1 = list_pop(cmd->oplist)) != NULL) {
        temp_str_op1 = (char *)get_data(temp_node1);

        temp_num = atoi(temp_str_op1); /* convert data to an int */

        /* check that data is within range of DATA_OP_MAX and DATA_OP_MIN */
        if (temp_num > DATA_OP_MAX || temp_num < DATA_OP_MIN) {
          printf(
              "line: %d\t* ERROR! - .data value: %d needs to be bigger "
              "then %d and smaller then %d *\n\n",
              cmd->line_index, temp_num, DATA_OP_MIN, DATA_OP_MAX);
          error_flag = TRUE;
        }

        if (print_file_building ==
            TRUE) { /* used for testing with build flag */
          printf("data:\t adress: %d\t data:%d\n",
                 (base_adress + IC + (cmd->DC) + i), temp_num);
          printf("adding to .ob:\n%s\t%s\n\n\n",
                 base32_conv(str1, base_adress + IC + (cmd->DC) + i),
                 base32_conv(str2, temp_num));
        }

        /* print to final_code after converting to base 32 */
        fprintf(final_code, "%s\t%s\n",
                base32_conv(str1, base_adress + IC + (cmd->DC) + i),
                base32_conv(str2, temp_num));

        i++;
        free_node(temp_node1, &free);
      }
      free_com_node(cmd_node);
    }
    /* struct_type */
    else if (cmd->op_type == struct_type) {
      i = 0;
      temp_node1 = list_pop(cmd->oplist);
      temp_node2 = list_pop(cmd->oplist);
      temp_str_op1 = get_data(temp_node1); /* get data part of the stract */
      temp_str_op2 = get_data(temp_node2); /* get string part of the stract */
      list_push(cmd->oplist, temp_node2);
      list_push(cmd->oplist, temp_node1);
      /* num part */
      temp_num = atoi(temp_str_op1); /* conver string to int */
      if (temp_num > DATA_OP_MAX ||
          temp_num <
              DATA_OP_MIN) { /* check that number is within llegal range */
        printf("line: %d* ERROR! - .data operator :\"%s\" is illegal *\n\n",
               cmd->line_index, temp_str_op1);
        error_flag = TRUE;
      }

      if (print_file_building == TRUE) { /* used for testing with build flag */
        printf("struct data:\t adress: %d\t data:%d\n",
               (base_adress + IC + (cmd->DC) + i), temp_num);
        printf("adding to .ob:\n%s\t%s\n\n\n",
               base32_conv(str1, base_adress + IC + (cmd->DC) + i),
               base32_conv(str2, temp_num));
      }

      /* print to final_code after converting to base 32 */
      fprintf(final_code, "%s\t%s\n",
              base32_conv(str1, base_adress + IC + (cmd->DC) + i),
              base32_conv(str2, temp_num));

      i = 2; /* there are '\"' at the start and end of the string we offset
                the index to ignore them */

      /* string part */
      while ((c = temp_str_op2[i]) != '\0') {
        /* used for testing with build flag */
        if (print_file_building == TRUE) {
          printf("struct string:\tadress: %d\t ascii: %c\n",
                 (base_adress + IC + (cmd->DC)) + i - 1, temp_str_op2[i - 1]);
          printf("adding to .ob:\n%s\t%s\n\n\n",
                 base32_conv(str1, (base_adress + IC + (cmd->DC)) + i - 1),
                 base32_conv(str2, temp_str_op2[i - 1]));
        }

        /* print to final_code after converting to base 32 */
        fprintf(final_code, "%s\t%s\n",
                base32_conv(str1, (base_adress + IC + (cmd->DC)) + i - 1),
                base32_conv(str2, temp_str_op2[i - 1]));
        i++;
      }

      if (print_file_building == TRUE) { /* used for testing with build flag */
        printf("struct string:\tadress: %d\t ascii: \\0\n",
               (base_adress + IC + (cmd->DC)) + i - 1);
        printf("adding to .ob:\n%s\t%s\n\n\n",
               base32_conv(str1, (base_adress + IC + (cmd->DC)) + i - 1),
               base32_conv(str2, 0));
      }

      /* print '\0' to final_code after converting to base 32 */
      fprintf(final_code, "%s\t%s\n",
              base32_conv(str1, (base_adress + IC + (cmd->DC)) + i - 1),
              base32_conv(str2, 0));

      free_com_node(cmd_node);
    }
    /* entry_type */
    else if (cmd->op_type == entry_type) {
      /* save all entrys to .ent */
      save_entry_to_ent_file(symb_table, cmd, ent_file);

      free_com_node(cmd_node);
    } else
      free_com_node(cmd_node);
  }
  free_com_list(instructions_cmd_list);
  return error_flag;
}

/*  makes a string with base 32 representation of the command for .ob file

    @param  *dest_str the string to write to
    @param  *cmd the command to convert
    @param  com_ad_type unsigned int A/R/E of the two operands
    @return a string representation if the command in base 32 for .ob file */
char *get_base32_com_str(char *dest_str, com *cmd, unsigned int com_ad_type) {
  char str[WORD];
  unsigned int assembler_build_files;

  /* make sure dest_str is empty */
  dest_str[0] = '\0';

  /* build unsigned for command bin code (opcode - ad_type - 00) */
  assembler_build_files = (cmd->opcode << 6) + (com_ad_type);

  /* make string for .ob */
  strcat(dest_str, base32_conv(str, cmd->IC + base_adress));
  strcat(dest_str, "\t");
  strcat(dest_str, base32_conv(str, assembler_build_files));
  strcat(dest_str, "\n");

  if (print_adressing_flag == TRUE) { /* used for testing with adressing flag */
    printf(
        "command bin code: %d%d%d%d - %d%d - %d%d - %d%d "
        "(assembler_build_files)\n\n",
        (assembler_build_files >> 9) % 2, (assembler_build_files >> 8) % 2,
        (assembler_build_files >> 7) % 2, (assembler_build_files >> 6) % 2,
        (assembler_build_files >> 5) % 2, (assembler_build_files >> 4) % 2,
        (assembler_build_files >> 3) % 2, (assembler_build_files >> 2) % 2,
        (assembler_build_files >> 1) % 2, (assembler_build_files) % 2);
    if (print_file_building == FALSE) printf("\n\n");
  }

  return dest_str;
}

/*  makes a string with base 32 representation of the command's operands
    for .ob file and returning

    @param  *dest_str the string to write to
    @param  *source_op_str the first operand to convert (null for no operand)
    @param  *dest_op_str the seconed operand to convert (null for no operand)
    @param  *cmd the hole command
    @param  *symb_table symbol table of all the symbols available
    @param  *ext_file extenral file to write to when found ext refrence

    @return unsigned int representation of the operands in base 32 for .ob file
    and a string representation in base 32 of command operands at dest_str */
unsigned int get_base32_op_str(char *dest_str, char *source_op_str,
                               char *dest_op_str, com *cmd, map *symb_table,
                               FILE *ext_file) {
  int source_op_reg_index = -1, dest_op_reg_index = -1, adress;
  symbol *old_symb, temp_symb; /* used for looking in symbol table */
  char str[WORD];              /* used for conv functions */
  char *token;                 /* for temps */
  int offset, op_adress, temp;
  bool source_op_done_flag = FALSE, dest_op_done_flag = FALSE; /* Flags */
  unsigned com_ad_type = 0; /* used to pass adressing type command line */
  unsigned op_a_r_e;        /* used for a/r/e in operand line */

  if (print_adressing_flag == TRUE) { /* used for testing with adressing flag */
    printf("\n");
    print_com(cmd);
    printf("source_op: %s\tdest_op: %s\n\n", source_op_str, dest_op_str);
  }
  if (print_file_building == TRUE &&
      print_adressing_flag == FALSE) { /* used for testing with build flag */
    printf("\n");
    print_com(cmd);
    printf("\n");
  }

  /* match adress */
  adress = base_adress + cmd->IC;
  if (cmd->op_type == command_type_1 || cmd->op_type == command_type_2 ||
      cmd->op_type == command_type_2)
    adress++;

  dest_str[0] = '\0'; /* empty string for strcat */

  if (source_op_str != NULL) {
    /* check for reg source */
    source_op_reg_index = is_op_register(source_op_str);
    /* source reg index now holds the number of the register -1 if op not a
     * register*/
  }

  if (dest_op_str != NULL) {
    /* check for reg dest */
    dest_op_reg_index = is_op_register(dest_op_str);
    /* dest reg index now holds the number of the register and -1 if op not a
     * register */
  }

  /* if lea command and source op is a register */
  if (cmd->opcode == LEA_OPCODE && (source_op_reg_index != -1)) {
    printf(
        "line:  %d\t* ERROR! - illegal adressing type %s source op type "
        "cannot be a register *\n\n",
        cmd->line_index, cmd->operation);
    error_flag = TRUE;
  }

  /* if both operators are registers */
  if ((dest_op_reg_index != -1) && (source_op_reg_index != -1)) {
    /* dest_str = "reg1"-"reg2"-"00" */
    /* make string for .ob */
    strcat(dest_str, base32_conv(str, adress++));
    strcat(dest_str, "\t");
    strcat(dest_str, base32_conv(str, (source_op_reg_index << 6) +
                                          (dest_op_reg_index << 2)));
    strcat(dest_str, "\n");

    com_ad_type += BOTH_OP_REG << 2;

    /* used for testing with adressing flag */
    if (print_adressing_flag == TRUE) {
      printf("both adress:\t\t %d%d%d%d - %d%d%d%d - 00\n",
             (source_op_reg_index >> 3) % 2, (source_op_reg_index >> 2) % 2,
             (source_op_reg_index >> 1) % 2, (source_op_reg_index) % 2,
             (dest_op_reg_index >> 3) % 2, (dest_op_reg_index >> 2) % 2,
             (dest_op_reg_index >> 1) % 2, (dest_op_reg_index >> 0) % 2);

      printf(
          "both are reg, dest=3, source=3, com_ad_type, \tsorce: "
          "%d%d\tdest: %d%d  \n\n",
          (com_ad_type >> 5) % 2, (com_ad_type >> 4) % 2,
          (com_ad_type >> 3) % 2, (com_ad_type >> 2) % 2);
    }
    return com_ad_type; /* unsigned int "0000111100" */
  }

  /* if source op is a register */
  if (source_op_reg_index != -1) {
    com_ad_type += (register_ad_type << 4);
    source_op_done_flag = TRUE;
    /* dest_str = "reg1"-"000"-"00" */
    /* make string for .ob */
    strcat(dest_str, base32_conv(str, adress++));
    strcat(dest_str, "\t");
    strcat(dest_str, base32_conv(str, source_op_reg_index << 6));
    strcat(dest_str, "\n");

    /* used for testing with adressing flag */
    if (print_adressing_flag == TRUE) {
      printf("source op adress:\t %d%d%d%d - 0000 - 00\n\n",
             (source_op_reg_index >> 3), (source_op_reg_index >> 2) % 2,
             (source_op_reg_index >> 1) % 2, (source_op_reg_index) % 2);
      printf(
          "source op is reg, source= 3 , com_ad_type, \tsorce: "
          "%d%d\tdest: --\n\n",
          (com_ad_type >> 5) % 2, (com_ad_type >> 4) % 2);
    }
  }

  /* check if source op is absolute op */
  if ((source_op_str != NULL) && is_absolute(source_op_str) == TRUE &&
      source_op_done_flag == FALSE) {
    com_ad_type += (absolute_ad_type << 4);
    source_op_done_flag = TRUE;
    temp = atoi(source_op_str + 1); /* +1 to ignore the # */

    /* check if its a lea command and source op is absolute */
    if (cmd->opcode == LEA_OPCODE) {
      printf(
          "line:  %d\t* ERROR! - illegal adressing type %s source op type "
          "cannot be a absolute *\n\n",
          cmd->line_index, cmd->operation);
      error_flag = TRUE;
    }

    /* check that absolute value is within range */
    if (temp > ABS_MAX || temp < ABS_MIN) {
      printf(
          "line: %d\t* ERROR! - absolute value: %d needs to be bigger "
          "then %d and smaller then %d *\n\n",
          cmd->line_index, temp, ABS_MIN, ABS_MAX);
      error_flag = TRUE;
    }
    op_a_r_e = absolute_operand; /* 00 */

    /* make string for .ob */
    strcat(dest_str, base32_conv(str, adress++));
    strcat(dest_str, "\t");
    strcat(dest_str, base32_conv(str, (temp << 2) + op_a_r_e));
    strcat(dest_str, "\n");

    /* used for testing with adressing flag */
    if (print_adressing_flag == TRUE) {
      printf("source op adress: %d%d%d%d%d%d%d%d - %d%d\n\n",
             ((temp & BIT_MUSK_10BIT) >> 7) % 2,
             ((temp & BIT_MUSK_10BIT) >> 6) % 2,
             ((temp & BIT_MUSK_10BIT) >> 5) % 2,
             ((temp & BIT_MUSK_10BIT) >> 4) % 2,
             ((temp & BIT_MUSK_10BIT) >> 3) % 2,
             ((temp & BIT_MUSK_10BIT) >> 2) % 2,
             ((temp & BIT_MUSK_10BIT) >> 1) % 2, (temp & BIT_MUSK_10BIT) % 2,
             op_a_r_e >> 1, op_a_r_e % 2);

      printf(
          "source op is absolute, source= 0 , com_ad_type, \tsorce: "
          "%d%d\tdest: --\n\n",
          (com_ad_type >> 5) % 2, (com_ad_type >> 4) % 2);
    }
  }

  /* check if sorce op is a symbol in symb table */
  if ((source_op_str != NULL) && source_op_done_flag == FALSE) {
    /* used to look in symbol table */
    temp_symb.label = source_op_str;
    if ((old_symb = table_get_symbol(symb_table, &temp_symb)) != NULL) {
      /* if we found lable */
      source_op_done_flag = TRUE;
      /* check for extern type and match op_a_r_e */
      if (old_symb->type == extern_type) {
        save_to_ext_file(ext_file, adress, source_op_str);
        op_a_r_e = external_operand; /* 01 */
      } else {
        op_a_r_e = relocatable_operand; /* 10 */
      }
      com_ad_type += (direct_ad_type << 4); /* 01 */

      /* make adress from IC DC and base_adress, based on symbol type */
      if (old_symb->type == command_type_1 ||
          old_symb->type == command_type_2 ||
          old_symb->type == command_type_3) {
        op_adress = base_adress + old_symb->IC;
      } else if (old_symb->type == extern_type) { /* match correct adress */
        op_adress = EXTERN_ADRESS;
      } else { /* match correct adress */
        op_adress = base_adress + IC + old_symb->DC;
      }
      /* make string for .ob */
      strcat(dest_str, base32_conv(str, adress++));
      strcat(dest_str, "\t");
      strcat(dest_str, base32_conv(str, (op_adress << 2) + op_a_r_e));
      strcat(dest_str, "\n");

      /* used for testing with adressing flag */
      if (print_adressing_flag == TRUE) {
        printf("source op adress: %d%d%d%d%d%d%d%d - %d%d\n\n",
               (op_adress >> 7) % 2, (op_adress >> 6) % 2, (op_adress >> 5) % 2,
               (op_adress >> 4) % 2, (op_adress >> 3) % 2, (op_adress >> 2) % 2,
               (op_adress >> 1) % 2, (op_adress) % 2, op_a_r_e >> 1,
               op_a_r_e % 2);
        printf(
            "source op is direct, source= 1 , com_ad_type, \tsorce: "
            "%d%d\tdest: --\n\n",
            (com_ad_type >> 5) % 2, (com_ad_type >> 4) % 2);
      }
    }
    /* else check if this is a lable with offset  "lable.offset" */
    else {
      /* token holds the label untill . */
      token = strtok(source_op_str, ".");
      temp_symb.label = token;
      if ((old_symb = table_get_symbol(symb_table, &temp_symb)) == NULL) {
        printf("line: %d\t* ^ERROR! - lable: \"%s\" no refrence to lable *\n\n",
               cmd->line_index, source_op_str);
        error_flag = TRUE;
        return 0;
      }

      /* we found a label maching */
      /* get the offset part */
      token = strtok(NULL, "\0");
      /* check that offset is llegal */
      if ((offset = get_offset(token)) == -1) {
        /* offset is illegal */
        printf("line: %d \t* ^ERROR! - offset: \"%s\" offset is illegal *\n\n",
               cmd->line_index, token);
        error_flag = TRUE;
      }

      else {
        /* check for extern symbol */
        if (old_symb->type == extern_type) {
          save_to_ext_file(ext_file, adress, source_op_str);
          op_a_r_e = external_operand; /* 01 */
        } else {
          op_a_r_e = relocatable_operand; /* 10 */
        }
        com_ad_type += (struct_ad_type << 4); /* 10 */

        /* make adress from IC DC and base_adress, based on symbol type */
        if (old_symb->type == command_type_1 ||
            old_symb->type == command_type_2 ||
            old_symb->type == command_type_3) {
          op_adress = base_adress + old_symb->IC;
        } else if (old_symb->type == extern_type) { /* match correct adress */
          op_adress = EXTERN_ADRESS;
        } else { /* match correct adress */
          op_adress = base_adress + IC + old_symb->DC;
        }

        source_op_done_flag = TRUE;
        /* make string for .ob */
        strcat(dest_str, base32_conv(str, adress++));
        strcat(dest_str, "\t");
        strcat(dest_str, base32_conv(str, (op_adress << 2) + op_a_r_e));
        strcat(dest_str, "\n");
        strcat(dest_str, base32_conv(str, adress++));
        strcat(dest_str, "\t");
        strcat(dest_str, base32_conv(str, (offset << 2)));
        strcat(dest_str, "\n");
      }

      /* used for testing with adressing flag */
      if (print_adressing_flag == TRUE) {
        printf("source op adress:\t %d%d%d%d%d%d%d%d - %d%d\n",
               (op_adress >> 7) % 2, (op_adress >> 6) % 2, (op_adress >> 5) % 2,
               (op_adress >> 4) % 2, (op_adress >> 3) % 2, (op_adress >> 2) % 2,
               (op_adress >> 1) % 2, (op_adress) % 2, op_a_r_e >> 1,
               op_a_r_e % 2);
        printf("source op offset adress: %d%d%d%d%d%d%d%d - 00\n\n",
               (offset >> 7) % 2, (offset >> 6) % 2, (offset >> 5) % 2,
               (offset >> 4) % 2, (offset >> 3) % 2, (offset >> 2) % 2,
               (offset >> 1) % 2, (offset) % 2);
        printf(
            "source op is struct, source= 2 , com_ad_type, \tsorce: "
            "%d%d\tdest: --\n\n",
            (com_ad_type >> 5) % 2, (com_ad_type >> 4) % 2);
      }
    }
  }
  /* found a register */
  if (dest_op_reg_index != -1) {
    com_ad_type += (register_ad_type << 2); /* "1100" */
    dest_op_done_flag = TRUE;
    /* make string for .ob */
    strcat(dest_str, base32_conv(str, adress++));
    strcat(dest_str, "\t");
    strcat(dest_str, base32_conv(str, dest_op_reg_index << 2));
    strcat(dest_str, "\n");

    /* used for testing with adressing flag */
    if (print_adressing_flag == TRUE) {
      printf("dest op adress:\t\t 0000 - %d%d%d%d - 00\n\n",
             (dest_op_reg_index >> 3) % 2, (dest_op_reg_index >> 2) % 2,
             (dest_op_reg_index >> 1) % 2, (dest_op_reg_index) % 2);

      printf(
          "dest op is reg, dest= 3 , com_ad_type, \t\tsorce: %d%d\tdest: "
          "%d%d  \n\n",
          (com_ad_type >> 5) % 2, (com_ad_type >> 4) % 2,
          (com_ad_type >> 3) % 2, (com_ad_type >> 2) % 2);
    }
  }

  /* check for absolute op */
  if ((dest_op_str != NULL) && is_absolute(dest_op_str) == TRUE &&
      dest_op_done_flag == FALSE) {
    com_ad_type += (absolute_ad_type << 2); /* "0000" */
    dest_op_done_flag = TRUE;
    op_a_r_e = absolute_operand;
    temp = atoi(dest_op_str + 1); /* +1 to ignore the # */
    if (temp > ABS_MAX || temp < ABS_MIN) {
      printf(
          "line: %d\t* ERROR! - absolute value: %d needs to be bigger "
          "then %d and smaller then %d *\n\n",
          cmd->line_index, temp, ABS_MIN, ABS_MAX);
      error_flag = TRUE;
    }

    /* check if command is diffrent then prn and cmp and dest of is absolute */
    if (cmd->opcode != CMP_OPCODE && cmd->opcode != PRN_OPCODE) {
      printf(
          "line:  %d\t* ERROR! - illegal adressing type %s destenation op "
          "type "
          "cannot be absolute *\n\n",
          cmd->line_index, cmd->operation);
      error_flag = TRUE;
    }
    /* make string for .ob */
    strcat(dest_str, base32_conv(str, adress++));
    strcat(dest_str, "\t");
    strcat(dest_str, base32_conv(str, (temp << 2) + op_a_r_e));
    strcat(dest_str, "\n");

    /* used for testing with adressing flag */
    if (print_adressing_flag == TRUE) {
      printf("dest op adress:\t\t %d%d%d%d%d%d%d%d - %d%d\n\n",
             ((temp & BIT_MUSK_10BIT) >> 7) % 2,
             ((temp & BIT_MUSK_10BIT) >> 6) % 2,
             ((temp & BIT_MUSK_10BIT) >> 5) % 2,
             ((temp & BIT_MUSK_10BIT) >> 4) % 2,
             ((temp & BIT_MUSK_10BIT) >> 3) % 2,
             ((temp & BIT_MUSK_10BIT) >> 2) % 2,
             ((temp & BIT_MUSK_10BIT) >> 1) % 2, ((temp & BIT_MUSK_10BIT)) % 2,
             op_a_r_e >> 1, op_a_r_e % 2);

      printf(
          "dest op is absolute, dest= 0 , com_ad_type, \tsorce: %d%d\tdest: "
          "%d%d  \n\n",
          (com_ad_type >> 5) % 2, (com_ad_type >> 4) % 2,
          (com_ad_type >> 3) % 2, (com_ad_type >> 2) % 2);
    }
  }

  /* look for symbol in seconed op */
  if ((dest_op_str != NULL) && dest_op_done_flag == FALSE) {
    /* used to look in symbol table */
    temp_symb.label = dest_op_str;
    if ((old_symb = table_get_symbol(symb_table, &temp_symb)) != NULL) {
      /* if we found lable */
      dest_op_done_flag = TRUE;
      /* check for extern type and match op_a_r_e */
      if (old_symb->type == extern_type) {
        save_to_ext_file(ext_file, adress, dest_op_str);
        op_a_r_e = external_operand; /* 01 */
      } else {
        op_a_r_e = relocatable_operand; /* 10 */
      }
      com_ad_type += (direct_ad_type << 2); /* 10 */

      /* make adress from IC DC and base_adress, based on symbol type */
      if (old_symb->type == command_type_1 ||
          old_symb->type == command_type_2 ||
          old_symb->type == command_type_3) {
        op_adress = base_adress + old_symb->IC;
      } else if (old_symb->type == extern_type) { /* match correct adress */
        op_adress = EXTERN_ADRESS;
      } else { /* match correct adress */
        op_adress = base_adress + IC + old_symb->DC;
      }
      /* make string for .ob */
      strcat(dest_str, base32_conv(str, adress++));
      strcat(dest_str, "\t");
      strcat(dest_str, base32_conv(str, (op_adress << 2) + op_a_r_e));
      strcat(dest_str, "\n");

      /* used for testing with adressing flag */
      if (print_adressing_flag == TRUE) {
        printf("dest op adress:\t\t %d%d%d%d%d%d%d%d - %d%d\n\n",
               (op_adress >> 7) % 2, (op_adress >> 6) % 2, (op_adress >> 5) % 2,
               (op_adress >> 4) % 2, (op_adress >> 3) % 2, (op_adress >> 2) % 2,
               (op_adress >> 1) % 2, (op_adress) % 2, op_a_r_e >> 1,
               op_a_r_e % 2);

        printf(
            "dest op is direct, dest= 1 , com_ad_type, \tsorce: %d%d\tdest: "
            "%d%d  \n\n",
            (com_ad_type >> 5) % 2, (com_ad_type >> 4) % 2,
            (com_ad_type >> 3) % 2, (com_ad_type >> 2) % 2);
      }
    }
    /* else check if this is a lable with offset  "lable.offset" */
    else {
      /* token holds the lable untill . */
      token = strtok(dest_op_str, ".");
      temp_symb.label = token;
      /* look for lable in symbol table */
      if ((old_symb = table_get_symbol(symb_table, &temp_symb)) == NULL) {
        printf(
            "line: %d \t* ^ERROR! - lable: \"%s\" no refrence to lable "
            "*\n\n",
            cmd->line_index, dest_op_str);
        error_flag = TRUE;
        return 0;
      }
      /* we found a label maching */
      /* get the offset part */
      token = strtok(NULL, "\0");
      /* check that offset is llegal */
      if ((offset = get_offset(token)) == -1) {
        /* offset is illegal */
        printf("line: %d \t* ^ERROR! - offset:\"%s\" offset is illegal *\n\n",
               cmd->line_index, token);
        error_flag = TRUE;
      }

      /* offset is llegal */
      else {
        /* check for extern type and match op_a_r_e */
        if (old_symb->type == extern_type) {
          save_to_ext_file(ext_file, adress, dest_op_str);
          op_a_r_e = external_operand; /* 01 */
        } else {
          op_a_r_e = relocatable_operand; /* 10 */
        }
        com_ad_type += (struct_ad_type << 2); /* 10 */

        /* make adress from IC DC and base_adress, based on symbol type */
        if (old_symb->type == command_type_1 ||
            old_symb->type == command_type_2 ||
            old_symb->type == command_type_3) {
          op_adress = base_adress + old_symb->IC;
        } else if (old_symb->type == extern_type) { /* match correct adress */
          op_adress = EXTERN_ADRESS;
        } else { /* match correct adress */
          op_adress = base_adress + IC + old_symb->DC;
        }

        dest_op_done_flag = TRUE;
        /* make string for .ob */
        strcat(dest_str, base32_conv(str, adress++));
        strcat(dest_str, "\t");
        strcat(dest_str, base32_conv(str, (op_adress << 2) + op_a_r_e));
        strcat(dest_str, "\n");
        strcat(dest_str, base32_conv(str, adress++));
        strcat(dest_str, "\t");
        strcat(dest_str, base32_conv(str, (offset << 2)));
        strcat(dest_str, "\n");
      }

      /* used for testing with adressing flag */
      if (print_adressing_flag == TRUE) {
        printf("dest op adress:\t\t\t %d%d%d%d%d%d%d%d - %d%d\n",
               (op_adress >> 7) % 2, (op_adress >> 6) % 2, (op_adress >> 5) % 2,
               (op_adress >> 4) % 2, (op_adress >> 3) % 2, (op_adress >> 2) % 2,
               (op_adress >> 1) % 2, (op_adress) % 2, op_a_r_e >> 1,
               op_a_r_e % 2);
        printf("dest op offset adress: %d%d%d%d%d%d%d%d - 00\n\n",
               (offset >> 7) % 2, (offset >> 6) % 2, (offset >> 5) % 2,
               (offset >> 4) % 2, (offset >> 3) % 2, (offset >> 2) % 2,
               (offset >> 1) % 2, (offset) % 2);
        printf(
            "dest op is struct, dest= 2 , com_ad_type, \tsorce: %d%d\tdest: "
            "%d%d  \n\n",
            (com_ad_type >> 5) % 2, (com_ad_type >> 4) % 2,
            (com_ad_type >> 3) % 2, (com_ad_type >> 2) % 2);
      }
    }
  }
  if (dest_op_done_flag == FALSE || dest_op_done_flag == FALSE) {
    printf("get_base32_op_str ERROR!\n");
    error_flag = TRUE;
  }
  return com_ad_type;
}

/*  writes to ext_file all the refrences to external symbol

    @param  *ext_file extenral file to write to when found ext refrence
    @param  adress the adress of the refrence
    @param  *label string name of the symbol */
void save_to_ext_file(FILE *ext_file, int adress, char *label) {
  char str[WORD];
  if (print_adressing_flag == TRUE) {
    printf("external adress:\t %d%d%d%d%d%d%d%d%d%d\t(for .ext)\n",
           (adress >> 9) % 2, (adress >> 8) % 2, (adress >> 7) % 2,
           (adress >> 6) % 2, (adress >> 5) % 2, (adress >> 4) % 2,
           (adress >> 3) % 2, (adress >> 2) % 2, (adress >> 1) % 2,
           (adress) % 2);
  }
  if (print_file_building == TRUE) { /* used for testing with build flag */
    printf("adding to .ext:\nlabel: %s\t adress: %d\n", label, adress);
    printf("adding to .ext:\n%s\t%s\n\n\n", label, base32_conv(str, adress));
  }
  fprintf(ext_file, "%s\t%s\n", label, base32_conv(str, adress));
}

/* converts an int to two base 32 letters string
    @param  *str the string to right to
    @param  i the int to convert
    @return a string representation in base 32  */
char *base32_conv(char *str, int i) {
  int num;

  str[0] = '\0';

  num = ((i & BIT_MUSK_10BIT) / 32) % 32;
  strcat(str, base32_conv_ary[num]);
  num = (i & BIT_MUSK_10BIT) % 32;
  strcat(str, base32_conv_ary[num]);

  return str;
}

/* converts an int to one base 32 letter string
    @param  *str the string to right to
    @param  i the int to convert
    @return a string representation in base 32  */
char *base32_conv1(char *str, int i) {
  int num;
  str[0] = '\0';

  if (i < 32) {
    num = (i & BIT_MUSK_10BIT) % 32;
    strcat(str, base32_conv_ary[num]);
  } else {
    num = ((i & BIT_MUSK_10BIT) / 32) % 32;
    strcat(str, base32_conv_ary[num]);
    num = (i & BIT_MUSK_10BIT) % 32;
    strcat(str, base32_conv_ary[num]);
  }

  return str;
}

void save_entry_to_ent_file(map *symb_table, com *cmd, FILE *ent_file) {
  node *temp_node1;
  char *temp_str_op1;
  symbol temp_symb, *old_symb;
  char str2[LONG_WORD];
  int op_adress;

  temp_node1 = list_pop(cmd->oplist);
  if (temp_node1 == NULL) { /* stop condition end of list */
    return;
  }

  temp_str_op1 = (char *)get_data(temp_node1);

  /* recursively call save_entry_to_ent_file */
  save_entry_to_ent_file(symb_table, cmd, ent_file);

  list_push(cmd->oplist, temp_node1);

  temp_symb.label = temp_str_op1; /* make temp symb to look in symbol table */

  if ((old_symb = table_get_symbol(symb_table, &temp_symb)) == NULL) {
    printf("line: %d* ERROR! - no refrence to lable :\"%s\" *\n\n",
           cmd->line_index, temp_symb.label);
    error_flag = TRUE;
    return;
  }
  /* if we are here we found a symbol */

  /* make sure refrence is not external */
  if (old_symb->type == extern_type) {
    printf(
        "line: %d* ERROR! - entry:\"%s\" is cannot refrence an external "
        "symbol *\n\n",
        cmd->line_index, temp_symb.label);
    error_flag = TRUE;
    return;
  }

  /* make sure lable is not in entry file alredy */
  if (old_symb->ent_used_flag == TRUE) {
    printf("line: %d* ERROR! - entry:\"%s\" was refrenced before *\n\n",
           cmd->line_index, temp_symb.label);
    error_flag = TRUE;
  }

  /* match adress with IC and DC to correct adress */
  if (old_symb->type == command_type_1 || old_symb->type == command_type_2 ||
      old_symb->type == command_type_3) {
    op_adress = base_adress + old_symb->IC;
  } else {
    op_adress = base_adress + IC + old_symb->DC;
  }

  /* used for testing with adressing flag */
  if (print_adressing_flag == TRUE) {
    printf("entry adress: %d%d%d%d%d%d%d%d%d%d\t (for .ent)\n",
           (op_adress >> 9) % 2, (op_adress >> 8) % 2, (op_adress >> 7) % 2,
           (op_adress >> 6) % 2, (op_adress >> 5) % 2, (op_adress >> 4) % 2,
           (op_adress >> 3) % 2, (op_adress >> 2) % 2, (op_adress >> 1) % 2,
           (op_adress) % 2);
  }
  if (print_file_building == TRUE) { /* used for testing with build flag */
    printf("adding to .ent:\nlabel: %s\t adress: %d\n", temp_str_op1,
           op_adress);
    printf("adding to .ent:\n%s\t%s\n\n\n", temp_str_op1,
           base32_conv(str2, op_adress));
  }
  /* print to file */
  fprintf(ent_file, "%s\t%s\n", temp_str_op1, base32_conv(str2, op_adress));
  old_symb->ent_used_flag = TRUE;
}

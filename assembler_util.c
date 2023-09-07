#include "data.h"
#include "extern.h"

/*  breaks a string command into:
    label, operation and operands (saved in a operand_list)
    @param line the string to work on
    @return Command in com struct */
com *break_com(map *reserved_table, char *line) {
  com *cmd;
  int i;

  /* allocating label memory */
  cmd = make_command();
  if (cmd == NULL) {
    error_flag = TRUE;
    return NULL;
  }

  /* check if label exists and if not skip it */
  if ((i = skip_label(line)) != 0) {
    cmd->label = get_label(line); /* save lable to cmd->lable */
  }

  i = skip_spaces(line, i);

  /* get the command operation (first word after lable) */
  cmd->operation = get_com_operation(line, &i);

  i = skip_spaces(line, i);

  /* saves all the operands to a list called oplist */
  cmd->oplist = get_op_list(line, i);

  /* saves the line number */
  cmd->line_index = line_count;

  /* checks if command is llegal at */
  if (is_cmd_llegal(reserved_table, cmd, line) == FALSE) {
    free_com(cmd);
    return NULL;
  }
  return cmd;
}

/*  creates a new stiring that gets the lable from given string string and
    returns it
    @param line the string to work on
    @return a string that holds the lable */
char *get_label(char *line) {
  char *label;
  int i = 0, j = 0;
  char c;

  /* allocating label memory */
  if ((label = (char *)malloc((LABLE_LENGTH + 1) * sizeof(char))) == NULL) {
    printf("ERROR! - allocating label memory\n");
    error_flag = TRUE;
    return NULL;
  }

  i = skip_spaces(line, i);

  c = line[i++];
  if (isalpha(c) == 0) { /* if c is not alpha print error and return */
    printf(
        "line %d:\t%s* ^ERROR - a label needs to start with an alphabetic "
        "letter *\n\n",
        line_count, line);
    error_flag = TRUE;
    free(label);
    return NULL;
  }
  label[j++] = c;

  while (isalnum(c = line[i])) /* saves label to label */
  {
    if (j >= (LABLE_LENGTH - 1)) {
      printf("line %d:\t%s* ^ERROR - label is too long *\n\n", line_count,
             line);
      error_flag = TRUE;
      return NULL;
    }
    label[j++] = c;
    i++;
  }
  label[j] = '\0'; /* and end of string letter */

  i = skip_spaces(line, i);

  if ((c = line[i]) != ':') { /* checks that lable ends with : */
    printf("line %d:\t%s* ^ERROR - label is illegal *\n\n", line_count, line);
    free(label);
    error_flag = TRUE;
    return NULL;
  }
  return label;
}

/*  creates a new stiring that gets the operation from given string string while
    advencing *i to the next letter after the operation

    @param line the string to work on
    @param *i the index after label ended
    @return a string that holds the operation */
char *get_com_operation(char *line, int *i) {
  int j = 0;
  char *str;
  char c;

  /* allocating label memory */
  if ((str = (char *)malloc((OP_LEGNTH + 1) * sizeof(char))) == NULL) {
    printf("ERROR! - allocating label memory\n");
    error_flag = TRUE;
    return NULL;
  }

  /* collect . for .string .data etc */
  if ((c = line[(*i)]) == '.') {
    str[j++] = c; /* save to str */
    (*i)++;
  }

  /* loop to collect the operation to str */
  while (isalpha(c = line[(*i)])) {
    if (j >= (OP_LEGNTH - 1)) { /* check that operation is not to long */
      printf("line %d:\t%s* ^ERROR - illegal operator *\n\n", line_count, line);
      error_flag = TRUE;
      free(str);
      return NULL;
    }
    str[j++] = c; /* save to str */
    (*i)++;
  }

  str[j] = '\0'; /* and end of string letter */

  return str;
}

/*  creates a new list that holds all the operands from a given string

    @param line the string to work on
    @param *i the index after the operation ended
    @return a list that holds all the operands */
node *get_op_list(char *line, int k) {
  node *oplist;
  char *str;
  char c;
  int i = k, sign_flag = 1;
  bool in_flag = FALSE, missing_op = FALSE;
  /* sign flag make sure there is a ',' for every op and no 2 ',' */

  oplist = make_list(); /* allocate list memory */

  if (oplist == NULL) { /* check if memory was alocated */
    error_flag = TRUE;
    return NULL;
  }

  str = get_op(line, &i); /* get a single op while advanceing i */
  /* loop to go over all the operans until end of line */
  while ((c = line[i]) != '\n' && c != '\0') {
    in_flag = TRUE;
    if (str != NULL) { /* if we found an operand */
      sign_flag = 0;
      add_to_tail(oplist, str); /* add str to oplist */
    }

    if ((c = line[i]) == ',') { /* make sure there is a ',' after the op */
      i++;
      sign_flag++;
    }
    if (sign_flag != 1) {
      missing_op = TRUE;
    }

    str = get_op(line, &i); /* get a single op while advanceing i */
  }

  if (str != NULL) {          /* if we found an operand */
    add_to_tail(oplist, str); /* add last operand */
  }

  /* if  sign_flag stoped the loop print error */
  if (missing_op == TRUE || (str == NULL && in_flag == TRUE)) {
    error_flag = TRUE;
    printf("line %d:\t%s* ^ERROR - missing operaned *\n\n", line_count, line);
  }

  return oplist;
}

/*  creates a new stiring that gets the next operand from given string string
    while advencing *i to the next letter after the operand

    @param line the string to work on
    @param *i the index after label ended
    @return a string that holds the operation */
char *get_op(char *line, int *i) {
  char *str;
  char c;
  int j, found_flag = 0;

  /* allocating str memory */
  if ((str = (char *)malloc((LABLE_LENGTH + 1) * sizeof(char))) == NULL) {
    printf("ERROR! - allocating operand memory\n");
    error_flag = TRUE;
    return NULL;
  }
  j = 0;
  *i = skip_spaces(line, *i);

  if ((c = line[(*i)]) == '\"') { /* get a string */
                                  /* increase str size */
    if ((str = (char *)realloc(str, (LINE_LENGTH) * sizeof(char))) == NULL) {
      printf("ERROR! - reallocating operand memory\n");
      error_flag = TRUE;
      return NULL;
    }
    str[j++] = c; /* save to str */ /* save to str */
    found_flag = 1;
    (*i)++;

    while ((c = line[*i]) != '\0') {
      if (c == '\"' && line[*i - 1] != '\\') { /* stop condition */
        str[j++] = c; /* save to str */        /* save to str */
        found_flag = 1;
        (*i)++;
        break;
      }

      /* check that the length of the op is not too long */
      if (j >= (LINE_LENGTH - 1)) {
        printf(
            "line %d:\t%s* ^ERROR - operaned name is too long max is %d *\n\n",
            line_count, line, LINE_LENGTH);
        error_flag = TRUE;
        str[j] = '\0';
        free(str);
        return NULL;
      }

      str[j++] = c; /* save to str */
      found_flag = 1;
      (*i)++;
    }
  }

  else /* get a normal operand */
    while (((c = line[*i]) != '\n') && (c != '\0') && (c != ',') &&
           (c != ' ') && (c != '\t')) {
      /* check that the length of the op is not too long */
      if (j >= (LABLE_LENGTH - 1)) {
        printf(
            "line %d:\t%s* ^ERROR - operaned name is too long max is %d *\n\n",
            line_count, line, LABLE_LENGTH);
        error_flag = TRUE;
        free(str);
        return NULL;
      }

      str[j++] = c; /* save to str */
      found_flag = 1;
      (*i)++;
    }

  str[j] = '\0';
  *i = skip_spaces(line, *i);
  c = line[*i];

  if ((c != '\n') && (c != '\0') && (c != ',')) {
    printf("line %d:\t%s* ^ERROR - found illegal operand *\n\n", line_count,
           line);
    error_flag = TRUE;
    free(str);
    return NULL;
  }

  if (found_flag == 0) {
    free(str);
    return NULL;
  }

  return str;
}

/*  this func returns the machine code legnth of the command

    @param *cmd the command to get the length from
    @return the length in machine code of the command */
int command_length(com *cmd) {
  node *temp_node1, *temp_node2;
  int temp_int;
  char *temp_str_op1, *temp_str_op2;

  switch (cmd->op_type) {
    case string_type:
      temp_node1 = list_pop(cmd->oplist);
      /* the string length includes two '\"' but not '\0' that we are about */
      temp_int = strlen((char *)get_data(temp_node1)) - 1;
      list_push(cmd->oplist, temp_node1);

      return temp_int;
      break;

    case data_type:
      return list_len(cmd->oplist);
      break;

    case struct_type: /* we need to check only the string length for struct */
      temp_node1 = list_pop(cmd->oplist);
      temp_node2 = list_pop(cmd->oplist);
      /* the string length includes two '\"' but not '\0' that we are about and
       * we count and number at temp */
      temp_int = strlen((char *)get_data(temp_node2));
      list_push(cmd->oplist, temp_node2);
      list_push(cmd->oplist, temp_node1);

      return temp_int;
      break;

    case extern_type:
      return 0;
      break;

    case entry_type:
      return 0;
      break;

    case command_type_1:
      /* get the two operands */
      temp_node1 = list_pop(cmd->oplist);
      temp_node2 = list_pop(cmd->oplist);
      temp_str_op1 = get_data(temp_node1);
      temp_str_op2 = get_data(temp_node2);
      list_push(cmd->oplist, temp_node2);
      list_push(cmd->oplist, temp_node1);

      /* return 1 (for command) + legnth of each operand */
      return 1 + get_operator_bin_len(temp_str_op1, temp_str_op2);

      break;

    case command_type_2:
      /*  pop operand and get data  */
      temp_node1 = list_pop(cmd->oplist);
      temp_str_op1 = get_data(temp_node1);
      list_push(cmd->oplist, temp_node1);
      /* return 1 (for command) + legnth of the operand */
      return 1 + get_operator_bin_len(temp_str_op1, NULL);
      break;

    case command_type_3: /* allways one */
      return 1;
      break;

    default:
      break;
  }

  printf("line %d:\t* command_length ERROR *\n\n", cmd->line_index);
  error_flag = TRUE;
  return 0;
}

/*  checks if command is llegal

    @param *reserved_table  the adress to reserve table with reserved words
    @param *cmd the adress to the command to check
    @param *line the current line for error massage

    @return true if command is llegal and false if not */
bool is_cmd_llegal(map *reserved_table, com *cmd, char *line) {
  reserv temp_res, *reserv_p;

  if (cmd->operation == NULL) {
    printf("line %d:\t%s* ^ERROR - illegal command missing operation *\n\n",
           line_count, line);
    error_flag = TRUE;
    return FALSE;
  }

  temp_res.title = cmd->operation;

  /* look cor command operation in reserved map */
  reserv_p = table_get_reserved(reserved_table, &temp_res);
  if (reserv_p == NULL) {
    printf("line %d:\t%s* ^ERROR - illegal command *\n\n", line_count, line);
    error_flag = TRUE;
    return FALSE;
  }

  /* get cmd_type and opcode from reserve table */
  cmd->op_type = reserv_p->op_type;
  cmd->opcode = reserv_p->opcode;

  /*   if (cmd->op_type == struct_type || cmd->op_type == data_type ||
        cmd->op_type == string_type) {
      if (cmd->label == NULL) {
        printf(
            "line %d:\t%s* ^ERROR - instruction line must include a lable*\n\n",
            line_count, line);
        error_flag = TRUE;
        return FALSE;
      }
    } */ /* not needed for project */

  /* check that operands match */
  if (check_cmd_operands(cmd->op_type, cmd->oplist, line) == TRUE) {
    return TRUE;
  }
  error_flag = TRUE;
  return FALSE;
}

/*  checks if operands are llegal by type

    @param *op_list_head  the head of the op_list
    @param type  the type of command we check
    @param *line the current line for error massage

    @return true if operands are llegal and false if not */
bool check_cmd_operands(operation_type type, node *op_list_head, char *line) {
  node *temp, *temp2;
  bool token = FALSE, token2 = FALSE;
  char *str;

  switch (type) {
    case command_type_1: /* mov */
      if (list_len(op_list_head) == 2) {
        return TRUE;
      }
      printf("line %d:\t%s* ^ERROR - illegal number of operands *\n\n",
             line_count, line);
      error_flag = TRUE;
      return FALSE;
      break;

      /* operator type 2 */
    case command_type_2: /* not/ */

      if (list_len(op_list_head) == 1) {
        return TRUE;
      }
      printf("line %d:\t%s* ^ERROR - illegal number of operands *\n\n",
             line_count, line);
      error_flag = TRUE;
      return FALSE;
      break;

      /* operator type 3 */
    case command_type_3: /* rts */
      if (list_len(op_list_head) == 0) {
        return TRUE;
      }
      printf("line %d:\t%s* ^ERROR - illegal number of operands *\n\n",
             line_count, line);
      error_flag = TRUE;
      return FALSE;
      break;

    case data_type /* data */:

      if (list_len(op_list_head) == 0) {
        printf("line %d:\t%s* ^ERROR - illegal number of operands *\n\n",
               line_count, line);
        error_flag = TRUE;
        return FALSE;
      }

      return check_data_op_list(op_list_head);
      break;

    case struct_type /* struct */:
      if (list_len(op_list_head) != 2) {
        printf("line %d:\t%s* ^ERROR - illegal number of operands *\n\n",
               line_count, line);
        error_flag = TRUE;
        return FALSE;
      }

      temp = list_pop(op_list_head);
      temp2 = list_pop(op_list_head);

      /* check the first cell for a number */
      token = check_data_op(get_data(temp));
      /* check seconed cell for string */
      token2 = check_string_op(get_data(temp2));

      list_push(op_list_head, temp2);
      list_push(op_list_head, temp);

      return (token && token2);
      break;

    case string_type /* string */:

      if (list_len(op_list_head) != 1) {
        printf("line %d:\t%s* ^ERROR - illegal number of operands *\n\n",
               line_count, line);
        error_flag = TRUE;
        return FALSE;
      }

      temp = list_pop(op_list_head);
      str = (char *)get_data(temp);
      list_push(op_list_head, temp);

      return check_string_op(str);
      break;

    case extern_type /* extern */:
      if (list_len(op_list_head) == 0) {
        printf("line %d:\t%s* ^ERROR - illegal number of operands *\n\n",
               line_count, line);
        error_flag = TRUE;
        return FALSE;
      }

      return check_extern_op_list(op_list_head);
      break;

    case entry_type /* entry */:
      if (list_len(op_list_head) == 0) {
        printf("line %d:\t%s* ^ERROR - illegal number of operands *\n\n",
               line_count, line);
        error_flag = TRUE;
        return FALSE;
      }

      return check_entry_op_list(op_list_head);
      break;

    default:
      break;
  }

  printf("line %d:\t%s* ^ERROR - illegal command *\n\n", line_count, line);
  error_flag = TRUE;
  return FALSE;
}

/* check if str represents a number

    @param *str string to check

    @return true if data is llegal and false if not*/
bool check_data_op(char *str) {
  int i = 0;
  char c = str[0];

  if (c == '+' || c == '-') { /* first letter is allowed to be + or - */
    i++;
  }

  while (isdigit(c = str[i++])) /* skip digits*/
    ;

  if (c != '\0') {
    error_flag = TRUE;
    printf("line %d: instruction * ERROR: %s needs to represent a number *\n\n",
           line_count, str);
    return FALSE;
  }

  return TRUE;
}

/* checks if data oplist operands are llegal

  @param *list_head the head of oplist

  @return true if data oplist is llegal and false if not*/
bool check_data_op_list(node *list_head) {
  node *temp;
  bool token1, token2;

  temp = list_pop(list_head);

  if (temp == NULL) {
    return TRUE;
  }
  token1 = check_data_op(get_data(temp));
  /* recursively call check_data_op_list */
  token2 = check_data_op_list(list_head);
  list_push(list_head, temp);

  return token1 && token2;
}

/* checks if string operand is llegal

  @param *str string to check

  @return true if sting is llegal and false if not */
bool check_string_op(char *str) {
  int i = 0;
  if (str[i++] != '\"') {
    error_flag = TRUE;
    printf("line %d: instruction * ERROR: %s is an illegal string *\n\n",
           line_count, str);
    return FALSE;
  }

  while (str[i] != '\0') i++;

  if (str[i - 1] != '\"' || str[i - 2] == '\\') {
    error_flag = TRUE;
    printf("line %d: instruction * ERROR: %s is an illegal string *\n\n",
           line_count, str);
    return FALSE;
  }

  return TRUE;
}

/* checks if operand of extern is llegal

  @param *str string to check

  @return true if extern is llegal and false if not*/
bool check_extern_op_list(node *op_list_head) {
  node *temp;
  bool token1, token2;

  temp = list_pop(op_list_head);

  if (temp == NULL) {
    return TRUE;
  }
  token1 = check_extern_op(temp);
  /* recursively call check_extern_op_list */
  token2 = check_extern_op_list(op_list_head);
  list_push(op_list_head, temp);

  return token1 && token2;
}

/* checks if operand list of entry is llegal

  @param *op_list_head oplist to check

  @return true if entery oplist is llegal and false if not*/
bool check_entry_op_list(node *op_list_head) {
  return check_extern_op_list(op_list_head);
}

/* checks if operand of extern is llegal

    @param *p the node to check

    @return true if extern is llegal and false if not*/
bool check_extern_op(node *p) { return check_entry_op(p); }

/* checks if operand of entry is llegal

    @param *p the node to check

    @return true if entry is llegal and false if not*/
bool check_entry_op(node *p) {
  int i = 0;
  char c, *str;

  if (p == NULL) {
    error_flag = TRUE;
    return FALSE;
  }

  if ((str = (char *)get_data(p)) == NULL) {
    error_flag = TRUE;
    return FALSE;
  }

  str = (char *)get_data(p);
  c = str[i++];
  if (isalpha(c) == 0) { /* if c in not alpha */
    printf(
        "line %d: lable name: \"%s\"* ERROR - lable needs to start with an "
        "alphabetic letter\n\n",
        line_count, str);

    error_flag = TRUE;
    return FALSE;
  }

  while ((c = str[i++]) != '\0') /* check for llegal entry */
  {
    if (isalnum(c) == 0) { /* i cis not alphanumeric */
      printf(
          "line %d: lable name: \"%s\"* ERROR - lable contains non "
          "alphanumeric letters\n\n",
          line_count, str);

      error_flag = TRUE;
      return FALSE;
    }
    if (i >= (LABLE_LENGTH - 1)) {
      printf(
          "line %d: lable name: \"%s\"* ERROR - lable name is too long max "
          "length is %d\n\n",
          line_count, str, LABLE_LENGTH);

      error_flag = TRUE;
      return FALSE;
    }
  }

  return TRUE;
}

/*  adds an extern symbol to a table

    @param *symb_table symbol table
    @param *reserved_table reserved word table
    @param *line for errors
    @param *cmd the command

    @return TRUE extern was added
    @return FALSE if extern is in the list already or */
bool extern_add_to_symb_table(map *symb_table, map *reserved_table, char *line,
                              com *cmd) {
  symbol *symb;
  node *temp_node;
  char *temp_str;
  bool token;
  reserv temp_res;

  temp_node = list_pop(cmd->oplist);
  if (temp_node == NULL) {
    return TRUE;
  }
  temp_str = (char *)get_data(temp_node);

  /* recursively call extern_add_to_symb_table */
  token = extern_add_to_symb_table(symb_table, reserved_table, line, cmd);

  list_push(cmd->oplist, temp_node);

  temp_res.title = temp_str;

  /* look in reserved map for a command with the same name as label*/
  if (find_in_reserv_table(reserved_table, &temp_res) == TRUE) {
    printf("line: %d %s* ^ERROR! - lable :\"%s\" is a reserved word *\n\n",
           line_count, line, temp_res.title);
    error_flag = TRUE;
    return FALSE;
  }

  symb = make_symbol();
  symb->type = cmd->op_type;
  strcpy(symb->label, temp_str);

  /* save to symbol_table */
  if (add_to_symbol_table(symb_table, symb) == FALSE) {
    printf("line: %d %s* ^ERROR! - lable :\"%s\" is already taken *\n\n",
           line_count, line, symb->label);
    error_flag = TRUE;
    free_symbol(symb);
    return FALSE;
  }
  return token;
}

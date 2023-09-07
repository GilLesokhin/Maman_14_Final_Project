#include "str_util.h"

#include "extern.h"
/*  skip spaces from index i
    @param line the string to work on
    @param i index to start at
    return the index to the next char that is not a white space */
int skip_spaces(char *line, int i) {
  char c;
  int j = i;
  while ((c = line[j]) == '\t' || c == ' ') j++; /* skip spaces */
  return j;
}

/*  check if line is empty or a comment line
    @param line the string to work on
    return TRUE if line is empty */
bool is_empty(char *line) {
  char c;
  int i = 0;

  i = skip_spaces(line, i);

  /* ignore an empty line or a comment line */
  if ((c = line[i]) == ';' || c == '\n' || c == '\0') {
    return TRUE;
  }

  return FALSE;
}

/*  checks if line has a label if line has a label skip it
    @param line the string to work on
    return first letter after label if there is no label return 0 */
int skip_label(char *line) {
  char c;
  int j = 0;
  /* skip everyting untill you hit : */
  while ((c = line[j++]) != ':') {
    if (c == '\0' || c == '\n') {
      return 0; /* if no label was found return i */
    }
  }
  return j;
}

/*  check if the string is shorter or equall to LINE_LENGTH
    @param line the string to work on
    @return TRUE the string shorter or equall to LINE_LENGTH else FALSE */
bool length_test(char *line) {
  char c;
  int j = 0;
  if (line == NULL) return TRUE;
  /* skip everyting untill you hit '\0' or '\n' and count */
  while ((c = line[j]) != '\0') {
    j++;
  }
  if (j <= LINE_LENGTH) { /* last line */
    return TRUE;
  }
  return FALSE;
}

/* convers an opcode int to a string */
char *int_to_opcode_str(int k) {
  int num = k, i;
  char *str;

  if (k > 15 || k < 0) return NULL;

  if ((str = (char *)malloc((OPCODE_LEGNTH) * sizeof(char))) == NULL) {
    printf("ERROR! - allocating title memory\n");
    error_flag = TRUE;
    return NULL;
  }

  str[OPCODE_LEGNTH] = '\0';

  for (i = OPCODE_LEGNTH - 1; i >= 0; i--, num /= 2) {
    if (num % 2)
      str[(i)] = '1';
    else
      str[(i)] = '0';
  }

  return str;
}

/* convers an reg_num to a string */
char *int_to_reg_num_str(int k) {
  int num = k, i;
  char *str;

  if (k > 7 || k < 0) return NULL;

  if ((str = (char *)malloc((REG_I_LENGTH) * sizeof(char))) == NULL) {
    printf("ERROR! - allocating title memory\n");
    error_flag = TRUE;
    return NULL;
  }

  str[REG_I_LENGTH] = '\0';

  for (i = REG_I_LENGTH - 1; i >= 0; i--, num /= 2) {
    if (num % 2)
      str[(i)] = '1';
    else
      str[(i)] = '0';
  }

  return str;
}

/* convers an int to a type string */
char *int_to_type_str(int k) {
  int num = k, i;
  char *str;

  if (k > 2 || k < 0) return NULL;

  if ((str = (char *)malloc((TYPE_LENGTH) * sizeof(char))) == NULL) {
    printf("ERROR! - allocating title memory\n");
    error_flag = TRUE;
    return NULL;
  }

  str[TYPE_LENGTH] = '\0';

  for (i = TYPE_LENGTH - 1; i >= 0; i--, num /= 2) {
    if (num % 2)
      str[(i)] = '1';
    else
      str[(i)] = '0';
  }

  return str;
}

/* removes a file ending from a string */
void remove_file_end(char *f_name) {
  int i = 0;
  char c;

  while ((c = f_name[i++]) != '\0') {
    if (c == '.') {
      f_name[--i] = '\0';
      return;
    }
  }
}

/* check if file name is legal */
bool check_file_name(char *f_name) {
  int i = 0;
  char c;

  while ((c = f_name[i++]) != '\0') {
    if (c == '.') {
      if (f_name[i++] == 'a' && f_name[i++] == 's' && f_name[i] == '\0')
        return TRUE;
      else
        return FALSE;
    }
  }
  return FALSE;
}

/* calculate an operator bin length for a command

    @param *operator1 first op (null is an option)
    @param *operator2 seconed op (null is an option)
    @return the length of the operation operands in bin code lines */
int get_operator_bin_len(char *operator1, char *operator2) {
  int i = 0, bin_len = 0;
  char c;

  /* check that operands 1 and 2 is a register */
  if (operator2 != NULL && operator1 != NULL) {
    if (operator1[i] == 'r' && operator2[i] == 'r') {
      i++;
      if (operator1[i] >= '0' && operator1[i] <= '7' && operator2[i] >= '0' &&
          operator2[i] <= '7') {
        i++;
        if (operator1[i] == '\0' && operator2[i] == '\0') return 1;
      }
    }
  }

  /* checks for struct type in op 1 "label.2" example */
  if (operator1 != NULL) {
    bin_len++;
    while ((c = operator1[i++]) != '\0') {
      if (c == '.') {
        bin_len++;
        break;
      }
    }
  }
  i = 0;
  /* checks for struct type in op 2  "label.2 example "*/
  if (operator2 != NULL) {
    bin_len++;
    while ((c = operator2[i++]) != '\0') {
      if (c == '.') {
        bin_len++;
        break;
      }
    }
  }

  return bin_len;
}

/* checks if operation is a register */
int is_op_register(char *str) {
  if (str != NULL)
    if (str[0] == 'r' && str[1] >= '0' && str[1] <= '7' && str[2] == '\0')
      return atoi(str + 1);

  return -1;
}

/* gets an offset from a string */
int get_offset(char *str) {
  int i = 0;
  char c;

  while ((c = str[i]) != '\0' && isdigit(c)) i++;

  if (c == '\0') {
    return atoi(str);
  }

  return -1;
}
/* checks if operand is absulute value */
bool is_absolute(char *str) {
  int i = 2;
  char c;

  if (str[0] != '#') {
    return FALSE;
  }

  if (((c = str[1]) == '+' || c == '-' || isdigit(c)) == FALSE) {
    return FALSE;
  }

  while ((c = str[i]) != '\0' && isdigit(c)) i++;

  if (c == '\0') {
    return TRUE;
  }

  return FALSE;
}

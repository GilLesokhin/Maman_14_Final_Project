#include "pre_prossesor.h"

#include "extern.h"

/*  reads all the macros in a given file and saves them to macro map
    while writing the contents of the given file to temp_code
    and replaceing macros with theirs data

    @param *macro_map to save macros is
    @param *reserved_table a reserved table that holds all the reserved words
    @param *sorce_code the File to read from
    @param *temp_code an empty file to write to
    @return TRUE if successful else return FALSE*/
bool pre_prossesor(map *macro_map, map *reserved_table, FILE *sorce_code,
                   FILE *temp_code) {
  char line[LINE_LENGTH + 2];
  macro *old_macro, *temp_macro; /* macro pointers */
  reserv temp_res;
  line_count = 0;

  /* loop over the file untill its over and get next line */
  while (fgets(line, LINE_LENGTH + 2, sorce_code)) {
    line_count++;

    /* check if there is a line longer then LINE_LENGTH */
    if (length_test(line) == FALSE) {
      error_flag = TRUE;
      printf("line %d: %s\n* ^ERROR - line longer then %d characters *\n\n",
             line_count, line, LINE_LENGTH);
      while (length_test(fgets(line, LINE_LENGTH + 2, sorce_code)) == FALSE) {
      };
    }

    /* check if its a comment line to no letters in the line */
    if (is_empty(line)) {
      fputs(line, temp_code); /* write line to temp file */
      continue;
    }

    /* check line for existing macro name */
    old_macro = find_macro(macro_map, line);
    if (old_macro != NULL) {             /* we found a macro */
      fputs(old_macro->data, temp_code); /* write macro data to temp file */
      continue;
    }

    temp_macro = find_new_macro(macro_map, line);
    if (temp_macro == NULL) { /* no new macro def in this line */
      fputs(line, temp_code); /* write line to temp file */
      continue;
    }

    /* if we are here we found a new macro title */

    /* check if macro title is a reserved word */
    temp_res.title = temp_macro->title;
    if (find_in_reserv_table(reserved_table, &temp_res) == TRUE) {
      printf(
          "line %d: %s* ^ERROR! - \"%s\" is a reserved word cannot define a "
          "macro with "
          "this name *\n\n",
          line_count, line, temp_res.title);
      error_flag = TRUE;
      free_macro(temp_macro);
      continue;
    }

    /* get new macro data */
    if (get_macro_data(sorce_code, line, temp_macro) == FALSE) {
      free_macro(temp_macro);
      fputs(line, temp_code);
      continue;
    }

    /* look for old macro with same title */
    old_macro = map_get_macro(macro_map, temp_macro);

    if (old_macro != NULL) { /* this means macro already exsists */
      /* remove old macro */
      if (remove_from_macro_map(macro_map, old_macro) == FALSE) {
        printf("ERROR! - cannot remove old macro from the map\n");
        error_flag = TRUE;
      }
      printf("* warning - macro \"%s\" multiple macro definitions found *\n\n",
             temp_macro->title);
      error_flag = TRUE;
    }

    /* save new macro */
    if (add_to_macro_map(macro_map, temp_macro) == FALSE) {
      printf("ERROR! - cannot save new macro from the map\n");
      error_flag = TRUE;
    }
  }

  return error_flag;
}

/* finds a old macro in a new line

  @param *macro_map the map to look for macro in
  @param *line the line read from
  @return if found a pointer to an old macro with its title data
  else return NULL*/
macro *find_macro(map *macro_map, char *line) {
  int i = 0, j = 0;
  char c;
  macro *temp_macro, *old_macro;

  /* allocating macro memory */
  if ((temp_macro = make_macro()) == NULL) {
    printf("ERROR! - allocating macro memory\n");
    error_flag = TRUE;
    return NULL;
  }

  i = skip_spaces(line, i);

  /* read macro title to temp_macro->title */
  while (isalnum(c = line[i]) && i < TITLE_SIZE) {
    temp_macro->title[j++] = c;
    i++;
  }
  temp_macro->title[j] = '\0'; /* add to temp_macro->title end of string */

  /* look for old macro with same title in macro map */
  old_macro = map_get_macro(macro_map, temp_macro);
  if (old_macro == NULL) {
    free_macro(temp_macro);
    return NULL;
  }

  i = skip_spaces(line, i);

  /* check for letters after macro call */
  c = line[i];
  if (c != '\n' && c != '\0') {
    printf("line %d: %s* ^ERROR - letters after macro call*\n\n", line_count,
           line);
    error_flag = TRUE;
    free_macro(temp_macro);
    return NULL;
  }

  free_macro(temp_macro);
  return old_macro;
}

/* finds a new macro announcement in a new line

  @param *macro_map the map to look for macro in
  @param *line the line read from
  @return if found a pointer to a new macro with its title
  else return NULL*/
macro *find_new_macro(map *macro_map, char *line) {
  int i = 0, j = 0, k;
  char c;
  macro *new_macro;
  char *str = "macro";

  i = skip_spaces(line, i);

  /* compere word to "macro" */
  for (k = 0; k < strlen(str); k++, i++) {
    if (str[k] != line[i]) {
      return NULL;
    }
  }

  i = skip_spaces(line, i);

  /* allocating macro memory */
  if ((new_macro = make_macro()) == NULL) {
    printf("ERROR! - allocating macro memory\n");
    error_flag = TRUE;
    return NULL;
  }

  /* save macro name to macro title */
  while (isalnum(c = line[i]) && i < TITLE_SIZE) {
    new_macro->title[j++] = c;
    i++;
  }
  new_macro->title[j] = '\0'; /* add string end */

  i = skip_spaces(line, i);

  c = line[i];
  /* check we reached end of line */
  if (c != '\n' && c != '\0') {
    printf("line %d: %s* ^ERROR - illegal macro announcement *\n\n", line_count,
           line);
    error_flag = TRUE;
    free_macro(new_macro);
    return NULL;
  }

  return new_macro;
}

/* check a line for end macro announcement

  @param *sorce_code to read from
  @param *line the line read to
  @param *new_macro new macro that was found
  @return TRUE if found end macro and saved all data correctly
  else return FALSE*/
bool get_macro_data(FILE *sorce_code, char *line, macro *new_macro) {
  int data_count = 0, n = 1;

  /* allocate data memory */
  if ((new_macro->data = (char *)malloc(MACRO_DATA_SIZE * sizeof(char))) ==
      NULL) {
    printf("ERROR! - cannot allocate data memory\n");
    error_flag = TRUE;
    free_macro(new_macro);
    new_macro = NULL;
    return FALSE;
  }
  new_macro->data[0] = '\0'; /* make data start end of string */

  while (fgets(line, LINE_LENGTH + 2, sorce_code)) {
    (line_count)++;
    /* check if there is a line longer then LINE_LENGTH */
    if (length_test(line) == FALSE) {
      error_flag = TRUE;
      printf("line %d: %s\n* ^ERROR - line longer then %d characters *\n\n",
             line_count, line, LINE_LENGTH);
      while (length_test(fgets(line, LINE_LENGTH + 2, sorce_code)) == FALSE) {
      };
      (line_count)++;
    }

    /* check if line not end of macro if so continue */
    if (is_macro_end(line) == TRUE) {
      return TRUE; /* break and start saving the macro */
    }

    /* data count is the length of the data we read */
    data_count += strlen(line);

    /* make sure no data leak happens */
    if (data_count > MACRO_DATA_SIZE * n) {
      /* allocate more memory if necessary */
      new_macro->data = (char *)realloc(
          new_macro->data, MACRO_DATA_SIZE * (n *= 2) * sizeof(char));
      if (new_macro->data == NULL) {
        printf("ERROR! - cannot allocate more data memory\n");
        error_flag = TRUE;
        free_macro(new_macro);
        new_macro = NULL;
        return FALSE;
      }
    }
    strncat(new_macro->data, line, strlen(line)); /* write line to macro data */
  }
  /* if we are here we reached eof */
  new_macro = NULL;
  return FALSE;
}

/* check a line for end macro announcement

  @param *line the line to check
  @return TRUE if end macro was found else return FALSE */
bool is_macro_end(char *line) {
  int i = 0, k;
  char c;
  char *str = "endmacro";

  i = skip_spaces(line, i);

  /* compere word to "endmacro" */
  for (k = 0; k < strlen(str); k++, i++) {
    if (str[k] != line[i]) {
      return FALSE;
    }
  }
  i = skip_spaces(line, i);

  c = line[i];
  /* makes sure no letter comes after endmacro */
  if (c == '\n' || c == '\0') return TRUE;
  /* print error if necessary */
  printf("line %d: %s* ^ERROR - illegal endmacro announcement *\n\n",
         line_count, line);

  error_flag = TRUE;
  return TRUE;
}

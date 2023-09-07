#ifndef PRE_P_H
#define PRE_P_H

#include "const.h"
#include "macro_map.h"
#include "reserv_table.h"
#include "str_util.h"

/*  reads all the macros in a given file and saves them to macro map
    while writing the contents of the given file to temp_code
    and replaceing macros with theirs data

    @param *macro_map to save macros is
    @param *reserved_table a reserved table that holds all the reserved words
    @param *sorce_code the File to read from
    @param *temp_code an empty file to write to
    @return TRUE if successful else return FALSE*/
bool pre_prossesor(map *macro_map, map *reserved_table, FILE *sorce_code,
                   FILE *temp_code);

/* finds a old macro in a new line

  @param *macro_map the map to look for macro in
  @param *line the line read from
  @return if found a pointer to an old macro with its title data
  else return NULL*/
macro *find_macro(map *macro_map, char *line);

/* finds a new macro announcement in a new line

  @param *macro_map the map to look for macro in
  @param *line the line read from
  @return if found a pointer to a new macro with its title
  else return NULL*/
macro *find_new_macro(map *macro_map, char *line);

/* check a line for end macro announcement

  @param *sorce_code to read from
  @param *line the line read to
  @param *new_macro new macro that was found
  @return TRUE if found end macro and saved all data correctly
  else return FALSE*/
bool get_macro_data(FILE *sorce_code, char *line, macro *new_macro);

/* check a line for end macro announcement

  @param *line the line to check
  @return TRUE if end macro was found else return FALSE */
bool is_macro_end(char *line);

#endif
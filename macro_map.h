#ifndef MACRO_M_H
#define MACRO_M_H

#include "myhash.h"
#include "str_util.h"

/*  get a macro from a map

    @param map to look in
    @param data a pointer to the data to compere to

    @return a pointer to the data found or NULL if not found */
unsigned macro_hash(void *p);

/*  get a macro from a macro_map

    @param map to look in
    @param macro a pointer to the macro to compere to

    @return a pointer to the macro found or NULL if not found */
macro *map_get_macro(map *map, macro *macro);

/*  adds a macro from a map

    @param map to look in
    @param m1 a pointer to the macro to compere to

    @return TRUE macro was added
    @return FALSE if macro is in the list already or */
bool add_to_macro_map(map *macro_map, macro *m1);

/*  removes a macro from a map

    @param map to look in
    @param m1 a pointer to the macro to compere to

    @return TRUE macro was removed
    @return FALSE if macro was not found in the list */
bool remove_from_macro_map(map *macro_map, macro *m1);

/*  comperes two macros

    @param data1 first macro to compere
    @param data2 seconed macro to compare

    @return TRUE if both have the same title
    @return FALSE if titles are diffrent */
bool macro_cmp(void *data1, void *data2);

/*  creates a new macro using malloc

    @return a pointer to the macro*/
macro *make_macro();

/*  create a new macro map using malloc

    @return a pointer to a new map */
map *make_macro_map();

/* free a macro created with malloc */
void free_macro(void *m);

/* frees a macro map

   @param - map a map to free*/
void free_macro_map(map *macro_map);

/* print a given macro map */
void print_macro_map(map *macro_map);

/* prints a given macro list */
void print_macro_list(node *head);

/* prints a given macro node */
void print_macro_node(node *p);

/*  find data in map

    @param map to add to
    @param data a pointer to the data to compere
    @param cmp a function to compare 2 variables of data's type
    @param hash_func a function to that converts a void *variable to
   unsigned

    @return TRUE data was found
    @return FALSE if data is not in the list already or */
bool find_in_macro_map(map *map, void *data);

#endif
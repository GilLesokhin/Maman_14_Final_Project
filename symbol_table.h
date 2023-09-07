#ifndef symbol_H
#define symbol_H

#include "myhash.h"

/*  get a symbol from a table

    @param table to look in
    @param data a pointer to the data to compere to

    @return a pointer to the data found or NULL if not found */
unsigned symbol_hash(void *p);

/*  get a symbol from a symbol_table

    @param table to look in
    @param symbol a pointer to the symbol to compere to

    @return a pointer to the symbol found or NULL if not found */
symbol *table_get_symbol(map *table, symbol *symbol);

/*  adds a symbol from a table

    @param table to look in
    @param m1 a pointer to the symbol to compere to

    @return TRUE symbol was added
    @return FALSE if symbol is in the list already or */
bool add_to_symbol_table(map *symbol_table, symbol *m1);

/*  removes a symbol from a table

    @param table to look in
    @param m1 a pointer to the symbol to compere to

    @return TRUE symbol was removed
    @return FALSE if symbol was not found in the list */
bool remove_from_symbol_table(map *symbol_table, symbol *m1);

/*  comperes two symbols

    @param data1 first symbol to compere
    @param data2 seconed symbol to compare

    @return TRUE if both have the same title
    @return FALSE if titles are diffrent */
bool symbol_cmp(void *data1, void *data2);

/*  creates a new symbol using malloc

    @return a pointer to the symbol*/
symbol *make_symbol();

/*  create a new symbol table using malloc

    @return a pointer to a new table */
map *make_symbol_table();

/* free a symbol created with malloc */
void free_symbol(void *m);

/* frees a symbol table

   @param - table a table to free*/
void free_symbol_table(map *symbol_table);

/* print a given symbol table */
void print_symbol_table(map *symbol_table);

/* prints a given symbol list */
void print_symbol_list(node *head);

/* prints a given symbol node */
void print_symbol_node(node *p);

/* prints a given symbol list */
void print_symbol_list(node *head);

/*  find data in map

    @param map to add to
    @param data a pointer to the data to compere
    @param cmp a function to compare 2 variables of data's type
    @param hash_func a function to that converts a void *variable to
   unsigned

    @return TRUE data was found
    @return FALSE if data is not in the list already or */
bool find_in_symbol_table(map *map, void *data);

#endif
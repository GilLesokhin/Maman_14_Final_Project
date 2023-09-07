#ifndef RESERV_H
#define RESERV_H

#include "macro_map.h"
#include "str_util.h"

/*   compares two reservs titles

    @param data1 first reserv to compere
    @param data2 seconed reserv to compare

    @return TRUE if both have the same title
    @return FALSE if titles are diffrent */
bool reserv_cmp(void *data1, void *data2);

/*  hashes a reservs

    @param *p reserv to hash

    @return the hash for the reserv*/
unsigned reserv_hash(void *p);

/* print a given reserved table */
void print_reserved_table(map *table);

/* prints a given reserv list */
void print_reserv_list(node *head);

/* prints a given reserv node */
void print_reserv_node(node *p);

/* frees reserv type
   @param p a pointer to the reserv type to free*/
void free_reserv(void *p);

/* frees a reserv table

   @param map a map to free*/
void free_reserv_table(map *map);

/*  find data in table

    @param table to add to
    @param data a pointer to the data to compere
    @param cmp a function to compare 2 variables of data's type
    @param hash_func a function to that converts a void *variable to
   unsigned

    @return TRUE data was found
    @return FALSE if data is not in the list already or */
bool find_in_reserv_table(map *table, void *data);

/* allocate memory for a reserv and return its adress

    @return the adress for the reserv */
reserv *make_reserved();

/* allocate memory for a reserv and return its adress
    for a reserved word

    @return the adress for the reserv */
reserv *make_reserved_word();

/* allocate memory for a reserv table and return its adress

    @return the adress for the reserv */
map *make_reserved_table();

/*  adds a reserv from a table

    @param table to look in
    @param m1 a pointer to the reserv to compere to

    @return TRUE reserv was added
    @return FALSE if reserv is in the list already or */
bool add_to_reserve_table(map *reserv_table, void *m1);

/*  get a reserv from a table

    @param table to look in
    @param m1 a pointer to the reserv to compere to

    @return a pointer to the reserv found or NULL if not found */
reserv *table_get_reserved(map *reserv_table, void *m1);

#endif
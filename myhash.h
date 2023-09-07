#ifndef MY_HASH_H
#define MY_HASH_H

#include "const.h"
#include "mylist.h"

/*  create a new map using malloc

    @return a pointer to a new map */
map *make_map();

/* frees a map
   @param - free_data is a function to free the date
   @param - map a map to free*/
void free_map(map *map, void free_data(void *));

/*  adding a data to the map while checking there are nodes in the map that hold
    the same data

    @param map to add to
    @param data a pointer to the data to save
    @param cmp a function to compare 2 variables of data's type
    @param hash_func a function to that converts a void *variable to unsigned

    @return TRUE data was added
    @return FALSE if data is in the list already or */
bool add_to_map(map *map, void *data, bool cmp(void *, void *),
                unsigned hash_func(void *));

/*  removeing data from a map

    @param map to remove from
    @param data a pointer to the data to remove
    @param cmp a function to compare 2 variables of data's type
    @param hash_func a function to that converts a void *variable to unsigned

    @return TRUE data was removed
    @return FALSE if data was not found in the list */
bool remove_from_map(map *map, void *data, bool cmp(void *, void *),
                     void free_data(void *), unsigned hash_func(void *));

/*  get a data from a map

    @param map to look in
    @param data a pointer to the data to compere to
    @param cmp a function to compare 2 variables of data's type
    @param hash_func a function to that converts a void *variable to unsigned

    @return a pointer to the data found or NULL if not found */
void *map_get_data(map *map, void *data, bool cmp(void *, void *),
                   unsigned hash_func(void *));

/*  find data in map

@param map to add to
@param data a pointer to the data to compere
@param cmp a function to compare 2 variables of data's type
@param hash_func a function to that converts a void *variable to unsigned

@return TRUE data was found
@return FALSE if data is not in the list already or */
bool find_in_map(map *map, void *data, bool cmp(void *, void *),
                 unsigned hash_func(void *));

#endif
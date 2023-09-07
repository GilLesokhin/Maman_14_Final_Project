#include "myhash.h"

/*  create a new map using malloc

    @return a pointer to a new map */
map *make_map() {
  unsigned i;
  map *new_map;
  /* allocating space in memory for map */
  new_map = malloc(sizeof(map));
  if (new_map == NULL) /* making sure memory was allocated */
  {
    printf("ERROR! - There was a problem allocating map memory\n");
    return NULL;
  }
  /* fill hash map with lists */
  for (i = 0; i < HASH_SIZE; i++) {
    new_map->lists[i] = make_list();
    if (new_map->lists[i] == NULL) /* making sure memory was allocated */
    {
      printf("ERROR! - There was a problem allocating list memory\n");
      return NULL;
    }
  }
  return (new_map);
}

/*  adding a data to the map while checking there are nodes in the map that hold
    the same data

    @param map to add to
    @param data a pointer to the data to save
    @param cmp a function to compare 2 variables of data's type
    @param hash_func a function to that converts a void *variable to unsigned

    @return TRUE data was added
    @return FALSE if data is in the list already or */
bool add_to_map(map *map, void *data, bool cmp(void *, void *),
                unsigned hash_func(void *)) {
  unsigned adr;
  if (map == NULL) /* check that map exists */
  {
    printf("ERROR! - Map dose not exist\n");
    return FALSE;
  }

  /* if data is in the map */
  if (find_in_map(map, data, cmp, hash_func) == TRUE) return FALSE;

  /* hash  data to be used as list map adress for the right list */
  adr = hash_func(data);
  adr = adr % HASH_SIZE;
  return add_to_head(map->lists[adr], data);
}

/*  removeing data from a map

    @param map to remove from
    @param data a pointer to the data to remove
    @param cmp a function to compare 2 variables of data's type
    @param hash_func a function to that converts a void *variable to unsigned
    @param free_data a function to free data

    @return TRUE data was removed
    @return FALSE if data was not found in the list */
bool remove_from_map(map *map, void *data, bool cmp(void *, void *),
                     void free_data(void *), unsigned hash_func(void *)) {
  unsigned adr;

  if (map == NULL) /* check that map exists */
  {
    printf("ERROR! - Map dose not exist\n");
    return FALSE;
  }
  /* hash  data to be used as list map adress for the right list */
  adr = hash_func(data);
  adr = adr % HASH_SIZE;

  return remove_node(map->lists[adr], data, cmp, free_data);
}

/*  find data in map

    @param map to add to
    @param data a pointer to the data to compere
    @param cmp a function to compare 2 variables of data's type
    @param hash_func a function to that converts a void *variable to unsigned

    @return TRUE data was found
    @return FALSE if data is not in the list already or */
bool find_in_map(map *map, void *data, bool cmp(void *, void *),
                 unsigned hash_func(void *)) {
  unsigned adr;

  if (map == NULL) /* check that map exists */
  {
    printf("ERROR! - Map dose not exist\n");
    return FALSE;
  }
  /* hash  data to be used as list map adress for the right list */
  adr = hash_func(data);
  adr = adr % HASH_SIZE;

  if (find_node(map->lists[adr], data, cmp) == NULL)
    return FALSE;
  else
    return TRUE;
}

/*  get a data from a map

    @param map to look in
    @param data a pointer to the data to compere to
    @param cmp a function to compare 2 variables of data's type
    @param hash_func a function to that converts a void *variable to unsigned

    @return a pointer to the data found or NULL if not found */
void *map_get_data(map *map, void *data, bool cmp(void *, void *),
                   unsigned hash_func(void *)) {
  unsigned adr;
  if (map == NULL) {
    printf("ERROR! - Map dose not exist\n");
    return NULL;
  }
  /* hash  data to be used as list map adress for the right list */
  adr = hash_func(data);
  adr = adr % HASH_SIZE;

  return list_get_data(map->lists[adr], data, cmp);
}

/* frees a map
   @param - free_data is a function to free the date
   @param - map a map to free*/
void free_map(map *map, void free_data(void *)) {
  int i;
  if (map == NULL) /* check that map exists */
  {
    printf("ERROR! - Map dose not exist\n");
  }
  /* free every list's memory */
  for (i = 0; i < HASH_SIZE; i++) {
    free_list(map->lists[i], free_data);
  }
  free(map); /* free map */
}

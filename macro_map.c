#include "macro_map.h"

/*  removes a macro from a map

    @param map to look in
    @param m1 a pointer to the macro to compere to

    @return TRUE macro was removed
    @return FALSE if macro was not found in the list */
bool remove_from_macro_map(map *macro_map, macro *m1) {
  if (m1->title == NULL) return FALSE;
  return remove_from_map(macro_map, m1, &macro_cmp, &free_macro, &macro_hash);
}

/*  adds a macro from a map

    @param map to look in
    @param m1 a pointer to the macro to compere to

    @return TRUE macro was added
    @return FALSE if macro is in the list already or */
bool add_to_macro_map(map *macro_map, macro *m1) {
  if (m1->title == NULL) return FALSE;
  return add_to_map(macro_map, m1, &macro_cmp, &macro_hash);
}

/*  get a macro from a map

    @param map to look in
    @param m1 a pointer to the macro to compere to

    @return a pointer to the macro found or NULL if not found */
macro *map_get_macro(map *macro_map, macro *m1) {
  if (m1->title == NULL) return NULL;
  return map_get_data(macro_map, m1, &macro_cmp, &macro_hash);
}

/*  comperes two macros

    @param data1 first macro to compere
    @param data2 seconed macro to compare

    @return TRUE if both have the same title
    @return FALSE if titles are diffrent */
bool macro_cmp(void *data1, void *data2) {
  macro *m1, *m2;
  m1 = (macro *)data1;
  m2 = (macro *)data2;
  if (strcmp(m1->title, m2->title) == 0) /* compares the two strings */
    return TRUE;
  return FALSE;
}

/*  creates a new macro using malloc

    @return a pointer to the macro*/
macro *make_macro() {
  macro *m1;
  /* allocating memory */
  if ((m1 = (macro *)malloc(sizeof(macro))) == NULL) {
    printf("ERROR! - allocating macro memory\n");
    return NULL;
  }
  /* allocating memory */
  if ((m1->title = (char *)malloc(TITLE_SIZE * sizeof(char))) == NULL) {
    printf("ERROR! - allocating title memory\n");
    return FALSE;
  }
  m1->data = NULL;
  return m1;
}

/* free a macro created with malloc */
void free_macro(void *m1) {
  macro *m;

  if (m1 == NULL) return;
  m = (macro *)m1;

  if (m->data != NULL) {
    free(m->data);
    m->data = NULL;
  }
  if (m->title != NULL) {
    free(m->title);
    m->title = NULL;
  }
  free(m);
}

/*  hash a macro

    @param p a pointer to the macro to hash

    @return unsigned number whtch is the hash value */
unsigned macro_hash(void *p) {
  unsigned hashval = 0;
  int i;
  macro *m1;
  m1 = (macro *)p;
  /* hash func */
  for (i = 0; *(m1->title + i) != '\0'; i++) {
    hashval = *(m1->title + i) + HASH_MULL * hashval;
  }
  return (hashval % HASH_SIZE);
}

/*  create a new macro map using malloc

    @return a pointer to a new map */
map *make_macro_map() { return make_map(); }

/* frees a macro map

   @param - map a map to free*/
void free_macro_map(map *map) { free_map(map, &free_macro); }

/* print a given macro map */
void print_macro_map(map *map) {
  unsigned i;
  if (map == NULL) {
    printf("ERROR! - Map dose not exist\n");
    return;
  }
  printf("\n|||||||Printing macro map:|||||||\n\n");
  for (i = 0; i < HASH_SIZE; i++) {
    printf("-------- List number: %02d --------\n\n", i);
    print_macro_list(map->lists[i]);
    printf("---------------------------------\n\n");
    printf("\n");
  }
  printf("\n");
}

/* prints a given macro list */
void print_macro_list(node *head) {
  node *p = head;
  unsigned i = 1;
  if (head == NULL) {
    printf("ERROR! - List dose not exist\n");
    return;
  }
  /* find the end of the list */
  while (p->next_node != NULL) {
    p = p->next_node;
    printf("Node number: %d\n", i++);
    print_macro_node(p);
  }
}

/* prints a given macro node */
void print_macro_node(node *p) {
  macro *m1;
  m1 = p->data;
  printf("title:\t\"%s\"\ndata:\n%s\n", m1->title, m1->data);
}

/*  find data in map

    @param map to add to
    @param data a pointer to the data to compere
    @param cmp a function to compare 2 variables of data's type
    @param hash_func a function to that converts a void *variable to unsigned

    @return TRUE data was found
    @return FALSE if data is not in the list already or */
bool find_in_macro_map(map *map, void *data) {
  return find_in_map(map, data, &macro_cmp, &macro_hash);
}

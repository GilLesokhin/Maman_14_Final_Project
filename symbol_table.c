#include "symbol_table.h"

map *make_table() { return make_map(); }

/*  removes a symbol from a table

    @param table to look in
    @param m1 a pointer to the symbol to compere to

    @return TRUE symbol was removed
    @return FALSE if symbol was not found in the list */
bool remove_from_symbol_table(map *symbol_table, symbol *m1) {
  if (m1->label == NULL) return FALSE;
  return remove_from_map(symbol_table, m1, &symbol_cmp, &free_symbol,
                         &symbol_hash);
}

/*  adds a symbol from a table

    @param table to look in
    @param m1 a pointer to the symbol to compere to

    @return TRUE symbol was added
    @return FALSE if symbol is in the list already or */
bool add_to_symbol_table(map *symbol_table, symbol *m1) {
  if (m1->label == NULL) return FALSE;
  return add_to_map(symbol_table, m1, &symbol_cmp, &symbol_hash);
}

/*  get a symbol from a table

    @param table to look in
    @param m1 a pointer to the symbol to compere to

    @return a pointer to the symbol found or NULL if not found */
symbol *table_get_symbol(map *symbol_table, symbol *m1) {
  if (m1->label == NULL) return NULL;
  return map_get_data(symbol_table, m1, &symbol_cmp, &symbol_hash);
}

/*  comperes two symbols

    @param data1 first symbol to compere
    @param data2 seconed symbol to compare

    @return TRUE if both have the same label
    @return FALSE if labels are diffrent */
bool symbol_cmp(void *data1, void *data2) {
  symbol *m1, *m2;
  m1 = (symbol *)data1;
  m2 = (symbol *)data2;
  if (strcmp(m1->label, m2->label) == 0) /* compares the two strings */
    return TRUE;
  return FALSE;
}

/*  creates a new symbol using malloc

@return a pointer to the symbol*/
symbol *make_symbol() {
  symbol *m1;
  /* allocating memory */
  if ((m1 = (symbol *)malloc(sizeof(symbol))) == NULL) {
    printf("ERROR! - allocating symbol memory\n");
    return NULL;
  }

  if ((m1->label = (char *)malloc(LABLE_LENGTH * sizeof(char))) == NULL) {
    printf("ERROR! - allocating label memory\n");
    return NULL;
  }

  m1->type = unknown;
  m1->DC = 0;
  m1->IC = 0;
  m1->length = 0;
  m1->ent_used_flag = FALSE;

  return m1;
}

/* free a symbol created with malloc */
void free_symbol(void *sym) {
  symbol *m;

  if (sym == NULL) return;

  m = sym;

  if (m->label != NULL) free(m->label);
  free(m);
}

/*  hash a symbol

    @param p a pointer to the symbol to hash

    @return unsigned number whtch is the hash value */
unsigned symbol_hash(void *p) {
  unsigned hashval = 0;
  int i;
  symbol *m1;

  m1 = (symbol *)p;
  /* hash func */
  for (i = 0; *((m1->label) + i) != '\0'; i++) {
    hashval = *((m1->label) + i) + HASH_MULL * hashval;
  }
  return (hashval % HASH_SIZE);
}

/*  create a new symbol table using malloc

    @return a pointer to a new table */
map *make_symbol_table() { return make_map(); }

/* frees a symbol table

   @param - table a table to free*/
void free_symbol_table(map *table) { free_map(table, &free_symbol); }

/* print a given symbol table */
void print_symbol_table(map *table) {
  unsigned i;
  if (table == NULL) {
    printf("ERROR! - table dose not exist\n");
    return;
  }
  printf("\n|||||Printing Symbol table:||||||\n\n");
  for (i = 0; i < HASH_SIZE; i++) {
    printf("-------- List number: %02d --------\n\n", i);
    print_symbol_list(table->lists[i]);
    printf("---------------------------------\n\n");
    printf("\n");
  }
  printf("\n");
}
/* prints a given symbol list */
void print_symbol_list(node *head) {
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
    print_symbol_node(p);
  }
}
/* prints a given symbol node */
void print_symbol_node(node *p) {
  const char *str[7];
  symbol *m1;
  int i;

  str[0] = "unknown";
  str[1] = "command";
  str[2] = ".string";
  str[3] = ".data";
  str[4] = ".struct";
  str[5] = ".extern";
  str[6] = ".entry";

  m1 = get_data(p);
  i = m1->type;
  if (i >= 6) i = 1;

  if (i > 9 || i < 0) {
    i = 0;
  }

  printf("label:\t\t%s\ntype:\t\t%s\nIC: %d\t\tDC: %d\n\n", m1->label, str[i],
         m1->IC, m1->DC);
}

/*  find data in map

    @param map to add to
    @param data a pointer to the data to compere
    @param cmp a function to compare 2 variables of data's type
    @param hash_func a function to that converts a void *variable to unsigned

    @return TRUE data was found
    @return FALSE if data is not in the list already or */
bool find_in_symbol_table(map *map, void *data) {
  return find_in_map(map, data, &symbol_cmp, &symbol_hash);
}

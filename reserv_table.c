
#include "reserv_table.h"

/*   compares two reservs titles

    @param data1 first reserv to compere
    @param data2 seconed reserv to compare

    @return TRUE if both have the same title
    @return FALSE if titles are diffrent */
bool reserv_cmp(void *data1, void *data2) {
  reserv *r1;
  reserv *r2;
  r1 = (reserv *)data1;
  r2 = (reserv *)data2;

  if (strcmp(r1->title, r2->title) == 0) /* compares the two strings */
    return TRUE;
  return FALSE;
}

/*  hashes a reservs

    @param *p reserv to hash

    @return the hash for the reserv*/
unsigned reserv_hash(void *p) {
  unsigned hashval = 0;
  int i;
  reserv *r1;
  r1 = (reserv *)p;
  /* hash func */
  for (i = 0; *(r1->title + i) != '\0'; i++) {
    hashval = *(r1->title + i) + HASH_MULL * hashval;
  }
  return (hashval % HASH_SIZE);
}

/* print a given reserved table */
void print_reserved_table(map *table) {
  unsigned i;
  if (table == NULL) {
    printf("ERROR! - table dose not exist\n");
    return;
  }
  printf("\n||||Printing reserved_table:|||||\n");
  for (i = 0; i < HASH_SIZE; i++) {
    printf("-------- List number: %02d --------\n\n", i);
    print_reserv_list(table->lists[i]);
    printf("---------------------------------\n\n");
    printf("\n");
  }
  printf("\n");
}

/* prints a given reserv list */
void print_reserv_list(node *head) {
  node *p = head;
  if (head == NULL) {
    printf("ERROR! - List dose not exist\n");
    return;
  }
  /* find the end of the list */
  while (p->next_node != NULL) {
    p = p->next_node;
    print_reserv_node(p);
  }
}

/* prints a given reserv node */
void print_reserv_node(node *p) {
  reserv *r1;
  r1 = (reserv *)p->data;
  printf("title: \"%s\"\nopcode:\t%d\top_type: %d\n\n", r1->title, r1->opcode,
         r1->op_type);
}

/* frees reserv type
   @param p a pointer to the reserv type to free*/
void free_reserv(void *p) {
  reserv *r1;

  if (p == NULL) return;
  r1 = p;

  if (r1->title != NULL) free(r1->title);

  free(r1);
}

/* frees a reserv table

   @param map a map to free*/
void free_reserv_table(map *map) { free_map(map, &free_reserv); }

/*  find data in table

    @param table to add to
    @param data a pointer to the data to compere
    @param cmp a function to compare 2 variables of data's type
    @param hash_func a function to that converts a void *variable to
   unsigned

    @return TRUE data was found
    @return FALSE if data is not in the list already or */
bool find_in_reserv_table(map *table, void *data) {
  return find_in_map(table, data, &reserv_cmp, &reserv_hash);
}

/* allocate memory for a reserv and return its adress

    @return the adress for the reserv */
reserv *make_reserved() {
  reserv *r1;
  /* allocating memory */
  if ((r1 = (reserv *)malloc(sizeof(reserv))) == NULL) {
    printf("ERROR! - allocating reserv memory\n");
    return NULL;
  }
  r1->title = NULL;
  r1->opcode = NULL_OPCODE;
  return r1;
}

/* allocate memory for a reserv and return its adress
    for a reserved word

    @return the adress for the reserv */
reserv *make_reserved_word() {
  reserv *r1;
  /* allocating memory */
  if ((r1 = (reserv *)malloc(sizeof(reserv))) == NULL) {
    printf("ERROR! - allocating reserv memory\n");
    return NULL;
  }
  if ((r1->title = (char *)malloc(OP_LEGNTH * sizeof(char))) == NULL) {
    printf("ERROR! - allocating title memory\n");
    return FALSE;
  }
  r1->opcode = NULL_OPCODE;

  return r1;
}

/* allocate memory for a reserv table and return its adress

    @return the adress for the reserv */
map *make_reserved_table() {
  map *reserv_table;
  reserv *reserv_word;
  int i;

  /* reserved words */
  const char *command_ary[] = {
      "mov",  "cmp",    "add",    "sub",     "not",     "clr",     "lea",
      "inc",  "dec",    "jmp",    "bne",     "get",     "prn",     "jsr",
      "rts",  "hlt",    ".data",  ".struct", ".string", ".extern", ".entry",
      "data", "struct", "string", "extern",  "entry"};

  const char *reg_ary[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};

  if ((reserv_table = make_map()) == NULL) return NULL;

  for (i = 0; i < COM_ARY_SIZE; i++) {
    if ((reserv_word = make_reserved_word()) == NULL) {
      return NULL;
    }

    strcpy(reserv_word->title, command_ary[i]);
    if (i < 16) {
      reserv_word->opcode = i;
    } else
      reserv_word->opcode = NULL_OPCODE;

    /* operator type 1 */
    /* mov/cmp/add/sup/lea */
    if ((i >= 0 && i <= 3) || i == 6) reserv_word->op_type = command_type_1;

    /* operator type 2 */
    /* not/clr/inc/dec/jmp/bne/get/prn/jsr */
    else if (i <= 13)
      reserv_word->op_type = command_type_2;

    /* operator type 3 */
    /* rts/hlt */
    else if (i == 14 || i == 15)
      reserv_word->op_type = command_type_3;

    if (i == 16) /* data */
      reserv_word->op_type = data_type;
    if (i == 17) /* struct */
      reserv_word->op_type = struct_type;
    if (i == 18) /* string */
      reserv_word->op_type = string_type;
    if (i == 19) /* extern */
      reserv_word->op_type = extern_type;
    if (i == 20) /* entry */
      reserv_word->op_type = entry_type;

    if (i > 20) /* entry */
      reserv_word->op_type = unknown;

    /*     reserv_word->reg_num = __INT_MAX__; */
    add_to_reserve_table(reserv_table, reserv_word);
  }

  for (i = 0; i < REG_ARY_SIZE; i++) {
    if ((reserv_word = make_reserved_word()) == NULL) {
      return NULL;
    }

    strcpy(reserv_word->title, reg_ary[i]);
    reserv_word->opcode = __INT_MAX__;
    /*     reserv_word->reg_num = i; */
    reserv_word->op_type = not_operation;

    add_to_reserve_table(reserv_table, reserv_word);
  }

  return reserv_table;
}

/*  adds a reserv from a table

    @param table to look in
    @param m1 a pointer to the reserv to compere to

    @return TRUE reserv was added
    @return FALSE if reserv is in the list already or */
bool add_to_reserve_table(map *reserv_table, void *m1) {
  reserv *m2;
  m2 = (reserv *)m1;
  if (m2->title == NULL) return FALSE;
  return add_to_map(reserv_table, m2, &reserv_cmp, &reserv_hash);
}

/*  get a reserv from a table

    @param table to look in
    @param m1 a pointer to the reserv to compere to

    @return a pointer to the reserv found or NULL if not found */
reserv *table_get_reserved(map *reserv_table, void *m1) {
  reserv *m2;
  m2 = (reserv *)m1;
  if (m2->title == NULL) return NULL;
  return map_get_data(reserv_table, m1, &reserv_cmp, &reserv_hash);
}

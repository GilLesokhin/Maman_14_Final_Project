#include "mylist.h"

/*  make a new list head
    @return pointer a new list head */
node *make_list() { return make_node(NULL); }

/*  creates a new node with apointer to data
    @param data the data to point to
    @return a pointer a new node */
node *make_node(void *data) {
  /* allocating space in memory for list variables */
  node *new_node = malloc(sizeof(node));
  if (new_node == NULL) {
    printf("There was a problem allocating list memory\n");
    return NULL;
  }
  set_node(new_node, data);
  new_node->next_node = NULL;
  new_node->prev_node = NULL;
  return new_node;
}

/*  set a node's data to the data recived
    @param n a node to set
    @param data the data to point to
    @return TRUE if secssesful */
bool set_node(node *n, void *data) {
  if (n == NULL) {
    printf("ERROR! - node dose not exist\n");
    return FALSE;
  }
  n->data = data;
  return TRUE;
}

/*  gets data from a node
    @param n a node to get data from
    @return a void pointer to list data */
void *get_data(node *n) {
  if (n == NULL) {
    printf("ERROR! - List dose not exist\n");
    return NULL;
  }
  return (n->data);
}

/*  removes the node at the head of the list and returns it
    @param head list's head
    @return the node at the head of the list */
node *list_pop(node *head) {
  node *p;
  if (head == NULL) {
    printf("ERROR! - List dose not exist\n");
    return NULL;
  }

  if (head->next_node == NULL) {
    return NULL;
  }

  p = head->next_node;
  head->next_node = p->next_node;

  if (p->next_node != NULL) {
    p->next_node->prev_node = head;
  }

  p->next_node = NULL;
  p->prev_node = NULL;

  return p;
}

/*  adds a node to the head of the list
    @param head list's head
    @return TRUE if secssesful */
bool list_push(node *head, node *p) {
  if (head == NULL) {
    printf("ERROR! - List dose not exist\n");
    return FALSE;
  }
  p->next_node = head->next_node;
  head->next_node = p;
  p->prev_node = head;

  if (p->next_node != NULL) {
    p->next_node->prev_node = p;
  }

  return TRUE;
}

/*  adds a new node to the head of the list with set data
    @param head list's head
    @param data to set in the node
    @return TRUE if secssesful */
bool add_to_head(node *head, void *data) {
  node *new_node;
  /* make a new node with given title */
  new_node = make_node(data);
  /* addind new node to the start of the list */
  return list_push(head, new_node);
}

/*  adds a new node to the tail of the list with set data
    @param head list's head
    @param data to set in the node
    @return TRUE if secssesful */
bool add_to_tail(node *head, void *data) {
  node *p, *new_node;
  if (head == NULL) {
    printf("ERROR! - List dose not exist\n");
    return FALSE;
  }
  p = head;

  /* find the end of the list */
  while (p->next_node != NULL) {
    p = p->next_node;
  }

  /* make a new node with given title and adding it to the end of the list */
  new_node = make_node(data);
  /*     new_node->prev_node = p;
      p->next_node = new_node;
      return TRUE; */
  return list_push(p, new_node);
}

/*  gets a node from a list with the same data
    @param head list's head
    @param data the data to compare to
    @param cmp a function to compare data with
    @return node with same data if one was found else return NULL */
void *list_get_data(node *head, void *data, bool cmp(void *, void *)) {
  node *p;
  p = head;
  if ((p = find_node(head, data, cmp)) != NULL) return p->data;

  return NULL;
}

/*  finds a node's data from a list with the same data
    @param head list's head
    @param data the data to compare to
    @param cmp a function to compare data with
    @return node's data with same data if one was found else return NULL */
node *find_node(node *head, void *data, bool cmp(void *, void *)) {
  node *p = head;
  if (head == NULL) {
    printf("ERROR! - List dose not exist\n");
    return NULL;
  }

  /* loop to go over all the list */
  while (p->next_node != NULL) {
    p = p->next_node;

    /* compare data using cmp */
    if (cmp(p->data, data) == TRUE) return p;
  }
  return NULL;
}

/*  removes a node from the list and frees the node removed and its data
    @param head the node to free
    @param data the data to compare to
    @param cmp a function to compare data with
    @param free_data a function to free data
    @return TRUE if secssesful */
bool remove_node(node *head, void *data, bool cmp(void *, void *),
                 void free_data(void *)) {
  node *n;
  if ((n = find_node(head, data, cmp)) == NULL) {
    return FALSE;
  }

  /* disconnect node from the list */
  n->prev_node->next_node = n->next_node;
  if (n->next_node != NULL) {
    n->next_node->prev_node = n->prev_node;
  }
  free_node(n, free_data);
  return TRUE;
}

/*  frees a list
    @param head list's head
    @param free_data a function to free list's data */
void free_list(node *head, void free_data(void *)) {
  node *next, *p;
  if (head == NULL) {
    printf("ERROR! - List dose not exist\n");
    return;
  }

  if (head->prev_node != NULL) {
    printf("ERROR! - This is not the head of the list\n");
    return;
  }

  p = head;
  /* loop to go over all nodes */
  while (p != NULL) {
    next = p->next_node;

    /* free node title */
    if (p->data != NULL) free_data(p->data);
    free(p);
    p = next;
  }
}

/*  frees a node and its data
    @param n the node to free
    @param free_data a function to free node's data */
void free_node(node *n, void free_data(void *)) {
  if (n != NULL && n->data != NULL) {
    free_data(n->data);
  }
  free(n);
}

/*  gets list's length
    @param head the head of the list
    @return the length of the list */
int list_len(node *head) {
  int i = 0;
  node *p = head;
  if (head == NULL) {
    printf("ERROR! - List dose not exist\n");
    return 0;
  }

  /* loop to go over all nodes */
  while (p->next_node != NULL) {
    p = p->next_node;
    i++;
  }

  return i;
}
#ifndef MY_LIST_H
#define MY_LIST_H

#include "const.h"

/*  make a new list head
    @return pointer a new list head */
node *make_list();

/*  creates a new node with apointer to data
    @param data the data to point to
    @return a pointer a new node */
node *make_node(void *data);

/*  set a node's data to the data recived
    @param n a node to set
    @param data the data to point to
    @return TRUE if secssesful*/
bool set_node(node *n, void *data);

/*  gets data from a node
    @param n a node to get data from
    @return a void pointer to list data */
void *get_data(node *n);

/*  removes the node at the head of the list and returns it
    @param head list's head
    @return the node at the head of the list */
node *list_pop(node *head);

/*  adds a node to the head of the list
    @param head list's head
    @return TRUE if secssesful */
bool list_push(node *head, node *p);

/*  adds a new node to the head of the list with set data
    @param head list's head
    @param data to set in the node
    @return TRUE if secssesful */
bool add_to_head(node *head, void *data);

/*  adds a new node to the tail of the list with set data
    @param head list's head
    @param data to set in the node
    @return TRUE if secssesful */
bool add_to_tail(node *head, void *data);

/*  removes a node from the list and frees the node removed and its data
    @param head the node to free
    @param data the data to compare to
    @param cmp a function to compare data with
    @param free_data a function to free data
    @return TRUE if secssesful */
bool remove_node(node *head, void *data, bool cmp(void *, void *),
                 void free_data(void *));

/*  gets a node's data from a list with the same data
    @param head list's head
    @param data the data to compare to
    @param cmp a function to compare data with
    @return node's data with same data if one was found else return NULL */
void *list_get_data(node *head, void *data, bool cmp(void *, void *));

/*  finds a node from a list with the same data
    @param head list's head
    @param data the data to compare to
    @param cmp a function to compare data with
    @return node with same data if one was found else return NULL */
node *find_node(node *head, void *data, bool cmp(void *, void *));

/*  frees a list
    @param head list's head
    @param free_data a function to free list's data */
void free_list(node *head, void free_data(void *));

/*  frees a node and its data
    @param n the node to free
    @param free_data a function to free node's data */
void free_node(node *n, void free_data(void *));

/*  gets list's length
    @param head the head of the list
    @return the length of the list */
int list_len(node *head);

#endif
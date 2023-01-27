#ifndef _LINKEDLIST_H_
#define _LINKEDLIST_H_

#define LIST_MAXLEN_TEXT 100

typedef struct node_t {
    char *value;
    char text[LIST_MAXLEN_TEXT];
    struct node_t  *next;
} node_t;

node_t *new_node(char *);
node_t *add_front(node_t *, node_t *);
node_t *add_end(node_t *, node_t *);
// this is the new created method
node_t *add_alpha_order(node_t *, node_t *);
node_t *peek_front(node_t *);
node_t *remove_front(node_t *);
void    apply(node_t *, void(*fn)(node_t *, void *), void *arg);

#endif

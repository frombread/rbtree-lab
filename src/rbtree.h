#ifndef _RBTREE_H_
#define _RBTREE_H_

#include <stddef.h>

typedef enum { RBTREE_RED, RBTREE_BLACK } color_t;

typedef int key_t;

typedef struct node_t {
  color_t color;
  key_t key;
  struct node_t *parent, *left, *right;
} node_t;

typedef struct {
  node_t *root;
  node_t *nil;  // for sentinel
} rbtree;

rbtree *new_rbtree(void);
void delete_rbtree(rbtree *);

node_t *rbtree_insert(rbtree *, const key_t);
node_t *rbtree_find(const rbtree *, const key_t);
node_t *rbtree_min(const rbtree *);
node_t *rbtree_max(const rbtree *);
int rbtree_erase(rbtree *, node_t *);

int rbtree_to_array(const rbtree *, key_t *, const size_t);


void delete_node_lrv(rbtree *t, node_t*node); // 트리 맨밑까지 가서 삭제 하기
void rbtree_insert_fixup(rbtree *t, node_t*node); // 삽입한 다음에 색맞추기,등등
void left_rotate(rbtree *t, node_t*node);
void right_rotate(rbtree *t, node_t*node);
void exchange_color(node_t *a , node_t *b);
node_t *next_node (const rbtree *t , node_t *p);
void rbtree_erase_fixup(rbtree *t, node_t *parent,int is_left);
#endif  // _RBTREE_H_

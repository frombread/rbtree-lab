#include "rbtree.h"

#include <stdlib.h>

// 트리만들기 시작
rbtree *new_rbtree(void) {
  
  rbtree *t = (rbtree *)calloc(1, sizeof(rbtree)); // tree 구조체 동적 할당
  node_t *nil =(rbtree *)calloc(1,sizeof(node_t)); // nil 노드 생성 및 초기화

  nil ->color =RBTREE_BLACK; //nil노드는 항상 검은색이다.
  
  // tree의 nil과 root를 nil 노드로 설정 (tree가 빈 경우 root는 nil노드여야 한다.)
  t->nil = t->root = nil;
  return t;
}

void delete_rbtree(rbtree *t) {


  free(t);
}

node_t *rbtree_insert(rbtree *t, const key_t key) {

  node_t *new_node = (node_t *)calloc(1,sizeof(node_t));
  new_node->key =key;
  new_node->color = RBTREE_RED;
  new_node->left = new_node->right = t->nil;
  node_t *current = t->root;
  while (current!=t->nil)
  {
    // t->nil = current;
    if(key < current->key)
    {
      
    }
  }
  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}

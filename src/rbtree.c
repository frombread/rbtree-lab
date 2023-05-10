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
  node_t *node = t->root;
  if (node != t->root){
    delete_node_lrv(t,node);
  }
  free(t->nil);
  free(t);
}

void delete_node_lrv(rbtree *t, node_t *node)
{
  if(node->left != t->nil)
  {
    delete_node_lrv(t, node->left);
  }
  if(node->right != t->nil){
    delete_node_lrv(t,node->right);
  }
  free(node);
}

node_t *rbtree_insert(rbtree *t, const key_t key) {

  node_t *new_node = (node_t *)calloc(1,sizeof(node_t));
  new_node->key =key;
  new_node->color = RBTREE_RED;
  new_node->left = new_node->right = t->nil; //추가한 노드의 자식들을 nil노드로 설정
  
  //새 노드를 삽입할 위치탐색
  node_t *current = t->root;
  while (current!=t->nil)
  {
    // t->nil = current;
    if(key < current->key)
    {
      if(current ->left ==t->nil)
      {
        current -> left =new_node;
        break;
      }
      current = current->left;
    }
    else{
      if (current->right ==t->nil)
      {
        current ->right = new_node;
        break;
      }
      current = current->right;
    }
  }
  new_node ->parent = current;
  if(current == t->nil){
    t->root = new_node;
  }
  rbtree_insert_fixup(t,new_node);
  
  return t->root;
}
// 삽입하고 재정렬
void rbtree_insert_fixup(rbtree *t , node_t *node)
{
  node_t *parent = node -> parent;
  node_t *grand = parent -> parent;
  node_t *uncle;

  int is_left = node ==parent->left; //내가 왼쪽 자식인지
  int parent_is_left; // 부모가 왼쪽인지

  if (node == t->root){
    node->color =RBTREE_RED;
    return;
  }
  if (parent->color ==RBTREE_BLACK){
    return;
  }
  parent_is_left = grand->left ==parent;
  uncle = (parent_is_left) ? grand ->right : grand ->left;

  //부모와 삼촌이 모두 Red인경우
  if (uncle->color == RBTREE_RED){
    parent->color =RBTREE_BLACK;
    grand->color =RBTREE_RED;
    uncle->color = RBTREE_BLACK;
    rbtree_insert_fixup(t,grand);
    return;
  }

  if (parent_is_left){
    if (is_left)
    //case 2 : 삼촌의 색이 black , 부모가 왼쪽 자식이면서, 현재 노드가 왼쪽자식인경우
    {
      right_rotate(t,node);
      exchange_color(parent,parent->right);
      return;
    }
    //case 3 : 삼촌의 색이 BLACK, 부모가 왼쪽 자식, 현재 노드가 오른쪽인 경우
    left_rotate(t,node);
    right_rotate(t,node);
    exchange_color(node, node->right);
  }
  if (is_left)
  // case 3-2 : 삼촌의 색이 BLACK, 부모가 오른쪽 자식, 현재 노드가 왼쪽자식 
  {
    right_rotate(t,node);
    left_rotate(t,node);
    exchange_color(node,node->left);
  }
  // case 2-2 : 삼촌의 색이 BLACK, 부모가 오른쪽 자식, 현재 노드가 오른쪽 자식
  left_rotate(t,node);
  exchange_color(parent,parent->left);
}

// 왼쪽 회전
void left_rotate(rbtree *t, node_t*node){
  node_t *parent = node->parent;
  node_t *grand = parent->parent;
  node_t *node_left = node->left;

  //루트가 부모이면 현재 노드를 루트로 지정
  if(parent ==t->root){
    t->root =node;
  }
  else{
    //1. 노드의 부모를 grand_parent로 변경
    if (grand->left ==parent){
      grand -> left =node;
    }
    else{
      grand->right =node; 
    }
    node->parent = grand; // 노드를 grand의 자식으로 변경
    parent->parent = node; // parent의 부모를 노드로 변경
    node ->left = parent; // parent를 노드의 자식으로 변경
    parent -> right = node_left; // 노드의 자식의 부모를 parent로 변경
    node_left->parent = parent; // 노드의 자식을 부모의 자식으로 변경
  }
}

// 오른쪽 회전
void right_rotate(rbtree *t,node_t*node){

  node_t *parent = node ->parent;
  node_t *grand = parent->parent;
  node_t *node_right = node ->right;

  if (parent == t->root){
    t->root =node;
  }
  else{
    if (grand->left ==parent){
      grand ->left = node;
    }
    else{
      grand ->right =node;
    }
    node->parent = grand; // 노드를 grand의 자식으로 변경
    parent->parent = node; // parent의 부모를 노드로 변경
    node->right = parent; // parent를 노드의 자식으로 변경
    node_right->parent = parent; // 노드의 자식의 부모를 parent로 변경
    parent->left = node_right; // 노드의 자식을 부모의 자식으로 변경
  }
}

void exchange_color(node_t *a , node_t *b){
  int tmp = a->color;
  a->color = b->color;
  b->color = (tmp ==RBTREE_BLACK) ? RBTREE_BLACK:RBTREE_RED; 
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

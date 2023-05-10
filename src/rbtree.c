#include "rbtree.h"

#include <stdlib.h>

// 트리만들기 시작
rbtree *new_rbtree(void) {
  
  rbtree *t = (rbtree *)calloc(1, sizeof(rbtree)); // tree 구조체 동적 할당
  
  node_t *nil = (node_t *)calloc(1, sizeof(node_t)); // nil 노드 생성 및 초기화

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

// 삽입하기
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

//색변경
void exchange_color(node_t *a , node_t *b){
  int tmp = a->color;
  a->color = b->color;
  b->color = (tmp ==RBTREE_BLACK) ? RBTREE_BLACK:RBTREE_RED; 
}

//key에 해당하는 노드를 반환하는 함수
node_t *rbtree_find(const rbtree *t, const key_t key) {

  node_t *current = t->root;
  while (current != t->nil)
  {
    if (key == current->key){
      return current;
    }
    else
    {
      current = (key<current->key) ? current->left:current->right;
    }
  }
  if (current == t->nil)
    return NULL;
  return current;
}

//최소값
node_t *rbtree_min(const rbtree *t) {
  node_t *current = t->root;

  while (current->left != t->nil){
    current = current->left;
  }
  return current;
}

//최대값
node_t *rbtree_max(const rbtree *t) {
  node_t *current =t->root;

  while (current->right != t->nil){
    current = current ->right;
  }
  return current;
}

//삭제를 시작하기가 싫다.......
int rbtree_erase(rbtree *t, node_t *delete)
{
  node_t *remove; // 트리에서 없어질 노드
  node_t *remove_parent, *replace_node;
  int is_remove_black, is_remove_left;

  //  delete 삭제 후 대체할 replace_node 찾기
  //  delete 노드의 자식이 둘인 경우: delete의 키를 후계자 노드의 키값으로 대체, 노드의 색은 delete의 색 유지
  if (delete->left != t->nil && delete->right != t->nil)
  {
    remove = next_node(t, delete); // 후계자 노드 (오른쪽 서브트리에서 가장 작은 노드)
    replace_node = remove->right;      // 대체할 노드: 지워질 노드인 후계자는 항상 왼쪽 자식이 없기 때문에, 자식이 있다면 오른쪽 자식 하나뿐임
    delete->key = remove->key;         // delete의 키를 후계자 노드의 키값으로 대체 (색은 변경 X)
  }
  else
  { // delete 노드의 자식이 없거나 하나인 경우: delete 노드를 자식으로 대체, 노드의 색도 대체되는 노드의 색으로 변경
    remove = delete;
    // 대체할 노드: 자식이 있으면 자식노드로, 없으면 nil 노드로 대체
    replace_node = (remove->right != t->nil) ? remove->right : remove->left;
  }
  remove_parent = remove->parent;

  // remove 노드 제거하기
  /* remove 노드가 루트인 경우 */
  if (remove == t->root)
  {
    t->root = replace_node;        // 대체할 노드를 트리의 루트로 지정
    t->root->color = RBTREE_BLACK; // 루트 노드는 항상 BLACK
    free(remove);
    return 0; // 불균형 복구 함수 호출 불필요 (제거 전 트리에 노드가 하나 혹은 두개이므로 불균형이 발생하지 않음)
  }

  //  'remove의 부모'와 'remove의 자식' 이어주기
  is_remove_black = remove->color; // remove 노드 제거 전에 지워진 노드의 색 저장
  is_remove_left = remove_parent->left == remove;

  //  자식 연결
  if (is_remove_left) // remove가 왼쪽 자식이었을 경우: remove 부모의 왼쪽에 이어주기
    remove_parent->left = replace_node;
  else // remove가 오른쪽 자식이었을 경우: remove 부모의 오른쪽에 이어주기
    remove_parent->right = replace_node;

  //  부모도 연결 (양방향 연결)
  replace_node->parent = remove_parent;
  free(remove);

  /*remove 노드가 검정 노드인 경우 */
  //불균형 복구 함수 호출
  if (is_remove_black)
    rbtree_erase_fixup(t, remove_parent, is_remove_left);
  return 0;
}


void rbtree_erase_fixup(rbtree *t, node_t *parent,int is_left){
  node_t *extra_black = is_left? parent->left:parent->right;
  if(extra_black->color ==RBTREE_RED){
    extra_black->color=RBTREE_BLACK;
    return;
  }
  node_t *sibling = is_left ? parent->right : parent->left;
  node_t *sibling_left = sibling->left;
  node_t *sibling_right = sibling->right;

  if(sibling->color ==RBTREE_RED)
  {
    if(is_left){
      left_rotate(t,sibling);
    }
    else{
      right_rotate(t,sibling);
    }
    exchange_color(sibling,parent);
    rbtree_erase_fixup(t,parent,is_left);
    return;
  }
  node_t *near = is_left ? sibling_left : sibling_right;    // 형제의 자식 중 extra_black으로부터 가까운 노드
  node_t *distant = is_left ? sibling_right : sibling_left; // 형제의 자식 중 extra_black으로부터 먼 노드

  if (is_left && near->color == RBTREE_RED && distant->color == RBTREE_BLACK)
  { // 형제가 BLACK, 형제의 가까운 자식이 RED, 형제의 더 먼 자식이 BLACK
    right_rotate(t, near);
    exchange_color(sibling, near);
    rbtree_erase_fixup(t, parent, is_left);
    return;
  }

  if (is_left && distant->color == RBTREE_RED)
  { // 형제가 BLACK, 형제의 더 먼 자식이 RED
    left_rotate(t, sibling);
    exchange_color(sibling, parent);
    distant->color = RBTREE_BLACK;
    return;
  }

  if (near->color == RBTREE_RED && distant->color == RBTREE_BLACK)
  { // 형제가 BLACK, 형제의 가까운 자식이 RED, 형제의 더 먼 자식이 BLACK
    left_rotate(t, near);
    exchange_color(sibling, near);
    rbtree_erase_fixup(t, parent, is_left);
    return;
  }

  if (distant->color == RBTREE_RED)
  { // 형제가 BLACK, 형제의 더 먼 자식이 RED
    right_rotate(t, sibling);
    exchange_color(sibling, parent);
    distant->color = RBTREE_BLACK;
    return;
  }

  // 형제가 BLACK, 형제의 자식이 둘 다 BLACK
  sibling->color = RBTREE_RED;

  if (parent != t->root)
    rbtree_erase_fixup(t, parent->parent, parent->parent->left == parent);
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n){
  
  node_t *current = rbtree_min(t);
  arr[0] = current->key;
  for (int i = 1; i < n; i++)
  {
    if (current == t->nil)
      break;                             // 노드가 끝까지 탐색된 경우 loop 탈출
    current = next_node(t, current); // 다음 노드로 이동
    if (current == t->nil)
      break;               // 노드가 끝까지 탐색된 경우 loop 탈출
    arr[i] = current->key; // 현재 노드의 key 값을 배열에 저장
  }
  return 0;
}


node_t *next_node(const rbtree *t, node_t *p)
{
  node_t *current = p->right;
  if (current == t->nil) // 오른쪽 자식이 없으면
  {
    current = p;
    while (1)
    {
      if (current->parent->right == current) // current가 오른쪽 자식인 경우
        current = current->parent;           // 부모 노드로 이동 후 이어서 탐색
      else
        return current->parent; // current가 왼쪽 자식인 경우 부모 리턴
    }
  }
  while (current->left != t->nil) // 왼쪽 자식이 있으면
    current = current->left;      // 왼쪽 끝으로 이동
  return current;
}

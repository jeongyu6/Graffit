/**
 * CSC A48 - Intro to Computer Science II
 * 
 * Assignment 3 - Graffit 
 * 
 * This is the file where you will implement your solution for
 * Assignment 3. Please make sure you read through this file carefully
 * and that you understand what is provided and what you need to complete.
 * 
 * You will also need to read the handout carefully. 
 * 
 * Parts where you have to implement functionality are clearly labeled TODO.
 * 
 * Be sure to test your work thoroughly, our testing will be extensive
 * and will check that your solution is *correct*, not only that it
 * provides functionality.
 * 
 * (c) William Song, Mustafa Quraish
 * (c) Charles Xu
 **/

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_STR_LEN 1024

#ifndef MAT_SIZE
#define MAT_SIZE 3// A small graph
#endif

//A users name, friends and its brands
typedef struct user_struct
{
  char name[MAX_STR_LEN];
  int total_brands;
  struct friend_node_struct *friends;
  struct brand_node_struct *brands;
  bool visited;
} User;

//A linked list sorted alphabetically with users name
typedef struct friend_node_struct
{
  User *user;
  struct friend_node_struct *next;
} FriendNode;


typedef struct brand_node_struct
{
  char brand_name[MAX_STR_LEN];
  struct brand_node_struct *next;
} BrandNode;


FriendNode *allUsers = NULL;


int brand_adjacency_matrix[MAT_SIZE][MAT_SIZE];
char brand_names[MAT_SIZE][MAX_STR_LEN];

//Added new struct
typedef struct suggested_FriendNode_struct
{
  User *user;
  int shared_brands;
  struct suggested_FriendNode_struct *next;
} Sugg_FriendNode;

typedef struct suggested_BrandNode_struct
{
  BrandNode *brand;
  int shared_brands;
  struct suggested_BrandNode_struct *next;
} Sugg_BrandNode;


/**
 * Given the head to a FriendNode linked list, returns true if a
 * given user's name exists in the list. Returns false otherwise.
 */
bool in_friend_list(FriendNode *head, User *node)
{
  for (FriendNode *cur = head; cur != NULL; cur = cur->next)
  {
    if (strcmp(cur->user->name, node->name) == 0)
    {
      return true;
    }
  }
  return false;
}

/**
 * Given the head to a BrandNode linked list, returns true if a
 * given brand's name exists in the list. Returns false otherwise.
 */
bool in_brand_list(BrandNode *head, char *name)
{
  for (BrandNode *cur = head; cur != NULL; cur = cur->next)
  {
    if (strcmp(cur->brand_name, name) == 0)
    {
      return true;
    }
  }
  return false;
}

/**
 * Given the head to a FriendNode linked list, inserts a given user
 * into the linked list and returns the head of the new linked list.
 * The insertion is done in alphabetical order. If the user already
 * exists, no modifications are made and the list is returned as is.
 */
FriendNode *insert_into_friend_list(FriendNode *head, User *node)
{
  if (node == NULL)
    return head;

  if (in_friend_list(head, node))
  {
    printf("User already in list\n");
    return head;
  }

  FriendNode *fn = calloc(1, sizeof(FriendNode));
  fn->user = node;
  fn->next = NULL;

  if (head == NULL)
    return fn;

  if (strcmp(head->user->name, node->name) > 0)
  {
    fn->next = head;
    return fn;
  }

  FriendNode *cur;
  for (cur = head; cur->next != NULL && strcmp(cur->next->user->name, node->name) < 0;
       cur = cur->next)
    ;

  fn->next = cur->next;
  cur->next = fn;

  return head;
}

/**
 * Given the head to a BrandNode linked list, inserts a given brand into
 * the linked list and returns the head of the new linked list. The
 * insertion is done in alphabetical order. If the brand already exists,
 * no modifications are made and the list is returned as is.
 */
BrandNode *insert_into_brand_list(BrandNode *head, char *node)
{
  if (node == NULL)
    return head;

  if (in_brand_list(head, node))
  {
    printf("Brand already in list\n");
    return head;
  }

  BrandNode *fn = calloc(1, sizeof(BrandNode));
  strcpy(fn->brand_name, node);
  fn->next = NULL;

  if (head == NULL)
    return fn;

  if (strcmp(head->brand_name, node) > 0)
  {
    fn->next = head;
    return fn;
  }

  BrandNode *cur;
  for (cur = head; cur->next != NULL && strcmp(cur->next->brand_name, node) < 0;
       cur = cur->next)
    ;

  fn->next = cur->next;
  cur->next = fn;

  return head;
}

/**
 * Given the head to a FriendNode linked list, removes a given user from
 * the linked list and returns the head of the new linked list. If the user
 * does not exist, no modifications are made and the list is returned as is.
 */
FriendNode *delete_from_friend_list(FriendNode *head, User *node)
{
  if (node == NULL)
    return head;

  if (!in_friend_list(head, node))
  {
    printf("User not in list\n");
    return head;
  }

  if (strcmp(head->user->name, node->name) == 0)
  {
    FriendNode *temp = head->next;
    free(head);
    return temp;
  }

  FriendNode *cur;
  for (cur = head; cur->next->user != node; cur = cur->next)
    ;

  FriendNode *temp = cur->next;
  cur->next = temp->next;
  free(temp);

  return head;
}

/**
 * Given the head to a BrandNode linked list, removes a given brand from
 * the linked list and returns the head of the new linked list. If the brand
 * does not exist, no modifications are made and the list is returned as is.
 */
BrandNode *delete_from_brand_list(BrandNode *head, char *node)
{
  if (node == NULL)
    return head;

  if (!in_brand_list(head, node))
  {
    printf("Brand not in list\n");
    return head;
  }

  if (strcmp(head->brand_name, node) == 0)
  {
    BrandNode *temp = head->next;
    free(head);
    return temp;
  }

  BrandNode *cur;
  for (cur = head; strcmp(cur->next->brand_name, node) != 0; cur = cur->next)
    ;

  BrandNode *temp = cur->next;
  cur->next = temp->next;
  free(temp);

  return head;
}

/**
 * Given a user, prints their name, friends, and liked brands.
 */
void print_user_data(User *user)
{
  printf("User name: %s\n", user->name);

  printf("Friends:\n");
  for (FriendNode *f = user->friends; f != NULL; f = f->next)
  {
    printf("   %s\n", f->user->name);
  }

  printf("Brands:\n");
  for (BrandNode *b = user->brands; b != NULL; b = b->next)
  {
    printf("   %s\n", b->brand_name);
  }
}

/**
 * Given a brand, returns the index of the brand inside the brand_names array.
 * If it doesn't exist in the array, return -1
 */
int get_brand_index(char *name)
{
  for (int i = 0; i < MAT_SIZE; i++)
  {
    if (strcmp(brand_names[i], name) == 0)
    {
      return i;
    }
  }

  printf("Brand '%s' not found\n", name);
  return -1; // Not found
}

/**
 * Given a brand, prints their name, index (inside the brand_names
 * array), and the names of other similar brands.
 */
void print_brand_data(char *brand_name)
{
  int idx = get_brand_index(brand_name);
  if (idx < 0)
  {
    printf("Brand '%s' not in the list.\n", brand_name);
    return;
  }

  printf("Brand name: %s\n", brand_name);
  printf("Brand idx: %d\n", idx);

  printf("Similar brands:\n");
  for (int i = 0; i < MAT_SIZE; i++)
  {
    if (brand_adjacency_matrix[idx][i] == 1 && strcmp(brand_names[i], "") != 0)
    {
      printf("   %s\n", brand_names[i]);
    }
  }
}

/**
 * Read from a given file and populate a the brand list and brand matrix.
 **/
void populate_brand_matrix(char *file_name)
{
  // Read the file
  char buff[MAX_STR_LEN];
  FILE *f = fopen(file_name, "r");
  fscanf(f, "%s", buff);
  char *line = buff;
  // Load up the brand_names matrix
  for (int i = 0; i < MAT_SIZE; i++)
  {
    if (i == MAT_SIZE - 1)
    {
      strcpy(brand_names[i], line);
      break;
    }
    int index = strchr(line, ',') - line;
    strncpy(brand_names[i], line, index);
    line = strchr(line, ',') + sizeof(char);
  }
  // Load up the brand_adjacency_matrix
  for (int x = 0; x < MAT_SIZE; x++)
  {
    fscanf(f, "%s", buff);
    for (int y = 0; y < MAT_SIZE; y++)
    {
      int value = (int)buff[y * 2];
      if (value == 48)
      {
        value = 0;
      }
      else
      {
        value = 1;
      }
      brand_adjacency_matrix[x][y] = value;
    }
  }
}
User* newUser(char *name){
  if(name==NULL) {
    return NULL;
  }

  if (strlen(name)==0) {
    return NULL;
  }
  
  User *new=(User*)calloc(1,sizeof(User));
  if(new!=NULL){
    strcpy(new->name,name);
    new->friends=NULL;
    new->brands=NULL;
    new->visited=false;
  }
  return new;
}

FriendNode *newFriendNode(User *user){
  FriendNode *new=(FriendNode *)calloc(1,sizeof(FriendNode));
  if(new!=NULL){
    new->user=user;
    new->next=NULL;
  }
  return new;
}

/**
 * TODO: Complete this function
 * Creates and returns a user.
 */
User *create_user(char *name)
{
  User *user=newUser(name);
  if(user==NULL){
    return NULL;
  }
  if(allUsers==NULL){
    allUsers=newFriendNode(user);
    return user;
  }
  
  //Condition 1:Check if a user already exists with the same name
  if(in_friend_list(allUsers,user)){
    return NULL;
  }
 
  allUsers=insert_into_friend_list(allUsers,user);
  //returns the created user
  return user;
}
void delete_friend_list(User *user){
  while(user->friends!=NULL){
    FriendNode *temp= user->friends;
    user->friends=user->friends->next;
    free(temp);
  }
}
void delete_friend_friend_list(User *user){
  FriendNode *trav=allUsers;
  while(trav!=NULL){
    trav->user->friends= delete_from_friend_list(trav->user->friends,user);
    trav=trav->next;
  }
}

void destory_friendList(FriendNode *flist) {
  if (!flist) return;
  while (flist) {
    FriendNode *fnode = flist;
    flist = flist->next;
    free(fnode);
  }
}

void delete_brand_list(User *user){
  while(user->brands!=NULL){
    BrandNode*temp=user->brands;
    user->brands=user->brands->next;
    free(temp);
  }
}
void user_delete(User *user){
  if(user==NULL){
  }
  delete_friend_list(user);
  delete_brand_list(user);
  free(user);
}
/**
 * TODO: Complete this function
 * Deletes a given user.
 */
int delete_user(User *user)
{
 if(user==NULL){
  return -1;
 }
 if(!in_friend_list(allUsers,user)){

  return -1;
 }
  allUsers=delete_from_friend_list(allUsers,user);
 if(in_friend_list(allUsers,user)){
    return -1;
 }
 delete_friend_friend_list(user);
 user_delete(user);
 return 0;
}

/**
 * TODO: Complete this function
 * Create a friendship between user and friend.
 */
int add_friend(User *user, User *friend)
{
  if(!(user && friend)) {
    return -1;
  }
  if(strcmp(user->name,friend->name)==0){
    return -1;
  }

  bool isUserFriend = in_friend_list(user->friends, friend);
  bool isFriendFriend =in_friend_list(friend->friends, user);

  if (!(isUserFriend == false && isFriendFriend == false)) {
    return -1;
  }
  
  user->friends=insert_into_friend_list(user->friends,friend);
  friend->friends=insert_into_friend_list(friend->friends,user);
  
  //Condition 2:friendship was successfully created
  if(in_friend_list(user->friends,friend)==true &&in_friend_list(friend->friends,user)==true){
    return 0;
  }
  return -1;
}
/**
 * TODO: Complete this function
 * Removes a friendship between user and friend.
 */
int remove_friend(User *user, User *friend)
{
  //check if they were friends before
  if(!(user&&friend)){
    return -1;
  }
  bool isUserFriends=in_friend_list(user->friends,friend);
  bool isFriendFriends=in_friend_list(friend->friends,user);
  //they are not friends to begin with
  if(!(isUserFriends==true&&isFriendFriends==true)){
    return -1;
  }
  user->friends=delete_from_friend_list(user->friends,friend);
  friend->friends=delete_from_friend_list(friend->friends,user);
  //if delete successful
  if(in_friend_list(user->friends,friend)==false &&in_friend_list(friend->friends,user)==false){
    return 0;
  }
  return -1;
}
/**
 * TODO: Complete this function
 * Creates a follow relationship, the user follows the brand.
 */
bool is_valid_brand_name(char *brand_name){
  if(brand_name==NULL){
    return false;
  }
  int length_of_brand=strlen(brand_name);
  if(length_of_brand==0){
    return false;
  }
  if(length_of_brand>MAX_STR_LEN-1){
    return false;
  }

  for(int i=0;i<MAT_SIZE;i++){
    if(strcmp(brand_names[i],brand_name)==0){
      return true;
    }
  }
  return false;
}

int follow_brand(User *user, char *brand_name)
{
  if(user==NULL){
    return -1;
  }
  if(!is_valid_brand_name(brand_name)){
    return -1;
  }
  //First, check if user is following that brand
  if(in_brand_list(user->brands,brand_name)){
    //if it already existed, then return -1
    return -1;
  }
  //if it did not exist previously, then add it onto the list
  user->brands=insert_into_brand_list(user->brands,brand_name);
  if(in_brand_list(user->brands,brand_name)){
    (user->total_brands) += 1;
    return 0;
  }
  return -1;
}
/**
 * TODO: Complete this function
 * Removes a follow relationship, the user unfollows the brand.
 */
int unfollow_brand(User *user, char *brand_name)
{
  if(user==NULL){
    return -1;
  }
  if(!is_valid_brand_name(brand_name)){
    return -1;
  }
  if(!in_brand_list(user->brands,brand_name)){
    return -1;
  }
  user->brands=delete_from_brand_list(user->brands,brand_name);
  if(!in_brand_list(user->brands,brand_name)){
    return 0;
  }
  return -1;
}
int total_no_of_mutual_friends(User *a,User *b){
  FriendNode *a_friends=a->friends;
  int count=0;
  while(a_friends!=NULL){
    if(in_friend_list(b->friends,a_friends->user)){
      count++;
    }
    a_friends=a_friends->next;
  }
  return count;
}
/**
 * TODO: Complete this function
 * Return the number of mutual friends between two users.
 */
int get_mutual_friends(User *a, User *b)
{
  if(!(a!=NULL&&b!=NULL)){
    return -1;
  }
  return total_no_of_mutual_friends(a,b);
}



typedef struct  bfs_friend_node {
  FriendNode *queue; // head of the queue
  FriendNode *tail;  // lasst node of the queue} BFSQueue;
}BFSQueue;


BFSQueue* BFSQueue_create() {
  BFSQueue *newBFS=(BFSQueue*)calloc(1,sizeof(BFSQueue));
  if(newBFS==NULL){
    return NULL;
  }
  if(newBFS!=NULL){
    newBFS->queue=newBFS->tail=NULL;
  }
  return newBFS;
}

void BFSQueue_destroy(BFSQueue *bfs) {
  if(bfs->queue==NULL){
    return;
  }
  while(bfs->queue!=NULL){
    FriendNode *temp = bfs->queue;
    bfs->queue = bfs->queue->next;
    free(temp);
  }
  free(bfs);
}


void enqueue(BFSQueue *bfs, User *user){
  if (bfs == NULL) {
    return;
  }

  FriendNode* fnode=newFriendNode(user);

  if(bfs->queue==NULL){
    bfs->queue = fnode;
  } else {
    bfs->tail->next = fnode;
  }
  bfs->tail = fnode;
  return;
}

void printQueue(BFSQueue *bfs){
  if(bfs==NULL){
    return;
  }  
  FriendNode *head = bfs->queue;
  fprintf(stderr, "bfs queue: \n");
  while(head != NULL){
    fprintf(stderr,"- %s -",bfs->queue->user->name);
    head = head->next;
  }
  fprintf(stderr, "\n");
}

User *dequeue(BFSQueue *bfs){
  if(bfs==NULL||bfs->queue==NULL){
    return NULL;
  }
  FriendNode *head=bfs->queue; // get a friend in the front of the queue
  User *friend = bfs->queue->user;
  bfs->queue=bfs->queue->next; 
  // move the friend list line-up to forward
  if (bfs->queue == NULL) {
    bfs->tail = NULL;
  }
  free(head);
  return friend;
}
void reset(){
  FriendNode *head=allUsers;
  while(head!=NULL){
    head->user->visited=false;
    head=head->next;
  }
}

/** 
 * TODO: Complete this function
 * A degree of connection is the number of steps it takes to get from
 * one user to another. Returns a non-negative integer representing
 * the degrees of connection between two users.
 */
int get_degrees_of_connection(User *a, User *b)
{
  if(!(a!=NULL&&b!=NULL)){
    return -1;
  }
  if(a==b){
    return 0;
  }
  reset();
  //perform algorithm
  BFSQueue *bfs=BFSQueue_create();
  enqueue(bfs,a);
  a->visited=true;
  FriendNode *nextQueue=NULL;
  int count=1;
  while(bfs->queue!=NULL){
    nextQueue=NULL;
    while(bfs->queue!=NULL){    
      User *adjacent=dequeue(bfs);
      FriendNode *queue_friends=adjacent->friends;
      for(;queue_friends!=NULL;queue_friends=queue_friends->next){
        User *friend=queue_friends->user;
        if(friend->visited==true){
          continue;
        }
        if (friend == b) {
          BFSQueue_destroy(bfs);
          destory_friendList(nextQueue);
          return count;
        }
        nextQueue = insert_into_friend_list(nextQueue,friend);
        friend->visited=true;
      }
    }
    bfs->queue=nextQueue;
    count++;
  } 
  BFSQueue_destroy(bfs);
  destory_friendList(nextQueue);

  return -1;
}


/** 
 * TODO: Complete this function
 * Marks two brands as similar.
 */
void connect_similar_brands(char *brandNameA, char *brandNameB)
{

  //if either brand is invalid, do nothing
  if(!(is_valid_brand_name(brandNameA)&&is_valid_brand_name(brandNameB))){
    return;
  }
  //get index for brandName A
  int x=get_brand_index(brandNameA);
  //get index for brandName B
  int y=get_brand_index(brandNameB);
  //we don't allow the same node to be connected to itself
  if(x==y){
    return;
  }
  if(x!=-1&&y!=-1){
    //Mark them as similar in the brand adjacency matrix
    brand_adjacency_matrix[x][y]=1;
    brand_adjacency_matrix[y][x]=1;
  }
}

//Helper Function
int total_shared_brands(User *user, User *friend){
  int total_count=0;
  BrandNode *first=user->brands;
  BrandNode *second=friend->brands;
  while(first!=NULL&&second!=NULL){
    if(strcmp(first->brand_name,second->brand_name)==0){
      total_count++;
    }
    first=first->next;
    second=second->next;
  }
  return total_count;
}

//helper function
bool isFriendsWithEveryone(User *user){
  FriendNode *all=allUsers;
  while (all!=NULL) {
    if (strcmp(all->user->name,user->name)==0) {
      all=all->next;
      continue;
    }
    if (!in_friend_list(all->user->friends, user)) {
      return false;
    }  
    all=all->next;
  }
  return true;
}

Sugg_FriendNode *create_suggested_friend_node(User *user,int shared_brand){
  Sugg_FriendNode *node= (Sugg_FriendNode*)calloc(1,sizeof(Sugg_FriendNode));
  if(node==NULL){
    return NULL;
  }
  node->user=user;
  node->shared_brands=shared_brand;
  node->next=NULL;
  return node;
}

void insert_suggested_friend(Sugg_FriendNode** head,Sugg_FriendNode *node){
  if((*head)==NULL) {
    *head=node;
    return;
  }
  if((*head)->shared_brands<node->shared_brands){
    node->next=*head;
    *head=node;
    return;
  }
  Sugg_FriendNode *current=(*head);
  Sugg_FriendNode *prev=NULL;
  while(current!=NULL){
    if(current->shared_brands < node->shared_brands){
      node->next=current;
      if(prev!=NULL){
        prev->next=node;
      }
      return;
    }
    else if(current->shared_brands>node->shared_brands){
      prev=current;
      current=current->next;
    }
    else{
      int result=strcmp(current->user->name,node->user->name);
      if(result>0){
        node->next=current->next;
        current->next=node;
        return;
      }
      else if(result<0){
        if(prev!=NULL){
          prev->next=node;
          node->next=current;
        }
        if(prev==NULL){
          node->next=(*head);
          (*head)=node;
        }
        return;
      }
    }
  }
  prev->next=node;
}

Sugg_FriendNode * create_suggested_friend_list(User *user){
  if(user==NULL){
    return NULL;
  }
  if(isFriendsWithEveryone(user)){
    return NULL;
  }
  Sugg_FriendNode *head=NULL;
  FriendNode *all=allUsers;
  while(all!=NULL){
    if(all->user!=user&&!in_friend_list(user->friends,all->user)){
       int result=total_shared_brands(user,all->user);
       Sugg_FriendNode *node = create_suggested_friend_node(all->user,result);
       if(node!=NULL){
        insert_suggested_friend(&head,node);
       }
    }
    all=all->next;
  }
  return head;
}

/** 
* TODO: Complete this function
* Get suggested friend
*/
User *get_suggested_friend(User *user)
{
  Sugg_FriendNode *head=create_suggested_friend_list(user);
  if(head==NULL){
    return NULL;
  }
  User *nominee=head->user;
  while(head!=NULL){
    Sugg_FriendNode* remove=head;
    head=head->next;
    free(remove);
  }
  return nominee;
}


/**
 * TODO: Complete this function
 * Adds n suggested friends for the given user.
 * Returns how many friends were successfully followed.
 */
int add_suggested_friends(User *user, int n)
{
  Sugg_FriendNode *head=create_suggested_friend_list(user);
  int i=0;
  while(head!=NULL&&i<=n){
    User *newFriend=head->user;
    user->friends=insert_into_friend_list(user->friends,newFriend);
    head=head->next;
    i++;
  }
  return i;
}


//helper function:create a brandNode
BrandNode *newBrand(char brand_name[MAX_STR_LEN])
{
 BrandNode* new=(BrandNode*)calloc(1,sizeof(BrandNode));
 if(new==NULL){
  return NULL;
 }
 if(new!=NULL){
  strcpy(new->brand_name,brand_name); 
  new->next=NULL;
 }
 return new;
}

//helper function: create a suggested brand node
Sugg_BrandNode *create_suggested_brand_node(char brand_name[MAX_STR_LEN],int shared_brand){
  Sugg_BrandNode *node= (Sugg_BrandNode*)calloc(1,sizeof(Sugg_BrandNode));
  if(node==NULL){
    return NULL;
  }
  node->brand=newBrand(brand_name);
  node->shared_brands=shared_brand;
  node->next=NULL;
  return node;
}

//helper function: create a list of brands to follow
void insert_suggested_brand(Sugg_BrandNode ** head,Sugg_BrandNode *node){
  if((*head)==NULL) {
    *head=node;
    node->next=NULL;
    return;
  }
  if(node->shared_brands>0 && (*head)->shared_brands>node->shared_brands){
    node->next=*head;
    *head=node;
    return;
  }
  //reverse alphanumerical order
  Sugg_BrandNode *current=(*head);
  Sugg_BrandNode *prev=NULL;
  while(current!=NULL){
    if(current->shared_brands>0 && node->shared_brands<0){
      prev=current;
      current=current->next;
    }
    else if(current->shared_brands > node->shared_brands){
      node->next=current;
      if(prev!=NULL){
        prev->next=node;
      }
      else{
        *head=node;
      }
      return;
    }
    else if(current->shared_brands < node->shared_brands){
      prev=current;
      current=current->next;
    }
    else{
      int result=strcmp(current->brand->brand_name,node->brand->brand_name);
      if(result>0){
        prev=current;
        current=current->next;
      }
      else if(result<0){
        if(prev!=NULL){
          prev->next=node;
          node->next=current;
        }
        if(prev==NULL){
          node->next=(*head);
          (*head)=node;
        }
        return;
      }
      else{
        node->next=current->next;
        current->next=node;
        return;
      }
    }
  }
  prev->next=node;
  node->next=NULL;
}

//helper function
void initialize_visited_array(int visited[]){
  for(int i=0;i<MAT_SIZE;i++){
    visited[i]=0;
  }
  return;
}

//helper function
typedef struct  bfs_brand_node {
  BrandNode *queue; 
  BrandNode *tail;  
}BrandQueue;

BrandQueue* BrandQueue_create() {
  BrandQueue *new=(BrandQueue*)calloc(1,sizeof(BrandQueue));
  if(new!=NULL){
    new->queue=NULL;
    new->tail=NULL;
    return new;
  }
  return NULL;
}

void BrandQueue_destroy(BrandQueue *bfs) {
  if(bfs->queue==NULL){
    return;
  }
  while(bfs->queue!=NULL){
    BrandNode *temp = bfs->queue;
    bfs->queue = bfs->queue->next;
    free(temp);
  }
  free(bfs);
}

void BrandQueue_enqueue(BrandQueue *bfs, char brand_name[MAX_STR_LEN]){
  if (bfs == NULL) {
    return;
  }
  BrandNode* fnode=newBrand(brand_name);

  if(bfs->queue==NULL){
    bfs->queue = fnode;
  } else {
    bfs->tail->next = fnode;
  }
    bfs->tail = fnode;
}

BrandNode* BrandQueue_dequeue(BrandQueue* bfs){
  if(bfs==NULL||bfs->queue==NULL){
    fprintf(stderr, "Queue or BrandQueue is NULL\n");
    return NULL;
  }
  BrandNode* temp = bfs->queue;
  bfs->queue = bfs->queue->next;
  if (bfs->queue == NULL) {
      bfs->tail = NULL;
  }
  return temp;
}

void insert_not_similar_brands(Sugg_BrandNode** head,User *user){
  BrandNode *brands = NULL;
  Sugg_BrandNode *temp = *head; 
  while(temp) {
    brands = insert_into_brand_list(brands, temp->brand->brand_name);
    temp = temp->next;
  }
  for(int i=0;i<MAT_SIZE;i++){
    if(!in_brand_list(brands, brand_names[i])&&!in_brand_list(user->brands,brand_names[i])){
       Sugg_BrandNode *new=create_suggested_brand_node(brand_names[i],-1);
        if(new!=NULL){
          insert_suggested_brand(head,new);
        }
    }
  }
}


Sugg_BrandNode * get_degrees_of_brand_list(User *user,Sugg_BrandNode **head)
{
  if(user==NULL){
    return NULL;
  }

  //create visited array
  int visited[MAT_SIZE] = {0};
  
  BrandQueue *bfs=BrandQueue_create();
  BrandNode *user_brand=user->brands;
  while(user_brand!=NULL){
    BrandQueue_enqueue(bfs,user_brand->brand_name);
    int index=get_brand_index(user_brand->brand_name);
    if(index<0){
      fprintf(stderr,"Invalid index for %s\n",user_brand->brand_name);
      BrandQueue_destroy(bfs);
      return NULL;
    }
    visited[index]=1;
    user_brand = user_brand->next;
  }
  BrandNode *nextBrand = NULL;
  int count=1;
  while(bfs->queue!=NULL){
    nextBrand=NULL;
    while(bfs->queue!=NULL){
      BrandNode *brand = BrandQueue_dequeue(bfs);
      if(brand==NULL){
        continue;
      }
      int brand_index=get_brand_index(brand->brand_name);
      if(brand_index<0){
        fprintf(stderr,"Invalid index for dequeued brand\n");
        free(brand);
        continue;
      }
      free(brand);
      for(int i=0;i<MAT_SIZE;i++){
        if(visited[i]==0&&brand_adjacency_matrix[brand_index][i]==1){
          visited[i]=1;
          nextBrand=insert_into_brand_list(nextBrand,brand_names[i]);
          Sugg_BrandNode *new=create_suggested_brand_node(brand_names[i],count);
          if(new!=NULL){
             insert_suggested_brand(head,new);
          }
        }
      }
    }
    count++;
    bfs->queue=nextBrand;
  }
  insert_not_similar_brands(head,user);
  BrandQueue_destroy(bfs);
  return *head;
}

/**
 * TODO: Complete this function
 * Follows n suggested brands for the given user.
 * Returns how many brands were successfully followed.
 */
int follow_suggested_brands(User *user, int n)
{
  if(user==NULL){
    return -1;
  }
  if(n<0){
    return -1;
  }
  if(user->total_brands==MAT_SIZE){
    return 0;
  }
  //get the brand list to add
  //add brand list into the user->brands 
  Sugg_BrandNode *brand_list=NULL;
  get_degrees_of_brand_list(user,&brand_list);
  int i=0;
  while(i<n && brand_list!=NULL){
    user->brands=insert_into_brand_list(user->brands,brand_list->brand->brand_name);
    brand_list=brand_list->next;
    i++;
  }
  return i;
}

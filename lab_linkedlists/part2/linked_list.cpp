// linked_list.cc -- functions for linked_list lab (cs221) 

#include "linked_list.h"

/**
 * Inserts a new Node (with key=newKey) at the head of the linked_list.
 * PRE: head is the first node in a linked_list (if NULL, linked_list is empty) 
 * PRE: newKey is the value for the key in the new Node 
 * POST: the new Node is now the head of the linked_list
 */ 
void insert(Node*& head, int newKey) {
  Node * curr = new Node;
  curr->key  = newKey;
  curr->next = head;

  head = curr;
}

/**
 * Print the keys of a linked_list in order, from head to tail 
 * PRE: head is the first node in a linked_list (if NULL, linked_list is empty) 
 */ 
void print(Node* head) {
  std::cout << "[";
  for (Node* curr = head; curr != NULL; curr = curr->next){ 
    std::cout << curr->key;
    if (curr->next != NULL) std::cout << ", ";
  }
  std::cout << "]" << std::endl;
}

/** 
 * Returns the size (number of Nodes) of the linked_list  
 * PRE: head is the first node in a linked_list (if NULL, linked_list is empty) 
 */ 
int size(Node* head){ 
  if (! head) return 0; 
  return 1 + size(head->next); 
}

/**
 * Copies the keys in a linked list to a vector.
 * PRE: head is the first node in a linked_list (if NULL, linked_list is empty) 
 * POST: a new vector<int> containing the keys in the correct order has been returned.  
 */ 
std::vector<int> to_vector(Node* head) {
  std::vector<int> result;
  for (Node* curr = head; curr != NULL; curr = curr->next){ 
    result.push_back(curr->key); 
  }
  return result; 
}

/** 
 * Delete the last Node in the linked_list
 * PRE: head is the first Node in a linked_list (if NULL, linked_list is empty) 
 * POST: the last Node of the linked_list has been removed
 * POST: if the linked_list is now empty, head has been changed
 * POST: else head remains the first Node in the linked_list
 */
void delete_last_element(Node*& head){
  // check if list has any elements at all
  if (head == NULL) return;
  // base case: if next node's 'next' is null, delete pointer to it & memory
  if (head->next == NULL) {
    delete head;
    head = NULL;
  } else {
    // run it again on next element of list
    delete_last_element(head->next);
  }
}

/**
 * Removes an existing Node (with key=oldKey) from the linked_list. 
 * PRE: head is the first node in a linked_list (if NULL, linked_list is empty) 
 * PRE: oldKey is the value of the key in the Node to be removed 
 * PRE: if no Node with key=oldKey exists, the linked_list has not changed 
 * POST: if a Node with key=oldKey was found, then it was deleted
 * POST: other Nodes with key=oldKey might still be in the linked_list
 * POST: head is the new first Node of the linked_list, if updated
 */ 
void remove(Node*& head, int oldKey) {
  // check if list has any elements at all
  if (head == NULL) return;
  // base case: if next node's 'key' is oldKey, 
  if (head->key == oldKey) {
    // toDelete = address of node to delete
    Node * toDelete = head;
    // head points to element after to delete
    head = head->next;
    // removing toDelete
    delete toDelete;
    toDelete = NULL;
  } else {
    // run it again on next element of list
    remove(head->next, oldKey);
  }
}

/**
 * Insert a new Node (with key=newKey) after an existing Node (with key=oldKey)
 * If there is no existing Node with key=oldKey, then no action.
 * PRE: head is the first Node in a linked_list (if NULL, linked_list is empty) 
 * PRE: oldKey is the value to look for (in the key of an existing Node)  
 * PRE: newKey is the value of the key in the new Node (that might be inserted) 
 * POST: if no Node with key=oldKey was found, then the linked_list has not changed
 * POST: else a new Node (with key=newKey) is right after the Node with key = oldKey.
 */
void insert_after(Node* head, int oldKey, int newKey){
  // check if list has any elements at all
  if (head == NULL) return;
  // base case: if next node's 'key' is oldKey,
  if (head->key == oldKey) {
    // make new node to insert
    Node * toInsert = new Node;
    toInsert->key = newKey;
    // new node now points at what's to come before it points at
    toInsert->next = head->next;
    // what comes before points at new node
    head->next = toInsert;
    toInsert = NULL;
  } else {
    insert_after(head->next, oldKey, newKey);
  }
}

/** 
 * Create a new linked_list by merging two existing linked_lists. 
 * PRE: list1 is the first Node in a linked_list (if NULL, then it is empty)
 * PRE: list2 is the first Node in another linked_list (if NULL, then it is empty)
 * POST: A new linked_list is returned that contains new Nodes with the keys from 
 * the Nodes in list1 and list2, starting with the key of the first Node of list1, 
 * then the key of the first Node of list2, etc. 
 * When one list is exhausted, the remaining keys come from the other list.
 * For example: [1, 2] and [3, 4, 5] would return [1, 3, 2, 4, 5]
 */
Node* interleave(Node* list1, Node* list2){
  // make new list and add to it by back-and-forth iteration (while + switch/boolean) across both lists (until null)

  Node * weavedList;
  Node * weavedListIndex;
  Node * toAdd;
  // case false: we take from list1 --- case true: we take from list2
  bool swappie;

  // initializing weavedListIndex by manually adding first node to weaved list
  if (list1 != NULL) {
    // create new Node, copy key from list1->key to the new Node
    toAdd = new Node();
    toAdd->key = list1->key;
    // whole point of this if statement - weavedList becomes head of new list
    weavedList = toAdd;
    // point index at latest element
    weavedListIndex = toAdd;
    // move on to next element of list1
    list1 = list1->next;
    // since we took from list1, while loop will begin with list2
    if (list2 != NULL) {
    swappie = true;
    }
  } else if (list2 != NULL) {
    toAdd = new Node();
    toAdd->key = list1->key;
    weavedList = toAdd;
    weavedListIndex = toAdd;
    list2 = list2->next;
    if (list1 != NULL) {
    swappie = false;
    }
  }

  while (list1 != NULL || list2 != NULL) {

    if (swappie == false) {
      // create new Node, copy key from list1->key to the new Node
      toAdd = new Node();
      toAdd->key = list1->key;
      // latest element points to new latest element
      weavedListIndex->next = toAdd;
      // point index at latest element
      weavedListIndex = toAdd;
      // go to next element of list1
      list1 = list1->next;
      // if list2 isnt done with, flip so that while loop begins w/ list2
      if (list2 != NULL) {
      swappie = true;
      }

    } else if (swappie == true) {
      toAdd = new Node();
      toAdd->key = list2->key;
      weavedListIndex->next = toAdd;
      weavedListIndex = toAdd;
      list2 = list2->next;
      if (list1 != NULL) {
      swappie = false;
      }
    }

  }

  return weavedList;
}

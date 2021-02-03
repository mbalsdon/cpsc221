#include "chain.h"
#include <cmath>
#include <iostream>


// PA1 functions

/**
 * Destroys the current Chain. This function should ensure that
 * memory does not leak on destruction of a chain.
 */
Chain::~Chain() {
  /* your code here */
}

/**
 * Inserts a new node after the node pointed to by p in the
 * chain (so p->next is the new node) and returns a pointer to
 * the newly created node.
 * If p is NULL, inserts a new head node to the chain.
 * This function **SHOULD** create a new Node and increase length_.
 *
 * @param p = The new node should be pointed to by p->next.
 *            If p is NULL, the new node becomes the head of the chain.
 * @param ndata = The data to be inserted.
 */
Chain::Node * Chain::insertAfter(Node * p, const Block &ndata) {
  
  // case 1: p is NULL -- add new head node to chain
  if (p == NULL) {
    head_ = new Node(ndata);
    p = head_;

    length_ += 1;
    return p;
  // case 2: p is last element of list -- add new last element
  } else if (p->next == NULL) {
    p->next = new Node(ndata);
    p->next->prev = p;

    length_ += 1;
    return p->next;
  // case 3: p is not last elmt of list -- insert between elmts
  } else {
    p->next->prev = new Node(ndata);
    p->next->prev->prev = p;
    p->next->prev->next = p->next;
    p->next = p->next->prev;

    length_ += 1;
    return p->next;
  }

}

/**
 * Swaps the position in the chain of the two nodes pointed to
 * by p and q.
 * If p or q is NULL or p==q, do nothing.
 * Change the chain's head pointer if necessary.
 */
void Chain::swap(Node *p, Node *q) {
  if (p == NULL || q == NULL) return;
  if (p == q) return;

  // if q is first, swap the pointers so that p is first
  Node * first = head_;
  while (first != NULL) {
    if (first == p) {
      break;
    } else if (first == q) {
      q = p;
      p = first;
      break;
    }
    first = first->next;
  }

  Node * origPNext = p->next;
  Node * origQPrev = q->prev;
  
  // first we deal with things to the left of p and right of q, as these do not depend
  // on if p and q are adjacent.

  // p is the first elmt in the list
  if (p->prev == NULL) {
    q->prev = NULL;
    head_ = q;
  // p is not the first elmt in the list
  } else {
    q->prev = p->prev;
    q->prev->next = q;
  }
  
  // q is the last elmt in the list
  if (q->next == NULL) {
    p->next = NULL;
  // q is not the last elmt in the list
  } else {
    p->next = q->next;
    p->next->prev = p;
  }

  // we now deal with the right of p and left of q, which depend on adjacency

  // p and q are adjacent
  if (origPNext == q) {
    p->prev = q;
    q->next = p;
  // p and q are not adjacent (1+ node between)
  } else {
    origPNext->prev = q;
    origQPrev->next = p;
    p->prev = origQPrev;
    q->next = origPNext;
  }
  // yes, this can be done in O(1) time (if you want to give yourself an aneurysm)
}

/**
 * Destroys all dynamically allocated memory associated with the
 * current Chain class.
 */
void Chain::clear() {
  /* your code here */
}

/**
 * Makes the current object into a copy of the parameter:
 * All member variables should have the same value as
 * those of other, but the memory should be completely
 * independent. This function is used in both the copy
 * constructor and the assignment operator for Chains.
 */
void Chain::copy(Chain const &other) {
  /* your code here */
}

/* Modifies the current chain: 
 * 1) Find the node with the first (leftmost) block in the unscrambled
 *    image and move it to the head of the chain.
 *	This block is the one whose closest match (to the left) is the
 *	largest.  That is, the distance (using distanceTo) to this block 
 *	is big for all other blocks.
 *	For each block B, find the distanceTo B from every other block
 *	and take the minimum of these distances as B's "value".
 *	Choose the block B with the maximum value over all blocks and
 *	swap its node to the head of the chain.
 *
 * 2) Starting with the (just found) first block B, find the node with
 *    the block that is the closest match to follow B (to the right)
 *    among the remaining blocks, move (swap) it to follow B's node,
 *    then repeat to unscramble the chain/image.
 */
void Chain::unscramble() {
  /* your code here */
}

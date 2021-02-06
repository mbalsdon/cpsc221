#include "chain.h"
#include <cmath>
#include <iostream>


// PA1 functions

/**
 * Destroys the current Chain. This function should ensure that
 * memory does not leak on destruction of a chain.
 */
Chain::~Chain() {

  clear();
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
  // yes, this can be done in constant time (if you want to give yourself an aneurysm)
}

/**
 * Destroys all dynamically allocated memory associated with the
 * current Chain class.
 */
void Chain::clear() {
  
  if (head_ == NULL) return;
  // index always one ahead of whats deleted (until last node)
  Node * index = head_->next;
  while (index != NULL) {
    // delete guy behind
    index->prev->next = NULL;
    delete index->prev;
    index->prev = NULL;
    // index is/isnt last element in list
    if (index->next == NULL) {
      delete index;
      index = NULL;
      head_ = NULL;
    } else {
      index = index->next;
    }
  } // end loop

}

/**
 * Makes the current object into a copy of the parameter:
 * All member variables should have the same value as
 * those of other, but the memory should be completely
 * independent. This function is used in both the copy
 * constructor and the assignment operator for Chains.
 */
void Chain::copy(Chain const &other) {

  Node * indexOld = other.head_;
  
  if (indexOld == NULL) return;
  // 1-long chain
  if (indexOld->next == NULL) {
    head_ = new Node(indexOld->data);
    length_++;
  } else {
    // indexOld is always "one ahead" of indexNew
    head_ = new Node(indexOld->data);
    indexOld = indexOld->next;
    length_++;
    Node * indexNew = head_;
    // run the list, copy the data
    while (indexOld != NULL) {
      indexNew->next = new Node(indexOld->data);
      length_++;
      indexNew->next->prev = indexNew;

      indexNew = indexNew->next;
      indexOld = indexOld->next;
    }
  }
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

  // one-long list; already unscrambled
  if (head_->next == NULL) return;

  double maxDist = 0;
  Node * leftNode;
  Node * rightNode = head_;
  Node * contender;

  // run every node against every other node,
  // find the one with greatest cumulative distance (left piece)
  while (rightNode != NULL) {
    leftNode = head_;
    double sumDist = 0;
    while (leftNode != NULL) {
      if (leftNode != rightNode) {
        // accumulate rightNode's dist against every other node
        sumDist = sumDist + leftNode->data.distanceTo(rightNode->data);
      }
      leftNode = leftNode->next;
    } // end inner loop
    // store the node with highest cumulative distance
    if (sumDist > maxDist) {
      maxDist = sumDist;
      contender = rightNode;
    }
    rightNode = rightNode->next;
    } // end outer loop

  // if victor is at back of list
  if (contender->next == NULL) {
    contender->prev->next = NULL;
    contender->next = head_;
    contender->prev = NULL;
    head_->prev = contender;
    head_ = contender;
    }
  // if victor is not at front
  if (contender->prev != NULL) {
    contender->prev->next = contender->next;
    contender->next->prev = contender->prev;
    contender->prev = NULL;
    head_->prev = contender;
    contender->next = head_;
    head_ = contender;
    }
    
  // putting da rest in place... same idea as above

  // two-long list; already unscrambled
  if (head_->next->next == NULL) return;

  Node * index = head_;
  Node * other = head_->next;
  Node * indexNext;
  Node * contenderTwo;

  while (index->next != NULL) {
    other = index->next;
    double dist = 0;
    double min = 1000;
    // compare index block with other blocks, store min distance
    while (other != NULL) {
      dist = index->data.distanceTo(other->data);
      if (dist < min) {
        min = dist;
        contenderTwo = other;
      }
      other = other->next;
    } // end inner loop
    // set min distance block to come after index
    indexNext = index->next;
    swap(indexNext, contenderTwo);
    index = index->next;
  } // end outer loop

}
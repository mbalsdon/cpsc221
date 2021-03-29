/**
 *
 * shifty quadtree (pa3)
 * sqtree.cpp
 * This file will be used for grading.
 *
 */

#include "sqtree.h"

// First Node constructor, given.
SQtree::Node::Node(pair<int,int> ul, int w, int h, RGBAPixel a, double v)
  :upLeft(ul),width(w),height(h),avg(a),var(v),NW(NULL),NE(NULL),SE(NULL),SW(NULL)
{}

// Second Node constructor, given
SQtree::Node::Node(stats & s, pair<int,int> ul, int w, int h)
  :upLeft(ul),width(w),height(h),NW(NULL),NE(NULL),SE(NULL),SW(NULL) {
  avg = s.getAvg(ul,w,h);
  var = s.getVar(ul,w,h);
}

// SQtree destructor, given.
SQtree::~SQtree() {
  clear();
}

// SQtree copy constructor, given.
SQtree::SQtree(const SQtree & other) {
  copy(other);
}

// SQtree assignment operator, given.
SQtree & SQtree::operator=(const SQtree & rhs) {
  if (this != &rhs) {
    clear();
    copy(rhs);
  }
  return *this;
}

/**
 * SQtree constructor given tolerance for variance.
 */
SQtree::SQtree(PNG & imIn, double tol) {
  // Your code here.

  /* Construct, starting at the root. */
  stats imStats = stats(imIn);
  int width = imIn.width();
  int height = imIn.height();
  pair<int, int> ul(0, 0);

  root = buildTree(imStats, ul, width, height, tol);
  
}

/**
 * Helper for the SQtree constructor.
 */
SQtree::Node * SQtree::buildTree(stats & s, pair<int,int> & ul, int w, int h, double tol) {
  // Your code here.

  /* 0-area rectangle */
  if (w == 0 || h == 0) return NULL;

  int ulX = ul.first;
  int ulY = ul.second;

  /* Initialize the root */
  Node *node = new Node(ul, w, h, s.getAvg(ul, w, h), s.getVar(ul, w, h));

  /* Base case 1: recursion stops if the desired variance is reached */
  if (node->var <= tol) return node;
  
  /* Base case 2: recursion stops if we can't split the image anymore, i.e. area = 1 */
  if (w*h == 1) {
    return node;
    
  /* If area = 2, there is only one possible cut of two possible nodes (Wide node and tall node) */
  } else if (w*h == 2) {

    pair<int, int> piece;

    /* If the node is wide; 2x1, we cut down the middle accordingly */
    if (w > h) {
      node->NW = buildTree(s, ul, w-1, h, tol);
      piece = make_pair(ulX+1, ulY);
      node->NE = buildTree(s, piece, w-1, h, tol);

    /* If the node is tall; 1x2, we cut down the middle accordingly */
    } else {
      node->NW = buildTree(s, ul, w, h-1, tol);
      piece = make_pair(ulX, ulY+1);
      node->SW = buildTree(s, piece, w, h-1, tol);
    }

    return node;
    
  /* If area = 3, there are two possible cuts of two possible nodes. Namely:
     For the wide node, we have: [X][X][ ] and [ ][X][X]
     For the height node, it is the same but vertical. */
  } else if (w*h == 3) {

    pair<int, int> rightCut;    // "Upper left" position of the "right" cut rectangle: [ ][O][ ]
    double leftVar;             // Variance of "left" cut:  [X][X][ ]
    double rightVar;            // Variance of "right" cut: [ ][X][X]

    /* If the node is wide; 3x1 */
    if (w > h) {

      /* Get the variances */
      leftVar = s.getVar(ul, w-1, h);
      rightCut = make_pair(ulX+1, ulY);
      rightVar = s.getVar(rightCut, w-1, h);

      /* Cut whichever way gives a smaller maximum variance */
      /* Right cut has less variance */
      if (leftVar > rightVar) {

        node->NW = buildTree(s, ul, w-2, h, tol);         // [X][ ][ ]
        node->SW = buildTree(s, rightCut, w-1, h, tol);   // [ ][X][X]

      /* Left cut has less variance */
      } else {

        node->NW = buildTree(s, ul, w-1, h, tol);         // [X][X][ ]
        pair<int, int> piece(ulX+2, ulY);                
        node->SW = buildTree(s, piece, w-2, h, tol);      // [ ][ ][X]
        
      }
    
    /* If the node is tall; 1x3 */
    } else {
      
      /* Same logic as the wide case */
      rightCut = make_pair(ulX, ulY+1);
      leftVar = s.getVar(ul, w, h-1);
      rightVar = s.getVar(rightCut, w, h-1);

      /* If it wasn't clear, we differentiate 1x2 and 1x3 from the general case because
         we usually compare variances with 4 children, which is not possible with the smallest rectangles. */
      if (leftVar > rightVar) {

        node->NW = buildTree(s, ul, w, h-2, tol);
        node->SW = buildTree(s, rightCut, w, h-1, tol);

      } else {
        
        node->NW = buildTree(s, ul, w, h-1, tol);
        pair<int, int> piece(ulX, ulY+2);
        node->SW = buildTree(s, piece, w, h-2, tol);

      }

    }
    
    return node;

  /* General case: We want to split such that the maximum variance among the 4 resultant rectangles
     is minimized. e.g. We choose (4, 4, 4, 4) over (1, 1, 5, 1) since 4 < 5. */
  } else {

    pair<int, int> bestCut(ulX, ulY);
    pair<int, int> currentCut;

    /* Test every possible cut, storing the best cut over every iteration. */
    for (int y = ulY; y < ulY + h; y++) {
      for (int x = ulX; x < ulX + w; x++) {

        currentCut = make_pair(x, y);
        if (maxVar(s, ul, bestCut, w, h) > maxVar(s, ul, currentCut, w, h)) bestCut = currentCut;
      }
    }

    /* Perform the best cut, and then repeat the process for the cut pieces. */
    pair<int, int> upLeftNE = pair<int, int>(bestCut.first, ulY);
    pair<int, int> upLeftSW = pair<int, int>(ulX, bestCut.second);
    pair<int, int> upLeftSE = bestCut;

    /* NW piece's upper left position is just the upper left position we began with */
    node->NW = buildTree(s, ul, bestCut.first-ul.first, bestCut.second-ul.second, tol);
    node->NE = buildTree(s, upLeftNE, w - (bestCut.first - ul.first), bestCut.second - ul.second, tol);
    node->SW = buildTree(s, upLeftSW, bestCut.first-ul.first, h-(bestCut.second-ul.second), tol);
    node->SE = buildTree(s, upLeftSE, w-(bestCut.first-ul.first), h-(bestCut.second-ul.second), tol);

    return node;
  }
}
  
/**
 * Render SQtree and return the resulting image.
 */
PNG SQtree::render() {
  // Your code here.

  int width = root->width;
  int height = root->height;
  PNG im = PNG(width, height);

  renderPixels(im, root);

  return im;
}

/**
 * Delete allocated memory.
 */
void SQtree::clear() {
  // Your code here.

  /* Recursive */
  deleteNode(root);
}

void SQtree::copy(const SQtree & other) {
  // Your code here.

  /* Recursive */
  root = ඞ(other.root);
}

int SQtree::size() {
  // Your code here.

  Node* n = root;
  return treeSize(n);
}

double SQtree::maxVar(stats& stats, pair<int, int>& ul, pair<int, int>& cut, int width, int height) {

  int ulX = ul.first;
  int ulY = ul.second;

  int cutX = cut.first;
  int cutY = cut.second;

  /* Given the top left of the node and the location of the cut, return the biggest variabilty
     among the 4 rectangles */
  double maxVar = stats.getVar(cut, width - (cutX - ulX), height - (cutY - ulY));

  if (maxVar < stats.getVar(make_pair(ulX, cutY), cutX - ulX, height - (cutY - ulY))) {
    maxVar = stats.getVar(make_pair(ulX, cutY), cutX - ulX, height - (cutY - ulY));
  }

  if (maxVar < stats.getVar(make_pair(cutX, ulY), width - (cutX - ulX), cutY - ulY)) {
    maxVar = stats.getVar(make_pair(cutX, ulY), width - (cutX - ulX), cutY - ulY);
  }

  if (maxVar < stats.getVar(ul, cutX - ulX, cutY - ulY)) {
    maxVar = stats.getVar(ul, cutX - ulX, cutY - ulY);
  }

  return maxVar;
}

/* Recursively descends the tree and renders the leaves' average colors onto the output image. */
void SQtree::renderPixels(PNG & image, Node* root) {
  
  if (root == NULL) return;

  bool leaf = root->SW == NULL && root->SE == NULL && root->NW == NULL && root->NE == NULL;

  /* Render the node if it's a leaf */
  if (leaf) {
    
    /* Iterate across the node's rectangle, drawing the average color across it. */
    for (int x = root->upLeft.first; x < root->upLeft.first + root->width; x++) {
      for (int y = root->upLeft.second; y < root->upLeft.second + root->height; y++) {
        RGBAPixel* p = image.getPixel(x, y);
        *p = root->avg;
      }
    }
    
  /* Descend more if the node isn't a leaf */
  } else {
    renderPixels(image, root->SW);
    renderPixels(image, root->SE);
    renderPixels(image, root->NW);
    renderPixels(image, root->NE);
  }
}

/* Recursively descends the tree and deallocates all memory and node pointers. */
void SQtree::deleteNode(Node* root) {

  if (root != NULL) {

    deleteNode(root->SW);
    deleteNode(root->SE);
    deleteNode(root->NW);
    deleteNode(root->NE);

    delete root;
    root = NULL;
  }
}

/* Same logic as deleteNode */
SQtree::Node* SQtree::ඞ(Node* root) {

  if (root == NULL) {
    return NULL;

  } else {
    Node* impostor = new Node(root->upLeft, root->width, root->height, root->avg, root->var);
    
    impostor->SW = ඞ(root->SW);
    impostor->SE = ඞ(root->SE);
    impostor->NW = ඞ(root->NW);
    impostor->NE = ඞ(root->NE);
    
    /* When Amogus!? */
    return impostor;
  }
}

/* Recursively sums the number of nodes starting from the given root */
int SQtree::treeSize(Node* root) {

  if (root == NULL) {
    return 0;

  } else {
    return(treeSize(root->NW) + treeSize(root->NE) + treeSize(root->SW) + treeSize(root->SE) + 1);
  }
}
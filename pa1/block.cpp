#include "block.h"
#include <cmath>
#include <iostream>

int Block::height() const {
  return data.back().size();
}

int Block::width() const {
  return data.size();
}

void Block::render(PNG &im, int x) const {
  vector<vector<HSLAPixel>> block = data;
  // get columns from right to left
  for (int i = x+data.size()-1; i > x-1; i--) {
    vector<HSLAPixel> column = block.back();
    block.pop_back();
    // draw pixels from column (bottom to top)
    for (int y = im.height()-1; y > -1; y--) {
      // put a pixel from column on img
      *(im.getPixel(i, y)) = column.back();
      column.pop_back();
    }
  }
}

void Block::build(PNG &im, int x, int width) {
  
  // push columns of pixels into data (left of img to right)
  for (int i = x; i < x+width; i++) {
    // gather pixels in a column (top of img to bottom)
    vector<HSLAPixel> column;
    for (unsigned y = 0; y < im.height(); y++) {
    column.push_back(*im.getPixel(i, y));
  }
  data.push_back(column);
  }
}
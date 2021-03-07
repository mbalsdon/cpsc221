#include "treasureMap.h"
#include "queue.h"
using namespace std;

treasureMap::treasureMap(const PNG & baseim, const PNG & mazeim, pair<int,int> s) {

    start = s;
    base = baseim;
    maze = mazeim;

}

void treasureMap::setGrey(PNG & im, pair<int,int> loc) {

    /* Shift RGBs right 2 then left 1, effectively clearing encoded bits and halving values */
    RGBAPixel * selectedpixel = im.getPixel(loc.first, loc.second);
    selectedpixel->r = (selectedpixel->r >> 2) << 1;
    selectedpixel->g = (selectedpixel->g >> 2) << 1;
    selectedpixel->b = (selectedpixel->r >> 2) << 1;
}

void treasureMap::setLOB(PNG & im, pair<int,int> loc, int d) {

    RGBAPixel * selectedpixel = im.getPixel(loc.first, loc.second);

    /* Encode leftmost bits of d (bbXXXX) into pixel's red channel */
    int encodeme = (d >> 4) % 4;
    selectedpixel->r = (selectedpixel->r >> 2) << 2;
    selectedpixel->r = selectedpixel->r + encodeme;
    /* Encode middle bits of d (XXbbXX) into pixel's green channel */
    encodeme = (d >> 2) % 4;
    selectedpixel->g = (selectedpixel->g >> 2) << 2;
    selectedpixel->g = selectedpixel->g + encodeme;
    /* Encode end bits of d (XXXXbb) into pixel's blue channel */
    encodeme = d % 4;
    selectedpixel->b = (selectedpixel->b >> 2) << 2;
    selectedpixel->b = selectedpixel->b + encodeme;
}

PNG treasureMap::renderMap() {

    /* YOUR CODE HERE */

}


PNG treasureMap::renderMaze() {

    /* YOUR CODE HERE */

}

bool treasureMap::good(vector<vector<bool>> & v, pair<int,int> curr, pair<int,int> next) {

    /* YOUR CODE HERE */

}

vector<pair<int,int>> treasureMap::neighbors(pair<int,int> curr) {

    /* YOUR CODE HERE */

}


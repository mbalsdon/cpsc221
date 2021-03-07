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
    int encodeme = (d >> 4) % 4; // => XXXXbb => 0000bb
    selectedpixel->r = (selectedpixel->r >> 2) << 2; // Set encoding bits to 00
    selectedpixel->r = selectedpixel->r + encodeme; // Place encoding bits

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

    /* Copy of base image */
    PNG image = base;

    /* Image mask of visited pixels. Outer = x, inner = y */
    vector<bool> vm(image.height());
    vector<vector<bool>> visitedmask(image.width(), vm);

    /* Image mask of pixel distances to start. Outer = x, inner = y */
    vector<int> sm(image.height());
    vector<vector<int>> shortestmask(image.width(), sm);

    /* Queue of pixels to explore */
    Queue<pair<int,int>> toexplore;
    
    /* Mark start as visited with distance 0, encode start in image, add to queue */
    visitedmask.at(start.first).at(start.second) = 1;
    shortestmask.at(start.first).at(start.second) = 0;
    setLOB(image, start, 0);
    toexplore.enqueue(start);

    /* While the queue is not empty... */
    while (!toexplore.isEmpty()) {
        /* Take a pixel from the queue of pixels to explore */
        pair<int,int> curr = toexplore.dequeue();
        /* Calculate distance from current pixel to start */
        int currdist = abs(curr.first - start.first + curr.second - start.second);
        /* Get current pixel's cardinal neighbours: {Left, Below, Right, Above} */
        vector<pair<int,int>> neighbours = neighbors(curr);
        /* For each of the current pixel's neighbours... */
        for (int i = 0; i < 4; i++) {
            pair<int,int> p = neighbours.at(i);
            /* If neighbour is "good"... */
            if (good(visitedmask, curr, p)) {
                /* Mark neighbour as visited */
                visitedmask.at(p.first).at(p.second) = 1;
                /* Mark neighbour's distance from start */
                shortestmask.at(p.first).at(p.second) = currdist + 1;
                /* Encode neighbour's distance in image */
                setLOB(image, p, currdist + 1);
                /* Add neighbour to queue of pixels to explore */
                toexplore.enqueue(p);
            }
        }
    }
    return image;
}


PNG treasureMap::renderMaze() {

    /* YOUR CODE HERE */

}

bool treasureMap::good(vector<vector<bool>> & v, pair<int,int> curr, pair<int,int> next) {

    /* YOUR CODE HERE */
    return 0;

}

vector<pair<int,int>> treasureMap::neighbors(pair<int,int> curr) {

    /* Create vector of locations for cardinal neighbours of curr */
    /* (0,0) is the top left of the image. */
    vector<pair<int,int>> neighbours;

    neighbours.push_back(make_pair(curr.first - 1, curr.second)); // Left
    neighbours.push_back(make_pair(curr.first, curr.second + 1)); // Below
    neighbours.push_back(make_pair(curr.first + 1, curr.second)); // Right
    neighbours.push_back(make_pair(curr.first, curr.second - 1)); // Above
    
    return neighbours;
}


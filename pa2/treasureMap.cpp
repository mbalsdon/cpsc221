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

    /* Copy of base image */
    PNG image = base;

    /* Image mask of visited pixels. Outer = x, inner = y */
    vector<bool> vm(image.height());
    vector<vector<bool>> visitedmask(image.width(), vm);

    /* Queue of pixels to explore */
    Queue<pair<int,int>> toexplore;

    /* Mark start as visited, darken on image, add to queue */
    visitedmask.at(start.first).at(start.second) = 1;
    setGrey(image, start);
    toexplore.enqueue(start);

    /* While the queue is not empty... */
    while (!toexplore.isEmpty()) {
        /* Take a pixel from the queue of pixels to explore */
        pair<int,int> curr = toexplore.dequeue();
        /* Get current pixel's cardinal neighbours: {Left, Below, Right, Above} */
        vector<pair<int,int>> neighbours = neighbors(curr);
        /* For each of the current pixel's neighbours... */
        for (int i = 0; i < 4; i++) {
            pair<int,int> p = neighbours.at(i);
            /* If neighbour is "good"... */
            if (good(visitedmask, curr, p)) {
                /* Mark neighbour as visited */
                visitedmask.at(p.first).at(p.second) = 1;
                /* Darken neighbour on image */
                setGrey(image, p);
                /* Add neighbour to queue of pixels to explore */
                toexplore.enqueue(p);
            }
        }
    }

    /* Draw a 7x7 red square at start location */
    for (int x = start.first - 3; x < start.first + 4; x++) {
        for (int y = start.second - 3; y < start.second + 4; y++) {
            /* If (x,y) is in bounds */
            if (x >= 0 && y >= 0 && x < (int) image.width() && y < (int) image.height()) {
                image.getPixel(x, y)->r = 255;
                image.getPixel(x, y)->g = 0;
                image.getPixel(x, y)->b = 0;
                image.getPixel(x, y)->a = 1;
            }
        }

    }
    return image;
}

bool treasureMap::good(vector<vector<bool>> & v, pair<int,int> curr, pair<int,int> next) {

    /* next is out of bounds */
    if (next.first < 0 || next.second < 0 || next.first >= (int) base.width() || next.second >= (int) base.height()) return 0;
    /* next is visited */
    if (v.at(next.first).at(next.second) == 1) return 0;
    /* curr and next's pixel in the maze are different */
    if (*maze.getPixel(next.first, next.second) != *maze.getPixel(curr.first, curr.second)) return 0;
    return 1;
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


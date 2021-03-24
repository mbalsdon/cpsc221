#include "stats.h"

stats::stats(PNG & im){
    /* Your code here!! */

    unsigned width = im.width();
    unsigned height = im.height();

    /* Initialize vectors to mirror the image's area */
    for (unsigned y = 0; y < height; y++) {
        vector<long> intermediary;
        for (unsigned x = 0; x < width; x++) {
            intermediary.push_back(0);
        }
        sumRed.push_back(intermediary);
        sumGreen.push_back(intermediary);
        sumBlue.push_back(intermediary);
        sumsqRed.push_back(intermediary);
        sumsqGreen.push_back(intermediary);
        sumsqBlue.push_back(intermediary);
    }

    for (unsigned y = 0; y < height; y++) {

        for (unsigned x = 0; x < width; x++) {

            long thisRed = im.getPixel(x, y)->r;
            long thisGreen = im.getPixel(x, y)->g;
            long thisBlue = im.getPixel(x, y)->b;

            /* CASE 1: (0, 0) */
            if (y == 0 && x == 0) {
                sumRed[y][x] = thisRed;
                sumGreen[y][x] = thisGreen;
                sumBlue[y][x] = thisBlue;

                sumsqRed[y][x] = thisRed*thisRed;
                sumsqGreen[y][x] = thisGreen*thisGreen;
                sumsqBlue[y][x] = thisBlue*thisBlue;

            /* CASE 2: (x, 0) */
            } else if (y == 0) {
                sumRed[y][x] = sumRed[y][x - 1] + thisRed;
                sumGreen[y][x] = sumGreen[y][x - 1] + thisGreen;
                sumBlue[y][x] = sumBlue[y][x - 1] + thisBlue;

                sumsqRed[y][x] = sumsqRed[y][x - 1] + thisRed*thisRed;
                sumsqGreen[y][x] = sumsqGreen[y][x - 1] + thisGreen*thisGreen;
                sumsqBlue[y][x] = sumsqBlue[y][x - 1] + thisBlue*thisBlue;

            /* CASE 3: (0, y) */
            } else if (x == 0) {
                sumRed[y][x] = sumRed[y - 1][x] + thisRed;
                sumGreen[y][x] = sumGreen[y - 1][x] + thisGreen;
                sumBlue[y][x] = sumBlue[y - 1][x] + thisBlue;

                sumsqRed[y][x] = sumsqRed[y - 1][x] + thisRed*thisRed;
                sumsqGreen[y][x] = sumsqGreen[y - 1][x] + thisGreen*thisGreen;
                sumsqBlue[y][x] = sumsqBlue[y - 1][x] + thisBlue*thisBlue;
            
            /* CASE 4: (x, y) */
            } else {
                sumRed[y][x] = sumRed[y - 1][x] + sumRed[y][x - 1] - sumRed[y - 1][x - 1] + thisRed;
                sumGreen[y][x] = sumGreen[y - 1][x] + sumGreen[y][x - 1] - sumGreen[y - 1][x - 1] + thisGreen;
                sumBlue[y][x] = sumBlue[y - 1][x] + sumBlue[y][x - 1] - sumBlue[y - 1][x - 1] + thisBlue;

                sumsqRed[y][x] = sumsqRed[y - 1][x] + sumsqRed[y][x - 1] - sumsqRed[y - 1][x - 1] + thisRed*thisRed;
                sumsqGreen[y][x] = sumsqGreen[y - 1][x] + sumsqGreen[y][x - 1] - sumsqGreen[y - 1][x - 1] + thisGreen*thisGreen;
                sumsqBlue[y][x] = sumsqBlue[y - 1][x] + sumsqBlue[y][x - 1] - sumsqBlue[y - 1][x - 1] + thisBlue*thisBlue;

            } // END IF STATEMENT

        } // END X LOOP

    } // END Y LOOP


}

/* Doesn't check bounds (based on input); will segfault if access happens outside of the image/vector size. */
long stats::getSum(char channel, pair<int,int> ul, int w, int h){
    /* Your code here!! */

    /* "Null" rectangle (no area) */
    if (w == 0 || h == 0) return 0;

    /**
     * Assume ul = (1, 2), w = 4, h = 3.
     * B represents the rectangle. We want to subtract out X's area:
     * 
     * X X X X X 0  X is the union of two rectangles, so
     * X X X X X 0  we subtract their areas and then add
     * X B B B B 0  the intersection.
     * X B B B B 0
     * X B B B B 0  Remember that in reality, we are dealing
     * 0 0 0 0 0 0  with sums and not areas.
     */

    /* If the rectangle starts at x = 0, there is no "left" rectangle.
       If the rectangle starts at y = 0, there is no "top" rectangle.
       If the rectangle starts at (0, 0), simply return the rectangle. */
    bool leftExists = ul.first > 0;
    bool topExists = ul.second > 0;

    /* Position of the bottom right of the rectangle (downRight) */
    int drX = ul.first + w - 1;
    int drY = ul.second + h - 1;
    pair<int, int> dr (drX, drY);

    /* Bottom right of the "above" rectangle we want to subtract */
    int topX = ul.first + w - 1;
    int topY = ul.second - 1;
    pair<int, int> top (topX, topY);

    /* Bottom right of the "left" rectangle we want to subtract */
    int leftX = ul.first - 1;
    int leftY = ul.second + h - 1;
    pair<int, int> left (leftX, leftY);

    /* Bottom right of the "intersected" rectangle we want to subtract */
    int intersectX = ul.first - 1;
    int intersectY = ul.second - 1;
    pair<int, int> intersect (intersectX, intersectY);

    long drVal;
    long topVal;
    long leftVal;
    long intersectVal;

    /* Get sums from vector */
    switch (channel) {
        case 'r':
        drVal = sumRed[dr.second][dr.first];
        if (topExists) topVal = sumRed[top.second][top.first];
        if (leftExists) leftVal = sumRed[left.second][left.first];
        if (topExists && leftExists) intersectVal = sumRed[intersect.second][intersect.first];
        break;

        case 'g':
        drVal = sumGreen[dr.second][dr.first];
        if (topExists) topVal = sumGreen[top.second][top.first];
        if (leftExists) leftVal = sumGreen[left.second][left.first];
        if (topExists && leftExists) intersectVal = sumGreen[intersect.second][intersect.first];
        break;

        case 'b':
        drVal = sumBlue[dr.second][dr.first];
        if (topExists) topVal = sumBlue[top.second][top.first];
        if (leftExists) leftVal = sumBlue[left.second][left.first];
        if (topExists && leftExists) intersectVal = sumBlue[intersect.second][intersect.first];
        break;
    }

    /* Do the thing :poggies: */
    if (!topExists && !leftExists) return drVal;
    if (!topExists) return drVal - leftVal;
    if (!leftExists) return drVal - topVal;
    return drVal - topVal - leftVal + intersectVal;
}

/* The exact same thing as getSum except accessing the other three vectors */
long stats::getSumSq(char channel, pair<int,int> ul, int w, int h){
    /* Your code here!! */

    if (w == 0 || h == 0) return 0;

    bool leftExists = ul.first > 0;
    bool topExists = ul.second > 0;

    int drX = ul.first + w - 1;
    int drY = ul.second + h - 1;
    pair<int, int> dr (drX, drY);

    int topX = ul.first + w - 1;
    int topY = ul.second - 1;
    pair<int, int> top (topX, topY);

    int leftX = ul.first - 1;
    int leftY = ul.second + h - 1;
    pair<int, int> left (leftX, leftY);

    int intersectX = ul.first - 1;
    int intersectY = ul.second - 1;
    pair<int, int> intersect (intersectX, intersectY);

    long drVal;
    long topVal;
    long leftVal;
    long intersectVal;

    switch (channel) {
        case 'r':
        drVal = sumsqRed[dr.second][dr.first];
        if (topExists) topVal = sumsqRed[top.second][top.first];
        if (leftExists) leftVal = sumsqRed[left.second][left.first];
        if (topExists && leftExists) intersectVal = sumsqRed[intersect.second][intersect.first];
        break;

        case 'g':
        drVal = sumsqGreen[dr.second][dr.first];
        if (topExists) topVal = sumsqGreen[top.second][top.first];
        if (leftExists) leftVal = sumsqGreen[left.second][left.first];
        if (topExists && leftExists) intersectVal = sumsqGreen[intersect.second][intersect.first];
        break;

        case 'b':
        drVal = sumsqBlue[dr.second][dr.first];
        if (topExists) topVal = sumsqBlue[top.second][top.first];
        if (leftExists) leftVal = sumsqBlue[left.second][left.first];
        if (topExists && leftExists) intersectVal = sumsqBlue[intersect.second][intersect.first];
        break;
    }

    if (!topExists && !leftExists) return drVal;
    if (!topExists) return drVal - leftVal;
    if (!leftExists) return drVal - topVal;
    return drVal - topVal - leftVal + intersectVal;
}

// given a rectangle, compute its sum of squared deviations from mean, over all color channels.
// see written specification for a description of this function.
double stats::getVar(pair<int,int> ul, int w, int h){
    /* Your code here!! */
    
    if (w == 0 || h == 0) return 0;

    /* Variability of a color in a rectangle = 
       (Sum of color squared over rectangle) - ((Sum of color over rectangle)^2)/(Area of rectangle)
       Variability of rectangle = (Variability of R) + (Variability of G) + (Variability of B) */

    double varRed = (getSumSq('r', ul, w, h)) - (double) (getSum('r', ul, w, h)*getSum('r', ul, w, h))/(w*h);
    double varGreen = (getSumSq('g', ul, w, h)) - (double) (getSum('g', ul, w, h)*getSum('g', ul, w, h))/(w*h);
    double varBlue = (getSumSq('b', ul, w, h)) - (double) (getSum('b', ul, w, h)*getSum('b', ul, w, h))/(w*h);

    return varRed + varGreen + varBlue;
}
		
RGBAPixel stats::getAvg(pair<int,int> ul, int w, int h){
    /* Your code here!! */

    if (w == 0 || h == 0) return RGBAPixel(0, 0, 0);

    /* Average color of a rectangle = (Sum of color over rectangle)/(Area of rectangle)) */
    int avgRed = getSum('r', ul, w, h)/(w*h);
    int avgGreen = getSum('g', ul, w, h)/(w*h);
    int avgBlue = getSum('b', ul, w, h)/(w*h);

    return RGBAPixel(avgRed, avgGreen, avgBlue);
}

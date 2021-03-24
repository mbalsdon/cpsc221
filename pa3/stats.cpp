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

    /** sumThing/sumSqThing use the value in the static map of pos' "upper left" recursively,
     * resulting in diagonals. So, we call it on the bottom and right lines of the grid.
     * 
     * Calling sumThing with pos (5, 5):
     * X 0 0 0 0 0
     * 0 X 0 0 0 0 
     * 0 0 X 0 0 0
     * 0 0 0 X 0 0
     * 0 0 0 0 X 0
     * 0 0 0 0 0 X <- (5, 5)
     * 
     * Calling sumThing with pos (5, 2):
     * 0 0 0 X 0 0
     * 0 0 0 0 X 0
     * 0 0 0 0 0 X <- (5, 2)
     * 0 0 0 0 0 0
     * 0 0 0 0 0 0
     * 0 0 0 0 0 0
     * 
     */
    pair<int, int> pos;
    // *** improvable? border calcs are repeated but recursion would prob have to be redone
    // could try going from (0,0) outwards.. this codes probably fast enough tho
    for (unsigned x = 0; x < width; x++) {
        pos = make_pair (x, (int) height - 1);
        sumThing('r', im, pos);
        sumThing('g', im, pos);
        sumThing('b', im, pos);

        sumSqThing('r', im, pos);
        sumSqThing('g', im, pos);
        sumSqThing('b', im, pos);
    }

    for (unsigned y = 0; y < height; y++) {
        pos = make_pair ((int) width - 1, y);
        sumThing('r', im, pos);
        sumThing('g', im, pos);
        sumThing('b', im, pos);

        sumSqThing('r', im, pos);
        sumSqThing('g', im, pos);
        sumSqThing('b', im, pos);
    }

    /* TEST CODE */
    /* Prints a sum vector */
    for (unsigned b = 0; b < im.height(); b++) {
        for (unsigned a = 0; a < im.width(); a++) {
            printf("%8lu ", sumsqBlue[b][a]);
        }
        printf("\n");
    }
    printf("\n");
    /* Prints a color channel */
    for (unsigned b = 0; b < im.height(); b++) {
        for (unsigned a = 0; a < im.width(); a++) {
            printf("%3u ", (unsigned int) im.getPixel(a, b)->b);
        }
        printf("\n");
    }
    printf("\n");
    /* Prints the value of getSum/getSumSq */
    printf("SUM = %ld\n", getSumSq('b', make_pair(0, 0), 1, 1));
    printf("\n");
    /* END OF TEST CODE */

}

/* Control flow nightmare. */
unsigned long stats::sumThing(unsigned char channel, PNG & im, pair<int, int> pos) {
    
    if (channel != 'r' && channel != 'g' && channel != 'b') return 0;

    static pair<int, int> upLeft (0, 0);
    unsigned long result = 0;

    /* Sum at (0, 0) is equal to the channel value at (0, 0) */
    static unsigned char firstRed = im.getPixel(0, 0)->r;
    static unsigned char firstGreen = im.getPixel(0, 0)->g;
    static unsigned char firstBlue = im.getPixel(0, 0)->b;

    /* Initialize memo with sum at (0, 0) */
    static map<pair<int, int>, unsigned long> redMemo = { { upLeft, firstRed } };
    static map<pair<int, int>, unsigned long> greenMemo = { { upLeft, firstGreen } };
    static map<pair<int, int>, unsigned long> blueMemo = { { upLeft, firstBlue } };

    /* Search memo to see if sum has already been calculated */
    auto redLookup = redMemo.find(pos);
    auto greenLookup = greenMemo.find(pos);
    auto blueLookup = blueMemo.find(pos);

    bool found;

    switch (channel) {
        case 'r':
        found = redLookup != redMemo.end();
        break;

        case 'g':
        found = greenLookup != greenMemo.end();
        break;

        case 'b':
        found = blueLookup != blueMemo.end();
        break;
    }

    /* If sum was already calculated, get it */
    if (found) {
        switch (channel) {
            case 'r':
            result = redLookup->second;
            break;

            case 'g':
            result = greenLookup->second;
            break;

            case 'b':
            result = blueLookup->second;
            break;
        }
    } else {

        /* Sum the "borders", the lines horizontal and vertical to pos */
        for (int x = 0; x < pos.first + 1; x++) {
            switch (channel) {
                case 'r':
                result = result + (unsigned long) im.getPixel(x, pos.second)->r;
                break;

                case 'g':
                result = result + (unsigned long) im.getPixel(x, pos.second)->g;
                break;

                case 'b':
                result = result + (unsigned long) im.getPixel(x, pos.second)->b;
                break;
            }
        } // END X LOOP

        for (int y = 0; y < pos.second; y++) {
            switch (channel) {
                case 'r':
                result = result + (unsigned long) im.getPixel(pos.first, y)->r;
                break;

                case 'g':
                result = result + (unsigned long) im.getPixel(pos.first, y)->g;
                break;

                case 'b':
                result = result + (unsigned long) im.getPixel(pos.first, y)->b;
                break;
            }
        } // END Y LOOP

        /* Get the sum of the "inner square", a.k.a. (x-1, y-1) where (x, y) are the values of pos */
        if (pos.first > 0 && pos.second > 0) {
            pair<int, int> newPos = make_pair(pos.first - 1, pos.second - 1);
            result = result + sumThing(channel, im, newPos);
        }

    } // END OUTER IF STATEMENT

    /* Memoize the calculated value, and store it in associated vector */
    switch (channel) {
        case 'r':
        redMemo[pos] = result;
        sumRed[pos.second][pos.first] = result;
        break;

        case 'g':
        greenMemo[pos] = result;
        sumGreen[pos.second][pos.first] = result;
        break;

        case 'b':
        blueMemo[pos] = result;
        sumBlue[pos.second][pos.first] = result;
        break;
    }

    return result;
}

/* The logic for this is the exact same as sumThing, but it squares color channel values before addition. */
unsigned long stats::sumSqThing(unsigned char channel, PNG & im, pair<int, int> pos) {

    if (channel != 'r' && channel != 'g' && channel != 'b') return 0;

    static pair<int, int> upLeft (0, 0);
    unsigned long result = 0;
    /* Val is used for readability, since there is a lot of squaring going on */
    unsigned long val = 0;

    /* Sum-squared base cases */
    val = (unsigned long) im.getPixel(0, 0)->r;
    static unsigned long firstRedSq = val*val;;
    val = (unsigned long) im.getPixel(0, 0)->g;
    static unsigned long firstGreenSq = val*val;
    val = (unsigned long) im.getPixel(0, 0)->b;
    static unsigned long firstBlueSq = val*val;

    /* Sum-squared memos */
    static map<pair<int, int>, unsigned long> redSqMemo = { { upLeft, firstRedSq } };
    static map<pair<int, int>, unsigned long> greenSqMemo = { { upLeft, firstGreenSq } };
    static map<pair<int, int>, unsigned long> blueSqMemo = { { upLeft, firstBlueSq } };

    /* Sum-squared lookups */
    auto redSqLookup = redSqMemo.find(pos);
    auto greenSqLookup = greenSqMemo.find(pos);
    auto blueSqLookup = blueSqMemo.find(pos);

    bool found;

    switch (channel) {
        case 'r':
        found = redSqLookup != redSqMemo.end();
        break;

        case 'g':
        found = greenSqLookup != greenSqMemo.end();
        break;

        case 'b':
        found = blueSqLookup != blueSqMemo.end();
        break;
    }

    if (found) {
        switch (channel) {
            case 'r':
            result = redSqLookup->second;
            break;

            case 'g':
            result = greenSqLookup->second;
            break;

            case 'b':
            result = blueSqLookup->second;
            break;
        }
    } else {

        for (int x = 0; x < pos.first + 1; x++) {
            switch (channel) {
                case 'r':
                val = (unsigned long) im.getPixel(x, pos.second)->r;
                result = result + val*val;
                break;

                case 'g':
                val = (unsigned long) im.getPixel(x, pos.second)->g;
                result = result + val*val;
                break;

                case 'b':
                val = (unsigned long) im.getPixel(x, pos.second)->b;
                result = result + val*val;
                break;
            }
        } // END X LOOP

        for (int y = 0; y < pos.second; y++) {
            switch (channel) {
                case 'r':
                val = (unsigned long) im.getPixel(pos.first, y)->r;
                result = result + val*val;
                break;

                case 'g':
                val = (unsigned long) im.getPixel(pos.first, y)->g;
                result = result + val*val;
                break;

                case 'b':
                val = (unsigned long) im.getPixel(pos.first, y)->b;
                result = result + val*val;
                break;
            }
        } // END Y LOOP

        if (pos.first > 0 && pos.second > 0) {
            pair<int, int> newPos = make_pair(pos.first - 1, pos.second - 1);
            val = sumSqThing(channel, im, newPos);
            result = result + val;
        }

    } // END OUTER IF STATEMENT

    switch (channel) {
        case 'r':
        redSqMemo[pos] = result;
        sumsqRed[pos.second][pos.first] = result;
        break;

        case 'g':
        greenSqMemo[pos] = result;
        sumsqGreen[pos.second][pos.first] = result;
        break;

        case 'b':
        blueSqMemo[pos] = result;
        sumsqBlue[pos.second][pos.first] = result;
        break;
    }

    return result;
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
    


}
		
RGBAPixel stats::getAvg(pair<int,int> ul, int w, int h){
    /* Your code here!! */
}

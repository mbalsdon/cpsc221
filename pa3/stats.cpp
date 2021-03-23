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

    // /* TEST CODE; a = x, b = y */
    // /* Prints a visual of a vector and the channel values */
    // for (unsigned b = 0; b < im.height(); b++) {
    //     for (unsigned a = 0; a < im.width(); a++) {
    //         printf("%8lu ", sumsqGreen[a][b]);
    //     }
    //     printf("\n");
    // }
    // printf("\n");
    // for (unsigned b = 0; b < im.height(); b++) {
    //     for (unsigned a = 0; a < im.width(); a++) {
    //         printf("%8u ", (unsigned int) im.getPixel(a, b)->g);
    //     }
    //     printf("\n");
    // }
    // /* END OF TEST CODE */

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
        sumRed[pos.first][pos.second] = result;
        break;

        case 'g':
        greenMemo[pos] = result;
        sumGreen[pos.first][pos.second] = result;
        break;

        case 'b':
        blueMemo[pos] = result;
        sumBlue[pos.first][pos.second] = result;
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
        sumsqRed[pos.first][pos.second] = result;
        break;

        case 'g':
        greenSqMemo[pos] = result;
        sumsqGreen[pos.first][pos.second] = result;
        break;

        case 'b':
        blueSqMemo[pos] = result;
        sumsqBlue[pos.first][pos.second] = result;
        break;
    }

    return result;
}

long stats::getSum(char channel, pair<int,int> ul, int w, int h){
    /* Your code here!! */
    return 0;
}

long stats::getSumSq(char channel, pair<int,int> ul, int w, int h){
    /* Your code here!! */
    return 0;
}

// given a rectangle, compute its sum of squared deviations from mean, over all color channels.
// see written specification for a description of this function.
double stats::getVar(pair<int,int> ul, int w, int h){
    /* Your code here!! */
    return 0;

}
		
RGBAPixel stats::getAvg(pair<int,int> ul, int w, int h){
    /* Your code here!! */
}

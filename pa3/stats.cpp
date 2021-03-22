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

    pair<int, int> pos;

    for (unsigned x = 0; x < width; x++) {
        pos = make_pair(x, (int) height - 1);
        sumThing(im, pos);
    }
    for (unsigned y = 0; y < height; y++) {
        pos = make_pair((int) width - 1, y);
        sumThing(im, pos);
    }

    /* Test code for vector values; a = x, b = y */
    for (unsigned b = 0; b < im.height(); b++) {
        for (unsigned a = 0; a < im.width(); a++) {
            /* Uncomment one of the two */
            printf("%5lu ", sumRed[a][b]);
            // printf("%3u ", (unsigned int) im.getPixel(a, b)->r);
        }
        printf("\n");
    }

}

unsigned long stats::sumThing(PNG & im, pair<int, int> pos) {
    
    static pair<int, int> upLeft (0, 0);
    static unsigned char firstRed = im.getPixel(0, 0)->r;
    // static unsigned char firstGreen = im.getPixel(0, 0)->g;
    // static unsigned char firstBlue = im.getPixel(0, 0)->b;
    static map<pair<int, int>, unsigned long> memo = { { upLeft, firstRed } };

    auto lookup = memo.find(pos);
    unsigned long result = 0;

    if (lookup != memo.end()) {
        result = lookup->second;
    } else {

        for (int x = 0; x < pos.first + 1; x++) {
            result = result + (unsigned long) im.getPixel(x, pos.second)->r;
        }

        for (int y = 0; y < pos.second; y++) {
            result = result + (unsigned long) im.getPixel(pos.first, y)->r;
        }

        if (pos.first > 0 && pos.second > 0) {
            pair<int, int> newPos = make_pair(pos.first - 1, pos.second - 1);
            result = result + sumThing(im, newPos);
        }

    }

    memo[pos] = result;
    sumRed[pos.first][pos.second] = result;
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

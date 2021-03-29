// File:        pa3.cpp
// Author:      cs221 instructors
// Date:        2021-03-03
// Description: Partial test of PA3 functionality
//              Reads Point data from external files
//              Produces PNG images of the point sets

#include "sqtree.h"

int main() {

  // read in image
  PNG orig;
  orig.readFromFile("images/orig/stanley-totem-poles.png");

  stats testStats(orig);

  // use it to build a sqtree (try other tolerances)
  SQtree t(orig,1000000);

  // copy the tree (to show you can)
  SQtree tCopy(t);

  // render the sqtree
  PNG pic = t.render();

  // write it out
  pic.writeToFile("images/out/out.png");

  // /* TEST CODE - *** requires making things in stats.h public */
  // /* Print sum vector */
  // printf("-- Sum vector blue --\n");
  // for (unsigned y = 0; y < orig.height(); y++) {
  //   for (unsigned x = 0; x < orig.width(); x++) {
  //     printf("%8lu ", testStats.sumBlue[y][x]);
  //   }
  //   printf("\n");
  // }
  // printf("\n");
  // /* Print sum-squared vector */
  // printf("-- Sum-squared vector blue --\n");
  // for (unsigned y = 0; y < orig.height(); y++) {
  //   for (unsigned x = 0; x < orig.width(); x++) {
  //     printf("%8lu ", testStats.sumsqBlue[y][x]);
  //   }
  //   printf("\n");
  // }
  // printf("\n");
  // /* Print color channels */
  // printf("-- Red channel --\n");
  // for (unsigned y = 0; y < orig.height(); y++) {
  //   for (unsigned x = 0; x < orig.width(); x++) {
  //     printf("%3u ", (unsigned int) orig.getPixel(x, y)->r);
  //   }
  //   printf("\n");
  // }
  // printf("-- Green channel --\n");
  // for (unsigned y = 0; y < orig.height(); y++) {
  //   for (unsigned x = 0; x < orig.width(); x++) {
  //     printf("%3u ", (unsigned int) orig.getPixel(x, y)->g);
  //   }
  //   printf("\n");
  // }
  // printf("-- Blue channel --\n");
  // for (unsigned y = 0; y < orig.height(); y++) {
  //   for (unsigned x = 0; x < orig.width(); x++) {
  //     printf("%3u ", (unsigned int) orig.getPixel(x, y)->b);
  //   }
  //   printf("\n");
  // }
  // printf("\n");
  // /* Print test variables */
  // int tWidth = 3;
  // int tHeight = 2;
  // unsigned char tChan = 'b';
  // pair<int, int> tUpLeft (2, 0);
  // printf("CHANNEL = %c, ", tChan);
  // printf("UPLEFT = (%i, %i), ", tUpLeft.first, tUpLeft.second);
  // printf("WIDTH = %i, HEIGHT = %i\n\n", tWidth, tHeight);
  // /* Print getAvg */
  // RGBAPixel tAvgPix = testStats.getAvg(tUpLeft, tWidth, tHeight);
  // printf("AVG = %i %i %i\n\n", tAvgPix.r, tAvgPix.g, tAvgPix.b);
  // /* Print getVar */
  // printf("VAR = %f\n\n", testStats.getVar(tUpLeft, tWidth, tHeight));
  // /* Print getSum and getSumSq */
  // printf("SUM = %ld\n\n", testStats.getSum(tChan, tUpLeft, tWidth, tHeight));
  // printf("SUMSQ = %ld\n\n", testStats.getSumSq(tChan, tUpLeft, tWidth, tHeight));
  // /* END OF TEST CODE */

  /*
  orig.readFromFile("images/orig/small2.png");
  orig.readFromFile("images/orig/pacman.png");
  orig.readFromFile("images/orig/huckleberry_hound.png");
  orig.readFromFile("images/orig/rocky.png");
  orig.readFromFile("images/orig/rocky_bullwinkle.png");
  orig.readFromFile("images/orig/colour.png");
  orig.readFromFile("images/orig/klee-flora-on-sand.png");
  orig.readFromFile("images/orig/stanley-totem-poles.png");
  */

  return 0;
}

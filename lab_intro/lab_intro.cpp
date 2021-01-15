#include <iostream>
#include <cmath>
#include <cstdlib>
//test
#include "cs221util/PNG.h"
#include "cs221util/HSLAPixel.h"
#include "lab_intro.h"

using namespace cs221util;

/**
 * Returns an image that has been transformed to grayscale.
 *
 * The saturation of every pixel is set to 0, removing any color.
 *
 * @return The grayscale image.
 */
PNG grayscale(PNG image) {
  /// This function is already written for you so you can see how to
  /// interact with our PNG class.
  for (unsigned x = 0; x < image.width(); x++) {
    for (unsigned y = 0; y < image.height(); y++) {
      HSLAPixel *pixel = image.getPixel(x, y);

      // `pixel` is a pointer to the memory stored inside of the PNG `image`,
      // which means you're changing the image directly.  No need to `set`
      // the pixel since you're directly changing the memory of the image.
      pixel->s = 0;
    }
  }

  return image;
}



/**
 * Returns an image with a spotlight centered at (`centerX`, `centerY`).
 *
 * A spotlight adjusts the luminance of a pixel based on the distance the pixel
 * is away from the center by decreasing the luminance by 0.5% per 1 pixel euclidean
 * distance away from the center.
 *
 * For example, a pixel 3 pixels above and 4 pixels to the right of the center
 * is a total of `sqrt((3 * 3) + (4 * 4)) = sqrt(25) = 5` pixels away and
 * its luminance is decreased by 2.5% (0.975x its original value).  At a
 * distance over 200 pixels away, the luminance will always 0.
 *
 * The modified PNG is then returned.
 *
 * @param image A PNG object which holds the image data to be modified.
 * @param centerX The center x coordinate of the crosshair which is to be drawn.
 * @param centerY The center y coordinate of the crosshair which is to be drawn.
 *
 * @return The image with a spotlight.
 */
PNG createSpotlight(PNG image, int centerX, int centerY) {

  for (unsigned int x = 0; x < image.width(); x++) {
    for (unsigned int y = 0; y < image.height(); y++) {


      double dx = centerX - (int) x;
      double dy = centerY - (int) y;
      double dx_squared = pow(dx, 2);
      double dy_squared = pow(dy, 2);
      double distance = sqrt(dx_squared + dy_squared);

      double lum_loss = distance * 0.005; // convert percent to decimal, multiply by 0.5
      if (lum_loss > 1) lum_loss = 1;
      HSLAPixel *pixel = image.getPixel(x, y);

      pixel->l = pixel->l * (1-lum_loss);
    }
  }

  return image;
}


/**
 * Returns a image transformed to UBC colors.
 *
 * The hue of every pixel is set to the a hue value of either yellow or
 * blue, based on if the pixel's hue value is closer to yellow than blue.
 *
 * @param image A PNG object which holds the image data to be modified.
 *
 * @return The UBCify'd image.
**/
PNG ubcify(PNG image) {

  for (unsigned int x = 0; x < image.width(); x++) {
    for (unsigned int y = 0; y < image.height(); y++) {

      HSLAPixel *pixel = image.getPixel(x, y);

      double current_hue = pixel->h;

      double dist1 = abs(current_hue - 40);
      double dist2 = 360 - dist1;
      double dist_to_yellow = min(dist1, dist2);

      double dist3 = abs(current_hue - 210);
      double dist4 = 360 - dist3;
      double dist_to_blue = min(dist3, dist4);

      if (dist_to_yellow < dist_to_blue) {
        pixel->h = 40;
      } else {
        pixel->h = 210;
      }
    }
  }

  return image;
}


/**
* Returns an immge that has been watermarked by another image.
*
* The luminance of every pixel of the second image is checked, if that
* pixel's luminance is 1 (100%), then the pixel at the same location on
* the first image has its luminance increased by 0.2.
*
* @param firstImage  The first of the two PNGs.
* @param secondImage The second of the two PNGs.
*
* @return The watermarked image.
*/
PNG watermark(PNG firstImage, PNG secondImage) {

  for (unsigned x = 0; x < secondImage.width(); x++) {
    for (unsigned y = 0; y < secondImage.height(); y++) {

      HSLAPixel *pixel2 = secondImage.getPixel(x, y);
      double current_lum = pixel2->l;

      if (current_lum == 1.) {
        HSLAPixel *pixel1 = firstImage.getPixel(x, y);
        pixel1->l = pixel1->l + 0.2;
      }
    }
  }

  return firstImage;
}


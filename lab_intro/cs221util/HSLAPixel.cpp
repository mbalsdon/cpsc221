#include "HSLAPixel.h"

HSLAPixel::HSLAPixel() {
    h = 0;
    s = 0;
    l = 1.0;
    a = 1.0;
}

HSLAPixel::HSLAPixel(double h, double s, double l) {
    this->h = h;
    this->s = s;
    this->l = l;
    this->a = 1.0;
}

HSLAPixel::HSLAPixel(double h, double s, double l, double a) {
    this->h = h;
    this->s = s;
    this->l = l;
    this->a = a;
}
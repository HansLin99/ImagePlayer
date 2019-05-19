#include "borderColorPicker.h"
#include <math.h>

borderColorPicker::borderColorPicker(HSLAPixel fillColor, PNG & img, double tolerance,HSLAPixel center):color(fillColor), im(img), tol(tolerance),ctr(center)
{
    
}

HSLAPixel borderColorPicker::operator()(int x, int y)
{
    for (int x1 = x - 3; x1 <= x + 3; x1++) {
        for (int y1 = y - 3; y1 <= y + 3; y1++) {
            if (x1 != x || y1 != y){
                if (x1< 0 || y1 < 0 || x1>= (int)im.width() || y1 >= (int)im.height()) {
                    return color;
                } else {
                    HSLAPixel p1 = *im.getPixel(x1,y1);
                    if ((pow(x1-x,2) + pow(y1-y,2)<=9) && p1.dist(ctr) > tol) {
                            return color;
                    }
                }
            }
        }
    }
    return *im.getPixel(x,y);
}

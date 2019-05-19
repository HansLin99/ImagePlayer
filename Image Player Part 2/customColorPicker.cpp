#include <math.h>
#include "customColorPicker.h"

customColorPicker::customColorPicker(HSLAPixel fillColorA, HSLAPixel fillColorB, int stripeSpacing){
    colorA = fillColorA;
    colorB = fillColorB;
    spacing = stripeSpacing;
}

HSLAPixel customColorPicker::operator()(int x, int y){
    if (x % spacing == 0 || y % spacing == 0)
        return colorA;
    else
        return colorB;
}

#include "stripeColorPicker.h"

stripeColorPicker::stripeColorPicker(HSLAPixel fillColor, int stripeSpacing)
{
    color = fillColor;
    spacing = stripeSpacing;
}

HSLAPixel stripeColorPicker::operator()(int x, int y)
{
    if (x % spacing == 0) {
        return color;
    } else {
        HSLAPixel p;
        return p;
        
    }
}

#ifndef customColorPicker_h
#define customColorPicker_h

#include <stdio.h>
#include "colorPicker.h"

class customColorPicker: public colorPicker
{
public:
    /**
     * Constructs a new customColorPicker.
     *
     * @param fillColor Color for the stripes
     * @param stripeSpacing Space between the stripes
     */
    
    customColorPicker(HSLAPixel fillColorA, HSLAPixel fillColorB, int stripeSpacing);
    
    /**
     * picks the color for pixel(x,y). If x coordinate or y coordinate is a multiple fo the spacing, it will be filled with the given color A
     * otherwise, it will be filled with given color B
     * @param x The x coordinate to pick a color for.
     * @param y The y coordinate to pick a color for.
     * @return The color chosen for (x,y)
     */
    virtual HSLAPixel operator()(int x, int y);
    
private:
    HSLAPixel colorA;
    HSLAPixel colorB;
    int spacing;
};

#endif

#include "block.h"

/*From im, grabs the vertical strip of pixels whose upper left corner is at position (column,0), and
 *whose width is width*/
void Block::build(PNG & im, int column, int width){
    for (int y=0; y < (int)im.height(); y++) {
        vector<HSLAPixel> tmp;
        for (int x=column; x< column + abs(width); x++) {
            HSLAPixel * pixel = im.getPixel(x,y);
            tmp.push_back(*pixel);
        }
        data.push_back(tmp);
    }
    
}

/*Draws the current block at position (column,0) in im*/
void Block::render(PNG & im, int column) const {
    for (int y=0; y< (int)height(); y++) {
        for (int x=column; x < column + width(); x++) {
            HSLAPixel * pixel = im.getPixel(x,y);
            * pixel = data.at(y).at(x-column);
        }
    }
}

/*This function changes the saturation of every pixel in the block to 0, which removes the color,
 * leaving grey.*/
void Block::greyscale(){
    for (int y=0; y < height(); y++) {
        for (int x=0; x < width(); x++) {
            HSLAPixel* pixel = & data.at(y).at(x);
            pixel->s = 0;
        }
    }
}

/*Returns the width of the current block*/
int Block::width() const {
    return data.at(0).size();
}

/*Returns the height of the current block*/
int Block::height() const {
    return data.size();
}

/**
 * @file filler.cpp
 * Implementation of functions in the filler namespace. 
 *
 */
//#include "filler.h"

animation filler::fillStripeDFS(PNG& img, int x, int y, HSLAPixel fillColor,
                                int stripeSpacing, double tolerance, int frameFreq)
{
    stripeColorPicker a(fillColor, stripeSpacing);
    return fill<Stack>(img, x, y, a, tolerance, frameFreq);
}

animation filler::fillBorderDFS(PNG& img, int x, int y,
                                    HSLAPixel borderColor, double tolerance, int frameFreq)
{
    borderColorPicker a(borderColor, img, tolerance, *img.getPixel(x,y));
    return fill<Stack>(img, x, y, a, tolerance, frameFreq);
}

/* Given implementation of a DFS rainbow fill. */
animation filler::fillRainDFS(PNG& img, int x, int y,
                                    long double freq, double tolerance, int frameFreq)
{
    rainbowColorPicker a(freq);
    return fill<Stack>(img, x, y, a, tolerance, frameFreq);
}

animation filler::fillStripeBFS(PNG& img, int x, int y, HSLAPixel fillColor,
                                int stripeSpacing, double tolerance, int frameFreq)
{
    stripeColorPicker a(fillColor, stripeSpacing);
    return fill<Queue>(img, x, y, a, tolerance, frameFreq);
}

animation filler::fillBorderBFS(PNG& img, int x, int y,
                                    HSLAPixel borderColor, double tolerance, int frameFreq)
{
    borderColorPicker a(borderColor, img, tolerance, *img.getPixel(x,y));
    return fill<Queue>(img, x, y, a, tolerance, frameFreq);
}

/* Given implementation of a BFS rainbow fill. */
animation filler::fillRainBFS(PNG& img, int x, int y,
                                    long double freq, double tolerance, int frameFreq)
{
    rainbowColorPicker a(freq);
    return fill<Queue>(img, x, y, a, tolerance, frameFreq);
}

template <template <class T> class OrderingStructure>
animation filler::fill(PNG& img, int x, int y, colorPicker& fillColor, double tolerance, int frameFreq)
{
    /**
     * @todo You need to implement this function!
     *
     * This is the basic description of a flood-fill algorithm: Every fill
     * algorithm requires an ordering structure, which is passed to this
     * function via its template parameter. For a breadth-first-search
     * fill, that structure is a Queue, for a depth-first-search, that
     * structure is a Stack. To begin the algorithm, you simply place the
     * given point in the ordering structure, marking it processed
     * (the way you mark it is a design decision you'll make yourself).
     * We have a choice to either change the color, if appropriate, when we
     * add the point to the OS, or when we take it off. In our test cases,
     * we have assumed that you will change the color when a point is added
     * to the structure. 
     *
     * Until the structure is empty, you do the following:
     *
     * 1.     Remove a point from the ordering structure, and then...
     *
     *        1.    add its unprocessed neighbors whose color values are 
     *              within (or equal to) tolerance distance from the center, 
     *              to the ordering structure.
     *        2.    use the colorPicker to set the new color of the point.
     *        3.    mark the point as processed.
     *        4.    if it is an appropriate frame, send the current PNG to the
     *              animation (as described below).
     *
     * 2.     When implementing your breadth-first-search and
     *        depth-first-search fills, you will need to explore neighboring
     *        pixels in some order.
     *
     *        For this assignment, each pixel p has 8 neighbors, consisting of 
     *        the 8 pixels who share an edge or corner with p. (We leave it to
     *        you to describe those 8 pixel locations, relative to the location
     *        of p.)
     *
     *        While the order in which you examine neighbors does not matter
     *        for a proper fill, you must use the same order as we do for
     *        your animations to come out like ours! 
     *
     *        The order you should put
     *        neighboring pixels **ONTO** the ordering structure (queue or stack) 
     *        is as follows: **UPRIGHT(+x,-y), UP(-y), UPLEFT(-x,-y), LEFT(-x), 
     *        DOWNLEFT(-x,+y), DOWN(+y), DOWNRIGHT(+x,+y), RIGHT(+x)**
     *
     *        If you do them in a different order, your fill may
     *        still work correctly, but your animations will be different
     *        from the grading scripts!
     *
     *        IMPORTANT NOTE: *UP*
     *        here means towards the top of the image, so since an image has
     *        smaller y coordinates at the top, this is in the *negative y*
     *        direction. Similarly, *DOWN* means in the *positive y*
     *        direction.  
     *
     * 3.     For every k pixels filled, **starting at the kth pixel**, you
     *        must add a frame to the animation, where k = frameFreq.
     *
     *        For example, if frameFreq is 4, then after the 4th pixel has
     *        been filled you should add a frame to the animation, then again
     *        after the 8th pixel, etc.  You must only add frames for the
     *        number of pixels that have been filled, not the number that
     *        have been checked. So if frameFreq is set to 1, a pixel should
     *        be filled every frame.
     * 4.     Finally, as you leave the function, send one last frame to the
     *        animation. This frame will be the final result of the fill, and 
     *        it will be the one we test against.
     */
    
    animation a;
    OrderingStructure<pair<int,int>> os;
    vector<vector<pair<bool,HSLAPixel>>> comp;
    
    int width = img.width();
    int height = img.height();
    
    for (int x1=0; x1<width; x1++) {
        vector<pair<bool,HSLAPixel>> temp;
        for (int y1=0; y1< height; y1++) {
            temp.push_back(make_pair(false,*img.getPixel(x1,y1)));
        }
        comp.push_back(temp);
    }
    
    int k = 0;
    
    *img.getPixel(x,y) = fillColor(x,y);
    os.add(make_pair(x,y));
    comp[x][y].first = true;
    
    k++;
    if (k == frameFreq) {
        a.addFrame(img);
        k = 0;
    }
 
    
    while (!os.isEmpty()) {
        
        pair<int,int> cord = os.remove();

        if ((cord.first+1 < width) && (cord.second-1 >= 0) && !comp[cord.first+1][cord.second-1].first) {
            if (tolerance >= (*img.getPixel(cord.first+1,cord.second-1)).dist(comp[x][y].second)) {
                os.add(make_pair(cord.first+1,cord.second-1));
                comp[cord.first+1][cord.second-1].first = true;
                *img.getPixel(cord.first+1,cord.second-1) = fillColor(cord.first+1,cord.second-1);
                k++;
                if (k == frameFreq) {
                    a.addFrame(img);
                    k = 0;
                }
            }
            
        }
     
        if (cord.second-1 >= 0 && !comp[cord.first][cord.second-1].first) {
            if (tolerance >= (*img.getPixel(cord.first,cord.second-1)).dist(comp[x][y].second)) {
                os.add(make_pair(cord.first,cord.second-1));
                comp[cord.first][cord.second-1].first = true;
                *img.getPixel(cord.first,cord.second-1) = fillColor(cord.first,cord.second-1);
                k++;
                if (k == frameFreq) {
                    a.addFrame(img);
                    k = 0;
                }
            }
            }
        
        
        if (cord.first-1 >=0 && cord.second-1 >= 0 && !comp[cord.first-1][cord.second-1].first) {
            if (tolerance >= (*img.getPixel(cord.first-1,cord.second-1)).dist(comp[x][y].second)) {
                os.add(make_pair(cord.first-1,cord.second-1));
                comp[cord.first-1][cord.second-1].first = true;
                *img.getPixel(cord.first-1,cord.second-1) = fillColor(cord.first-1,cord.second-1);
                k++;
                if (k == frameFreq) {
                    a.addFrame(img);
                    k = 0;
                }
            }
            
        }
        
        if (cord.first-1 >= 0 && !comp[cord.first-1][cord.second].first) {
            if (tolerance >= (*img.getPixel(cord.first-1,cord.second)).dist(comp[x][y].second)) {
                os.add(make_pair(cord.first-1,cord.second));
                comp[cord.first-1][cord.second].first = true;
                *img.getPixel(cord.first-1,cord.second) = fillColor(cord.first-1,cord.second);
                k++;
                if (k == frameFreq) {
                    a.addFrame(img);
                    k = 0;
                }
            }
        }
        
        if (cord.first-1 >=0 && cord.second+1 < height && !comp[cord.first-1][cord.second+1].first) {
            if (tolerance >= (*img.getPixel(cord.first-1,cord.second+1)).dist(comp[x][y].second)) {
                os.add(make_pair(cord.first-1,cord.second+1));
                comp[cord.first-1][cord.second+1].first = true;
                *img.getPixel(cord.first-1,cord.second+1) = fillColor(cord.first-1,cord.second+1);
                k++;
                if (k == frameFreq) {
                    a.addFrame(img);
                    k = 0;
                }
            }
        }
        
        if (cord.second+1 < height && !comp[cord.first][cord.second+1].first) {
            if (tolerance >= (*img.getPixel(cord.first,cord.second+1)).dist(comp[x][y].second)) {
                os.add(make_pair(cord.first,cord.second+1));
                comp[cord.first][cord.second+1].first = true;
                *img.getPixel(cord.first,cord.second+1) = fillColor(cord.first,cord.second+1);
                k++;
                if (k == frameFreq) {
                    a.addFrame(img);
                    k = 0;
                }
            }
            
        }
        
        if (cord.first+1 < width && cord.second+1 < height && !comp[cord.first+1][cord.second+1].first) {
            if (tolerance >= (*img.getPixel(cord.first+1,cord.second+1)).dist(comp[x][y].second)) {
                os.add(make_pair(cord.first+1,cord.second+1));
                comp[cord.first+1][cord.second+1].first = true;
                *img.getPixel(cord.first+1,cord.second+1) = fillColor(cord.first+1,cord.second+1);
                k++;
                if (k == frameFreq) {
                    a.addFrame(img);
                    k = 0;
                }
            }
            
        }
        
        if (cord.first + 1 < width && !comp[cord.first+1][cord.second].first) {
            if (tolerance >= (*img.getPixel(cord.first+1,cord.second)).dist(comp[x][y].second)) {
                os.add(make_pair(cord.first+1,cord.second));
                comp[cord.first+1][cord.second].first = true;
                *img.getPixel(cord.first+1,cord.second) = fillColor(cord.first+1,cord.second);
                k++;
                if (k == frameFreq) {
                    a.addFrame(img);
                    k = 0;
                }
            }
        }
//        *img.getPixel(cord.first,cord.second) = fillColor(cord.first,cord.second);
//        comp[cord.first][cord.second].first = true;
    }
    a.addFrame(img);
    return a;
} 

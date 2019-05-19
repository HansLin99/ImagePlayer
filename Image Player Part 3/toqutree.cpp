
/**
 *
 * toqutree (pa3)
 * significant modification of a quadtree .
 * toqutree.cpp
 * This file will be used for grading.
 *
 */

#include "toqutree.h"
#include <math.h>

toqutree::Node::Node(pair<int,int> ctr, int dim, HSLAPixel a)
:center(ctr),dimension(dim),avg(a),NW(NULL),NE(NULL),SE(NULL),SW(NULL)
{}

toqutree::~toqutree(){
	clear(root);
}

toqutree::toqutree(const toqutree & other) {
	root = copy(other.root);
}


toqutree & toqutree::operator=(const toqutree & rhs){
	if (this != &rhs) {
		clear(root);
		root = copy(rhs.root);
	}
	return *this;
}

/* This constructor grabs the 2^k x 2^k sub-image centered */
/* in imIn and uses it to build a quadtree. It may assume  */
/* that imIn is large enough to contain an image of that size. */
toqutree::toqutree(PNG & imIn, int k){

	PNG* png = new PNG(pow(2,k), pow(2,k)); 
	/*
	for (int i = (int)imIn.width()/2  - pow(2,k-1); i < (int)imIn.width()/2 + pow(2, k-1); ++i){
		for (int j = (int)im.height()/2 - pow(2, k-1); j < (int)imIn.height()/2 + pow(2, k-1); ++j){
			png->getPixel(i,j)  = */

	for (int i = 0; i < pow(2,k); ++i)
	{
		for (int j = 0; j < pow (2,k); ++j)
		{
			*(png->getPixel(i,j)) = *(imIn.getPixel(i+imIn.width()/2 - pow(2,k-1), j+imIn.height()/2 - pow(2,k-1)));
		}
	}

	buildTree(png, k);
}

int toqutree::size() {
    // call recursive helper function on root
    // return size(root);
	int d = root->dimension;
	return d * d;
}

// Note that you will want to practice careful memory use
// In this function. We pass the dynamically allocated image
// via pointer so that it may be released after it is used .
// similarly, at each level of the tree you will want to
// declare a dynamically allocated stats object, and free it
// once you've used it to choose a split point, and calculate
// an average.

toqutree::Node * toqutree::buildTree(PNG * im, int k) {
	Node * curr;
	stats* statsObj = new stats(*im);
	int width = pow(2,k);
	int area = pow(2,2*k-2);

	if (k == 0){
		curr = new Node(make_pair(0,0), k, statsObj->getAvg(make_pair(0,0), make_pair(0,0)));
		curr->NW = curr->NE = curr->SW = curr->SE = NULL;

		delete im;	im = NULL;
		delete statsObj; statsObj = NULL;
		root = curr;
		return curr;
	}else if (k == 1){
		// (1,1) is SE child 
		curr = new Node(make_pair(1,1), k, statsObj->getAvg(make_pair(0,0), make_pair(1,1)));

		PNG* nwpng = getPixel(width, im, 0, 0);
		PNG* nepng = getPixel(width, im, 1, 0);
		PNG* sepng = getPixel(width, im, 1, 1);
		PNG* swpng = getPixel(width, im, 0, 1);

		delete im;	im = NULL;
		delete statsObj; statsObj = NULL;
		curr->NW = buildTree(nwpng, k-1);
		curr->NE = buildTree(nepng, k-1);
		curr->SE = buildTree(sepng, k-1);
		curr->SW = buildTree(swpng, k-1);
		root = curr;
		return curr;
	}else{
		double minEntropy = .0;
		pair <int, int> minPt = make_pair(width/4,width/4);
		for (int i = width/4; i < width/4+width/2; ++i){
			for (int j = width/4; j < width/4+width/2; ++j){	
				// the lr of the block to right of splitting point
				int exceedX;
				int exceedY;

				if (i <= width/2)
					exceedX = i-1;
				else
					exceedX = (i+width/2-1)%width;
				if (j <= width/2)
					exceedY = j-1;
				else 
					exceedY = (j+width/2-1)%width;

				vector <int> topLeft = statsObj->buildHist(make_pair(0,0), make_pair(exceedX, exceedY));
				vector <int> topMid = statsObj->buildHist(make_pair(exceedX+1, 0), make_pair(exceedX+width/2, exceedY));
				vector <int> topRight = statsObj->buildHist(make_pair(exceedX+width/2+1, 0), make_pair(width-1, exceedY));
				vector <int> midLeft = statsObj->buildHist(make_pair(0, exceedY+1), make_pair(exceedX, exceedY+width/2));
				vector <int> midMid = statsObj->buildHist(make_pair(exceedX+1, exceedY+1), make_pair(exceedX+width/2, exceedY+width/2));
				vector <int> midRight = statsObj->buildHist(make_pair(exceedX+width/2+1, exceedY+1), make_pair(width-1, exceedY+width/2));
				vector <int> btmLeft = statsObj->buildHist(make_pair(0, exceedY+width/2+1), make_pair(exceedX, width-1));
				vector <int> btmMid = statsObj->buildHist(make_pair(exceedX+1, exceedY+width/2+1), make_pair(exceedX+width/2, width-1));
				vector <int> btmRight = statsObj->buildHist(make_pair(exceedX+width/2+1, exceedY+width/2+1), make_pair(width-1, width-1));

				vector<int> distn0;
				vector<int> distn1;
				vector<int> distn2;
				vector<int> distn3;

				// usually take the topleft corner as 0, and then goes to right first, then down
				if (i == width/2 && j == width/2){
					for (int z = 0; z < 36; ++z) {
						distn0.push_back(topLeft[z]);
						distn1.push_back(midLeft[z]);
						distn2.push_back(topMid[z]);
						distn3.push_back(midRight[z]);
					}
				}else if (i == width/2 && j !=width/2){
					for (int z = 0; z < 36; ++z) {
						distn0.push_back(topLeft[z]+btmLeft[z]);
						distn1.push_back(topMid[z]+btmMid[z]);
						distn2.push_back(midLeft[z]);
						distn3.push_back(midMid[z]);
					}		
				}else if (i != width/2 &&  j == width/2){
					for (int z = 0; z < 36; ++z){
						distn0.push_back(topLeft[z]+topRight[z]);
						distn1.push_back(topMid[z]);
						distn2.push_back(midLeft[z]+midRight[z]);
						distn3.push_back(midMid[z]);
					}
				}else {
					for (int z = 0; z < 36; ++z){
						distn0.push_back(topLeft[z]+topRight[z]+btmLeft[z]+btmRight[z]);
						distn1.push_back(topMid[z]+btmMid[z]);
						distn2.push_back(midLeft[z]+midRight[z]);
						distn3.push_back(midMid[z]);
					}		
				}

				double avg = (statsObj->entropy(distn0,area)+statsObj->entropy(distn1,area)+statsObj->entropy(distn2,area)+statsObj->entropy(distn3,area))/4;
				if (i == width/4 && j == width/4)
					minEntropy = avg;
				else{
					if (avg < minEntropy) {
						minEntropy = avg;
						minPt.first = i;
						minPt.second = j;
					} 
				}
			}
		}
		
		curr = new Node(minPt, k, statsObj->getAvg(make_pair(0,0), make_pair(width-1, width-1)));
		pair <int, int> SW_ul = make_pair((minPt.first+width/2)%width, minPt.second);
		pair <int, int> NE_ul = make_pair(minPt.first, (minPt.second+width/2)%width);
		pair <int, int> NW_ul = make_pair((minPt.first+width/2)%width, (minPt.second+width/2)%width);

		PNG* nwpng = getPixel(width, im, NW_ul.first, NW_ul.second);
		PNG* nepng = getPixel(width, im, NE_ul.first, NE_ul.second);
		PNG* sepng = getPixel(width, im, minPt.first, minPt.second);
		PNG* swpng = getPixel(width, im, SW_ul.first, SW_ul.second);
		
		delete im;	im = NULL;
		delete statsObj; statsObj = NULL;
		curr->NW = buildTree(nwpng, k-1);
		curr->NE = buildTree(nepng, k-1);
		curr->SE = buildTree(sepng, k-1);
		curr->SW = buildTree(swpng, k-1);
		
		root = curr;
		return curr;
	}

}

PNG toqutree::render(){
	PNG newpng = PNG(pow(2, root->dimension), pow(2, root->dimension));
	return renderHelper(root);
}

PNG toqutree::renderHelper(Node* subRoot){
	if (subRoot->NW == NULL && subRoot->SW == NULL && subRoot->SE  == NULL && subRoot->NE == NULL){
		PNG newpng = PNG(pow(2,root->dimension),pow(2,root->dimension));
		for (int i = 0; i < pow(2, root->dimension); ++i)
			for (int j = 0; j < pow(2, root->dimension); ++j)
				*(newpng.getPixel(i,j)) = subRoot->avg;
		return newpng;
	}else{
		PNG se = renderHelper(subRoot->SE);
		PNG sw = renderHelper(subRoot->SW);
		PNG ne = renderHelper(subRoot->NE);
		PNG nw = renderHelper(subRoot->NW);

		int splitX = subRoot->center.first;
		int splitY = subRoot->center.second;
		int sideLength = pow(2,subRoot->dimension);

		PNG newpng = PNG(sideLength,sideLength);
		int westX;
		int northY;
		if (splitX > sideLength/2)
			westX = (splitX + sideLength/2)%sideLength;
		else
			westX = splitX + sideLength/2;

		if (splitY > sideLength)
			northY = (splitY + sideLength/2)%sideLength;
		else
			northY = splitY + sideLength/2;

		newpng = getPixel4R(&newpng, se, splitX, splitY, sideLength);
		newpng = getPixel4R(&newpng, nw, westX, northY, sideLength);
		newpng = getPixel4R(&newpng, sw, westX, splitY, sideLength);
		newpng = getPixel4R(&newpng, ne, splitX, northY, sideLength);

		return newpng;
	}
}

/* oops, i left the implementation of this one in the file! */
void toqutree::prune(double tol){
	prune(root,tol);
}

//A subtree is pruned (cleared) if ALL of the subtree's leaves are within tol of the average color stored in the root of the subtree.
void toqutree::prune(Node* subRoot, double tolerance){
	if (subRoot != NULL){
		if (inTolerence(subRoot, tolerance, subRoot->avg) == 0){
			clear(subRoot->NW);
			clear(subRoot->NE);
			clear(subRoot->SW);
			clear(subRoot->SE);
			return;
		}else{
			prune(subRoot->NW, tolerance);
			prune(subRoot->NE, tolerance);
			prune(subRoot->SE, tolerance);
			prune(subRoot->SW, tolerance);
		}
	}

}

int toqutree::inTolerence(Node* subRoot, double tolerance, HSLAPixel avg){
	if (subRoot->NW == NULL){
		if (avg.dist(subRoot->avg) <= tolerance)
			return 0;
		else
			return 1;
	}else {
		return inTolerence(subRoot->NW, tolerance, avg) + inTolerence(subRoot->NE, tolerance, avg) + 
		inTolerence(subRoot->SE, tolerance, avg) + inTolerence(subRoot->SW, tolerance, avg);
	}
}

/* called by destructor and assignment operator*/
void toqutree::clear(Node * & curr){
	if (curr != NULL) {
		clear(curr->NW);
		clear(curr->NE);
		clear(curr->SW);
		clear(curr->SE);

		delete curr;
		curr = NULL;
	}

}

/* done */
/* called by assignment operator and copy constructor */
toqutree::Node * toqutree::copy(const Node * other) {
   // Node* curr = other;
	this->root = copyHelper(other);
	return root;
}

toqutree::Node* toqutree::copyHelper(const Node* curr){
	if (curr == NULL) {
		return NULL;
	}

	Node* newNode = new Node(curr->center, curr->dimension, curr->avg);
	newNode->NW = copyHelper(curr->NW);
	newNode->NE = copyHelper(curr->NE);
	newNode->SE = copyHelper(curr->SE);
	newNode->SW = copyHelper(curr->SW);

	return newNode;
}

PNG* toqutree::getPixel(int side, PNG* im, int offsetX, int offsetY){
	PNG* png = new PNG(side/2,side/2);
	for (int i = 0; i < side/2; i++){
		for (int j = 0; j < side/2; j++){
			*(png->getPixel(i,j)) = *(im->getPixel((i+offsetX)%side, (j+offsetY)%side));
		}
	}
	return png;
}

PNG toqutree::getPixel4R(PNG* im, PNG children, int offsetX, int offsetY, int sideLength){
	for (int i = 0; i < sideLength/2; i++){
		for (int j = 0; j < sideLength/2; j++){
			*(im->getPixel((i+offsetX)%sideLength,(j+offsetY)%sideLength)) = *(children.getPixel(i, j));
		}
	}
	return *im;
}

#include "stats.h"
#include <math.h>
#include <stdio.h>

stats::stats(PNG & im){
    int width = im.width();
    int height = im.height();
    
    for (int i = 0; i < width; i++) {
        vector<double> vectorOfValHX;
        vector<double> vectorOfValHY;
        vector<double> vectorOfValS;
        vector<double> vectorOfValL;
        vector<vector<int>> coord;
        for (int j = 0; j < height; j++) {
            vector<int> vectorOfK;
            HSLAPixel* curr = im.getPixel(i,j);

            if (i == 0 && j == 0) {
                vectorOfValHX.push_back(cos(curr->h*PI/180));
                vectorOfValHY.push_back(sin(curr->h*PI/180));
                vectorOfValS.push_back(curr->s);
                vectorOfValL.push_back(curr->l);
                
                for (int k = 0; k < 36; k++) {
                    if ((curr->h >= k*10) && (curr->h < (k+1)*10))
                        vectorOfK.push_back(1);
                    else
                        vectorOfK.push_back(0);
                }
                
            }   else if (i > 0 && j == 0){
                vectorOfValHX.push_back(sumHueX[i-1][j] + cos(curr->h*PI/180));
                vectorOfValHY.push_back(sumHueY[i-1][j] + sin(curr->h*PI/180));
                vectorOfValS.push_back(sumSat[i-1][j] + curr->s);
                vectorOfValL.push_back(sumLum[i-1][j] + curr->l);
                
                for (int k = 0; k < 36; k++) {
                    if ((curr->h >= k*10) && (curr->h < (k+1)*10))
                        vectorOfK.push_back(1+hist[i-1][j][k]);
                    else
                        vectorOfK.push_back(0+hist[i-1][j][k]);
                }
                
            }else if (i == 0 && j > 0){
                vectorOfValHX.push_back(vectorOfValHX[j-1] + cos(curr->h*PI/180));
                vectorOfValHY.push_back(vectorOfValHY[j-1] + sin(curr->h*PI/180));
                vectorOfValS.push_back(vectorOfValS[j-1] + curr->s);
                vectorOfValL.push_back(vectorOfValL[j-1] + curr->l);
                
                for (int k = 0; k < 36; k++) {
                    if ((curr->h >= k*10) && (curr->h < (k+1)*10))
                        vectorOfK.push_back(1+coord[j-1][k]);
                    else
                        vectorOfK.push_back(0+coord[j-1][k]);
                }
            }else if (i > 0 && j > 0){
                vectorOfValHX.push_back(vectorOfValHX[j-1] + sumHueX[i-1][j] - sumHueX[i-1][j-1]+ cos(curr->h*PI/180));
                vectorOfValHY.push_back(vectorOfValHY[j-1] + sumHueY[i-1][j] - sumHueY[i-1][j-1] + sin(curr->h*PI/180));
                vectorOfValS.push_back(vectorOfValS[j-1] + sumSat[i-1][j] - sumSat[i-1][j-1] + curr->s);
                vectorOfValL.push_back(vectorOfValL[j-1] + sumLum[i-1][j] - sumLum[i-1][j-1] + curr->l);
                
                for (int k = 0; k < 36; k++) {
                    if ((curr->h >= k*10) && (curr->h < (k+1)*10))
                        vectorOfK.push_back(1+coord[j-1][k]+hist[i-1][j][k]-hist[i-1][j-1][k]);
                    else
                        vectorOfK.push_back(0+coord[j-1][k]+hist[i-1][j][k]-hist[i-1][j-1][k]);
                }
            }
            coord.push_back(vectorOfK);
        }
        sumHueX.push_back(vectorOfValHX);
        sumHueY.push_back(vectorOfValHY);
        sumSat.push_back(vectorOfValS);
        sumLum.push_back(vectorOfValL);
        hist.push_back(coord);
    }
}

long stats::rectArea(pair<int,int> ul, pair<int,int> lr){
    return (lr.first-ul.first+1)*(lr.second-ul.second+1);
}

HSLAPixel stats::getAvg(pair<int,int> ul, pair<int,int> lr){
    HSLAPixel ret;
    double hueX;
    double hueY;
    double sat;
    double lum;
    
    if (ul.first == 0 && ul.second == 0) {
        hueX = sumHueX[lr.first][lr.second];
        hueY = sumHueY[lr.first][lr.second];
        sat = sumSat[lr.first][lr.second];
        lum = sumLum[lr.first][lr.second];       
    }else if (ul.first == 0 && ul.second != 0){
        hueX = sumHueX[lr.first][lr.second] - sumHueX[lr.first][ul.second-1];       
        hueY = sumHueY[lr.first][lr.second] - sumHueY[lr.first][ul.second-1];
        sat = sumSat[lr.first][lr.second] - sumSat[lr.first][ul.second-1];
        lum = sumLum[lr.first][lr.second] - sumLum[lr.first][ul.second-1];
    }else if (ul.first != 0 && ul.second == 0){
        hueX = sumHueX[lr.first][lr.second] - sumHueX[ul.first-1][lr.second];
        hueY = sumHueY[lr.first][lr.second] - sumHueY[ul.first-1][lr.second];
        sat = sumSat[lr.first][lr.second] - sumSat[ul.first-1][lr.second];
        lum = sumLum[lr.first][lr.second] - sumLum[ul.first-1][lr.second];
    }else{
        hueX = sumHueX[lr.first][lr.second] - sumHueX[ul.first-1][lr.second] - sumHueX[lr.first][ul.second-1] + sumHueX[ul.first-1][ul.second-1];
        hueY = sumHueY[lr.first][lr.second] - sumHueY[ul.first-1][lr.second] - sumHueY[lr.first][ul.second-1] + sumHueY[ul.first-1][ul.second-1];
        sat = sumSat[lr.first][lr.second] - sumSat[ul.first-1][lr.second] - sumSat[lr.first][ul.second-1] + sumSat[ul.first-1][ul.second-1];
        lum = sumLum[lr.first][lr.second] - sumLum[ul.first-1][lr.second] - sumLum[lr.first][ul.second-1] + sumLum[ul.first-1][ul.second-1];
    }

    long pixelcount = rectArea(ul, lr);
    
    ret.s = sat/pixelcount;
    ret.l = lum/pixelcount;
    ret.a = 1.0;
    ret.h = atan2(hueY,hueX)*180/PI;
    while (ret.h < 0)
        ret.h+=360;   
    return ret;
}

vector<int> stats::buildHist(pair<int,int> ul, pair<int,int> lr){
    vector<int> distribution;
    for (int k = 0; k < 36; ++k){
        if (ul.first == 0 && ul.second == 0) {
            distribution.push_back(hist[lr.first][lr.second][k]);
        }else if (ul.first == 0 && ul.second != 0){
            distribution.push_back(hist[lr.first][lr.second][k] - hist[lr.first][ul.second-1][k]);
        }else if (ul.first != 0 && ul.second == 0){
           distribution.push_back(hist[lr.first][lr.second][k] - hist[ul.first-1][lr.second][k]);
       }else{
        distribution.push_back(hist[lr.first][lr.second][k] - hist[ul.first-1][lr.second][k] - hist[lr.first][ul.second-1][k] + hist[ul.first-1][ul.second-1][k]);
        }
    }
    return distribution;
}

// takes a distribution and returns entropy
// partially implemented so as to avoid rounding issues.
double stats::entropy(vector<int> & distn,int area){
    double entropy = 0.;
    for (int i = 0; i < 36; i++) {
        if (distn[i] > 0 ) 
            entropy += ((double) distn[i]/(double) area) 
        * log2((double) distn[i]/(double) area);
    }
    return  -1 * entropy;
}

double stats::entropy(pair<int,int> ul, pair<int,int> lr){
    vector<int> distribution = buildHist(ul,lr);
    int area = rectArea(ul, lr);
    return stats::entropy(distribution, area);
}

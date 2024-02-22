/*****************************************************************************
    Digital Image Processing 2023 Autumn
    Homework #3     : Chromatic Adaptation & Image Enhancement
    Author          : Zheng-Gang, Ciou (nycu311511022.ee11@nycu.edu.tw)
    File name       : hw3.cpp
*****************************************************************************/
#include "BmpImage.h"
using namespace std;

int main() {
    // BmpImage img1_ori("input1_ori.bmp");
    BmpImage img1("../img/input1.bmp");
    BmpImage img2("../img/input2.bmp");

    // img1.deMotionBlur(25, 0.2, 5, 1).writeBMP("../img/output1.bmp");
    img1.deMotionBlur(25, 0.2, 5, 1).sharpness(3).writeBMP("../img/output1.bmp");
    img2.deMotionBlur(25, 0.1, 5, 1).sharpness(2).writeBMP("../img/output2.bmp");

    return 0;
}
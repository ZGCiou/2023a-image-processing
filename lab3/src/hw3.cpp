/*****************************************************************************
    Digital Image Processing 2023 Autumn
    Homework #3     : Chromatic Adaptation & Image Enhancement
    Author          : Zheng-Gang, Ciou (nycu311511022.ee11@nycu.edu.tw)
    File name       : hw3.cpp
*****************************************************************************/
#include "BmpImage.h"
using namespace std;

int main(int argc, char **argv) {
    // Read input image
    BmpImage img1("../img/input1.bmp");
    BmpImage img2("../img/input2.bmp");
    BmpImage img3("../img/input3.bmp");
    BmpImage img4("../img/input4.bmp");

    // Chromatic Adaptation
    BmpImage img1_1(img1.grayWorld(false));
    BmpImage img2_1(img2.maxRGB());
    BmpImage img3_1(img3.maxRGB());
    BmpImage img4_1(img4.grayWorld(false));

    img1_1.writeBMP("../img/output1_1.bmp");
    img2_1.writeBMP("../img/output2_1.bmp");
    img3_1.writeBMP("../img/output3_1.bmp");
    img4_1.writeBMP("../img/output4_1.bmp");

    // Image Enhancement
    img1_1.histoEqual().lumiPowLawTransform(1.25).writeBMP("../img/output1_2.bmp");
    img2_1.histoEqual().writeBMP("../img/output2_2.bmp");
    img3_1.lumiPowLawTransform(0.95).writeBMP("../img/output3_2.bmp");
    img4_1.histoEqual().lumiPowLawTransform(0.65).writeBMP("../img/output4_2.bmp");

    return 0;
}
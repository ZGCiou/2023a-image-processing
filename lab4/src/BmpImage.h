/*****************************************************************************
    Digital Image Processing 2023 Autumn
    Homework #3     : Chromatic Adaptation & Image Enhancement
    Author          : Zheng-Gang, Ciou (nycu311511022.ee11@nycu.edu.tw)
    File name       : BmpImage.h
*****************************************************************************/
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <opencv2/core.hpp>
using namespace std;

#ifndef _BMPIMAGE_H_
#define _BMPIMAGE_H_

struct BmpFileHeader {
    uint16_t bfTybe;// = 0x424D;
    uint32_t bfSize;
    uint16_t bfReserved1;// = 0;
    uint16_t bfReserved2;// = 0;
    uint32_t bfOffBits;// = 54;
} __attribute__((packed));

struct BmpInfoHeader {
    uint32_t biSize;// = 40;
    uint32_t biWidth;
    uint32_t biHeight;
    uint16_t biPlanes;// = 1; // 1=defeaul, 0=custom
    uint16_t biBitCount;
    uint32_t biCompression;// = 0;
    uint32_t biSizeImage;// = 0;
    uint32_t biXPelsPerMeter;// = 0; // 72dpi=2835, 96dpi=3780
    uint32_t biYPelsPerMeter;// = 0; // 120dpi=4724, 300dpi=11811
    uint32_t biClrUsed;// = 0;
    uint32_t biClrImportant;// = 0;
} __attribute__((packed));

struct Pixel {
    uint8_t A;
    uint8_t B;
    uint8_t G;
    uint8_t R;
};

struct YCbCr {
    int Y;
    int Cb;
    int Cr;
};

class BmpImage
{
private:
/* Data Members */
    BmpFileHeader fileHeader;
    BmpInfoHeader infoHeader;
    Pixel **rawImage;

public:
/* Function Members */
    /* Constructor */
    BmpImage();
    BmpImage(string);
    /* Copy constructor */
    BmpImage(const BmpImage&);
    /* Destructor */
    ~BmpImage();
    /* Print haeder */
    void showHeader();
    /* Read image from bmp file */
    void readBMP(string);
    /* Write image to bmp file */
    void writeBMP(string);
    /* Assignment operator */
    BmpImage& operator=(const BmpImage&);
    /* Transform RGB to YCbCr */
    YCbCr** toYCbCr() const;
    /* read YCbCr to RGB raw image */
    BmpImage& readYCbCr(YCbCr**);

    /**************** HW1 ****************/
    /* Flip image */
    BmpImage flip() const;
    /* Quantization resolution */
    BmpImage resolution(int) const;
    /* Scale image with rate 1.5 */
    BmpImage scaling50(bool) const;

    /**************** HW2 ****************/
    /* Low-luminosity enhancement */
    BmpImage histoEqual() const;
    BmpImage lowLumiEnhanceLocal(int) const;
    BmpImage lumiPowLawTransform(double) const;
    /* Sharpness enhancement */
    BmpImage sharpness(int) const;
    /* Denoise */
    BmpImage denoise(int) const;

    /**************** HW3 ****************/
    /* Color Constancy */
    BmpImage grayWorld(bool avgGrary=true, bool clip=false) const;
    BmpImage maxRGB(bool autoGain=true) const;

    /**************** HW4 ****************/
    void getSize(int &height, int &width) const;
    void toSpecturm(cv::Mat2f &dftR, cv::Mat2f &dftG, cv::Mat2f &dftB);
    void showColorSpecturm(string imgName="");
    vector<vector<float> > filterShiftAndPad(vector<vector<float> > &filterPSF);
    BmpImage freqOperator(cv::Mat &OTF);
    BmpImage deMotionBlur(int motionSize, float snr, int guassSize=0, float guassSigma=0);
};
/* Convolution */
double** conv2D(double** image, int imageHeight, int imageWidth, double** filter, int kernelSize);
/* Mean filter */
double** meanFilter(double**, int, int, int);
/* Show Specturm */
void showSpecturm(const cv::Mat2f &complexImag, string title);
/* Guassian Filter */
vector<vector<float> > genGaussPSF(int size, float sigma);
/* Motion Blur */
vector<vector<float> > genMotionPSF(int size);
/* Wiener Filter */
void wienerFilterOTF(const BmpImage &img, vector<vector<float> > degPSF, cv::Mat2f &wnrOTF, float snr);
void wienerFilterOTF(cv::Mat2f &degOTF, cv::Mat2f &wnrOTF, float snr);
/* Frequency Domain Filter */
void filterToFreq(vector<vector<float> > filterPSF, cv::Mat2f &filterOTF);
/* Align kernal */
vector<vector<float> > filterShiftAndPad(const BmpImage &img, vector<vector<float> > &filterPSF);

#endif
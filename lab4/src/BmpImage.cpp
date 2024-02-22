/*****************************************************************************
    Digital Image Processing 2023 Autumn
    Homework #3     : Chromatic Adaptation & Image Enhancement
    Author          : Zheng-Gang, Ciou (nycu311511022.ee11@nycu.edu.tw)
    File name       : BmpImage.cpp
*****************************************************************************/
#include "BmpImage.h"
#include <cmath>
#include <complex>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
using namespace std;

BmpImage::BmpImage() {
}

BmpImage::BmpImage(const BmpImage& origBmp) {
    fileHeader = origBmp.fileHeader;
    infoHeader = origBmp.infoHeader;
    rawImage = new Pixel* [origBmp.infoHeader.biHeight];
    for (int i=0; i<origBmp.infoHeader.biHeight; i++) {
        rawImage[i] = new Pixel[origBmp.infoHeader.biWidth];
        for (int j=0; j<origBmp.infoHeader.biWidth; j++) {
            rawImage[i][j] = origBmp.rawImage[i][j];
        }
    }
}

BmpImage::BmpImage(string fileName) {
    readBMP(fileName);
}

BmpImage::~BmpImage() {
    for (int i=0; i<infoHeader.biHeight; i++) {
        delete [] rawImage[i];
    }
}

BmpImage& BmpImage::operator=(const BmpImage& origBmp) {
    for (int i=0; i<infoHeader.biHeight; i++) {
        delete [] rawImage[i];
    }

    fileHeader = origBmp.fileHeader;
    infoHeader = origBmp.infoHeader;
    
    rawImage = new Pixel* [origBmp.infoHeader.biHeight];
    for (int i=0; i<origBmp.infoHeader.biHeight; i++) {
        rawImage[i] = new Pixel[origBmp.infoHeader.biWidth];
        for (int j=0; j<origBmp.infoHeader.biWidth; j++) {
            rawImage[i][j] = origBmp.rawImage[i][j];
        }
    }

    return *this;
}

void BmpImage::showHeader() {
    cout << "--- File Header --- " << endl;
    cout << "bfTybe = " << hex << (*this).fileHeader.bfTybe << endl;
    cout << "bfSize = " << dec << (*this).fileHeader.bfSize << endl;
    cout << "bfReserved1 = " << (*this).fileHeader.bfReserved1 << endl;
    cout << "bfReserved2 = " << (*this).fileHeader.bfReserved2 << endl;
    cout << "bfOffBits = " << (*this).fileHeader.bfOffBits << endl;
    cout << "--- Info Header --- " << endl;
    cout << "biSize = " << (*this).infoHeader.biSize << endl;
    cout << "biWidth = " << (*this).infoHeader.biWidth << endl;
    cout << "biHeight = " << (*this).infoHeader.biHeight << endl;
    cout << "biPlanes = " << (*this).infoHeader.biPlanes << endl;
    cout << "biBitCount = " << (*this).infoHeader.biBitCount << endl;
    cout << "biCompression = " << (*this).infoHeader.biCompression << endl;
    cout << "biSizeImage = " << (*this).infoHeader.biSizeImage << endl;
    cout << "biXPelsPerMeter = " << (*this).infoHeader.biXPelsPerMeter << endl;
    cout << "biYPelsPerMeter = " << (*this).infoHeader.biYPelsPerMeter << endl;
    cout << "biClrUsed = " << (*this).infoHeader.biClrUsed << endl;
    cout << "biClrImportant = " << (*this).infoHeader.biClrImportant << endl;
    cout << "------------------- " << endl;
}

void BmpImage::readBMP(string inFileName) {
    /*-------------------------------------------------------------
    Read FileHeader, InfoHaeder, and Raw image to a bmp file.
    -------------------------------------------------------------*/
    ifstream fin(inFileName, ios::binary);
    /* Read header */
    fin.read((char *)&fileHeader, sizeof(BmpFileHeader));
    fin.read((char *)&infoHeader, sizeof(BmpInfoHeader));
    /* Calculate aligned or not */
    int channel = infoHeader.biBitCount / 8;
    int alig = (infoHeader.biWidth * channel) % 4;
    int pad0 = (alig == 0) ? 0 : (4 - alig);
    /* Read raw image */
    rawImage = new Pixel* [infoHeader.biHeight];
    for (int i=0; i<infoHeader.biHeight; i++) {
        rawImage[i] = new Pixel[infoHeader.biWidth];
        // fin.read((char *)rawImage24[i], sizeof(rawImage24[i]));
        // why error?

        for (int j=0; j<infoHeader.biWidth; j++) {  
            fin.read((char *)&rawImage[i][j].B, sizeof(uint8_t));
            fin.read((char *)&rawImage[i][j].G, sizeof(uint8_t));
            fin.read((char *)&rawImage[i][j].R, sizeof(uint8_t));
            if (infoHeader.biBitCount == 32) {
                fin.read((char *)&rawImage[i][j].A, sizeof(uint8_t));
            }
            // cout << hex << (int)rawImage[i][j].B << " " << (int)rawImage[i][j].G << " " << (int)rawImage[i][j].R << " ";
        }
        /* If not aligned, skip the padding zeros */
        for (int j=0; j<pad0; j++) {
            fin.get();
        }
    }

    fin.close();
}

void BmpImage::writeBMP(string outFileName) {
    /*-------------------------------------------------------------
    Write FileHeader, InfoHaeder, and Raw image to a bmp file.
    -------------------------------------------------------------*/
    ofstream fout(outFileName, ios::binary);
    /* Write header */
    fout.write((char *)&fileHeader, sizeof(BmpFileHeader));
    fout.write((char *)&infoHeader, sizeof(BmpInfoHeader));
    /* Calculate aligned or not */
    int channel = infoHeader.biBitCount / 8;
    int alig = (infoHeader.biWidth * channel) % 4;
    int pad0 = (alig == 0) ? 0 : (4 - alig); 
    /* Write header */
    for (int i=0; i<infoHeader.biHeight; i++) {
        // fout.write((char *)rawImage24[i], sizeof(rawImage24[i]));
        // why error?

        for (int j=0; j<infoHeader.biWidth; j++) {
            fout.write((char *)&rawImage[i][j].B, sizeof(uint8_t));
            fout.write((char *)&rawImage[i][j].G, sizeof(uint8_t));
            fout.write((char *)&rawImage[i][j].R, sizeof(uint8_t));
            if (infoHeader.biBitCount == 32) {
                fout.write((char *)&rawImage[i][j].A, sizeof(uint8_t));
            }
        }
        /* If not aligned, write padding zeros */
        for (int j=0; j<pad0; j++) {
            fout.put(0);
        }
    }

    fout.close();
}

BmpImage BmpImage::flip() const {
    /*------------------------------------------------------------------------
    Return a BmpImage class that the raw image is fliped from input BmpImage.
    ------------------------------------------------------------------------*/
    BmpImage flipBmp;
    flipBmp.fileHeader = fileHeader;
    flipBmp.infoHeader = infoHeader;
    flipBmp.rawImage = new Pixel* [infoHeader.biHeight];
    for (int i=0; i<infoHeader.biHeight; i++) {
        flipBmp.rawImage[i] = new Pixel[infoHeader.biWidth];
        for (int j=0; j<infoHeader.biWidth; j++) {
            flipBmp.rawImage[i][j] = rawImage[i][infoHeader.biWidth - j];
        }
    }
    return flipBmp;
}

BmpImage BmpImage::resolution(int quanBit=6) const {
    /*----------------------------------------------------------------------------
    Return a BmpImage class that the quantization resolution from input BmpImage.
    By bitwise-and 111100 can trancate 8-bits to 6-bits.
    ----------------------------------------------------------------------------*/
    /* Make a mask accroding to trancation bit number */
    uint8_t mask = 0xFF;
    for (int i=0; i<(8-quanBit); i++) {
        mask = mask << 1;
    }

    BmpImage resolBmp;
    resolBmp.fileHeader = fileHeader;
    resolBmp.infoHeader = infoHeader;
    resolBmp.rawImage = new Pixel* [infoHeader.biHeight];
    /* bitwise-and the hole raw image */
    for (int i=0; i<infoHeader.biHeight; i++) {
        resolBmp.rawImage[i] = new Pixel[infoHeader.biWidth];
        for (int j=0; j<infoHeader.biWidth; j++) {
            if (infoHeader.biBitCount == 32) {
                resolBmp.rawImage[i][j].A = rawImage[i][j].A & mask;
            }
            resolBmp.rawImage[i][j].B = rawImage[i][j].B & mask;
            resolBmp.rawImage[i][j].G = rawImage[i][j].G & mask;
            resolBmp.rawImage[i][j].R = rawImage[i][j].R & mask;
        }
    }
    return resolBmp;
}

BmpImage BmpImage::scaling50(bool upScale) const {
    /*------------------------------------------------------------------------
    Return a BmpImage class that the raw image is scaled from input BmpImage.
    By inverse wrapping and bilinear interpolation.
    ------------------------------------------------------------------------*/
    int dstHeight, dstWidth;
    /* Calculate the new size */
    if (upScale) {
        dstHeight = infoHeader.biHeight * 1.5;
        dstWidth = infoHeader.biWidth * 1.5;
    }
    else {
        dstHeight = infoHeader.biHeight / 1.5;
        dstWidth = infoHeader.biWidth / 1.5;
    }
    
    BmpImage scaleBmp;
    scaleBmp.fileHeader = fileHeader;
    scaleBmp.infoHeader = infoHeader;
    scaleBmp.infoHeader.biHeight = dstHeight;
    scaleBmp.infoHeader.biWidth = dstWidth;
    /* Check aligned or not, and write the new size in the info header */
    int channel = scaleBmp.infoHeader.biBitCount / 8;
    int alig = (scaleBmp.infoHeader.biWidth * channel) % 4;
    int pad0 = (alig == 0) ? 0 : (4 - alig); 
    scaleBmp.fileHeader.bfSize = scaleBmp.infoHeader.biHeight * (scaleBmp.infoHeader.biWidth * channel + pad0)
                                + scaleBmp.fileHeader.bfOffBits;
    /* Inverse wrapping */
    scaleBmp.rawImage = new Pixel* [scaleBmp.infoHeader.biHeight];
    float srcX, srcY, offsetX, offsetY;                                 // bilinear interpolation arguments
    int biasX, biasY, nextX, nextY;                                     // bias->0, next->1
    for (int i=0; i<scaleBmp.infoHeader.biHeight; i++) {
        scaleBmp.rawImage[i] = new Pixel[scaleBmp.infoHeader.biWidth];
        srcY = i * (float(infoHeader.biHeight - 1) / float(scaleBmp.infoHeader.biHeight - 1));
        biasY = floorf(srcY);
        nextY = (biasY == infoHeader.biHeight-1) ? biasY : biasY+1;     // avoid to acess over the image size
        offsetY = srcY - biasY;                                         // set the coordinate to 0 and 1

        for (int j=0; j<scaleBmp.infoHeader.biWidth; j++) {
            srcX = j * (float(infoHeader.biWidth - 1) / float(scaleBmp.infoHeader.biWidth - 1));
            biasX = floorf(srcX);
            nextX = (biasX == infoHeader.biWidth-1) ? biasX : biasX+1;  // avoid to acess over the image size
            offsetX = srcX - biasX;                                     // set the coordinate to 0 and 1
            /* Bilinear interpolation: 
            f(x,y) = f(0,0)(1-x)(1-y) + f(1,0)x(1-y) + f(0,1)(1-x)y + f(1,1)xy */
            if (infoHeader.biBitCount == 32) {
                // A
                scaleBmp.rawImage[i][j].A = (float)rawImage[biasY][biasX].A * (1-offsetX) * (1-offsetY)
                                            + (float)rawImage[biasY][nextX].A * offsetX * (1-offsetY)
                                            + (float)rawImage[nextY][biasX].A * (1-offsetX) * offsetY
                                            + (float)rawImage[nextY][nextX].A * offsetX * offsetY;
            }
            // B
            scaleBmp.rawImage[i][j].B = (float)rawImage[biasY][biasX].B * (1-offsetX) * (1-offsetY)
                                        + (float)rawImage[biasY][nextX].B * offsetX * (1-offsetY)
                                        + (float)rawImage[nextY][biasX].B * (1-offsetX) * offsetY
                                        + (float)rawImage[nextY][nextX].B * offsetX * offsetY;
            // G
            scaleBmp.rawImage[i][j].G = (float)rawImage[biasY][biasX].G * (1-offsetX) * (1-offsetY)
                                        + (float)rawImage[biasY][nextX].G * offsetX * (1-offsetY)
                                        + (float)rawImage[nextY][biasX].G * (1-offsetX) * offsetY
                                        + (float)rawImage[nextY][nextX].G * offsetX * offsetY;
            // R
            scaleBmp.rawImage[i][j].R = (float)rawImage[biasY][biasX].R * (1-offsetX) * (1-offsetY)
                                        + (float)rawImage[biasY][nextX].R * offsetX * (1-offsetY)
                                        + (float)rawImage[nextY][biasX].R * (1-offsetX) * offsetY
                                        + (float)rawImage[nextY][nextX].R * offsetX * offsetY;
        }
    }

    return scaleBmp;
}

YCbCr** BmpImage::toYCbCr() const {
    /*------------------------------------------------------------------------
    Return a YCbCr array that convert form RGB raw image
    RGB range
    R: 0~255, G: 0~255, B: 0~255
    YCbCr range
    Y: 16~235, Cb: 16~240, Cr: 16~240
    ------------------------------------------------------------------------*/
    YCbCr** processImage = new YCbCr* [infoHeader.biHeight];
    for (int i=0; i<infoHeader.biHeight; i++) {
        processImage[i] = new YCbCr[infoHeader.biWidth];
        for (int j=0; j<infoHeader.biWidth; j++) {
            int R = rawImage[i][j].R;
            int G = rawImage[i][j].G;
            int B = rawImage[i][j].B;

            processImage[i][j].Y = (0.257 * R) + (0.504 * G) + (0.098 * B) + 16;
            processImage[i][j].Cb = (-0.148 * R) - (0.291 * G) + (0.439 * B) + 128;
            processImage[i][j].Cr = (0.439 * R) - (0.368 * G) - (0.071 * B) + 128;
        }
    }
    return processImage;
} // End of toYCbCr()

BmpImage& BmpImage::readYCbCr(YCbCr** iYCbCr) {
    /*------------------------------------------------------------------------
    Read a YCbCr array and convert to RGB raw image
    RGB range
    R: 0~255, G: 0~255, B: 0~255
    YCbCr range
    Y: 16~235, Cb: 16~240, Cr: 16~240
    ------------------------------------------------------------------------*/
    for (int i=0; i<infoHeader.biHeight; i++) {
        for (int j=0; j<infoHeader.biWidth; j++) {
            double R = 1.164 * (iYCbCr[i][j].Y - 16) + 1.596 * (iYCbCr[i][j].Cr - 128);
            double G = 1.164 * (iYCbCr[i][j].Y - 16) - 0.813 * (iYCbCr[i][j].Cr - 128) - 0.392 * (iYCbCr[i][j].Cb - 128);
            double B = 1.164 * (iYCbCr[i][j].Y - 16) + 2.017 * (iYCbCr[i][j].Cb - 128);

            rawImage[i][j].R = (R > 255) ? 255 : R; // Saturation arithmetic
            rawImage[i][j].G = (G > 255) ? 255 : G; // Saturation arithmetic
            rawImage[i][j].B = (B > 255) ? 255 : B; // Saturation arithmetic
        }
    }
    return *this;
} // End of readYCbCr(YCbCr** iYCbCr)

BmpImage BmpImage::lumiPowLawTransform(double g) const {
    /*------------------------------------------------------------------------
    Power-Law Transformations
    Return a BmpImage class that the raw image is transformed in intensity 
    domain.
    ------------------------------------------------------------------------*/
    YCbCr** processImage = toYCbCr();                       // convert RGB to YCbCr
    double imax=0;

    for (int i=0; i<infoHeader.biHeight; i++) {             // find max intensity(Luma)
        for (int j=0; j<infoHeader.biWidth; j++) {
            int intensity = processImage[i][j].Y;
            imax = (intensity > imax) ? intensity : imax;
        }
    }

    for (int i=0; i<infoHeader.biHeight; i++) {             // power-law transform
        for (int j=0; j<infoHeader.biWidth; j++) {
            int intensity = processImage[i][j].Y;
            processImage[i][j].Y = imax * pow((processImage[i][j].Y / imax), (1.0 / g));
        }
    }

    BmpImage enhanceImage(*this);
    enhanceImage.readYCbCr(processImage);

    for (int i=0; i<infoHeader.biHeight; i++) {             // release YCbCr array
        delete [] processImage[i];
    }

    return enhanceImage;
} // End of lumiPowLawTransform(double)

BmpImage BmpImage::histoEqual() const {
    YCbCr** processImage = toYCbCr();                       // convert RGB to YCbCr
    int imin=255, imax=0;
    double iHis[256];
    for (int i=0; i<256; i++) {
        iHis[i] = 0;
    }

    for (int i=0; i<infoHeader.biHeight; i++) {
        for (int j=0; j<infoHeader.biWidth; j++) {
            int intensity = processImage[i][j].Y;
            iHis[intensity] += 1;                           // count histogram
            imin = (intensity < imin) ? intensity : imin;   // find min intensity(Luma)
            imax = (intensity > imax) ? intensity : imax;   // find max intensity(Luma)
        }
    }

    for (int i=0; i<256; i++) {
        iHis[i] /= (infoHeader.biHeight * infoHeader.biWidth);  // convert histogram to pdf
    }

    double transform[256];
    double k = imax - imin;

    for (int i=0; i<256; i++) {                             // integrate
        transform[i] = 0;
        if (i < imin) continue;
        if (i==0) {
            transform[i] += iHis[i] * k;
        }
        else {
            transform[i] += transform[i-1] + iHis[i] * k;
        }
    }
    for (int i=imin; i<256; i++) {
        transform[i] += imin;
    }

    for (int i=0; i<infoHeader.biHeight; i++) {             // transform
        for (int j=0; j<infoHeader.biWidth; j++) {
            int intensity = processImage[i][j].Y;
            processImage[i][j].Y = transform[intensity];
        }
    }

    BmpImage enhanceImage(*this);
    enhanceImage.readYCbCr(processImage);

    for (int i=0; i<infoHeader.biHeight; i++) {             // release YCbCr array
        delete [] processImage[i];
    }

    return enhanceImage;
} // End of histoEqual()

BmpImage BmpImage::lowLumiEnhanceLocal(int size) const {
    YCbCr** processImage = toYCbCr();

    int countWidth = infoHeader.biWidth / size;
    int countHeight = infoHeader.biHeight / size;
    int offsetW = 0, offsetH = 0;
    if (infoHeader.biWidth % size != 0) {
        countWidth += 1;
        offsetW = infoHeader.biWidth % size;
    }
    
    if (infoHeader.biHeight % size != 0) {
        countHeight += 1;
        offsetH = infoHeader.biHeight % size;
    }

    for (int i=0; i<countHeight; i++) {
        for (int j=0; j<countWidth; j++) {
            int blockSizeH, blockSizeW;
            blockSizeH = (offsetH != 0 && i == countHeight-1) ? offsetH : size;
            blockSizeW = (offsetW != 0 && i == countWidth-1) ? offsetW : size;

            int iminLocal=255, imaxLocal=0;
            double iHisLocal[256];
            for (int i=0; i<256; i++) {
                iHisLocal[i] = 0;
            }

            for (int m=0; m<blockSizeH; m++) {
                for (int n=0; n<blockSizeW; n++) {
                    int intensity = processImage[i*size + m][j*size + n].Y;
                    iHisLocal[intensity] += 1;
                    iminLocal = (intensity < iminLocal) ? intensity : iminLocal;
                    imaxLocal = (intensity > imaxLocal) ? intensity : imaxLocal;
                }
            }
            for (int i=0; i<256; i++) {
                iHisLocal[i] /= (blockSizeH * blockSizeW);
            }

            double transform[256];
            double k = imaxLocal - iminLocal;

            for (int i=0; i<256; i++) {
                transform[i] = 0;
                if (i < iminLocal) continue;
                if (i==0) {
                    transform[i] += iHisLocal[i] * k;
                }
                else {
                    transform[i] += transform[i-1] + iHisLocal[i] * k;
                }
            }
            for (int i=iminLocal; i<256; i++) {
                transform[i] += iminLocal;
            }

            for (int m=0; m<blockSizeH; m++) {
                for (int n=0; n<blockSizeW; n++) {
                    int intensity = processImage[i*size + m][j*size + n].Y;
                    processImage[i*size + m][j*size + n].Y = transform[intensity];
                }
            }
        }
    }

    BmpImage enhanceImage(*this);
    enhanceImage.readYCbCr(processImage);

    for (int i=0; i<infoHeader.biHeight; i++) {
        delete [] processImage[i];
    }

    return enhanceImage;
} // End of lowLumiEnhanceLocal(int size)

BmpImage BmpImage::sharpness(int k) const {
    /*------------------------------------------------------------------------
    Unsharp Masking & High-Boosting Filtering
    Return a BmpImage class that the raw image is sharpness in intensity 
    domain.
    ------------------------------------------------------------------------*/
    YCbCr** processImage = toYCbCr();                   // convert RGB to YCbCr

    double **I = new double* [infoHeader.biHeight];     // new a Y array
    for (int i=0; i<infoHeader.biHeight; i++) {
        I[i] = new double [infoHeader.biWidth];
        for (int j=0; j<infoHeader.biWidth; j++) {
            I[i][j] = processImage[i][j].Y;
        }
    }

    double **I_blur = meanFilter(I, infoHeader.biHeight, infoHeader.biWidth, 15);   // new a I_blur array that conv by a mean filter
    
    for (int i=0; i<infoHeader.biHeight; i++) {         // I_sharp = I + k * mask
        for (int j=0; j<infoHeader.biWidth; j++) {
            double mask = processImage[i][j].Y - I_blur[i][j];
            processImage[i][j].Y += k * mask;
        }
    }

    BmpImage sharpnessImage(*this);
    sharpnessImage.readYCbCr(processImage);

    for (int i=0; i<infoHeader.biHeight; i++) {
        delete [] processImage[i];
        delete [] I[i];
        delete [] I_blur[i];
    }

    return sharpnessImage;
} // End of sharpness(int k)

BmpImage BmpImage::denoise(int size) const {
    /*------------------------------------------------------------------------
    Mean filter smoothing
    Return a BmpImage class that the raw image is smoothing by a mean filter
    in RGB channel
    ------------------------------------------------------------------------*/
    double **R = new double* [infoHeader.biHeight];     // new a R array
    double **G = new double* [infoHeader.biHeight];     // new a G array
    double **B = new double* [infoHeader.biHeight];     // new a B array
    for (int i=0; i<infoHeader.biHeight; i++) {
        R[i] = new double [infoHeader.biWidth];
        G[i] = new double [infoHeader.biWidth];
        B[i] = new double [infoHeader.biWidth];
        for (int j=0; j<infoHeader.biWidth; j++) {
            R[i][j] = rawImage[i][j].R;
            G[i][j] = rawImage[i][j].G;
            B[i][j] = rawImage[i][j].B;
        }
    }
    double **R_mean = meanFilter(R, infoHeader.biHeight, infoHeader.biWidth, size);
    double **G_mean = meanFilter(G, infoHeader.biHeight, infoHeader.biWidth, size);
    double **B_mean = meanFilter(B, infoHeader.biHeight, infoHeader.biWidth, size);

    BmpImage denoiseImage(*this);

    for (int i=0; i<denoiseImage.infoHeader.biHeight; i++) {
        for (int j=0; j<denoiseImage.infoHeader.biWidth; j++) {
            denoiseImage.rawImage[i][j].R = R_mean[i][j];
            denoiseImage.rawImage[i][j].G = G_mean[i][j];
            denoiseImage.rawImage[i][j].B = B_mean[i][j];
        }
    }
    /* Process in intensity domain */
    // YCbCr** processImage = toYCbCr();
    // double **I = new double* [infoHeader.biHeight];
    // for (int i=0; i<infoHeader.biHeight; i++) {
    //     I[i] = new double [infoHeader.biWidth];
    //     for (int j=0; j<infoHeader.biWidth; j++) {
    //         I[i][j] = processImage[i][j].Y;
    //     }
    // }
    // double **I_blur = meanFilter(I, infoHeader.biHeight, infoHeader.biWidth, size);
    // for (int i=0; i<infoHeader.biHeight; i++) {
    //     for (int j=0; j<infoHeader.biWidth; j++) {
    //         processImage[i][j].Y = I_blur[i][j];
    //     }
    // }
    // denoiseImage.readYCbCr(processImage);

    for (int i=0; i<infoHeader.biHeight; i++) {
        delete [] R[i];
        delete [] G[i];
        delete [] B[i];
        delete [] R_mean[i];
        delete [] G_mean[i];
        delete [] B_mean[i];
    }

    return denoiseImage;
} // End of denoise(int size)

double** meanFilter(double **image, int height, int width ,int size) {
    double **filter = new double* [size];           // new a mean filter according to the given size
    for (int i=0; i<size; i++) {
        filter[i] = new double [size];
        for (int j=0; j<size; j++) {
            filter[i][j] = 1.0 / (size * size);
        }
    }

    int padding = (size-1) / 2;
    int idx, idy;
    double **imagePadding = new double* [height + 2*padding];   // padding image let the output can match input
    for (int i=0; i < height + 2*padding; i++) {                // rep padding
        imagePadding[i] = new double [width + 2*padding];
        if (i<padding){
            idx = 0;
        }
        else if (i > height+padding-1) {
            idx = height-1;
        }
        else {
            idx = i - padding;
        }
        for (int j=0; j< width + 2*padding; j++) {
            if (j<padding){
                idy = 0;
            }
            else if (j > width+padding-1) {
                idy = width-1;
            }
            else {
                idy = j - padding;
            }
            imagePadding[i][j] = image[idx][idy];
        }
    }

    double** outImage = conv2D(imagePadding, height+2*padding, width+2*padding, filter, size);

    for (int i=0; i < height + 2*padding; i++) {
        delete [] imagePadding[i];
    }

    for (int i=0; i<size; i++) {
        delete [] filter[i];
    }

    return outImage;
} // End of meanFilter(double **image, int height, int width ,int size)

double** conv2D(double** image, int imageHeight, int imageWidth, double** filter, int kernelSize) {
    int offset = (kernelSize - 1) / 2;
    int outHeight = imageHeight - 2*offset;
    int outWidth = imageWidth - 2*offset;
    double** outImage = new double* [outHeight];
    for (int i=0; i<outHeight; i++) {
        outImage[i] = new double [outWidth];
        for (int j=0; j<outWidth; j++) {
            outImage[i][j] = 0;
            for (int m=0; m<kernelSize; m++) {
                for (int n=0; n<kernelSize; n++) {
                    outImage[i][j] += filter[m][n] * image[m+i][n+j];
                }
            }
        }
    }
    return outImage;
} // End of conv2D(double** image, int imageHeight, int imageWidth, double** filter, int kernelSize)

BmpImage BmpImage::grayWorld(bool avgGrary, bool clip) const {
    double R_avg(0), G_avg(0), B_avg(0), gray(128);
    double R_max(0), G_max(0), B_max(0);
    double kr, kg, kb;
    for (int i=0; i<infoHeader.biHeight; i++) {
        for (int j=0; j<infoHeader.biWidth; j++) {
            B_avg += rawImage[i][j].B;
            G_avg += rawImage[i][j].G;
            R_avg += rawImage[i][j].R;

            if (rawImage[i][j].R > R_max) R_max = rawImage[i][j].R;
            if (rawImage[i][j].G > G_max) G_max = rawImage[i][j].G;
            if (rawImage[i][j].B > B_max) B_max = rawImage[i][j].B;
        }
    }

    R_avg /= (infoHeader.biHeight * infoHeader.biWidth);
    G_avg /= (infoHeader.biHeight * infoHeader.biWidth);
    B_avg /= (infoHeader.biHeight * infoHeader.biWidth);

    if (avgGrary) {
        gray = (R_avg + G_avg + B_avg) / 3.0;
    }

    kr = gray / R_avg;
    kg = gray / G_avg;
    kb = gray / B_avg;

    R_max *= kr;
    G_max *= kg;
    B_max *= kb;

    if (clip) {
        kr *= 255 / R_max;
        kg *= 255 / G_max;
        kb *= 255 / B_max;
    }

    BmpImage outImage(*this);
    for (int i=0; i<outImage.infoHeader.biHeight; i++) {
        for (int j=0; j<outImage.infoHeader.biWidth; j++) {
            outImage.rawImage[i][j].B = (outImage.rawImage[i][j].B * kb > 255) ? 255 : (outImage.rawImage[i][j].B * kb);
            outImage.rawImage[i][j].G = (outImage.rawImage[i][j].G * kg > 255) ? 255 : (outImage.rawImage[i][j].G * kg);
            outImage.rawImage[i][j].R = (outImage.rawImage[i][j].R * kr > 255) ? 255 : (outImage.rawImage[i][j].R * kr);
        }
    }

    return outImage;
} // End of grayWorld(bool avgGrary, bool clip)

BmpImage BmpImage::maxRGB(bool autoGain) const {
    double R_max(0), G_max(0), B_max(0), max(0);
    double kr, kg, kb;
    for (int i=0; i<infoHeader.biHeight; i++) {
        for (int j=0; j<infoHeader.biWidth; j++) {
            if (autoGain) {
                if (rawImage[i][j].R > R_max) R_max = rawImage[i][j].R;
                if (rawImage[i][j].G > G_max) G_max = rawImage[i][j].G;
                if (rawImage[i][j].B > B_max) B_max = rawImage[i][j].B;

                if (R_max > max) max = R_max;
                if (G_max > max) max = G_max;
                if (B_max > max) max = B_max;
            }
        }
    }

    kr = 255 / R_max;
    kg = 255 / G_max;
    kb = 255 / B_max;

    BmpImage outImage(*this);
    for (int i=0; i<outImage.infoHeader.biHeight; i++) {
        for (int j=0; j<outImage.infoHeader.biWidth; j++) {
            outImage.rawImage[i][j].B = (outImage.rawImage[i][j].B * kb > 255) ? 255 : (outImage.rawImage[i][j].B * kb);
            outImage.rawImage[i][j].G = (outImage.rawImage[i][j].G * kg > 255) ? 255 : (outImage.rawImage[i][j].G * kg);
            outImage.rawImage[i][j].R = (outImage.rawImage[i][j].R * kr > 255) ? 255 : (outImage.rawImage[i][j].R * kr);
        }
    }

    return outImage;
}// End of maxRGB(bool autoGain)

void BmpImage::toSpecturm(cv::Mat2f &dftR, cv::Mat2f &dftG, cv::Mat2f &dftB) {
    cv::Mat imgR(int(infoHeader.biHeight), int(infoHeader.biWidth), CV_32FC2);
    cv::Mat imgG(int(infoHeader.biHeight), int(infoHeader.biWidth), CV_32FC2);
    cv::Mat imgB(int(infoHeader.biHeight), int(infoHeader.biWidth), CV_32FC2);
    for (int i=0; i<infoHeader.biHeight; i++) {
        for (int j=0; j<infoHeader.biWidth; j++) {
            // R
            imgR.at<cv::Vec2f>(i, j)[0] = rawImage[i][j].R;
            imgR.at<cv::Vec2f>(i, j)[1] = 0;
            // G
            imgG.at<cv::Vec2f>(i, j)[0] = rawImage[i][j].G;
            imgG.at<cv::Vec2f>(i, j)[1] = 0;
            // B
            imgB.at<cv::Vec2f>(i, j)[0] = rawImage[i][j].B;
            imgB.at<cv::Vec2f>(i, j)[1] = 0;
        }
    }
    cv::dft(imgR, dftR);
    cv::dft(imgG, dftG);
    cv::dft(imgB, dftB);
}

void BmpImage::showColorSpecturm(string imgName) {
    cv::Mat2f dftR, dftG, dftB;
    toSpecturm(dftR, dftG, dftB);
    showSpecturm(dftR, imgName + " Spectrum Magnitude R");
    showSpecturm(dftG, imgName + " Spectrum Magnitude G");
    showSpecturm(dftB, imgName + " Spectrum Magnitude B");
}

vector<vector<float> > BmpImage::filterShiftAndPad(vector<vector<float> > &filterPSF) {
    vector<vector<float> > newPSF(infoHeader.biHeight, vector<float>(infoHeader.biWidth));
    int x = filterPSF.size();
    int y = filterPSF[0].size();
    int cx = x / 2, cy = y / 2;
    //  A | B      D | C
    // ---+--- => ---+---
    //  C | D      B | A
    int shiftx = infoHeader.biHeight - cx;
    int shifty = infoHeader.biWidth - cy;
    // cout << endl << x << " " << y << endl << shiftx << " " << shifty << endl;
    for (int i=0; i<x; i++) {
        for (int j=0; j<y; j++) {
            if (i < cx && j < cy) { // A
                newPSF[shiftx + i][shifty + j] = filterPSF[i][j];
            }
            else if (i < cx) { // C
                newPSF[shiftx + i][j - cy] = filterPSF[i][j];
            }
            else if (j < cy) { // B
                newPSF[i - cx][shifty + j] = filterPSF[i][j];
            }
            else { // D
                newPSF[i - cx][j - cy] = filterPSF[i][j];
            }
        }
    }

    // for (int i=0; i<infoHeader.biHeight; i++) {
    //     for (int j=0; j<infoHeader.biWidth; j++) {
    //         cout << newPSF[i][j] << " ";
    //     }
    //     cout << endl;
    // }

    return newPSF;
}

void BmpImage::getSize(int &height, int &width) const {
    height = infoHeader.biHeight;
    width = infoHeader.biWidth;
}

BmpImage BmpImage::freqOperator(cv::Mat &OTF) {
    cv::Mat2f freqR, freqG, freqB;
    toSpecturm(freqR, freqG, freqB);
    complex<float> complexR, complexG, complexB, complexH;
    for (int i=0; i<infoHeader.biHeight; i++) {
        for (int j=0; j<infoHeader.biWidth; j++) {
            complexH.real(OTF.at<cv::Vec2f>(i, j)[0]);
            complexH.imag(OTF.at<cv::Vec2f>(i, j)[1]);
            complexR.real(freqR.at<cv::Vec2f>(i, j)[0]);
            complexR.imag(freqR.at<cv::Vec2f>(i, j)[1]);
            complexG.real(freqG.at<cv::Vec2f>(i, j)[0]);
            complexG.imag(freqG.at<cv::Vec2f>(i, j)[1]);
            complexB.real(freqB.at<cv::Vec2f>(i, j)[0]);
            complexB.imag(freqB.at<cv::Vec2f>(i, j)[1]);

            complexR = complexR * complexH;
            complexG = complexG * complexH;
            complexB = complexB * complexH;

            freqR.at<cv::Vec2f>(i, j)[0] = complexR.real();
            freqR.at<cv::Vec2f>(i, j)[1] = complexR.imag();
            freqG.at<cv::Vec2f>(i, j)[0] = complexG.real();
            freqG.at<cv::Vec2f>(i, j)[1] = complexG.imag();
            freqB.at<cv::Vec2f>(i, j)[0] = complexB.real();
            freqB.at<cv::Vec2f>(i, j)[1] = complexB.imag();
        }
    }

    // showSpecturm(freqR, "R specturm after filtering");
    // showSpecturm(freqG, "G specturm after filtering");
    // showSpecturm(freqB, "B specturm after filtering");

    idft(freqR, freqR);
    idft(freqG, freqG);
    idft(freqB, freqB);

    // freqR.convertTo(freqR, CV_8U);
    // freqG.convertTo(freqG, CV_8U);
    // freqB.convertTo(freqB, CV_8U);
    cv::normalize(freqR, freqR, 0, 255, cv::NORM_MINMAX);
    cv::normalize(freqG, freqG, 0, 255, cv::NORM_MINMAX);
    cv::normalize(freqB, freqB, 0, 255, cv::NORM_MINMAX);

    BmpImage outImage(*this);
    for (int i=0; i<outImage.infoHeader.biHeight; i++) {
        for (int j=0; j<outImage.infoHeader.biWidth; j++) {
            outImage.rawImage[i][j].B = freqB.at<cv::Vec2f>(i, j)[0];
            outImage.rawImage[i][j].G = freqG.at<cv::Vec2f>(i, j)[0];
            outImage.rawImage[i][j].R = freqR.at<cv::Vec2f>(i, j)[0];
        }
    }
    return outImage;
}

BmpImage BmpImage::deMotionBlur(int motionSize, float snr, int guassSize, float guassSigma) {
    // Motion Blur Kernal
    vector<vector<float> > motionPSF = genMotionPSF(motionSize);
    // Guassian Blurv Kernal
    vector<vector<float> > guassPSF;
    if (guassSize > 0) {
        guassPSF = genGaussPSF(guassSize, guassSigma);
    }
    // PSF to OTF
    cv::Mat2f motionOTF, guassOTF;
    filterToFreq(filterShiftAndPad(motionPSF), motionOTF);
    filterToFreq(filterShiftAndPad(guassPSF), guassOTF);
    #ifdef SPECTURM
    showSpecturm(motionOTF, "Motion Blur Specturm");
    showSpecturm(guassOTF, "Guassian Blur Specturm");
    #endif

    // OTF multiplication
    cv::Mat2f degOTF(int(infoHeader.biHeight), int(infoHeader.biWidth), CV_32FC2);
    complex<float> complexM, complexG, complexD;
    for (int i=0; i<infoHeader.biHeight; i++) {
        for (int j=0; j<infoHeader.biWidth; j++) {
            complexM.real(motionOTF.at<cv::Vec2f>(i, j)[0]);
            complexM.imag(motionOTF.at<cv::Vec2f>(i, j)[1]);
            complexG.real(guassOTF.at<cv::Vec2f>(i, j)[0]);
            complexG.imag(guassOTF.at<cv::Vec2f>(i, j)[1]);

            complexD = complexM * complexG;

            degOTF.at<cv::Vec2f>(i, j)[0] = complexD.real();
            degOTF.at<cv::Vec2f>(i, j)[1] = complexD.imag();
        }
    }
    #ifdef SPECTURM
    showSpecturm(degOTF, "Degradation Model Specturm");
    #endif

    // Wiener Filter
    cv::Mat2f wnrOTF(int(infoHeader.biHeight), int(infoHeader.biWidth), CV_32FC2);
    wienerFilterOTF(degOTF, wnrOTF, snr);
    #ifdef SPECTURM
    showSpecturm(wnrOTF, "Wiener Filter Specturm");
    #endif

    BmpImage img;
    img = freqOperator(wnrOTF);
    return img;
}

// ==========================================
// External Function

void showSpecturm(const cv::Mat2f &complexImag, string title) {
    // 計算傅立葉變換的振幅，並轉換為對數尺度
    cv::Mat dft_result[2];
    cv::Mat magnitude;
    // 分離複數影像的實部和虛部
    cv::split(complexImag, dft_result); 
    // 使用實部和虛部計算大小
    cv::magnitude(dft_result[0], dft_result[1], magnitude);
    // 轉換為對數尺度
    cv::Mat magI = magnitude + cv::Scalar::all(1);
    cv::log(magI, magI);
    // 裁剪頻譜以適應偶數大小
    magI = magI(cv::Rect(0, 0, magI.cols & -2, magI.rows & -2));
    // 重新排列傅立葉變換的象限，使原點位於影像中心
    int cx = magI.cols / 2;
    int cy = magI.rows / 2;
    cv::Mat q0(magI, cv::Rect(0, 0, cx, cy)); // 左上 - 建立象限區域
    cv::Mat q1(magI, cv::Rect(cx, 0, cx, cy)); // 右上
    cv::Mat q2(magI, cv::Rect(0, cy, cx, cy)); // 左下
    cv::Mat q3(magI, cv::Rect(cx, cy, cx, cy)); // 右下
    cv::Mat tmp; // 交換象限（左上和右下）
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);
    q1.copyTo(tmp); // 交換象限（右上和左下）
    q2.copyTo(q1);
    tmp.copyTo(q2);
    // 正規化至 [0, 1]
    normalize(magI, magI, 0, 1, cv::NORM_MINMAX);
    // 如果輸入影像為浮點數，影像會自動乘以 255 並顯示
    cv::imshow(title, magI);
    cv::waitKey();
}

vector<vector<float> > genMotionPSF(int size) {
    vector<vector<float> > psf(size, vector<float>(size));
    for (int i=0; i<size; i++) {
        psf[i][i] = 1.0 / size;
    }
    return psf;
}

vector<vector<float> > genGaussPSF(int size, float sigma) {
    vector<vector<float> > psf(size, vector<float>(size));
    float var = sigma * sigma;
    int center = size / 2;
    int x, y;
    for (int i=0; i<size; i++) {
        x = i - center;
        for (int j=0; j<size; j++) {
            y = j - center;
            psf[i][j] = (exp(-(x * x + y * y) / 2.0 * var) / (2.0 * M_PI * var));
            // cout << psf[i][j] << " ";
        }
        // cout << endl;
    }
    return psf;
}

void filterToFreq(vector<vector<float> > filterPSF, cv::Mat2f &filterOTF) {
    int sizex = filterPSF.size();
    int sizey = filterPSF[0].size();
    cv::Mat OTF(sizex, sizey, CV_32FC2);
    for (int i=0; i<sizex; i++) {
        for (int j=0; j<sizey; j++) {
            OTF.at<cv::Vec2f>(i, j)[0] = filterPSF[i][j];
            OTF.at<cv::Vec2f>(i, j)[1] = 0;
        }
    }
    cv::dft(OTF, filterOTF);
}

vector<vector<float> > filterShiftAndPad(const BmpImage &img, vector<vector<float> > &filterPSF) {
    int height, width;
    img.getSize(height, width);
    vector<vector<float> > newPSF(height, vector<float>(width));
    int x = filterPSF.size();
    int y = filterPSF[0].size();
    int cx = x / 2, cy = y / 2;
    //  A | B      D | C
    // ---+--- => ---+---
    //  C | D      B | A
    int shiftx = height - cx;
    int shifty = width - cy;
    // cout << endl << x << " " << y << endl << shiftx << " " << shifty << endl;
    for (int i=0; i<x; i++) {
        for (int j=0; j<y; j++) {
            if (i < cx && j < cy) { // A
                newPSF[shiftx + i][shifty + j] = filterPSF[i][j];
            }
            else if (i < cx) { // C
                newPSF[shiftx + i][j - cy] = filterPSF[i][j];
            }
            else if (j < cy) { // B
                newPSF[i - cx][shifty + j] = filterPSF[i][j];
            }
            else { // D
                newPSF[i - cx][j - cy] = filterPSF[i][j];
            }
        }
    }

    // for (int i=0; i<height; i++) {
    //     for (int j=0; j<width; j++) {
    //         cout << newPSF[i][j] << " ";
    //     }
    //     cout << endl;
    // }

    return newPSF;
}

void wienerFilterOTF(const BmpImage &img, vector<vector<float> > degPSF, cv::Mat2f &wnrOTF, float snr) {
    vector<vector<float> > degPSF_shifted;
    degPSF_shifted = filterShiftAndPad(img, degPSF);
    cv::Mat2f degOTF;
    filterToFreq(degPSF_shifted, degOTF);
    int height, width;
    img.getSize(height, width);
    complex<float> Huv;
    for (int i=0; i<height; i++) {
        for (int j=0; j<width; j++) {
            Huv.real(degOTF.at<cv::Vec2f>(i, j)[0]);
            Huv.imag(degOTF.at<cv::Vec2f>(i, j)[1]);
            Huv = conj(Huv) / (norm(Huv) + complex<float>{snr, 0});

            wnrOTF.at<cv::Vec2f>(i, j)[0] = Huv.real();
            wnrOTF.at<cv::Vec2f>(i, j)[1] = Huv.imag();
        }
    }
}

void wienerFilterOTF(cv::Mat2f &degOTF, cv::Mat2f &wnrOTF, float snr) {
    int height, width;
    height = degOTF.rows;
    width = degOTF.cols;
    // cout << endl << "height= " << height << " width= " << width << endl;
    complex<float> Huv;
    for (int i=0; i<height; i++) {
        for (int j=0; j<width; j++) {
            Huv.real(degOTF.at<cv::Vec2f>(i, j)[0]);
            Huv.imag(degOTF.at<cv::Vec2f>(i, j)[1]);
            Huv = conj(Huv) / (norm(Huv) + complex<float>{snr, 0});

            wnrOTF.at<cv::Vec2f>(i, j)[0] = Huv.real();
            wnrOTF.at<cv::Vec2f>(i, j)[1] = Huv.imag();
        }
    }
}
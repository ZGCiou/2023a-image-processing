#include "BmpImage.h"
#include <cmath>
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
            if (infoHeader.biBitCount == 32) {
                fin.read((char *)&rawImage[i][j].A, sizeof(uint8_t));
            }
            fin.read((char *)&rawImage[i][j].B, sizeof(uint8_t));
            fin.read((char *)&rawImage[i][j].G, sizeof(uint8_t));
            fin.read((char *)&rawImage[i][j].R, sizeof(uint8_t));
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
            if (infoHeader.biBitCount == 32) {
                fout.write((char *)&rawImage[i][j].A, sizeof(uint8_t));
            }
            fout.write((char *)&rawImage[i][j].B, sizeof(uint8_t));
            fout.write((char *)&rawImage[i][j].G, sizeof(uint8_t));
            fout.write((char *)&rawImage[i][j].R, sizeof(uint8_t));
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
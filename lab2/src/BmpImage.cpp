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
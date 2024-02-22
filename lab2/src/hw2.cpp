#include "BmpImage.h"
using namespace std;

int main(int argc, char **argv) {
    BmpImage img1(argv[1]);
    BmpImage img2(argv[2]);
    BmpImage img3(argv[3]);
    // BmpImage img4("input4.bmp");
    /* Image 1 */
    img1.histoEqual().writeBMP("../img/output1_2.bmp");
    // img1.lumiPowLawTransform(1.5).writeBMP("output1_1.bmp");
    img1.lumiPowLawTransform(2).writeBMP("../img/output1_1.bmp");
    /* Image 2 */
    img2.sharpness(1).writeBMP("../img/output2_1.bmp");
    img2.sharpness(3).writeBMP("../img/output2_2.bmp");
    /* Image 3 */
    img3.denoise(3).writeBMP("../img/output3_1.bmp");
    img3.denoise(9).writeBMP("../img/output3_2.bmp");

    // img4.lumiPowLawTransform(1.5).writeBMP("test1_1.bmp");
    // img4.lumiPowLawTransform(2).writeBMP("test1_2.bmp");
    // img4.sharpness(1).writeBMP("test2_1.bmp");
    // img4.sharpness(3).writeBMP("test2_2.bmp");
    // img4.denoise(3).writeBMP("test3_1.bmp");
    // img4.denoise(9).writeBMP("test3_2.bmp");

    return 0;
}
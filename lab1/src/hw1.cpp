#include "BmpImage.h"
using namespace std;

int main() {
    BmpImage img1("../img/input1.bmp");
    BmpImage img2("../img/input2.bmp");
    // img1.showHeader();
    // img2.showHeader();

    /* Flip */
    img1.flip().writeBMP("../img/output1_flip.bmp");
    img2.flip().writeBMP("../img/output2_flip.bmp");
    /* Resolution */
    img1.resolution(6).writeBMP("../img/output1_1.bmp");
    img1.resolution(4).writeBMP("../img/output1_2.bmp");
    img1.resolution(2).writeBMP("../img/output1_3.bmp");
    img2.resolution(6).writeBMP("../img/output2_1.bmp");
    img2.resolution(4).writeBMP("../img/output2_2.bmp");
    img2.resolution(2).writeBMP("../img/output2_3.bmp");
    /* Scaling */
    img1.scaling50(true).writeBMP("../img/output1_up.bmp");
    img1.scaling50(false).writeBMP("../img/output1_down.bmp");
    img2.scaling50(true).writeBMP("../img/output2_up.bmp");
    img2.scaling50(false).writeBMP("../img/output2_down.bmp");

    return 0;
}
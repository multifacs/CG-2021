#include <QCoreApplication>
#include <string>
#include "filter_logic.h"
#include "dot_filters.h"
#include "kernel_filters.h"
#include "morpho_filters.h"
#include <ctime>

int main(int argc, char* argv[])
{
    srand(time(NULL));

    std::string s;
    QImage img;

    for (int i = 0; i < argc; i++)
    {
        if (!strcmp(argv[i], "-p") && (i + 1 < argc))
        {
            s = argv[i + 1];
        }
    }

    img.load(QString(s.c_str()));
    //img.save("Result.png");

   /* To do at home tasks */

    InvertFilter invert;
    invert.process(img).save("invert.png");
    printf_s("Invert filter done\n");

    BlurFilter blur;
    blur.process(img).save("blur.png");
    printf_s("Blur filter done\n");

    GaussianFilter gaussian;
    gaussian.process(img).save("gaussian.png");
    printf_s("Gaussian filter done\n");

    GrayScaleFilter grayscale;
    grayscale.process(img).save("greyscale.png");
    printf_s("Grey scale filter done\n");

    SepiaFilter sepia;
    sepia.process(img).save("sepia.png");
    printf_s("Sepia filter done\n");

    BrightenFilter brighten;
    brighten.process(img).save("brighten.png");
    printf_s("Brighten filter done\n");

    SharpenFilter sharpen;
    sharpen.process(img).save("sharpen.png");
    printf_s("Sharpen filter done\n");

    SobelFilter sobel;
    sobel.process(img).save("sobel.png");
    printf_s("Sobel filter done\n");

    /* Lab1 */

    GreyWorldFilter greyworld;
    greyworld.process(img).save("greyworld.png");
    printf_s("Grey World filter done\n");

    GlassFilter glass;
    glass.process(img).save("glass.png");
    printf_s("Glass filter done\n");

    LinearCorrectionFilter lin;
    lin.process(img).save("lin.png");
    printf_s("Linear correction filter done\n");

    MotionBlurFilter motion;
    motion.process(img).save("motion.png");
    printf_s("Motion blur filter done\n");

    DilationFilter dilate;
    dilate.process(img).save("dilate.png");
    printf_s("Dilation filter done\n");

    ErosionFilter erose;
    erose.process(img).save("erose.png");
    printf_s("Erosion filter done\n");

    OpeningFilter opening;
    opening.process(img).save("opening.png");
    printf_s("Opening filter done\n");

    ClosingFilter closing;
    closing.process(img).save("closing.png");
    printf_s("Closing filter done\n");

    GradFilter grad;
    grad.process(img).save("grad.png");
    printf_s("Grad filter done\n");

    MorphoKernel mKer;
    mKer[0] = 1;
    mKer[8] = 1;

    ErosionFilter erode(mKer);
    erode.process(img).save("erose2.png");
    printf_s("Erosing filter with a diff kernel done\n");

    MedianFilter median(2);
    median.process(img).save("median.png");
    printf_s("Median filter done\n");

    TopHatFilter top;
    top.process(img).save("tophat.png");
    printf_s("TopHat filter done\n");

    BlackHatFilter black;
    black.process(img).save("blackhat.png");
    printf_s("BlackHat filter done\n");

}

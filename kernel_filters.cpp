#include "kernel_filters.h"

QImage SobelFilter::process(const QImage& img)
{
    QImage result(img);

    SobelXFilter sobelX;
    QImage imgx = sobelX.process(img);

    SobelYFilter sobelY;
    QImage imgy = sobelY.process(img);

    for (int x = 0; x < img.width(); x++)
        for (int y = 0; y < img.height(); y++)
        {
            QColor colorx = imgx.pixelColor(x, y);
            QColor colory = imgy.pixelColor(x, y);

            int returnR = clamp(sqrt(colorx.red() * colorx.red() + colory.red() * colory.red()), 255.0, 0.0);
            int returnG = clamp(sqrt(colorx.green() * colorx.green() + colory.green() * colory.green()), 255.0, 0.0);
            int returnB = clamp(sqrt(colorx.blue() * colorx.blue() + colory.blue() * colory.blue()), 255.0, 0.0);

            QColor color = QColor();

            color.setRgb(returnR, returnG, returnB);

            result.setPixelColor(x, y, color);
        }
    return result;
}



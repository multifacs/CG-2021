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

QColor DilationFilter::calcNewPixelColor(const QImage& img, int x, int y) const
{
    float returnR = 0;
    float returnB = 0;
    float returnG = 0;

    int size = mKernel.getSize();
    int radius = mKernel.getRadius();
    for (int i = -radius; i <= radius; i++)
        for (int j = -radius; j <= radius; j++)
        {
            int idx = (i + radius) * size + j + radius;
            QColor color = img.pixelColor(clamp(x + j, img.width() - 1, 0),
                clamp(y + i, img.height() - 1, 0));

            if (mKernel[idx])
            {
                if (color.red() > returnR)
                    returnR = color.red();
                if (color.green() > returnG)
                    returnG = color.green();
                if (color.blue() > returnB)
                    returnB = color.blue();
            }
            
        }
    return QColor(returnR, returnG, returnB);
}

QColor ErosionFilter::calcNewPixelColor(const QImage& img, int x, int y) const
{
    float returnR = 255;
    float returnB = 255;
    float returnG = 255;

    int size = mKernel.getSize();
    int radius = mKernel.getRadius();
    for (int i = -radius; i <= radius; i++)
        for (int j = -radius; j <= radius; j++)
        {
            int idx = (i + radius) * size + j + radius;
            QColor color = img.pixelColor(clamp(x + j, img.width() - 1, 0),
                clamp(y + i, img.height() - 1, 0));

            if (mKernel[idx])
            {
                if (color.red() < returnR)
                    returnR = color.red();
                if (color.green() < returnG)
                    returnG = color.green();
                if (color.blue() < returnB)
                    returnB = color.blue();
            }

        }
    return QColor(returnR, returnG, returnB);
}

QImage OpeningFilter::process(const QImage& img)
{
    ErosionFilter erode;
    QImage imgA = erode.process(img);

    DilationFilter dilate;
    QImage imgB = dilate.process(imgA);

    return imgB;
}

QImage ClosingFilter::process(const QImage& img)
{
    DilationFilter dilate;
    QImage imgA = dilate.process(img);

    ErosionFilter erode;
    QImage imgB = erode.process(imgA);

    return imgB;
}

QImage GradFilter::process(const QImage& img)
{
    QImage result(img);

    DilationFilter dilate;
    QImage imgA = dilate.process(img);

    ErosionFilter erode;
    QImage imgB = erode.process(img);

    for (int x = 0; x < img.width(); x++)
        for (int y = 0; y < img.height(); y++)
        {
            QColor colorA = imgA.pixelColor(x, y);
            QColor colorB = imgB.pixelColor(x, y);

            int returnR = clamp(colorA.red() - colorB.red(), 255, 0);
            int returnG = clamp(colorA.green() - colorB.green(), 255, 0);
            int returnB = clamp(colorA.blue() - colorB.blue(), 255, 0);

            result.setPixelColor(x, y, QColor(returnR, returnG, returnB));
        }
    return result;
}

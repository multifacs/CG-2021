#include "filter_logic.h"

QImage Filter::process(const QImage& img) const
{
    QImage result(img);

    for (int x = 0; x < img.width(); x++)
        for (int y = 0; y < img.height(); y++)
        {
            QColor color = calcNewPixelColor(img, x, y);
            result.setPixelColor(x, y, color);
        }
    return result;
}

QColor MatrixFilter::calcNewPixelColor(const QImage& img, int x, int y) const
{
    float returnR = 0;
    float returnG = 0;
    float returnB = 0;
    int size = mKernel.getSize();
    int radius = mKernel.getRadius();
    for (int i = -radius; i <= radius; i++)
        for (int j = -radius; j <= radius; j++)
        {
            int idx = (i + radius) * size + j + radius;
            QColor color = img.pixelColor(clamp(x + j, img.width() - 1, 0),
                clamp(y + i, img.height() - 1, 0));
            returnR += color.red() * mKernel[idx];
            returnG += color.green() * mKernel[idx];
            returnB += color.blue() * mKernel[idx];
        }
    return QColor(clamp(returnR, 255.f, 0.f),
        clamp(returnG, 255.f, 0.f),
        clamp(returnB, 255.f, 0.f));
}
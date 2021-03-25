#pragma once
#include "filter_logic.h"
#include <random>
#include <algorithm>

class InvertFilter : public Filter
{
    QColor calcNewPixelColor(const QImage& img, int x, int y) const override;
};

// homework

class GrayScaleFilter : public Filter
{
    QColor calcNewPixelColor(const QImage& img, int x, int y) const override;
};

class SepiaFilter : public Filter
{
    QColor calcNewPixelColor(const QImage& img, int x, int y) const override;
};

class BrightenFilter : public Filter
{
    QColor calcNewPixelColor(const QImage& img, int x, int y) const override;
};

// Lab1

class GreyWorldFilter : public Filter
{
protected:
    int mutable N = 0;

    float mutable AvgR = 0;
    float mutable AvgG = 0;
    float mutable AvgB = 0;

    float mutable Avg = 0;

    QColor calcNewPixelColor(const QImage& img, int x, int y) const override;

public:
    QImage process(const QImage& img) const override;    
};

class GlassFilter : public Filter
{
    QColor calcNewPixelColor(const QImage& img, int x, int y) const override;
};

class LinearCorrectionFilter : public Filter
{
protected:
    int mutable max = 0;
    int mutable min = 255;

    QColor calcNewPixelColor(const QImage& img, int x, int y) const override;

public:
    QImage process(const QImage& img) const override;
};

class MedianFilter : public Filter
{
protected:
    int radius;
    QColor calcNewPixelColor(const QImage& img, int x, int y) const override;
public:
    MedianFilter(int _r) : radius(_r) {}
};

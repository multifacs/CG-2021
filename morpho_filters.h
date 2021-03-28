#pragma once

#include "kernel_filters.h"

class MorphoKernel : public Kernel
{
public:
    using Kernel::Kernel;
    MorphoKernel(std::size_t radius = 1) : Kernel(radius)
    {
        data[0] = 0; data[1] = 1; data[2] = 0;
        data[3] = 1; data[4] = 1; data[5] = 1;
        data[6] = 0; data[7] = 1; data[8] = 0;
    }
};

class DilationFilter : public MatrixFilter
{
protected:
    QColor calcNewPixelColor(const QImage& img, int x, int y) const;
public:
    DilationFilter(const Kernel& kernel) : MatrixFilter(kernel) {}
    DilationFilter(std::size_t radius = 1) : MatrixFilter(MorphoKernel(radius)) {}
};

class ErosionFilter : public MatrixFilter
{
protected:
    QColor calcNewPixelColor(const QImage& img, int x, int y) const;
public:
    ErosionFilter(const Kernel& kernel) : MatrixFilter(kernel) {}
    ErosionFilter(std::size_t radius = 1) : MatrixFilter(MorphoKernel(radius)) {}
};

class OpeningFilter
{
public:
    QImage process(const QImage& img);
};

class ClosingFilter
{
public:
    QImage process(const QImage& img);
};

class GradFilter
{
public:
    QImage process(const QImage& img);
};
class TopHatFilter
{
public:
    QImage process(const QImage& img);
};
class BlackHatFilter
{
public:
    QImage process(const QImage& img);
};

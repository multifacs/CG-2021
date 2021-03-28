#pragma once
#include "filter_logic.h"

class BlurKernel : public Kernel
{
public:
    using Kernel::Kernel;
    BlurKernel(std::size_t radius = 1) : Kernel(radius)
    {
        for (std::size_t i = 0; i < getLen(); i++)
            data[i] = 1.0f / getLen();
    }
};

class BlurFilter : public MatrixFilter
{
public:
    BlurFilter(std::size_t radius = 1) : MatrixFilter(BlurKernel(radius)) {}
};


class GaussianKernel : public Kernel
{
public:
    using Kernel::Kernel;
    GaussianKernel(std::size_t radius = 2, float sigma = 3.f) : Kernel(radius)
    {
        float norm = 0;
        int signed_radius = static_cast<int>(radius);
        for (int x = -signed_radius; x <= signed_radius; x++)
            for (int y = -signed_radius; y <= signed_radius; y++)
            {
                std::size_t idx = (x + radius) * getSize() + (y + radius);
                data[idx] = std::exp(-(x * x + y * y) / (sigma * sigma));
                norm += data[idx];
            }
        for (std::size_t i = 0; i < getLen(); i++)
        {
            data[i] /= norm;
        }
    }
};

class GaussianFilter : public MatrixFilter
{
public:
    GaussianFilter(std::size_t radius = 1) : MatrixFilter(GaussianKernel(radius)) {}
};


// homework

class SharpenKernel : public Kernel
{
public:
    using Kernel::Kernel;
    SharpenKernel(std::size_t radius = 1) : Kernel(radius)
    {
        for (std::size_t i = 0; i < getLen(); i++)
            data[i] = -1;
        data[4] = 9;
    }
};

class SharpenFilter : public MatrixFilter
{
public:
    SharpenFilter(std::size_t radius = 1) : MatrixFilter(SharpenKernel(radius)) {}
};

class SobelXKernel : public Kernel
{
public:
    using Kernel::Kernel;
    SobelXKernel(std::size_t radius = 1) : Kernel(radius)
    {
        data[0] = -1; data[1] = -2; data[2] = -1;
        data[3] = 0; data[4] = 0; data[5] = 0;
        data[6] = 1; data[7] = 2; data[8] = 1;
    }
};

class SobelXFilter : public MatrixFilter
{
public:
    SobelXFilter(std::size_t radius = 1) : MatrixFilter(SobelXKernel(radius)) {}
};

class SobelYKernel : public Kernel
{
public:
    using Kernel::Kernel;
    SobelYKernel(std::size_t radius = 1) : Kernel(radius)
    {
        data[0] = -1; data[3] = -2; data[6] = -1;
        data[1] = 0; data[4] = 0; data[7] = 0;
        data[2] = 1; data[5] = 2; data[8] = 1;
    }
};

class SobelYFilter : public MatrixFilter
{
public:
    SobelYFilter(std::size_t radius = 1) : MatrixFilter(SobelYKernel(radius)) {}
};

class SobelFilter
{
public:
    QImage process(const QImage& img);
};

/* Lab1 */

class MotionBlurKernel : public Kernel
{
public:
    using Kernel::Kernel;
    MotionBlurKernel(std::size_t radius = 3) : Kernel(radius)
    {
        int counter = 0;
        for (std::size_t i = 0; i < getLen(); i++)
        {
            if (int(i) == counter)
            {
                data[i] = 1.0 / getSize();
                counter += getSize() + 1;
            }
            else
                data[i] = 0;
        }
    }
};

class MotionBlurFilter : public MatrixFilter
{
public:
    MotionBlurFilter(std::size_t radius = 3) : MatrixFilter(MotionBlurKernel(radius)) {}
};



#pragma once
#include <QImage>

class Filter
{
protected:
    virtual QColor calcNewPixelColor(const QImage& img, int x, int y) const = 0;
public:
    virtual ~Filter() = default;
    virtual QImage process(const QImage& img) const;
};

template <class T>
T clamp(T value, T max, T min)
{
    if (value > max)
        return max;
    if (value < min)
        return min;
    return value;
}

class Kernel
{
protected:
    std::unique_ptr<float[]> data;
    std::size_t radius;
    std::size_t getLen() const { return getSize() * getSize(); }
public:
    Kernel(std::size_t radius) : radius(radius)
    {
        data = std::make_unique<float[]>(getLen());
    }

    Kernel(const Kernel& other) : Kernel(other.radius)
    {
        std::copy(other.data.get(), other.data.get() + getLen(), data.get());
    }

    std::size_t getRadius() const { return radius; }
    std::size_t getSize() const { return 2 * radius + 1; }
    float operator [] (std::size_t id) const { return data[id]; }
    float& operator [] (std::size_t id) { return data[id]; }
};

class MatrixFilter : public Filter
{
protected:
    Kernel mKernel;
    QColor calcNewPixelColor(const QImage& img, int x, int y) const override;
public:
    MatrixFilter(const Kernel& kernel) : mKernel(kernel) {};
    virtual ~MatrixFilter() = default;
};


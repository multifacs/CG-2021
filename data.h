#pragma once

#include <fstream>
#include <string>
#include <limits>

class Data {
protected:
    int width_, height_, depth_;
    short *data_, min_, max_;
    float x_voxel_size_, y_voxel_size_, z_voxel_size_;
public:
    Data();
    Data(std::string filename);

    void ReadFile(std::string filename);

    short operator[](int i);

    short GetMin();
    short GetMax();

    void SetMin(short value);
    void SetMax(short value);

    int GetWidth();
    int GetHeight();
    int GetDepth();
};

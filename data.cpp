#include "data.h"

Data::Data() : width_(0), height_(0), depth_(0), min_(0), max_(0) {}

Data::Data(std::string filename) {
    std::ifstream input(filename);

    input >> width_ >> height_ >> depth_;
    input >> x_voxel_size_ >> y_voxel_size_ >> z_voxel_size_;

    data_ = new short[width_ * height_ * depth_];
    min_ = SHRT_MAX; max_ = SHRT_MIN;

    for (int i = 0; i < width_ * height_ * depth_; i++) {
        input >> data_[i];
        if (data_[i] < min_) min_ = data_[i];
        if (data_[i] > max_) max_ = data_[i];
    }
}

void Data::ReadFile(std::string filename) {
    std::ifstream input(filename, std::ios::binary);

    //input >> width_ >> height_ >> depth_;
    //input >> x_voxel_size_ >> y_voxel_size_ >> z_voxel_size_;
    input.read((char*)&width_, sizeof(int));
    input.read((char*)&height_, sizeof(int));
    input.read((char*)&depth_, sizeof(int));

    input.read((char*)&x_voxel_size_, sizeof(float));
    input.read((char*)&y_voxel_size_, sizeof(float));
    input.read((char*)&z_voxel_size_, sizeof(float));

    data_ = new short[width_ * height_ * depth_];
    min_ = SHRT_MAX; max_ = SHRT_MIN;

    for (int i = 0; i < width_ * height_ * depth_; i++) {
        //input >> data_[i];
        input.read((char*)(data_ + i), sizeof(short));
        if (data_[i] < min_) min_ = data_[i];
        if (data_[i] > max_) max_ = data_[i];
    }
}

short Data::operator[](int i) {
    return data_[i];
}

short Data::GetMin() {
    return min_;
}

short Data::GetMax() {
    return max_;
}

int Data::GetWidth() {
    return width_;
}

int Data::GetHeight() {
    return height_;
}

int Data::GetDepth() {
    return depth_;
}

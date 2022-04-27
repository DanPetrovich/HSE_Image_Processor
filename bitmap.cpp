#include "bitmap.h"
#include <string>
#include <fstream>
#include <stdexcept>

bool Bitmap::Read(std::ifstream &file_name) {

    file_name.read(reinterpret_cast <char *>(&bmp_header), sizeof(bmp_header));

    if (!CheckBMPHeader(bmp_header)) { return false; }

    file_name.read(reinterpret_cast <char *>(&dib_header), sizeof(dib_header));

    int num_of_skipped_pixel = ((4 - (3 * dib_header.width) % 4) % 4);

    p_storage.SetWidth(dib_header.width);
    p_storage.SetHeight(dib_header.height);
    for (size_t y_coord = 0; y_coord < dib_header.height; y_coord++) {
        for (size_t x_coord = 0; x_coord < dib_header.width; x_coord++) {
            unsigned char color[3];
            file_name.read(reinterpret_cast<char *>(color), 3);
            p_storage.storage_[y_coord * dib_header.width + x_coord].blue = color[0];
            p_storage.storage_[y_coord * dib_header.width + x_coord].green = color[1];
            p_storage.storage_[y_coord * dib_header.width + x_coord].red = color[2];
        }
        file_name.ignore(num_of_skipped_pixel);
    }
    file_name.close();
    return true;
}

bool Bitmap::Read(std::string_view file_name) {

    if (file_name.find("bmp") != std::string::npos) {
        std::string str(file_name);
        // std::ifstream file(str, std::ios_base::in | std::ios_base::binary);
        std::ifstream file;
        file.open(str, std::ios_base::in | std::ios_base::binary);

        if (!file.is_open()) {
            return false;
        }
        return Read(file);
    } else {
        throw std::exception();
    }
    return false;
}

bool Bitmap::CheckBMPHeader(const Bitmap::BMPHeader &header) {
    return (header.signature == 0x4D42);
}

bool Bitmap::Write(std::string_view file_name) {
    std::string str(file_name);
    // std::ifstream file(str, std::ios_base::in | std::ios_base::binary);
    std::ofstream file;
    file.open(str, std::ios_base::out | std::ios_base::binary);

    if (!file.is_open()) {
        return false;
    }

    file.write(reinterpret_cast <char *>(&bmp_header), sizeof(bmp_header));

    file.write(reinterpret_cast <char *>(&dib_header), sizeof(dib_header));

    unsigned char trash[3] = {0, 0, 0};

    size_t add = (4 - (3 * dib_header.width % 4)) % 4;

    for (size_t y_coord = 0; y_coord < dib_header.height; ++y_coord) {
        for (size_t x_coord = 0; x_coord < dib_header.width; ++x_coord) {
            unsigned char r = static_cast<unsigned char>(p_storage.storage_[y_coord * dib_header.width + x_coord].red);
            unsigned char g = static_cast<unsigned char>(p_storage.storage_[y_coord * dib_header.width +
                                                                            x_coord].green);
            unsigned char b = static_cast<unsigned char>(p_storage.storage_[y_coord * dib_header.width + x_coord].blue);
            unsigned char colour[3] = {b, g, r};
            file.write(reinterpret_cast<char *>(colour), 3);

            // file.write(reinterpret_cast <char *>(&p_storage(y_coord, x_coord)), 3);
        }
        file.write(reinterpret_cast <char *>(trash), add);
    }
// p_storage.storage_[y_coord*dib_header.width + x_coord]

    file.close();
    return true;
}


Bitmap::PixelStorage::PixelStorage(size_t width, size_t height, Bitmap::Colour base_colour) : storage_(nullptr),
                                                                                              width(width),
                                                                                              height(height) {
    if ((width == 0) || (height == 0)) {
        width = 0;
        height = 0;
    }
    if (width == 0) {
        return;
    }
    storage_ = new Colour[width * height];
    for (size_t i = 0; i < height; i++) {
        for (size_t j = 0; j < width; j++) {
            (*this)(i, j) = base_colour;
        }
    }
}

Bitmap::Colour &Bitmap::PixelStorage::GetPixel(size_t i, size_t j) {
    if (i >= width) {
        throw std::out_of_range(" i coord not corrected: i > width");
    }
    if (j >= height) {
        throw std::out_of_range(" j coord not corrected: j > height");
    }
    return (*this)(j, i);
}

const Bitmap::Colour &Bitmap::PixelStorage::GetPixel(size_t i, size_t j) const {
    if (i >= width) {
        throw std::out_of_range(" i coord not corrected: i > width");
    }
    if (j >= height) {
        throw std::out_of_range(" j coord not corrected: j > height");
    }
    return (*this)(i, j);
}

Bitmap::PixelStorage::~PixelStorage() {
    if (storage_ != nullptr) {
        delete[] storage_;
    }
}

Bitmap::PixelStorage::PixelStorage(const Bitmap::PixelStorage &other) : width(other.width), height(other.height) {
    if (!other.storage_) {
        storage_ = nullptr;
        return;
    }
    storage_ = new Colour[width * height];
    for (size_t i = 0; i < height * width; ++i) {
        storage_[i] = other.storage_[i];
    }
}

Bitmap::PixelStorage &Bitmap::PixelStorage::operator=(const Bitmap::PixelStorage &other) {
    if (&other == this) {
        return *this;
    }
    Bitmap::PixelStorage ps(other);
    Swap(ps);
    return *this;


}

void Bitmap::PixelStorage::SetPixel(size_t i, size_t j, Colour colour) {
    if (i >= width) {
        throw std::out_of_range(" i coord not corrected: i > width");
    }
    if (j >= height) {
        throw std::out_of_range(" j coord not corrected: j > height");
    }
    storage_[j * width + i].blue = colour.blue;
    storage_[j * width + i].red = colour.red;
    storage_[j * width + i].green = colour.green;
}

#ifndef HSE_PROJECT_02_2022_PHOTOSHOP_BITMAP_H
#define HSE_PROJECT_02_2022_PHOTOSHOP_BITMAP_H

#include <cstdint>
#include <iostream>
#include <string_view>
#include <tuple>
#include <vector>


// Bipmap - последовательность 2ичных данных.
//класс Bitmap
class Bitmap {
public:
    struct Colour {
        double blue;
        double green;
        double red;

        bool operator==(const Colour &other) const {
            return std::tie(blue, green, red) == std::tie(other.blue, other.green, other.red);
        }

        bool operator!=(const Colour &other) const {
            return !(*this == other);
        }

        Colour operator=(const Colour &other) {
            this->blue = other.blue;
            this->red = other.red;
            this->green = other.green;
            return *this;
        }
    };

    // Нельзя изменять размер хранилища. Если делаем crop - создаем новый объект
    class PixelStorage {
    public:
        Colour *storage_;
    public:
        PixelStorage() : storage_(nullptr), width(0), height(0) {}

        PixelStorage(size_t width, size_t height, Colour base_colour = {0, 0, 0});

        Colour &GetPixel(size_t i, size_t j);

        void SetPixel(size_t i, size_t j, Colour colour);


        const Colour &GetPixel(size_t i, size_t j) const;

        Colour &operator()(size_t x, size_t y) {
            return storage_[x * width + y];
        }

        const Colour &operator()(size_t x, size_t y) const {
            return storage_[x * width + y];
        }

        size_t GetWidth() const {
            return width;
        }

        void SetWidth(size_t w) {
            width = w;
            storage_ = new Colour[width * height];
        }

        size_t GetHeight() const {
            return height;
        }

        void SetHeight(size_t h) {
            height = h;
            storage_ = new Colour[width * height];
        }

        PixelStorage(const PixelStorage &other);

        PixelStorage &operator=(const PixelStorage &other);

        ~PixelStorage();


    protected:
        void Swap(PixelStorage &other) {
            // std::swap(*this, other); - бесконечная рекурсия тк внутри swap позавется оператор равенства
            // (который в свою очередь позовет swap)
            std::swap(width, other.width);
            std::swap(height, other.height);
            std::swap(storage_, other.storage_);
        }

    protected:
        size_t width;
        size_t height;
    };

public:
    PixelStorage p_storage;
    struct BMPHeader {
        int16_t signature;
        int32_t size;
        int16_t dammie1;
        int16_t dammie2;
        int32_t offset;

    } __attribute__((packed));
    struct DIBHeader {
        uint32_t size;
        int32_t width;
        int32_t height;
        uint16_t number_of_planes;
        uint16_t bit_per_pixel;
        uint32_t compr_method;
        uint32_t image_size;
        int32_t x_resolution;
        int32_t y_resolution;
        uint32_t number_of_colors;
        uint32_t important_color_number;
    }__attribute__((packed));

    BMPHeader bmp_header;
    DIBHeader dib_header;


    //возвращает true, если переданный header хороший, иначе false
    static bool CheckBMPHeader(const BMPHeader &header);

    bool Read(std::ifstream &file_name);

    bool Read(std::string_view file_name);

    bool Write(std::string_view file_name);

};


#endif //HSE_PROJECT_02_2022_PHOTOSHOP_BITMAP_H

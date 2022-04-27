#include "colour_filter.h"
#include <math.h>

void BlurFilter::Apply(Bitmap &bitmap) {

    for (size_t y_coord = 0; y_coord < int(bitmap.dib_header.height); y_coord++) {
        for (size_t x_coord = 0; x_coord < int(bitmap.dib_header.width); x_coord++) {
            double blue_sum = 0;
            double red_sum = 0;
            double green_sum = 0;
            for (size_t i = std::max(0, int(y_coord - 3 * sigma_));
                 i <= std::min(int(y_coord + 3 * sigma_), int(bitmap.dib_header.height - 1)); ++i) {

                for (size_t j = std::max(0, int(x_coord - 3 * sigma_));
                     j <= std::min(int(x_coord + 3 * sigma_), int(bitmap.dib_header.width - 1)); ++j) {

                    double result = Summ(x_coord, y_coord, j, i);
                    red_sum += bitmap.p_storage.GetPixel(j, i).red * result;
                    blue_sum += bitmap.p_storage.GetPixel(j, i).blue * result;
                    green_sum += bitmap.p_storage.GetPixel(j, i).green * result;
                }
            }
            bitmap.p_storage.SetPixel(x_coord, y_coord,
                                      {blue_sum, green_sum, red_sum});
        }
    }

}


double BlurFilter::Summ(size_t x_0, size_t y_0, size_t x, size_t y) const {
    return (1 / (2 * M_PI * powl(sigma_, 2))) *
           exp(-(powl(std::max(x_0, x) - std::min(x_0, x), 2) + powl(std::max(y_0, y) - std::min(y_0, y), 2)) /
               (2 * powl(sigma_, 2)));
}

void NegativeFilter::Apply(Bitmap &bitmap) {
    // std::cout << "negative_filter apply implementation";
    for (size_t y_coord = 0; y_coord < bitmap.dib_header.height; y_coord++) {
        for (size_t x_coord = 0; x_coord < bitmap.dib_header.width; x_coord++) {
            bitmap.p_storage.storage_[y_coord * bitmap.dib_header.width + x_coord].blue =
                    255 - bitmap.p_storage.GetPixel(x_coord, y_coord).blue;
            bitmap.p_storage.storage_[y_coord * bitmap.dib_header.width + x_coord].green =
                    255 - bitmap.p_storage.GetPixel(x_coord, y_coord).green;
            bitmap.p_storage.storage_[y_coord * bitmap.dib_header.width + x_coord].red =
                    255 - bitmap.p_storage.GetPixel(x_coord, y_coord).red;
        }
    }
}

void GrayscaleFilter::Apply(Bitmap &bitmap) {
    // std::cout << "grayscale_filter apply implementation";
    for (size_t y_coord = 0; y_coord < bitmap.dib_header.height; y_coord++) {
        for (size_t x_coord = 0; x_coord < bitmap.dib_header.width; x_coord++) {
            double r = bitmap.p_storage.GetPixel(x_coord, y_coord).red;
            double g = bitmap.p_storage.GetPixel(x_coord, y_coord).green;
            double b = bitmap.p_storage.GetPixel(x_coord, y_coord).blue;
            bitmap.p_storage.SetPixel(x_coord, y_coord,
                                      {0.299 * r + 0.578 * g + 0.114 * b, 0.299 * r + 0.578 * g + 0.114 * b,
                                       0.299 * r + 0.578 * g + 0.114 * b});
        }
    }

}

void Crystallize::FillCentresVec(int width, int height) {

    for (size_t i = 0; i < amount_; ++i) {
        int coord = rand() % (width * height);
        Point point;
        point.y = int(coord / width);
        point.x = coord - width * point.y;
        centres_.push_back(point);
    }
}

void Crystallize::Apply(Bitmap &bitmap) {
    FillCentresVec(bitmap.dib_header.width, bitmap.dib_header.height);

    Bitmap::PixelStorage storage;
    storage.SetHeight(bitmap.dib_header.height);
    storage.SetWidth(bitmap.dib_header.width);

    for (size_t y_coord = 0; y_coord < bitmap.dib_header.height; y_coord++) {
        for (size_t x_coord = 0; x_coord < bitmap.dib_header.width; x_coord++) {
            storage.SetPixel(x_coord, y_coord, {0, 0, 0});
        }
    }

    for (auto pixel: centres_) {
        storage.SetPixel(pixel.x, pixel.y, bitmap.p_storage.GetPixel(pixel.x, pixel.y));
    }

    Bitmap::Colour colour;
    colour.green = 0;
    colour.blue = 0;
    colour.red = 0;
    for (size_t y_coord = 0; y_coord < bitmap.dib_header.height; y_coord++) {
        for (size_t x_coord = 0; x_coord < bitmap.dib_header.width; x_coord++) {
            if (storage.GetPixel(x_coord, y_coord) == colour) {
                double dist = 100000000; // very big number
                Bitmap::Colour add_colour;
                for (auto pixel: centres_) {
                    double d = Distance(x_coord, y_coord, pixel.x, pixel.y);
                    if (dist > d) {
                        dist = d;
                        add_colour = bitmap.p_storage.GetPixel(pixel.x, pixel.y);
                    }
                }
                storage.SetPixel(x_coord, y_coord, add_colour);
            }
        }
    }

    for (size_t y_coord = 0; y_coord < bitmap.dib_header.height; y_coord++) {
        for (size_t x_coord = 0; x_coord < bitmap.dib_header.width; x_coord++) {
            bitmap.p_storage.SetPixel(x_coord, y_coord, storage.GetPixel(x_coord, y_coord));
        }
    }

}

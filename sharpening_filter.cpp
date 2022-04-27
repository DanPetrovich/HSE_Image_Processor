//
// Created by Danya Petrovich on 03.03.2022.
//

#include "sharpening_filter.h"

void SharpeningFilter::Apply(Bitmap &bitmap) {
    Bitmap::PixelStorage storage;
    storage.SetHeight(bitmap.dib_header.height);
    storage.SetWidth(bitmap.dib_header.width);

    for (size_t y_coord = 0; y_coord < bitmap.dib_header.height; y_coord++) {
        for (size_t x_coord = 0; x_coord < bitmap.dib_header.width; x_coord++) {
            storage.SetPixel(x_coord, y_coord, bitmap.p_storage.GetPixel(x_coord, y_coord));
        }
    }
    for (size_t y_coord = 1; y_coord < bitmap.dib_header.height - 1; ++y_coord) {
        for (size_t x_coord = 1; x_coord < bitmap.dib_header.width - 1; ++x_coord) {
            Bitmap::Colour colour = Matrix(x_coord, y_coord, storage);
            bitmap.p_storage.SetPixel(x_coord, y_coord, colour);
        }
    }

}

Bitmap::Colour SharpeningFilter::Matrix(size_t x, size_t y, Bitmap::PixelStorage &p_storage) {
    Bitmap::Colour colour;
    colour.blue = fmin(fmax(
            -p_storage.GetPixel(x, y - 1).blue - p_storage.GetPixel(x - 1, y).blue +
            5 * p_storage.GetPixel(x, y).blue - p_storage.GetPixel(x + 1, y).blue -
            p_storage.GetPixel(x, y + 1).blue, 0), 255.0);

    colour.green = fmin(fmax(
            -p_storage.GetPixel(x, y - 1).green - p_storage.GetPixel(x - 1, y).green +
            5 * p_storage.GetPixel(x, y).green - p_storage.GetPixel(x + 1, y).green -
            p_storage.GetPixel(x, y + 1).green, 0), 255.0);

    colour.red = fmin(fmax(
            -p_storage.GetPixel(x, y - 1).red - p_storage.GetPixel(x - 1, y).red +
            5 * p_storage.GetPixel(x, y).red - p_storage.GetPixel(x + 1, y).red -
            p_storage.GetPixel(x, y + 1).red, 0), 255.0);

    return colour;
}

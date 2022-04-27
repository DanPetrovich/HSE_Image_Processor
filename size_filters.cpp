#include "size_filters.h"


void CropFilter::Apply(Bitmap &bitmap) {
    Bitmap::PixelStorage storage;
    storage.SetHeight(bitmap.dib_header.height);
    storage.SetWidth(bitmap.dib_header.width);

    for (size_t y_coord = 0; y_coord < bitmap.dib_header.height; y_coord++) {
        for (size_t x_coord = 0; x_coord < bitmap.dib_header.width; x_coord++) {
            storage.SetPixel(x_coord, y_coord, bitmap.p_storage.GetPixel(x_coord, y_coord));
        }
    }

    if (width > bitmap.dib_header.width) {
        width = bitmap.dib_header.width;
        if (height > bitmap.dib_header.height) {
            height = bitmap.dib_header.height;
        }
    } else {
        if (height > bitmap.dib_header.height) {
            height = bitmap.dib_header.height;
        }
    }

    bitmap.p_storage.SetHeight(height);
    bitmap.p_storage.SetWidth(width);
    bitmap.dib_header.height = height;
    bitmap.dib_header.width = width;
    bitmap.bmp_header.size = height * width * 3;

    for (ssize_t y_coord = 0; y_coord < bitmap.dib_header.height ; ++y_coord) {
        for (size_t x_coord = 0; x_coord < bitmap.dib_header.width; ++x_coord) {
            bitmap.p_storage.SetPixel(x_coord, y_coord, storage.GetPixel(x_coord, y_coord));
        }
    }


}

Bitmap::Colour EdgeDetectionFilter::Matrix(size_t x, size_t y, Bitmap::PixelStorage &p_storage) const {
    Bitmap::Colour colour;
    colour.blue =
            -p_storage.GetPixel(x, y - 1).blue - p_storage.GetPixel(x - 1, y).blue +
            4 * p_storage.GetPixel(x, y).blue - p_storage.GetPixel(x + 1, y).blue -
            p_storage.GetPixel(x, y + 1).blue;

    return colour;

}

void EdgeDetectionFilter::Apply(Bitmap &bitmap) {
    for (size_t y_coord = 0; y_coord < bitmap.dib_header.height; y_coord++) {
        for (size_t x_coord = 0; x_coord < bitmap.dib_header.width; x_coord++) {
            double r = bitmap.p_storage.GetPixel(x_coord, y_coord).red;
            double g = bitmap.p_storage.GetPixel(x_coord, y_coord).green;
            double b = bitmap.p_storage.GetPixel(x_coord, y_coord).blue;
            bitmap.p_storage.SetPixel(x_coord, y_coord, {0.299 * r + 0.578 * g + 0.114 * b});
        }
    }
    Bitmap::PixelStorage storage;
    storage.SetHeight(bitmap.dib_header.height);
    storage.SetWidth(bitmap.dib_header.width);

    for (size_t y_coord = 0; y_coord < bitmap.dib_header.height; y_coord++) {
        for (size_t x_coord = 0; x_coord < bitmap.dib_header.width; x_coord++) {
            storage.SetPixel(x_coord, y_coord, bitmap.p_storage.GetPixel(x_coord, y_coord));
        }
    }


    for (size_t y_coord = 1; y_coord < bitmap.dib_header.height - 1; y_coord++) {
        for (size_t x_coord = 1; x_coord < bitmap.dib_header.width - 1; x_coord++) {
            Bitmap::Colour colour = Matrix(x_coord, y_coord, storage);
            if (colour.blue < threshold_) {
                colour = {0, 0, 0};
            } else {
                colour = {255, 255, 255};
            }
            bitmap.p_storage.SetPixel(x_coord, y_coord, colour);
        }
    }
}

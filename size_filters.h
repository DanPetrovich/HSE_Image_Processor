//
// Created by Danya Petrovich on 03.03.2022.
//

#ifndef HSE_PROJECT_02_2022_PHOTOSHOP_SIZE_FILTERS_H
#define HSE_PROJECT_02_2022_PHOTOSHOP_SIZE_FILTERS_H

#include "base_filter.h"
#include "bitmap.h"

class CropFilter : public BaseFilter {
public:
    size_t width;
    size_t height;

    CropFilter(size_t w, size_t h) : width(w), height(h) {}

    void Apply(Bitmap &bitmap) override;

};

class EdgeDetectionFilter : public BaseFilter {
public:
    EdgeDetectionFilter(double threshold): threshold_(threshold){}

private:
    double threshold_;
    void Apply(Bitmap &) override;

    Bitmap::Colour Matrix(size_t x, size_t y, Bitmap::PixelStorage &p_storage) const;
};

#endif //HSE_PROJECT_02_2022_PHOTOSHOP_SIZE_FILTERS_H

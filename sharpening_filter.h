//
// Created by Danya Petrovich on 03.03.2022.
//

#ifndef HSE_PROJECT_02_2022_PHOTOSHOP_SHARPENING_FILTER_H
#define HSE_PROJECT_02_2022_PHOTOSHOP_SHARPENING_FILTER_H

#include "base_filter.h"

class SharpeningFilter : public BaseFilter {
    void Apply(Bitmap &) override;

    Bitmap::Colour Matrix(size_t x, size_t y, Bitmap::PixelStorage &p_storage);
};


#endif //HSE_PROJECT_02_2022_PHOTOSHOP_SHARPENING_FILTER_H

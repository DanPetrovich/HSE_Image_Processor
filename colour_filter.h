#ifndef HSE_PROJECT_02_2022_PHOTOSHOP_COLOR_FILTERS_H
#define HSE_PROJECT_02_2022_PHOTOSHOP_COLOR_FILTERS_H

#include "base_filter.h"
#include "bitmap.h"
#include <vector>

class BlurFilter : public BaseFilter {
    double sigma_;
public:

    explicit BlurFilter(size_t s) : sigma_(s) {}

    void Apply(Bitmap &bitmap) override;

    double Summ (size_t x_0, size_t y_0, size_t x, size_t y) const;

};

class NegativeFilter : public BaseFilter {
public:
    void Apply(Bitmap &bitmap) override;
};

class GrayscaleFilter : public BaseFilter {
public:
    void Apply(Bitmap &bitmap) override;

};

class Crystallize : public BaseFilter{
    struct Point{
        int x;
        int y;
    };
    std::vector<Point> centres_;
    int amount_;
public:
    Crystallize(int count): amount_(count){}

    void FillCentresVec(int width, int height);

    double Distance (double pixel_x, double pixel_y, double center_x, double center_y){
        double x = powl(pixel_x - center_x, 2);
        double y = powl(pixel_y - center_y, 2);
        return sqrt(x + y);
    }

    void Apply (Bitmap &bitmap) override;
};

#endif //HSE_PROJECT_02_2022_PHOTOSHOP_COLOR_FILTERS_H

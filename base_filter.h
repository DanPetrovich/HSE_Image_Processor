//
// Created by Danya Petrovich on 01.03.2022.
//

#include <utility>
#include <vector>
#include <iostream>

#ifndef HSE_PROJECT_02_2022_PHOTOSHOP_BASE_FILTER_H
#define HSE_PROJECT_02_2022_PHOTOSHOP_BASE_FILTER_H

#include "bitmap.h"

// класс шаблон для классов фильтров
class BaseFilter {
public:
    //применяет фильтр к фотке
    virtual void Apply( Bitmap& bitmap) = 0;
};




// Класс хранит вектор из фильтров и их параметров

class FilterPipeline {
public:
    explicit FilterPipeline(std::vector<BaseFilter *> vec) : filters_(std::move(vec)) {};
    FilterPipeline(){}
    std::vector<BaseFilter *> filters_;
    ~FilterPipeline();
};

#endif //HSE_PROJECT_02_2022_PHOTOSHOP_BASE_FILTER_H

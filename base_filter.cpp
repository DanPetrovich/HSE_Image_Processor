#include "base_filter.h"

FilterPipeline::~FilterPipeline() {
    for (auto &filter : filters_){
        delete filter;
    }
}

//
// Created by Danya Petrovich on 01.03.2022.
//

#include <stdexcept>

#include "filter_pipeline_maker.h"
#include "colour_filter.h"
#include "size_filters.h"
#include "sharpening_filter.h"

namespace FilterMakers {
    BaseFilter *MakeBlurFilter(const FilterDiscripter &fd) {
        if (fd.filter_name_ != "-blur") {
            throw std::invalid_argument("invalid filter descriptor passed to MakeBlurFilter");
        }
        size_t sigma;
        if (fd.parameters_.empty()) {
            sigma = 0;
        } else {
            sigma = stoi(std::string(fd.parameters_[0]));
        }
        return new BlurFilter(sigma);
    }

    BaseFilter *MakeCropFilter(const FilterDiscripter &fd) {
        if (fd.filter_name_ != "-crop") {
            throw std::invalid_argument("invalid filter descriptor passed to MakeCropFilter");
        }
        size_t width = 0;
        size_t height = 0;
        if (fd.parameters_.size() == 2){
            width = stoi(std::string(fd.parameters_[0]));
            height = stoi(std::string(fd.parameters_[1]));
        } else if (fd.parameters_.size() == 1){
            width = stoi(std::string(fd.parameters_[0]));
        }

        return new CropFilter(width, height);
    }

    BaseFilter *MakeGrayScaleFilter(const FilterDiscripter &fd) {
        if (fd.filter_name_ != "-gs") {
            throw std::invalid_argument("invalid filter descriptor passed to MakeGrayScaleFilter");
        }
        return new GrayscaleFilter;

    }
    BaseFilter *MakeNegativeFilter(const FilterDiscripter &fd){
        if (fd.filter_name_ != "-neg") {
            throw std::invalid_argument("invalid filter descriptor passed to MakeNegativeFilter");
        }
        return new NegativeFilter;
    }

    BaseFilter *MakeSharpeningFilter(const FilterDiscripter &fd){
        if (fd.filter_name_ != "-sharp") {
            throw std::invalid_argument("invalid filter descriptor passed to MakeSharpeningFilter");
        }
        return new SharpeningFilter;
    }

    BaseFilter *MakeEdgeDetectionFilter(const FilterDiscripter &fd){
        if (fd.filter_name_ != "-edge") {
            throw std::invalid_argument("invalid filter descriptor passed to MakeCropFilter");
        }

        double th = 0;
        if (!fd.parameters_.empty()) {
            th = std::stoi(std::string(fd.parameters_[0]));
        }
        return new EdgeDetectionFilter(th);
    }

    BaseFilter *MakeCrystallizeFilter(const FilterDiscripter &fd){
        if (fd.filter_name_ != "-cryst") {
            throw std::invalid_argument("invalid filter descriptor passed to makeCrystallizeFiletr");
        }
        int count = 10;
        if (!fd.parameters_.empty()){
            count = std::stoi(std::string(fd.parameters_[0]));
        }
        return new Crystallize(count);
    }
}

BaseFilter *FilterPipelineMaker::MakeFilter(const FilterDiscripter &descripter) const {
    FilterMakerPtr f_maker = GetFilterMaker(descripter.filter_name_);
    if (!f_maker) {
        return nullptr;
    }
    return f_maker(descripter);
}

FilterPipelineMaker::FilterMakerPtr FilterPipelineMaker::GetFilterMaker(std::string_view name) const {
    auto ix = filter_creators_.find(name);
    if (ix == filter_creators_.end()) {
        return nullptr;
    }
    return ix->second;
}

FilterPipeline FilterPipelineMaker::MakeFromCmdArgs(const CommandLineParser &commad_line) const {
    FilterPipeline pipeline;
    for (const auto &filter: commad_line.GetVecOfFilters()) {
        pipeline.filters_.push_back(MakeFilter(filter));
    }
    return pipeline;
}

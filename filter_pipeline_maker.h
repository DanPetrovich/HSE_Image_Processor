#pragma once

#include <map>
#include <string_view>

#include "command_line_parser.h"
#include "base_filter.h"


namespace FilterMakers {
    BaseFilter *MakeBlurFilter(const FilterDiscripter &fd);

    BaseFilter *MakeCropFilter(const FilterDiscripter &fd);

    BaseFilter *MakeGrayScaleFilter(const FilterDiscripter &fd);

    BaseFilter *MakeNegativeFilter(const FilterDiscripter &fd);

    BaseFilter *MakeSharpeningFilter(const FilterDiscripter &fd);

    BaseFilter *MakeEdgeDetectionFilter(const FilterDiscripter &fd);

    BaseFilter *MakeCrystallizeFilter(const FilterDiscripter &fd);

}

class FilterPipelineMaker {
public:

    // тип ссылки на функцию, вызывающую создание определенного фильтра
    typedef BaseFilter *(*FilterMakerPtr)(const FilterDiscripter &);

    // тип map с названием фильтра и функцией, которая этот фильтр вызывает
    typedef std::map<std::string_view, FilterMakerPtr> FilterCreators;
public:

    FilterPipelineMaker() {
        AddFilterCreater("-blur", FilterMakers::MakeBlurFilter);
        AddFilterCreater("-crop", FilterMakers::MakeCropFilter);
        AddFilterCreater("-gs", FilterMakers::MakeGrayScaleFilter);
        AddFilterCreater("-neg", FilterMakers::MakeNegativeFilter);
        AddFilterCreater("-sharp", FilterMakers::MakeSharpeningFilter);
        AddFilterCreater("-edge", FilterMakers::MakeEdgeDetectionFilter);
        AddFilterCreater("-cryst", FilterMakers::MakeCrystallizeFilter);
    }

    void AddFilterCreater(std::string_view filter_name, FilterMakerPtr filter_maker) {
        filter_creators_.insert({filter_name, filter_maker});
    }

    //создает объект класса FilterPipeline
    FilterPipeline MakeFromCmdArgs(const CommandLineParser &commad_line) const;

    // берет фнкцию из GetFilterMaker и передает туда аргументы. Возвращает объект BaseFilter (но уже тот нужный фильтр)
    BaseFilter *MakeFilter(const FilterDiscripter &descripter) const;

    // проверяет, что фильтр есть в списке и возвращает подходящую функцию
    FilterMakerPtr GetFilterMaker(std::string_view name) const;


protected:
    FilterCreators filter_creators_;

};

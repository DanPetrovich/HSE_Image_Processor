#pragma once

#include <string_view>
#include <cstddef>
#include <vector>
#include <map>

class FilterDiscripter {
public:
    std::string_view filter_name_;
    std::vector<std::string_view> parameters_;

};


// разбирает командную строку на имена входного и выходного файлов, а так же на имена и параментры фильтров
class CommandLineParser {
public: //public constans
    static const size_t MIN_AGR_NUMBER = 3; //минимальное количество аргументов, при котором пограмма начинает работать
    static const size_t INPUT_FILE_POS = 1; // позиция названия входного файла во входных данных
    static const size_t OUTPUT_FILE_POS = 2; // позиция названия выходного файла во входных данных
public:

    // парсит строку, устанавливает имена входного и выходного файлов и вектор фильтров. Вызывается в main.cpp
    bool Parse(int argc, char *argv[]);

    std::string_view GetInputFileName() const {
        return input_file_name_;
    }

    std::string_view GetOutputFileName() const {
        return output_file_name_;
    }

    struct FilterParams {
        std::string_view filter_name;
        std::vector<int> filter_params;
    };

    // возвращает ветор фильтров с параметрами
    std::vector<FilterDiscripter> GetVecOfFilters() const {
        return filters_;
    }

    // вызывается в Parse. выявляет фильтры и параметры
    std::vector<FilterDiscripter> MakeFilters(int argc, char *argv[]);

protected:
    std::string_view input_file_name_;
    std::string_view output_file_name_;
    std::vector<FilterDiscripter> filters_;

};
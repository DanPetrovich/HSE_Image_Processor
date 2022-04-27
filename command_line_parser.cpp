#include "command_line_parser.h"
#include <iostream>

bool CommandLineParser::Parse(int argc, char **argv) {
    if (argc - 1 < MIN_AGR_NUMBER) {
        std::cout << "Hello! It is Image Processor. Program must get at least 3 parameters. You can choose some filters from this list: " << std::endl <<
                  "BlurFilter: -blur. With parameter sigma" << std::endl <<
                  "CropFilter: -crop. With parameters height and width" << std::endl <<
                  "GrayscaleFilter: -gs. Without parameters" << std::endl <<
                  "NegativeFilter: -neg. Without parameters" << std::endl <<
                  "SharpeningFilter: -sharp. Without parameters" << std::endl <<
                  "EdgeDetectionFilter: -edge. With parameter threshold" << std::endl <<
                  "CrystallizeFilter: -cryst. with parameter amount of crystals";
        return false;
    }
    input_file_name_ = argv[INPUT_FILE_POS];
    output_file_name_ = argv[OUTPUT_FILE_POS];

    filters_ = MakeFilters(argc, argv);
    return true;
}

std::vector<FilterDiscripter> CommandLineParser::MakeFilters(int argc, char *argv[]) {
    std::vector<FilterDiscripter> vec_of_filters;
    size_t pos = OUTPUT_FILE_POS;
    while (pos < argc) {
        if (argv[pos][0] == '-') {
            size_t filter_pos = pos;
            FilterDiscripter new_filter;
            new_filter.filter_name_ = argv[filter_pos];
            pos++;
            while ((pos < argc) && (argv[pos][0] != '-')) {
                new_filter.parameters_.emplace_back(argv[pos]);
                pos++;
            }
            vec_of_filters.push_back(new_filter);
            pos--;
        }
        pos++;
    }
    return vec_of_filters;
}

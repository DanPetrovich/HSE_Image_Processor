cmake_minimum_required(VERSION 3.21)
project(untitled2)

set(CMAKE_CXX_STANDARD 14)

add_executable(untitled2 bitmap.cpp bitmap.h base_filter.cpp base_filter.h colour_filter.cpp colour_filter.h command_line_parser.cpp command_line_parser.h filter_pipeline_maker.cpp filter_pipeline_maker.h sharpening_filter.cpp sharpening_filter.h size_filters.cpp size_filters.h image_processor.cpp)
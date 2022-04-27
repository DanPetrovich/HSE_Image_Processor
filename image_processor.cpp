#include <iostream>
#include <cassert>
#include <cstdint>
#include "command_line_parser.h"
#include "filter_pipeline_maker.h"
#include "bitmap.h"

struct TestStruct {
    uint8_t a1;
    uint16_t a2;
    uint32_t a3;
} __attribute__((packed)); //

void TestComArg1(int argc, char *argv[]) {
    CommandLineParser parser;
    assert(parser.Parse(argc, argv));

//    std::cout << argc << std::endl;
//    for (auto i = 0; i < argc; i++) {
//        std::cout << argv[i] << std::endl;
//    }
    assert(parser.GetInputFileName() == "input.bmp");
    assert(parser.GetOutputFileName() == "output.bmp");
}

void Test2() {
    FilterPipelineMaker fpm;
    fpm.AddFilterCreater("Blur", &FilterMakers::MakeBlurFilter);
    fpm.AddFilterCreater("Crop", &FilterMakers::MakeCropFilter);
}

//Пусть есть filterdescripter, который мы извлекли из командной строки. Реализуем процесс преобразования filter descripter в сам filter
void Test3() {
    FilterPipelineMaker fpm;
    fpm.AddFilterCreater("Blur", &FilterMakers::MakeBlurFilter);
    fpm.AddFilterCreater("Crop", &FilterMakers::MakeCropFilter);

    FilterDiscripter fd = {"Blur", {"abc", "42"}};
    BaseFilter *filter = fpm.MakeFilter(fd);
    assert(filter);
    // delete filter;

}

void Test4() {
    TestStruct s = {0x55, 0xAAAA, 0x55555555};
    int a = 0;
}

void Test5() {
    std::string_view TEST_FILE_NAME = "/Users/danyapetrovich/CLionProjects/HSE_Project_02.2022_Photoshop/example.bmp";
    Bitmap bmp;
    bmp.Read(TEST_FILE_NAME);
}

void Test6() {
    Bitmap::PixelStorage ps;
    assert(ps.GetHeight() == 0);
    assert(ps.GetWidth() == 0);
    bool try_flag = false;
    try {
        ps.GetPixel(1, 5);
    }
    catch (std::out_of_range &) {
        try_flag = true;
    }
    assert(try_flag);

    Bitmap::PixelStorage ps2(5, 4, {0, 0, 250});
    const Bitmap::Colour RED = {0, 0, 250};
    assert(ps2.GetPixel(2, 3) == RED);
}

void Test7() {
    const Bitmap::Colour RED = {0, 0, 250};
    Bitmap::PixelStorage ps(10, 10, RED);
    Bitmap::PixelStorage ps2(ps);
    assert(ps2.GetPixel(2, 3) == ps.GetPixel(2, 3));
    const Bitmap::Colour BLUE = {250, 0, 0};
    Bitmap::PixelStorage ps3(11, 5, BLUE);
    ps3 = ps;
    assert(ps3.GetPixel(2, 7) == ps.GetPixel(2, 7));
}

int main(int argc, char *argv[]) {
    CommandLineParser parser;
    if (!parser.Parse(argc, argv)) {
        return 1;
    }

    Bitmap bitmap;
    try {
        bitmap.Read(parser.GetInputFileName());
    } catch (std::exception) {
        std::cout << "bad file was given. File must be in bmp format" << std::endl;
        return 1;
    }
    FilterPipelineMaker maker;
    FilterPipeline pipeline = maker.MakeFromCmdArgs(parser);
    try {
        for (auto filter: pipeline.filters_) {
            filter->Apply(bitmap);
        }
    } catch (std::invalid_argument()) {
        std::cout << "You've made mistake in filter name";
        return 1;
    }
    bitmap.Write(parser.GetOutputFileName());
    return 0;
}
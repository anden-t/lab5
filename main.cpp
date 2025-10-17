#define cimg_use_jpeg
#define cimg_OS 1

#include <iostream> 
#include <cmath>
#include "cimg/CImg.h"

using namespace std;
using namespace cimg_library;

int main() {

    CImg<unsigned char> image1;
    image1.load("../test_images/3.jpg");

    CImg<unsigned char> image2;
    image2.load("../test_images/4.jpg");

    ulong average[3] =  {0, 0, 0};
    ulong c = image1.height() * image1.width();

    for (size_t i = 0; i < 3; i++)
        for (size_t y = 0; y < image1.height(); y++)
            for (size_t x = 0; x < image1.width(); x++)
                average[i] += image1(x, y, 0, i);
            
    for (size_t i = 0; i < 3; i++)
        average[i] /= c;
    
    size_t min_index = 0;

    for (size_t i = 0; i < 3; i++)
        if(average[i] < average[min_index])
            min_index = i;
    
    double k[3];

    for (size_t i = 0; i < 3; i++)
        k[i] = static_cast<double>(average[i]) / static_cast<double>(average[min_index]);
    
    cout << "r " << average[0] << " g " << average[1] << " b " << average[2] << "\n";
    cout << "r " << k[0] << " g " << k[1] << " b " << k[2] << "\n";
    
    for (size_t y = 0; y < image1.height(); y++)
    {
        for (size_t x = 0; x < image1.width(); x++)
        {
            double r = static_cast<double>(image1(x, y, 0, 0)) * k[0];
            double g = static_cast<double>(image1(x, y, 0, 1)) * k[1];
            double b = static_cast<double>(image1(x, y, 0, 2)) * k[2];
            unsigned char color[] = { 
                static_cast<unsigned char>(r > 255 ? 255.0 : r),
                static_cast<unsigned char>(g > 255 ? 255.0 : g),
                static_cast<unsigned char>(b > 255 ? 255.0 : b)
            };
            image1.draw_point(x, y, color);
        }
    }

    
    image1.save_pnm_p3("output_white_balance.ppm");
    image1.display("White balance");
    
    int x0 = image2.width() / 2 - image1.width() / 2;
    int y0 = image2.height() / 2 - image1.height() / 2;

    for (size_t y = 0; y < image1.height(); y++)
    {
        for (size_t x = 0; x < image1.width(); x++)
        {
            double r = 1 - (1 - static_cast<double>(image2(x + x0, y + y0, 0, 0)) / 255.0) * (1 - static_cast<double>(image1(x, y, 0, 0)) / 255.0);
            double g = 1 - (1 - static_cast<double>(image2(x + x0, y + y0, 0, 1)) / 255.0) * (1 - static_cast<double>(image1(x, y, 0, 1)) / 255.0);
            double b = 1 - (1 - static_cast<double>(image2(x + x0, y + y0, 0, 2)) / 255.0) * (1 - static_cast<double>(image1(x, y, 0, 2)) / 255.0);

            unsigned char color[] = { 
                static_cast<unsigned char>(r * 255.0),
                static_cast<unsigned char>(g * 255.0),
                static_cast<unsigned char>(b * 255.0)
            };
            image2.draw_point(x + x0, y + y0, color);
        }
    }

    image2.save_pnm_p3("output_overlay.ppm");
    image2.display("Image overlay");
    
    return 0;    
}
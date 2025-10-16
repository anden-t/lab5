#define cimg_use_jpeg
#define cimg_OS 1

#include <iostream> 
#include <cmath>
#include "cimg/CImg.h"

using namespace std;
using namespace cimg_library;

void circleEquation(CImg<unsigned char> & image, int xc, int yc, int radius, const unsigned char* color)
{
    int x = 0;
    int y = 0;

    double radius_sqr = pow(radius, 2);

    for (; x <= radius_sqr; x++)
    {
        y = sqrt(radius_sqr - pow(x, 2));
        image.draw_point(x + xc, y + yc, color);
        image.draw_point(xc - x, y + yc, color);
        image.draw_point(xc - x, yc - y, color);
        image.draw_point(x + xc, yc - y, color);
    }
}

void circleParamEquation(CImg<unsigned char> & image, int xc, int yc, int radius, const unsigned char* color)
{
    double t_delta = 1.0 / radius;

    for (double t = 0; t < M_PI_2; t += t_delta)
    {
        int x = radius * cos(t);
        int y = radius * sin(t);
        image.draw_point(x + xc, y + yc, color);
        image.draw_point(xc - x, y + yc, color);
        image.draw_point(xc - x, yc - y, color);
        image.draw_point(x + xc, yc - y, color);
    }
}

void circleBresenhem(CImg<unsigned char>& image, int xc, int yc, int radius, const unsigned char* color)
{
    int x = 0;
    int y = radius;
    int delta = 2 - 2 * radius;

    while (x <= y) {
        image.draw_point(xc + x, yc + y, color);
        image.draw_point(xc - x, yc + y, color);
        image.draw_point(xc + x, yc - y, color);
        image.draw_point(xc - x, yc - y, color);
        image.draw_point(xc + y, yc + x, color);
        image.draw_point(xc - y, yc + x, color);
        image.draw_point(xc + y, yc - x, color);
        image.draw_point(xc - y, yc - x, color);

        int d1 = 2 * (delta + y) - 1;
        int d2 = 2 * (delta - x) - 1;

        if (delta < 0 && d1 <= 0)
            delta += 2 * ++x + 1;
        else if (delta > 0 && d2 > 0)
            delta -= 2 * --y - 1;
        else
            delta += 2 * (++x - --y);
    }
}

void circleCimg(CImg<unsigned char>& image, int xc, int yc, int r, const unsigned char* color)
{
    image.draw_circle(xc, yc, r, color, 1.0f, UINT_MAX);
}

int main() {

    CImg<unsigned char> original_image;
    original_image.load("../test_images/2.jpeg");

    CImg<unsigned char> paste_image;
    paste_image.load("../test_images/1.jpg");

    ulong average[3] =  {0, 0, 0};
    ulong c = original_image.height() * original_image.width();

    for (size_t i = 0; i < 3; i++)
        for (size_t y = 0; y < original_image.height(); y++)
            for (size_t x = 0; x < original_image.width(); x++)
                average[i] += original_image(x, y, 0, i);
            
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
    
    for (size_t y = 0; y < original_image.height(); y++)
    {
        for (size_t x = 0; x < original_image.width(); x++)
        {
            double r = static_cast<double>(original_image(x, y, 0, 0)) * k[0];
            double g = static_cast<double>(original_image(x, y, 0, 1)) * k[1];
            double b = static_cast<double>(original_image(x, y, 0, 2)) * k[2];
            unsigned char color[] = { 
                static_cast<unsigned char>(r > 255 ? 255.0 : r),
                static_cast<unsigned char>(g > 255 ? 255.0 : g),
                static_cast<unsigned char>(b > 255 ? 255.0 : b)
            };
            //original_image.draw_point(x, y, color);
        }
    }

    
    original_image.save_pnm_p3("output_white_balance.ppm");
    original_image.display("White balance");
    
    int x0 = original_image.width() / 2 - paste_image.width() / 2;
    int y0 = original_image.height() / 2 - paste_image.height() / 2;

    for (size_t y = 0; y < paste_image.height(); y++)
    {
        for (size_t x = 0; x < paste_image.width(); x++)
        {
            double r = 1 - (1 - static_cast<double>(original_image(x + x0, y + y0, 0, 0)) / 255.0) * (1 - static_cast<double>(paste_image(x, y, 0, 0)) / 255.0);
            double g = 1 - (1 - static_cast<double>(original_image(x + x0, y + y0, 0, 1)) / 255.0) * (1 - static_cast<double>(paste_image(x, y, 0, 1)) / 255.0);
            double b = 1 - (1 - static_cast<double>(original_image(x + x0, y + y0, 0, 2)) / 255.0) * (1 - static_cast<double>(paste_image(x, y, 0, 2)) / 255.0);

            unsigned char color[] = { 
                static_cast<unsigned char>(r * 255.0),
                static_cast<unsigned char>(g * 255.0),
                static_cast<unsigned char>(b * 255.0)
            };
            original_image.draw_point(x + x0, y + y0, color);
        }
    }

    original_image.save_pnm_p3("output_overlay.ppm");
    original_image.display("Image overlay");
    
    return 0;    
}
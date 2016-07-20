//
//  main.cpp
//  Image
//
//  Created by Shuto on 2016/07/18.
//  Copyright © 2016年 Shuto. All rights reserved.
//

#include <array>
#include <iostream>
#include <memory>

#include <image/BitmapImage.hpp>
#include <type/NormalizedColor.hpp>

int main(int argc, const char * argv[])
{
    uint32_t r;
    fj::BitmapImage image(480, 640);
    
    for (int i = 0; i < image.getWidth(); i++)
    {
        for (int j = 0; j < image.getHeight(); j++)
        {
            r = r  * 69069 + 255;
            const double kColor = static_cast<double>(r % 255) / 255;
            image.setAt(i, j, fj::NormalizedColor(kColor, 0, 0));
        }
    }
    
    image.saveToFile("test.bmp");
    
    return EXIT_SUCCESS;
}

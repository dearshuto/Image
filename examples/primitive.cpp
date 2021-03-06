//
//  primitive.cpp
//  Image
//
//  Created by Shuto on 2016/07/21.
//  Copyright © 2016年 Shuto. All rights reserved.
//

#include <iostream>

#include <image/BitmapImage.hpp>
#include <image/JpegImage.hpp>
#include <type/NormalizedColor.hpp>

int main(int argc, const char * argv[])
{
    constexpr unsigned int kRadius = 20;
    fj::BitmapImage image(640, 480);
    fj::JpegImage jpeg;
    jpeg.initialize(640, 480);
    
    image.drawCircle(image.getWidth() / 0.6, image.getHeight() / 2.0, kRadius, fj::NormalizedColor::RED);
    image.drawCircle(image.getWidth() / 2.0, image.getHeight() / 2.0, kRadius, fj::NormalizedColor::WHITE);
    image.drawCircle(image.getWidth() / 3.0, image.getHeight() / 2.0, kRadius, fj::NormalizedColor::BLUE);
    image.drawCircle(image.getWidth() / 6.0, image.getHeight() / 2.0, kRadius, fj::NormalizedColor::GREEN);

    jpeg.drawCircle(image.getWidth() / 0.6, image.getHeight() / 2.0, kRadius, fj::NormalizedColor::RED);
    jpeg.drawCircle(image.getWidth() / 2.0, image.getHeight() / 2.0, kRadius, fj::NormalizedColor::WHITE);
    jpeg.drawCircle(image.getWidth() / 3.0, image.getHeight() / 2.0, kRadius, fj::NormalizedColor::BLUE);
    jpeg.drawCircle(image.getWidth() / 6.0, image.getHeight() / 2.0, kRadius, fj::NormalizedColor::GREEN);

    
    image.saveToFile("primitive.bmp");
    jpeg.saveToFile("primitive.jpg");
    
    return EXIT_SUCCESS;
}

//
//  Image.cpp
//  RayTracing
//
//  Created by Shuto Shikama on 2015/08/23.
//  Copyright (c) 2015年 Shuto Shikama. All rights reserved.
//

#include <type/NormalizedColor.hpp>
#include <image/Image.h>

bool fj::Image::loadFromFile(std::string &&filename)
{
    const std::string& filename_ref = filename;
    
    return loadFromFile(filename_ref);
}

bool fj::Image::saveToFile(std::string &&filename)const
{
    const std::string& filename_ref = filename;
    
    return saveToFile(filename_ref);
}

void fj::Image::drawCircle(const unsigned int x, const unsigned int y, const int radius, const fj::NormalizedColor& color)
{
    const unsigned int kLeft = x - radius;
    const unsigned int kUp = y - radius;
    
    for (int x = kLeft; x < radius*2; x++){
        for (int y = kUp; y < kUp; y++) {
            setAt(x, y, color);
        }
    }
}

bool fj::Image::isActiveAt(const int x, const int y)const
{
    if ((0 <= x) && (x < getWidth()) && (0 <= y) && (y < getHeight()) )
    {
        return true;
    }
    
    return false;
}

void fj::Image::setAt(const int x, const int y, const int R, const int G, const int B, const int A)
{
    setAt(x, y, fj::NormalizedColor(R, G, B, A));
}
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

void fj::Image::setAt(const int x, const int y, const int R, const int G, const int B, const int A)
{
    setAt(x, y, fj::NormalizedColor(R, G, B, A));
}
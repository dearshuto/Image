//
//  Image.cpp
//  RayTracing
//
//  Created by Shuto Shikama on 2015/08/23.
//  Copyright (c) 2015年 Shuto Shikama. All rights reserved.
//

#include <cmath>

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

void fj::Image::fillIn(const fj::NormalizedColor &color)
{
    
    for (int i = 0; i < getWidth(); i++){
        for (int j = 0; j < getHeight(); j++)
        {
            setAt(i, j, color);
        }
    }

}

void fj::Image::drawCircle(const unsigned int x, const unsigned int y, const int radius, const fj::NormalizedColor& color)
{
// 中心が(x, y)で一辺が2*radiusの正方形の内部にあるピクセルを走査し、円の内部であればcolorで塗る
    
    for (int i = -radius; i < radius; i++){
        for (int j = -radius; j < radius; j++) {
            const int kX = x + i;
            const int kY = y + j;
            
            if (isActiveAt(kX, kY))
            {
                const unsigned int kDistance = std::sqrt( std::pow(i, 2) + std::pow(j, 2));
                if (kDistance < radius)
                {
                    setAt(kX, kY, color);
                }
            }
            
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
    if (m_coord == CoordY::kDown)
    {
        setAt(x, getHeight() - y, fj::NormalizedColor(R, G, B, A));
    }
    else
    {
        setAt(x, y, fj::NormalizedColor(R, G, B, A));
    }
}
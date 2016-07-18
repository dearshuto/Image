//
//  NormalizedColor.cpp
//  FUJI-Ray
//
//  Created by Shuto Shikama on 2016/02/27.
//
//

#include <algorithm>
#include <cmath>
#include <unordered_map>

#include <type/NormalizedColor.hpp>

fj::NormalizedColor fj::NormalizedColor::WHITE = fj::NormalizedColor(1, 1, 1, 1);
fj::NormalizedColor fj::NormalizedColor::BLACK = fj::NormalizedColor(0, 0, 0, 1);
fj::NormalizedColor fj::NormalizedColor::RED = fj::NormalizedColor(1, 0, 0, 1);
fj::NormalizedColor fj::NormalizedColor::GREEN = fj::NormalizedColor(0, 1, 0, 1);
fj::NormalizedColor fj::NormalizedColor::BLUE = fj::NormalizedColor(0, 0, 1, 1);

void fj::NormalizedColor::clamp(const int index)
{
    fj::Scalar& value = this->operator[](index);
    
    // under clamp
    value = std::max<fj::Scalar>(fj::Scalar(0), value);
    
    //over clamp
    value = std::min<fj::Scalar>(value, fj::Scalar(1));
}

void fj::NormalizedColor::clamped()
{
    clamp(0);//R
    clamp(1);//G
    clamp(2);//B
    clamp(3);//A
}

bool fj::NormalizedColor::isZero()const
{
    if (r() != fj::Scalar(0))
    {
        return false;
    }
    if (g() != fj::Scalar(0))
    {
        return false;
    }
    if (b() != fj::Scalar(0))
    {
        return false;
    }
    if (a() != fj::Scalar(0))
    {
        return false;
    }
    
    return true;
}
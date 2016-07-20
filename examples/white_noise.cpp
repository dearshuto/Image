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
// 線形代入法を利用して擬似乱数を発生させる
// 線形代入法を実現するためにはmod計算が途中で必要になるが、32bitのint型のオーバーフローを利用することでmod計算としている。
    
    uint32_t r = 0;
    fj::BitmapImage image(640, 480);
    
    for (int i = 0; i < image.getWidth(); i++)
    {
        for (int j = 0; j < image.getHeight(); j++)
        {
            r = r  * 69069 + 255;
            const double kColor = static_cast<double>(r % 255) / 255;
            image.setAt(i, j, fj::NormalizedColor(kColor, kColor, kColor));
        }
    }
    
    image.saveToFile("white_noise.bmp");
    
    return EXIT_SUCCESS;
}

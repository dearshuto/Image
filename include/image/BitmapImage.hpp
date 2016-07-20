//
//  BitmapImage.hpp
//  OpenFJ
//
//  Created by Shuto on 2015/11/21.
//
//

#ifndef BitmapImage_hpp
#define BitmapImage_hpp

#include <bitset>
#include <functional>
#include <memory>
#include <vector>
#include "Image.h"


namespace fj {
    class NormalizedColor;
    class BitmapImage;
}

// BitmaoImage
class fj::BitmapImage : public fj::Image
{
    class BitmapFileHeader;
    class BitmapColorData;
    class BitmapInfoHeader;
    
    typedef fj::BitmapImage::BitmapFileHeader FileHeader;
    typedef fj::BitmapImage::BitmapInfoHeader InfoHeader;
    typedef fj::BitmapImage::BitmapColorData ColorData;
    typedef std::vector<uint8_t> ColorContainer;
public:
    
    BitmapImage() = default;
    ~BitmapImage() = default;
    
    BitmapImage(const BitmapImage& other) = delete;

    BitmapImage(const int width, const int height);
    
    void setAt(const int x, const int y, const fj::NormalizedColor& color)override;
    fj::NormalizedColor getAt(const unsigned int x, const unsigned int y)const override;
    
    bool loadFromFile(const std::string& filename) override;
    bool saveToFile(const std::string& filename)const override;
    int getWidth()const override;
    int getHeight()const override;
    
    int convert2DPositionToIntOffset(const int x, const int y)const
    {
        constexpr int kChannelNum = 3;
        return (x + getWidth() * y) * kChannelNum;
    }
protected:
    bool allocateDatamMemory(const ColorContainer& buffer);
    
public:
    const FileHeader& getFileHeader()const
    {
        return std::cref(*m_fileHeader);
    }
    
    const InfoHeader& getInfoHeader()const
    {
        return std::cref(*m_infoHeader);
    }
    
    const ColorData& getColorData()const
    {
        return std::cref(*m_colorData);
    }
    
private:
    // 概念的には以下のメンバ変数のメモリ管理はunique_ptrを使用したほうがいい
    // しかしunique_ptrを利用するためにはクラスの前方宣言だけではなくクラスの完全な宣言が必要である
    // ヘッダの記述を減らしたい, かつスマートポインタを使いたいということでshared_ptrに落ち着く
    
    std::shared_ptr<FileHeader> m_fileHeader;
    std::shared_ptr<InfoHeader> m_infoHeader;
    std::shared_ptr<ColorData> m_colorData;
};

#endif /* BitmapImage_hpp */
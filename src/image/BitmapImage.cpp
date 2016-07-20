//
//  BitmapImage.cpp
//  OpenFJ
//
//  Created by Shuto on 2015/11/21.
//
//

#include <cassert>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <vector>

#include <type/NormalizedColor.hpp>
#include <image/BitmapImage.hpp>

namespace  {
    constexpr int R = 0;
    constexpr int G = 1;
    constexpr int B = 2;

    constexpr double TO_255_RATE = 255.0;
    constexpr double TO_NORMALIZE = 1 / TO_255_RATE;

    constexpr int kRGB = 3;
    constexpr int k8bitRGB = 24;
    constexpr int kNoCompression = 0;

    typedef uint16_t FJWORD;
    typedef uint32_t FJDWORD;

    char* data(FJWORD* data)
    {
        return (char*)data + sizeof(*data);
    }

    char* data(FJDWORD* data)
    {
        return (char*)data + sizeof(*data);
    }

}

//---------------------------------------
class fj::BitmapImage::BitmapFileHeader
{
public:
    BitmapFileHeader() = delete;
    ~BitmapFileHeader() = default;

    BitmapFileHeader(const int width, const int height);

    bool setData(const ColorContainer& buffer);
    void output(std::ofstream* stream)const;

public:

    /**
    * Bitmap画像フォーマットで定義されているファイルヘッダのサイズ
    */
    static constexpr int SIZE = 14;

    /**
    * Bitmap画像フォーマットで定義されている予約領域
    */
    static constexpr int RESERVED_SIZE = 0;
private:
    bool isBitmapData(unsigned short type)const;

    /**
    * BMかWindowsかどちらかのBitmap画像タイプ.
    *
    */
    uint16_t m_type;

    /**
    * 全データを合計したときのファイルサイズ
    */
    uint32_t m_size;

    uint16_t m_reserved1;
    uint16_t m_reserved2;
    uint32_t m_offset;
};


//---------------------------------------//
class fj::BitmapImage::BitmapInfoHeader
{
public:
    BitmapInfoHeader() = delete;
    ~BitmapInfoHeader() = default;

    BitmapInfoHeader(const uint32_t width, const uint32_t height);

    bool setData(const ColorContainer& buffer);
    void output(std::ofstream* stream)const;

    static const int OFFSET = 40;//NOTE: Windows data

private:
    unsigned int m_headerSize;
    uint32_t m_width;
    uint32_t m_height;
    uint16_t m_planes;
    uint16_t m_bitCount;
    uint32_t m_compression;
    uint32_t m_sizeImage;
    uint32_t m_xPixPerMeter;
    uint32_t m_yPixPerMeter;
    uint32_t m_cirUsed;
    uint32_t m_cirImportant;
public:
    uint32_t getWidth()const
    { return m_width; }

    uint32_t getHeight()const
    { return m_height; }
};




class fj::BitmapImage::BitmapColorData
{
public:
    BitmapColorData() = delete;
    ~BitmapColorData() = default;

    BitmapColorData(const int width, const int height);

    bool setData(const ColorContainer& buffer, const int width, const int height);
    void output(std::ofstream* stream)const;

    unsigned char* getRAt(const int x, const int y)const;

    fj::NormalizedColor getColorAt(const int offset)const;
    void setColorAt(const int offset, const fj::NormalizedColor& color);

    std::string getBinaryData()const;

public:

    const ColorContainer& getColorContainer()const
    {
        return m_colorContainer;
    }

    int getWidth()const
    {
        return m_width;
    }

    int getHeight()const
    {
        return m_height;
    }

protected:
    ColorContainer* getColorContainerPtr()
    {
        return &m_colorContainer;
    }

private:
    ColorContainer m_colorContainer;
    int m_width;
    int m_height;
};



/**
 * 内部クラスの宣言をcppファイルでおこなっているので, ヘッダファイルではインスタンス化できない.
 * やむなくコンストラクタの実装をcppファイルに書く
 */
fj::BitmapImage::BitmapImage(const int width, const int height)
{
    m_fileHeader.reset(new BitmapFileHeader(width, height));
    m_infoHeader.reset(new BitmapInfoHeader(width, height));
    m_colorData.reset(new BitmapColorData(width, height));
}

bool fj::BitmapImage::loadFromFile(const std::string &filename)
{
    assert(filename != "");

    std::ifstream file(filename);
    ColorContainer buffer;

    if (!file) {
        std::cout << filename << " is not found" << std::endl;
        return false;
    }

    file.seekg(0,std::ios::end);
    std::streampos length = file.tellg();
    file.seekg(0,std::ios::beg);

    buffer.resize(length);
    file.read( reinterpret_cast<char*>( &(buffer.front()) ),length);

    if (!allocateDatamMemory(buffer)){
        std::cout << "cannot load data" << std::endl;
        return false;
    }

    return true;
}

bool fj::BitmapImage::allocateDatamMemory(const ColorContainer &buffer)
{
    if (!m_fileHeader->setData(buffer)){
        std::cout << "header data is invalid" << std::endl;
        return false;
    }

    if (!m_infoHeader->setData(buffer)){
        std::cout << "file header data is invalid" << std::endl;
        return false;
    }

    if (!m_colorData->setData(buffer, getInfoHeader().getWidth(), getInfoHeader().getHeight())){
        return false;
    }

    return true;
}

bool fj::BitmapImage::saveToFile(const std::string &filename)const
{
    const std::ios_base::openmode kOpenMode = std::ios_base::out | std::ios_base::trunc | std::ios_base::binary;
    std::ofstream stream(filename, kOpenMode);

    if (stream.fail()) {
        return false;
    }

    getFileHeader().output(&stream);
    getInfoHeader().output(&stream);
    getColorData().output(&stream);

    stream.close();

    return true;
}

void fj::BitmapImage::setAt(const int x, const int y, const fj::NormalizedColor &color)
{
    const int kDataOffset = convert2DPositionToIntOffset(x, y);

    m_colorData->setColorAt(kDataOffset, color);
}

fj::NormalizedColor fj::BitmapImage::getAt(const unsigned int x, const unsigned int y)const
{
    const int kDataOffset = convert2DPositionToIntOffset(x, y);

    return getColorData().getColorAt(kDataOffset);
}

int fj::BitmapImage::getWidth()const
{
    return getColorData().getWidth();
}

int fj::BitmapImage::getHeight()const
{
    return getColorData().getHeight();
}

// ---------------------------------------------------
fj::BitmapImage::BitmapFileHeader::BitmapFileHeader(const int width, const int height)
{
    m_type = 19778;//BM as char
    m_size = BitmapFileHeader::SIZE + BitmapInfoHeader::OFFSET + (width * height * kRGB);
    m_reserved1 = BitmapFileHeader::RESERVED_SIZE;
    m_reserved2 = BitmapFileHeader::RESERVED_SIZE;
    m_offset = BitmapFileHeader::SIZE + BitmapInfoHeader::OFFSET;
}

bool fj::BitmapImage::BitmapFileHeader::isBitmapData(unsigned short type)const
{
    char* B = (char*)(&type);
    char* M = B + sizeof(*B);

    if (*B == 'B' && *M == 'M') {
        return true;
    }

    return false;
}

bool fj::BitmapImage::BitmapFileHeader::setData(const ColorContainer &buffer)
{
    FJWORD* word = (FJWORD*)(&buffer[0]);
    FJDWORD* dword;

    if (! isBitmapData(*word)) {
        std::cout << "this header data is not bitmap" << std::endl;
        return false;
    }

    m_type = *word;

    dword = (FJDWORD*)data(word);
    m_size = *dword;

    word = (FJWORD*)data(dword);
    m_reserved1 = *word;

    word = (FJWORD*)data(word);
    m_reserved2 = *word;

    dword = (FJDWORD*)data(word);
    m_offset = *dword;

    return true;
}

void fj::BitmapImage::BitmapFileHeader::output(std::ofstream *stream)const
{
    // writeの関数の仕様上, char型ポインタに変換しないといけない
    stream->write(reinterpret_cast<const char*>(&m_type), sizeof(m_type));
    stream->write(reinterpret_cast<const char*>(&m_size), sizeof(m_size));
    stream->write(reinterpret_cast<const char*>(&m_reserved1), sizeof(m_reserved1));
    stream->write(reinterpret_cast<const char*>(&m_reserved2), sizeof(m_reserved2));
    stream->write(reinterpret_cast<const char*>(&m_offset), sizeof(m_offset));
}

// ---------------------------------------------------
fj::BitmapImage::BitmapInfoHeader::BitmapInfoHeader(const uint32_t width, const uint32_t height)
{
    // NOTE: Windowsタイプしかサポートしてません！

    m_headerSize = 40;//Windows
    m_width = width;
    m_height = height;
    m_planes = 1;
    m_bitCount = k8bitRGB;
    m_compression = kNoCompression;
    m_sizeImage = width * height * kRGB;
    m_xPixPerMeter = 0;//maybe
    m_yPixPerMeter = 0;//maybe
    m_cirUsed = 0;
    m_cirImportant = 0;
}

bool fj::BitmapImage::BitmapInfoHeader::setData(const ColorContainer& buffer)
{
    FJWORD* word = nullptr;
    FJDWORD* dword  = (FJDWORD*)( (char*)&buffer[0] + fj::BitmapImage::BitmapFileHeader::SIZE);

    if (*dword != 40) {
        std::cout << "this file is invalid: it is not Windows bitmap file" << std::endl;
    }

    m_headerSize = *dword;

    dword = (FJDWORD*)data(dword);
    m_width = *dword;

    dword = (FJDWORD*)data(dword);
    m_height = *dword;

    word = (FJWORD*)data(dword);
    m_planes = *word;

    word = (FJWORD*)data(word);
    m_bitCount = *word;

    dword = (FJDWORD*)data(word);
    m_compression = *dword;

    dword = (FJDWORD*)data(dword);
    m_sizeImage = *dword;

    dword = (FJDWORD*)data(dword);
    m_xPixPerMeter = *dword;

    dword = (FJDWORD*)data(dword);
    m_yPixPerMeter = *dword;

    dword = (FJDWORD*)data(dword);
    m_cirUsed = *dword;

    dword = (FJDWORD*)data(dword);
    m_cirImportant = *dword;

    return true;
}

void fj::BitmapImage::BitmapInfoHeader::output(std::ofstream *stream)const
{
    stream->write(reinterpret_cast<const char*>(&m_headerSize), sizeof(m_headerSize));
    stream->write(reinterpret_cast<const char*>(&m_width), sizeof(m_width));
    stream->write(reinterpret_cast<const char*>(&m_height), sizeof(m_height));
    stream->write(reinterpret_cast<const char*>(&m_planes), sizeof(m_planes));
    stream->write(reinterpret_cast<const char*>(&m_bitCount), sizeof(m_bitCount));
    stream->write(reinterpret_cast<const char*>(&m_compression), sizeof(m_compression));
    stream->write(reinterpret_cast<const char*>(&m_sizeImage), sizeof(m_sizeImage));
    stream->write(reinterpret_cast<const char*>(&m_xPixPerMeter), sizeof(m_xPixPerMeter));
    stream->write(reinterpret_cast<const char*>(&m_yPixPerMeter), sizeof(m_yPixPerMeter));
    stream->write(reinterpret_cast<const char*>(&m_cirUsed), sizeof(m_cirUsed));
    stream->write(reinterpret_cast<const char*>(&m_cirImportant), sizeof(m_cirImportant));
}

// ---------------------------------------------------
fj::BitmapImage::BitmapColorData::BitmapColorData(const int width, const int height): m_width(width), m_height(height)
{
     getColorContainerPtr()->resize(width * height * kRGB);
}

fj::NormalizedColor fj::BitmapImage::BitmapColorData::getColorAt(const int offset)const
{
    const ColorContainer& kColorData = getColorContainer();
    const double kR = kColorData[offset    ] * TO_NORMALIZE;
    const double kG = kColorData[offset + 1] * TO_NORMALIZE;
    const double kB = kColorData[offset + 2] * TO_NORMALIZE;

    return fj::NormalizedColor(kR, kG, kB);
}

void fj::BitmapImage::BitmapColorData::setColorAt(const int offset, const fj::NormalizedColor &color)
{
    // コードが煩雑にならないように、あえて参照に変換する
    ColorContainer& colorData = *getColorContainerPtr();

    colorData[offset] = char(color[B] * TO_255_RATE);
    colorData[offset + 1] = char(color[G] * TO_255_RATE);
    colorData[offset + 2] = char(color[R] * TO_255_RATE);

}

bool fj::BitmapImage::BitmapColorData::setData(const ColorContainer &buffer, const int width, const int height)
{
    const int kDataSize = width * height * kRGB;
    ColorContainer& colorContainer = *getColorContainerPtr();
    
    m_width = width;
    m_height = height;

    colorContainer.resize(kDataSize);

    char* color = (char*)&buffer[0] + fj::BitmapImage::BitmapFileHeader::SIZE + fj::BitmapImage::BitmapInfoHeader::OFFSET;

    for (auto& data : colorContainer)
    {
        data = *color;
        color++;
    }

    return true;
}

void fj::BitmapImage::BitmapColorData::output(std::ofstream *stream)const
{

    for (const auto& colorData : getColorContainer())
    {
        stream->write(reinterpret_cast<const char*>(&colorData), sizeof(colorData));
    }

}

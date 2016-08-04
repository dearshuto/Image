//
//  Image.h
//  RayTracing
//
//  Created by Shuto Shikama on 2015/08/23.
//  Copyright (c) 2015年 Shuto Shikama. All rights reserved.
//

#ifndef __RayTracing__Image__
#define __RayTracing__Image__

#include <string>

namespace fj {
    class NormalizedColor;
    class Image;
}

/**
 * 画像をあらわすスーパークラス
 */
class fj::Image
{
public:
    enum class CoordY
    {
        kUp,
        kDown
    };
public:
    Image()
    : m_coord(CoordY::kDown)
    {
        
    }
    
    virtual ~Image() = default;
    
    Image(const fj::Image& other) = delete;
    Image& operator=(const fj::Image& other) = delete;
    
#ifndef _WIN32
    Image(fj::Image&& other) = default;
    Image& operator&&(fj::Image&& image) = delete;
#endif
    
    /**
     * ファイルシステムのパスを指定して画像を読み込む
     */
    virtual bool loadFromFile(const std::string& filename) = 0;
    
    /**
     * ファイルシステムのパスを指定して画像を読み込む. "filename"形式対応用.
     */
    bool loadFromFile(std::string&& filename);
    
    /**
     * ファイルシステムのパスを指定して画像を保存する.
     */
    virtual bool saveToFile(const std::string& filename)const = 0;
    
    /**
     * ファイルシステムのパスを指定して画像を保存する. "filename"形式対応用.
     */
    bool saveToFile(std::string&& filename)const;
    
    /**
     * すべてのピクセルを同じ色で塗りつぶす
     */
    void fillIn(const fj::NormalizedColor& color);
    
    void drawCircle(const unsigned int x, const unsigned int y, const int radius, const fj::NormalizedColor& color);
    
    bool isActiveAt(const int x, const int y)const;
    
// getters & setters
public:
    
    /**
     * 画像の横幅を取得する
     */
    virtual int getWidth()const = 0;
    
    /**
     * 画像の縦幅を取得する
     */
    virtual int getHeight()const = 0;
    
    /**
     * 画像の(x, y)の位置にRGBをセットする
     */
    void setAt(const int x, const int y, const int R, const int G, const int B, const int A) ;
    
    /**
     * 画像の(x, y)の位置の色情報をfj::NormalizedColorとして取得する
     */
    virtual fj::NormalizedColor getAt(const unsigned int x, const unsigned int y)const = 0;
    
    /**
     * 画像の(x, y)の位置にfj::NormalizedColorをセットする
     */
    virtual void setAt(const int x, const int y, const fj::NormalizedColor& color) = 0;
    
    /**
     * yアップかyダウンか選択する
     */
    void setCoordY(const fj::Image::CoordY& coord)
    {
        m_coord = coord;
    }

private:
    fj::Image::CoordY m_coord;
};

#endif /* defined(__RayTracing__Image__) */

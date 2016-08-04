//
//  JpegImage.hpp
//  Image
//
//  Created by Shuto on 2016/08/04.
//
//

#ifndef JpegImage_hpp
#define JpegImage_hpp

#include <vector>
#include "image/Image.h"
#include "type/NormalizedColor.hpp"

namespace fj {
    class JpegImage;
}

class fj::JpegImage : public fj::Image
{
public:
    JpegImage() = default;
    ~JpegImage() = default;
    
    void initialize(const unsigned int width, const unsigned int height);
    
    /**
     * ファイルシステムのパスを指定して画像を読み込む
     */
    virtual bool loadFromFile(const std::string& filename) override;
        
    /**
     * ファイルシステムのパスを指定して画像を保存する.
     */
    virtual bool saveToFile(const std::string& filename)const override;
    
    /**
     * 画像の横幅を取得する
     */
    virtual int getWidth()const override;
    
    /**
     * 画像の縦幅を取得する
     */
    virtual int getHeight()const override;
    
    /**
     * 画像の(x, y)の位置の色情報をfj::NormalizedColorとして取得する
     */
    virtual fj::NormalizedColor getAt(const unsigned int x, const unsigned int y)const override;
    
    /**
     * 画像の(x, y)の位置にfj::NormalizedColorをセットする
     */
    virtual void setAt(const int x, const int y, const fj::NormalizedColor& color) override;
    
private:
    unsigned int convertPosition2D(const unsigned int x, const unsigned int y)const;
private:
    unsigned int m_width;
    
    unsigned int m_height;
    
    std::vector<fj::NormalizedColor> m_colorBuffer;
};

#endif /* JpegImage_hpp */

//
//  JpegImage.cpp
//  Image
//
//  Created by Shuto on 2016/08/04.
//
//

#include <iostream>
#include <jpeglib.h>
#include "image/JpegImage.hpp"

void fj::JpegImage::initialize(const unsigned int width, const unsigned int height)
{
    m_width = width;
    m_height = height;
    m_colorBuffer.resize(width * height);
}

bool fj::JpegImage::loadFromFile(const std::string &filename)
{
    return false;
}

bool fj::JpegImage::saveToFile(const std::string &string)const
{
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;
    
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);
    
    /* 出力ファイルの設定 */
    const std::string filename = string;
    FILE *fp = fopen(filename.c_str(), "wb");
    if (fp == NULL) {
        fprintf(stderr, "cannot open %s\n", filename.c_str());
        return false;
    }
    jpeg_stdio_dest(&cinfo, fp);
    
    cinfo.image_width = m_width;
    cinfo.image_height = m_height;
    cinfo.input_components = 3;
    cinfo.in_color_space = JCS_RGB;
    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, 75, TRUE);
    
    /* 圧縮開始 */
    jpeg_start_compress(&cinfo, TRUE);
    
    JSAMPARRAY img = (JSAMPARRAY) malloc(sizeof(JSAMPROW) * m_height);
    for (int i = 0; i < m_height; i++) {
        img[i] = (JSAMPROW) malloc(sizeof(JSAMPLE) * 3 * m_width);
        for (int j = 0; j < m_width; j++) {
            const auto kColor = getAt(j, i);
            
            //TODO: たんじゅんな255倍だと誤差が出る
            img[i][j*3 + 0] = kColor.r() * 255;
            img[i][j*3 + 1] = kColor.g() * 255;
            img[i][j*3 + 2] = kColor.b() * 255;
        }
    }
    
    /* 書き込む */
    jpeg_write_scanlines(&cinfo, img, m_height);
    
    /* 圧縮終了 */
    jpeg_finish_compress(&cinfo);
    
    jpeg_destroy_compress(&cinfo);
    
    for (int i = 0; i < m_height; i++) {
        free(img[i]);
    }
    
    free(img);
    fclose(fp);
    
    return true;
}

unsigned int fj::JpegImage::convertPosition2D(const unsigned int x, const unsigned int y)const
{
    return y * m_width + x;
}

fj::NormalizedColor fj::JpegImage::getAt(unsigned int x, unsigned int y)const
{
    const unsigned int kIndex = convertPosition2D(x, y);
    return m_colorBuffer[kIndex];
}

void fj::JpegImage::setAt(const int x, const int y, const fj::NormalizedColor &color)
{
    const unsigned int kIndex = convertPosition2D(x, y);
    m_colorBuffer[kIndex] = color;
}

int fj::JpegImage::getWidth()const
{
    return m_width;
}

int fj::JpegImage::getHeight()const
{
    return m_height;
}
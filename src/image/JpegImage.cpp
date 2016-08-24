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

struct my_error_mgr {
    struct jpeg_error_mgr pub;	/* "public" fields */
    
//    jmp_buf setjmp_buffer;	/* for return to caller */
};


void fj::JpegImage::initialize(const unsigned int width, const unsigned int height)
{
    m_width = width;
    m_height = height;
    m_colorBuffer.resize(width * height);
}

bool fj::JpegImage::loadFromFile(const std::string &filename)
{
    /* This struct contains the JPEG decompression parameters and pointers to
     * working space (which is allocated as needed by the JPEG library).
     */
    struct jpeg_decompress_struct cinfo;
    /* We use our private extension JPEG error handler.
     * Note that this struct must live as long as the main JPEG parameter
     * struct, to avoid dangling-pointer problems.
     */
    struct my_error_mgr jerr;
    /* More stuff */
    FILE * infile;		/* source file */
    JSAMPARRAY buffer;		/* Output row buffer */
    int row_stride;		/* physical row width in output buffer */
    
    /* In this example we want to open the input file before doing anything else,
     * so that the setjmp() error recovery below can assume the file is open.
     * VERY IMPORTANT: use "b" option to fopen() if you are on a machine that
     * requires it in order to read binary files.
     */
    
    if ((infile = fopen(filename.c_str(), "rb")) == NULL) {
        return false;
    }
    
    /* Step 1: allocate and initialize JPEG decompression object */
    
    /* We set up the normal JPEG error routines, then override error_exit. */
    cinfo.err = jpeg_std_error(&jerr.pub);
//    jerr.pub.error_exit = my_error_exit;
//    /* Establish the setjmp return context for my_error_exit to use. */
//    if (setjmp(jerr.setjmp_buffer)) {
//        /* If we get here, the JPEG code has signaled an error.
//         * We need to clean up the JPEG object, close the input file, and return.
//         */
//        jpeg_destroy_decompress(&cinfo);
//        fclose(infile);
//        return 0;
//    }
    /* Now we can initialize the JPEG decompression object. */
    jpeg_create_decompress(&cinfo);
    
    /* Step 2: specify data source (eg, a file) */
    
    jpeg_stdio_src(&cinfo, infile);
    
    /* Step 3: read file parameters with jpeg_read_header() */
    
    (void) jpeg_read_header(&cinfo, TRUE);
    /* We can ignore the return value from jpeg_read_header since
     *   (a) suspension is not possible with the stdio data source, and
     *   (b) we passed TRUE to reject a tables-only JPEG file as an error.
     * See libjpeg.txt for more info.
     */
    
    /* Step 4: set parameters for decompression */
    
    /* In this example, we don't need to change any of the defaults set by
     * jpeg_read_header(), so we do nothing here.
     */
    
    /* Step 5: Start decompressor */
    
    (void) jpeg_start_decompress(&cinfo);
    /* We can ignore the return value since suspension is not possible
     * with the stdio data source.
     */
    
    /* We may need to do some setup of our own at this point before reading
     * the data.  After jpeg_start_decompress() we have the correct scaled
     * output image dimensions available, as well as the output colormap
     * if we asked for color quantization.
     * In this example, we need to make an output work buffer of the right size.
     */
    /* JSAMPLEs per row in output buffer */
    row_stride = cinfo.output_width * cinfo.output_components;
    /* Make a one-row-high sample array that will go away when done with image */
    buffer = (*cinfo.mem->alloc_sarray)
    ((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);
    
    initialize(cinfo.output_width, cinfo.output_height);
    
    /* Step 6: while (scan lines remain to be read) */
    /*           jpeg_read_scanlines(...); */
    
    /* Here we use the library's state variable cinfo.output_scanline as the
     * loop counter, so that we don't have to keep track ourselves.
     */
    initialize(cinfo.output_width, cinfo.output_height);
    for (int y = 0; y < cinfo.output_height; y++) {
        
        // 1行だけ読み込ませる
        jpeg_read_scanlines(&cinfo, buffer, 1);
        for (int x = 0; x < cinfo.output_width; x++) {
            fj::NormalizedColor color;
            
            // 1行しか読み込んでないのでbufferのyは常に0
            color.r() = double(buffer[0][x*3 + 0]) / 255.0;
            color.g() = double(buffer[0][x*3 + 1]) / 255.0;
            color.b() = double(buffer[0][x*3 + 2]) / 255.0;
            color.a() = 1.0;
            
            setAt(x, y, color);
        }
    }
    
    /* Step 7: Finish decompression */
    
    (void) jpeg_finish_decompress(&cinfo);
    /* We can ignore the return value since suspension is not possible
     * with the stdio data source.
     */
    
    /* Step 8: Release JPEG decompression object */
    
    /* This is an important step since it will release a good deal of memory. */
    jpeg_destroy_decompress(&cinfo);
    
    /* After finish_decompress, we can close the input file.
     * Here we postpone it until after no more JPEG errors are possible,
     * so as to simplify the setjmp error logic above.  (Actually, I don't
     * think that jpeg_destroy can do an error exit, but why assume anything...)
     */
    fclose(infile);
    
    /* At this point you may want to check to see whether any corrupt-data
     * warnings occurred (test whether jerr.pub.num_warnings is nonzero).
     */
    
    /* And we're done! */
    return 1;
    
    return true;
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
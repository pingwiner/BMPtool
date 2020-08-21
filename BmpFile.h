//
// Created by ilya on 30.09.2019.
//

#ifndef DEMO_BMPFILE_H
#define DEMO_BMPFILE_H

#include <zconf.h>
#include <string>

typedef struct __attribute__ ((packed)) tagBITMAPFILEHEADER {
    u_int16_t   bfType;
    u_int32_t   bfSize;
    u_int16_t   bfReserved1;
    u_int16_t   bfReserved2;
    u_int32_t   bfOffBits;
} BitmapFileHeader, *PBitmapFileHeader;

typedef struct __attribute__ ((packed)) tagBITMAPINFOHEADER
{
    u_int32_t   biSize;
    u_int32_t   biWidth;
    u_int32_t   biHeight;
    u_int16_t   biPlanes;
    u_int16_t   biBitCount;
    u_int32_t   biCompression;
    u_int32_t   biSizeImage;
    u_int32_t   biXPelsPerMeter;
    u_int32_t   biYPelsPerMeter;
    u_int32_t   biClrUsed;
    u_int32_t   biClrImportant;
} BitmapInfoHeader, *PBitmapInfoHeader;

class BmpFile {

public:
    BmpFile();
    ~BmpFile();
    void open(const std::string& filename);
    void save(const std::string& filename);
    void saveRaw(const std::string& filename);
    u_int8_t* data;
    u_int8_t* palette;
    BitmapFileHeader fileHeader;
    BitmapInfoHeader infoHeader;
};


#endif //DEMO_BMPFILE_H

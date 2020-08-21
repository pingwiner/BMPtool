//
// Created by ilya on 30.09.2019.
//

#include <fstream>
#include "string.h"
#include "BmpFile.h"

using namespace std;

BmpFile::BmpFile() {
    memset(&fileHeader, 0, sizeof(fileHeader));
    memset(&infoHeader, 0, sizeof(infoHeader));
    data = 0;
    palette = 0;
}

BmpFile::~BmpFile() {
    if (data) {
        delete[] data;
    }
    if (palette) {
        delete[] palette;
    }
}

void BmpFile::open(const std::string &filename) {
    ifstream file (filename, ios::in | ios::binary);
    file.read ((char*) &fileHeader, sizeof(fileHeader));
    if (!file) return;
    file.read ((char*) &infoHeader, sizeof(infoHeader));
    if (!file) return;
    int palLength = fileHeader.bfOffBits - sizeof(fileHeader) - sizeof(infoHeader);
    palette = new u_int8_t[palLength];
    file.read((char*) palette, palLength);
    data = new u_int8_t[infoHeader.biSizeImage];
    file.read((char*) data, infoHeader.biSizeImage);
    file.close();
}

void BmpFile::save(const std::string &filename) {
    ofstream outfile(filename, ios::out | ios::binary);
    outfile.write((char*) &fileHeader, sizeof(fileHeader));
    outfile.write((char*) &infoHeader, sizeof(infoHeader));
    int palLength = fileHeader.bfOffBits - sizeof(fileHeader) - sizeof(infoHeader);
    outfile.write((char*) palette, palLength);
    outfile.write((char*) data, infoHeader.biSizeImage);
    outfile.close();
}

void BmpFile::saveRaw(const std::string &filename) {
    ofstream outfile(filename, ios::out | ios::binary);
    outfile.write((char*) data, infoHeader.biSizeImage);
    outfile.close();
}


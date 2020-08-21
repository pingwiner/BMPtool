
#include <iostream>
#include <fstream>
#include "BmpFile.h"
#include "Converter.h"

using namespace std;

Converter converter;

void encode(const std::string& file1, const std::string& file2, const std::string& diff) {
    BmpFile b1;
    BmpFile b2;
    b1.open(file1);
    b2.open(file2);
    ofstream outfile;
    outfile.open(diff, ios::out | ios::binary);
    for (int i = 0; i < 128; i++) {
        uint8_t offset = 0;
        uint8_t length = 0;
        uint8_t f = 0;
        for (int j = 0; j < 16; j++) {
            u_int8_t a1 = b1.data[i * 16 + j];
            u_int8_t a2 = b2.data[i * 16 + j];
            if (a1 == a2) {
                if (f) {
                    length = j - offset;
                }
            } else {
                if (f == 0) {
                    f = 1;
                    offset = j;
                } else {
                    length = j - offset + 1;
                }
            }
        }

        uint8_t x;
        if (offset > 15) offset = 15;
        if (length == 0) {
            x = 0xff;
        } else {
            x = (offset << 4) | (length - 1);
        }

        outfile.write((char* )&x, 1);
        if (length) {
            uint8_t buf[length];
            for (int k = 0; k < length; k++) {
                buf[k] = b1.data[i * 16 + offset + k] ^ b2.data[i * 16 + offset + k];
            }
            outfile.write((char*) buf, length);
        }
    }
    outfile.close();
}

void restore (const string& file1, const string& diff, const string& output) {
    ifstream diffile(diff, ios::in | ios::binary);
    BmpFile bmp;
    bmp.open(file1);
    uint8_t x;
    int line = 0;

    while (1) {
        diffile.read((char *) &x, 1);
        if (!diffile) break;
        if (x != 0xff) {
            uint8_t length = (x & 0xf) + 1;
            if (length > 0) {
                uint8_t offset = x >> 4;
                diffile.read((char *) &bmp.data[line * 16 + offset], length);
            }
        }
        line++;
    }
    diffile.close();
    bmp.save(output);
}

#define RL_WHITE 0xc0
#define RL_BLACK 0x00
#define RL_AS_IS 0x80
#define RL_END   0x40

int rle(uint8_t* p, int length, ofstream& outfile) {
    uint8_t buf[4096];
    uint8_t c = 1; // sequence length
    int in = 1;    // input position
    int out = 0;   // output position
    int prev = 0;

    while (in < length) {
        switch(p[in]) {
            case 0:
                if (p[in-1] == 0) {
                    c++;
                } else {
                    if (p[in-1] == 0xff) {
                        buf[out++] = RL_WHITE | c;
                    } else {
                        buf[out++] = RL_AS_IS | c;
                        while (prev < in) {
                            buf[out++] = p[prev++];
                        }
                    }
                    c = 1;
                }
                break;
            case 0xff:
                if (p[in-1] == 0xff) {
                    c++;
                } else {
                    if (p[in-1] == 0) {
                        buf[out++] = RL_BLACK | c;
                    } else {
                        buf[out++] = RL_AS_IS | c;
                        while (prev < in) {
                            buf[out++] = p[prev++];
                        }
                    }
                    c = 1;
                }
                break;
            default:
                if (p[in-1] == 0) {
                    buf[out++] = RL_BLACK | c;
                    c = 1;
                } else if (p[in-1] == 0xff) {
                    buf[out++] = RL_WHITE | c;
                    c = 1;
                } else {
                    c++;
                }
                prev = in;
        }
        in++;
    }
    if (p[in-1] == 0) {
        buf[out++] = RL_BLACK | c;
    } else if (p[in-1] == 0xff) {
        buf[out++] = RL_WHITE | c;
    } else {
        buf[out++] = RL_AS_IS | c;
    }
    buf[out++] = RL_END;
    outfile.write((char*) buf, out);
    return out;
}

void unrle(uint8_t* p, int length, ofstream& outfile) {
    uint8_t buf[4096];
    int j = 0;
    int i = 0;

    while(i < length) {
        int c = p[i] & 0x3f;
        switch(p[i] & 0xc0) {
            case RL_WHITE:
                while(c--) buf[j++] = 0;
                i++;
                break;
            case RL_BLACK:
                while(c--) buf[j++] = 0xff;
                i++;
                break;
            case RL_AS_IS:
                while(c--) buf[j++] = p[i++];
                break;
            case RL_END:
                i++;
                break;
        }
    }
    outfile.write((char*) buf, j);
}

int main() {
    std::string data = "data.asm";
    BmpFile bmp;
    bmp.open("0.bmp");
    bmp.saveRaw("img.bin");
    ofstream outfile;
    outfile.open("rle.bin", ios::out | ios::binary);
    int size = rle(bmp.data, bmp.infoHeader.biSizeImage, outfile);
    outfile.close();

    uint8_t buf[size];
    ifstream infile("rle.bin", ios::in | ios::binary);
    infile.read((char*)buf, size);
    infile.close();
    outfile.open("unrle.bin", ios::out | ios::binary);
    unrle(buf, size, outfile);
    outfile.close();

    /*
    converter.convert("img.bin", data, "image");

    for (int i = 0; i < 19; i++) {
        std::string filename = ".bin";
        encode(std::to_string(i) + ".bmp",
                std::to_string(i + 1) + ".bmp",
                "diff" + std::to_string(i + 1) + ".bin");
        converter.convert("diff" + std::to_string(i + 1) + ".bin", data, "frame" + std::to_string(i + 1));
    }
    */
    return 0;
}

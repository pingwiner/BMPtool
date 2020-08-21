//
// Created by ilya on 01.10.2019.
//

#include <fstream>
#include "Converter.h"

using namespace std;

void Converter::convert(const std::string &input, const std::string &output, const std::string& label) {
    u_int8_t buf[16];
    ifstream infile (input, ifstream::in | ifstream::binary);
    ofstream outfile(output, ofstream::out | ofstream::app);

    outfile << label << ":\n";

    while(1) {
        infile.read((char *) buf, sizeof(buf));
        std::streamsize count = infile.gcount();
        if (count > 0) outfile << "  db ";

        for (int i = 0; i < count; i++) {
            outfile << (i ? ", " : "");
            outfile << to_string(buf[i]);
        }
        outfile << "\n";
        if (count < sizeof(buf)) break;
    }
    outfile << "\n";
    infile.close();
    outfile.close();
}

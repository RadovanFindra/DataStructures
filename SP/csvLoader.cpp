#include "CsvLoader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>

CsvLoader::CsvLoader(const std::string& filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error while opening .csv file." << std::endl;
        return;
    }
    std::string firstLine;
    std::getline(file, firstLine);


    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string source, prefix, metric, next_hop, time_str;

        if (std::getline(iss, source, ';') &&
            std::getline(iss, prefix, ';') &&
            std::getline(iss, metric, ';') &&
            std::getline(iss, next_hop, ';') &&
            std::getline(iss, time_str, ';')) {
            unsigned int time = prevodCasuNaSekundy(time_str);

            lines.push_back({ source, prefix, metric, next_hop, time });
        }
        else {
            std::cerr << "Error while reading line on .csv file " << std::endl;
        }
    }
}

unsigned int CsvLoader::prevodCasuNaSekundy(std::string& cas) {
    unsigned int  sekundy = 0;
    std::string hodnota_str;
    bool dvojbka = false;
    for (char c : cas) {
        if (!isdigit(c) && c != 'w' && c != 'd' && c != 'h' && c != ':') {
            return  0;
        }
        if (isdigit(c)) {
            hodnota_str += c;
        }
        else {
            if (c == 'w') {
                sekundy += std::stoi(hodnota_str) * 7 * 24 * 60 * 60;
                hodnota_str = "";
            }
            else if (c == 'd') {
                sekundy += std::stoi(hodnota_str) * 24 * 60 * 60;
                hodnota_str = "";
            }
            else if (c == 'h') {
                sekundy += std::stoi(hodnota_str) * 60 * 60;
                hodnota_str = "";
            }
            else if (c == ':') {
                if (sekundy == 0) {
                    sekundy += std::stoi(hodnota_str) * 60 * 60;
                    hodnota_str = "";
                }
                else if (sekundy % 3600 == 0) {
                    dvojbka = true;
                    sekundy += std::stoi(hodnota_str) * 60;
                    hodnota_str = "";
                }
            }
        }
    }
    if (dvojbka)
    {
        sekundy += std::stoi(hodnota_str);
        hodnota_str = "";
        dvojbka = false;

    }
    return sekundy;
}
const std::vector<Line>& CsvLoader::getLines() const {
    return lines;
}



#include "CsvLoader.h"
#include <iostream>
#include <fstream>
#include <sstream>

CsvLoader::CsvLoader(const std::string& filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error while opening .csv file." << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string source, prefix, metric, next_hop, time_str;

        if (std::getline(iss, source, ';') &&
            std::getline(iss, prefix, ';') &&
            std::getline(iss, metric, ';') &&
            std::getline(iss, next_hop, ';') &&
            std::getline(iss, time_str, ';')) {
            int time = 0;
            lines.push_back({ source, prefix, metric, next_hop, time });
        }
        else {
            std::cerr << "Error while reading line on .csv file " << std::endl;
        }
    }
}

const std::vector<Line>& CsvLoader::getLines() const {
    return lines;
}
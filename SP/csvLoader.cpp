#include "CsvLoader.h"
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>

unsigned int CsvLoader::strToSec(std::string& time)
{
    unsigned int  sec = 0;
    std::string val_str;
    bool colon = false;
    for (char c : time) {
        if (!isdigit(c) && c != 'w' && c != 'd' && c != 'h' && c != ':') {
            return  0;
        }
        if (isdigit(c)) {
            val_str += c;
        }
        else {
            if (c == 'w') {
                sec += std::stoi(val_str) * 7 * 24 * 60 * 60;
                val_str = "";
            }
            else if (c == 'd') {
                sec += std::stoi(val_str) * 24 * 60 * 60;
                val_str = "";
            }
            else if (c == 'h') {
                sec += std::stoi(val_str) * 60 * 60;
                val_str = "";
            }
            else if (c == ':') {
                if (sec == 0) {
                    sec += std::stoi(val_str) * 60 * 60;
                    val_str = "";
                }
                else if (sec % 3600 == 0) {
                    colon = true;
                    sec += std::stoi(val_str) * 60;
                    val_str = "";
                }
            }
        }
    }
    if (colon)
    {
        sec += std::stoi(val_str);
        val_str = "";
        colon = false;

    }
    return sec;
}

std::vector<RoutingTableEntry> CsvLoader::load(const std::string& filename)
{
    std::vector<RoutingTableEntry> entries;
    std::ifstream file(filename);
    std::string line;

    std::getline(file, line); // header

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string source, prefix, metric, next_hop, time_str;

        if (std::getline(iss, source, ';') &&
            std::getline(iss, prefix, ';') &&
            std::getline(iss, metric, ';') &&
            std::getline(iss, next_hop, ';') &&
            std::getline(iss, time_str, ';')) {

            std::string address;
            int networkBits;
            size_t slashPos = prefix.find('/');
            address = prefix.substr(0, slashPos);

            networkBits = std::stoi(prefix.substr(slashPos + 1));

            next_hop.erase(0, 4);
            unsigned int time = strToSec(time_str);


            entries.push_back({ source, { address, networkBits }, metric, next_hop, time
                });
        }


    }
    return entries;
}



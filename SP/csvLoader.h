#pragma once

#include <vector>
#include <string>

struct IPAddress {
    std::string address;
    int netMask;
};

struct Line {
    std::string source;
    IPAddress prefix;
    std::string metric;
    std::string next_hop;
    unsigned int  time;
};

class CsvLoader {
private:
    std::vector<Line> lines;
    unsigned int prevodCasuNaSekundy(std::string& cas);
public:
    CsvLoader(const std::string& filename);
    std::vector<Line>& getLines();
    void load();
};



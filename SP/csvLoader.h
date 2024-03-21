#pragma once

#include <vector>
#include <string>

struct Line {
    std::string source;
    std::string prefix;
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
    const std::vector<Line>& getLines() const;
    void load();
};



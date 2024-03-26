#pragma once

#include <vector>
#include <string>

struct IPAddress {
    std::string address;
    int netMask;
};

struct RoutingTableEntry {
    std::string source;
    IPAddress prefix;
    std::string metric;
    std::string next_hop;
    unsigned int  time;
};

class CsvLoader {
private:
    static unsigned int strToSec(std::string& time);
public:
    static std::vector<RoutingTableEntry> load(const std::string& filename);
};



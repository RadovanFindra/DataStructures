#include <bitset>
#include "Algoritmus.h"
#include <sstream>




std::bitset<32> Algoritmus::ipToBits(const std::string& ipAddress)
{
    unsigned int ipUint = 0;
    int octet = 0;
    std::istringstream octetStream(ipAddress);
    while (octetStream >> octet) {
        ipUint = (ipUint << 8) + octet;
        if (octetStream.peek() == '.') octetStream.ignore();
    }
    return std::bitset<32>(ipUint);
}

std::pair<time_t, time_t> Algoritmus::parseTimeRange(const std::string& timeRange)
{
    std::istringstream ss(timeRange);
    std::string token;
    std::getline(ss, token, '-');
    time_t startTime = std::stoi(token);
    std::getline(ss, token);
    time_t endTime = std::stoi(token);
    return std::make_pair(startTime, endTime);
}



#include <fstream>

#include <libds/amt/implicit_sequence.h>
#include <bitset>


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
    static unsigned int strToSec(std::string& time) {
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
public:
    static std::vector<RoutingTableEntry> load(const std::string& filename) {
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
    };
};

std::bitset<32> ipToBits(const std::string& ipAddress) {
    unsigned int ipUint = 0;
    int octet = 0;
    std::istringstream octetStream(ipAddress);
    while (octetStream >> octet) {
        ipUint = (ipUint << 8) + octet;
        if (octetStream.peek() == '.') octetStream.ignore();
    }
    return std::bitset<32>(ipUint);
}

std::pair<time_t, time_t> parseTimeRange(const std::string& timeRange) {
    std::istringstream ss(timeRange);
    std::string token;
    std::getline(ss, token, '-');
    time_t startTime = std::stoi(token);
    std::getline(ss, token);
    time_t endTime = std::stoi(token);
    return std::make_pair(startTime, endTime);
}

template<typename Iterator>
std::vector<RoutingTableEntry> process_data(Iterator begin, Iterator end, std::function<bool(const typename Iterator::value_type&, const std::string&)> predicate, std::string userDefined) {
	std::vector<RoutingTableEntry> matched;
	for (Iterator it = begin; it != end; ++it) {
        if (predicate(*it, userDefined)) {
            matched.push_back(*it);
        }
    }
    return matched;
}

int main()
{
    ds::amt::ImplicitSequence<int> is;
    std::vector<RoutingTableEntry> routingTableData = CsvLoader::load("RT.csv");
    std::string  userIn = "1.1.128.1";

    std::function<bool(const RoutingTableEntry&, const std::string&)> matchWithAddress;
    matchWithAddress = [&](const RoutingTableEntry& entry, const std::string& userDefined)
    {
	    std::bitset<32> userIPBits = ipToBits(userDefined);
	    std::bitset<32> prefixBits = ipToBits(entry.prefix.address);


	    userIPBits >>= (32 - entry.prefix.netMask);
	    prefixBits >>= (32 - entry.prefix.netMask);

	    return userIPBits == prefixBits;
    };
    
    std::function<bool(const RoutingTableEntry&, std::string)> matchWithTime = [&](const RoutingTableEntry& entry, const std::string& userDefined) {
        auto timeRange = parseTimeRange(userIn);
        time_t startTime = timeRange.first;
        time_t endTime = timeRange.second;
        return entry.time >= startTime && entry.time <= endTime;
        };

	std::vector<RoutingTableEntry> processedIP   = process_data(routingTableData.begin(), routingTableData.end(), matchWithAddress, userIn);
    userIn = "10-3600";
    std::vector<RoutingTableEntry> processedTime = process_data(routingTableData.begin(), routingTableData.end(), matchWithTime   , userIn);

    for (const auto& entry : processedIP) {
        std::cout << "Address: " << entry.prefix.address << "/" << entry.prefix.netMask << std::endl;
    }
    std::cout << std::endl;
    for (const auto& entry : processedTime) {
        std::cout << "Address: " << entry.prefix.address << "/" << entry.prefix.netMask << "           " << entry.time << std::endl;
    }
    return 0;
}


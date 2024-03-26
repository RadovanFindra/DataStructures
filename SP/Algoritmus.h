#pragma once
#include <bitset>
#include <functional>
#include <vector>
#include <string>
#include "csvLoader.h"


class Algoritmus
{
private:
	 static std::bitset<32> ipToBits(const std::string& ipAddress);
	 static std::pair<time_t, time_t> parseTimeRange(const std::string& timeRange);

	

public:
	 template<typename Iterator>
	 static std::vector<RoutingTableEntry> process_data(Iterator begin, Iterator end, std::function<bool(const typename Iterator::value_type&, const std::string&)> predicate, std::string userDefined) {
		std::vector<RoutingTableEntry> matched;
		for (Iterator it = begin; it != end; ++it) {
			if (predicate(*it, userDefined)) {
				matched.push_back(*it);
			}
		}
		return matched;
	}

	 std::function<bool(const RoutingTableEntry&, const std::string&)> matchWithAddress = [&](const RoutingTableEntry& entry, const std::string& userDefined)
		{
			std::bitset<32> userIPBits = Algoritmus::ipToBits(userDefined);
			std::bitset<32> prefixBits = Algoritmus::ipToBits(entry.prefix.address);


			userIPBits >>= (32 - entry.prefix.netMask);
			prefixBits >>= (32 - entry.prefix.netMask);

			return userIPBits == prefixBits;
		};

	 std::function<bool(const RoutingTableEntry&, std::string)> matchWithTime = [&](const RoutingTableEntry& entry, const std::string& userDefined)
		{
			auto timeRange = parseTimeRange(userDefined);
			time_t startTime = timeRange.first;
			time_t endTime = timeRange.second;
			return entry.time >= startTime && entry.time <= endTime;
		};

};


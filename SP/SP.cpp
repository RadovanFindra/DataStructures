
#include <bitset>
#include <iostream>

#include "Algoritmus.h"
#include "csvLoader.h"

int main()
{
   
    std::vector<RoutingTableEntry> routingTableData = CsvLoader::load("RT.csv");
    Algoritmus algoritmus;

    
    std::cout << "Vitajte v konzolovom menu!\n";
    std::cout << "Autor: Radovan Findra, 2024\n\n";

    std::vector<RoutingTableEntry> processedTime;
    std::vector<RoutingTableEntry> processedIP;
    char choice;
    do {
        std::cout << "Vyberte moznost:\n";
        std::cout << "1) matchWithAddress\n";
        std::cout << "2) matchLifetime\n";
        std::cout << "3) Obidva algoritmy (Jeden a nad jeho vysledkom druhy)\n";
        std::cout << "q) Ukoncit program\n";
        std::cout << "Vyber: ";
        std::cin >> choice;

        switch (choice) {
        case '1': {
            std::string userIn;
            std::cout << "Zadajte IP adresu: ";
            std::cin >> userIn;
            processedIP = Algoritmus::process_data(routingTableData.begin(), routingTableData.end(), algoritmus.matchWithAddress, userIn);
            std::cout << std::endl;
            std::cout << "Zdroj" << ";" << "prefix" << ";" << "metrika" << ";" << "next-hop" << ";" << "cas" << std::endl;
            for (const auto& entry : processedIP) {
                std::cout << entry.source << ";" << entry.prefix.address << "/" << entry.prefix.netMask << ";" << entry.metric << ";" << entry.next_hop << ";" << entry.time << std::endl;    
            }

            break;
        }
        case '2': {
            std::string userIn;
            std::cout << "Zadajte casovy interval (format: od-do): ";
            std::cin >> userIn;
            processedTime = Algoritmus::process_data(routingTableData.begin(), routingTableData.end(), algoritmus.matchWithTime, userIn);
            std::cout << std::endl;
            std::cout << "Zdroj" << ";" << "prefix" << ";" << "metrika" << ";" << "next-hop" << ";" << "cas" << std::endl;
            for (const auto& entry : processedTime) {
                std::cout << entry.source << ";" << entry.prefix.address << "/" << entry.prefix.netMask << ";" << entry.metric << ";" << entry.next_hop << ";" << entry.time << std::endl;
            }
            break;
        }
        case '3': {
            char subChoice;
            std::cout << "Vyberte prvy filter:\n";
            std::cout << "a) Spracovat IP adresu\n";
            std::cout << "b) Spracovat casovy interval\n";
            std::cout << "Vyber: ";
            std::cin >> subChoice;

            if (subChoice == 'a') {
                std::string userIn;
                std::cout << "Zadajte IP adresu: ";
                std::cin >> userIn;
                std::vector<RoutingTableEntry> processedIP = Algoritmus::process_data(routingTableData.begin(), routingTableData.end(), algoritmus.matchWithAddress, userIn);
                std::cout << std::endl;
                std::cout << "Zdroj" << ";" << "prefix" << ";" << "metrika" << ";" << "next-hop" << ";" << "cas" << std::endl;
            	for (const auto& entry : processedIP) {
                    std::cout << entry.source << ";" << entry.prefix.address << "/" << entry.prefix.netMask << ";" << entry.metric << ";" << entry.next_hop << ";" << entry.time << std::endl;
                }
                std::cout << "Zadajte casovy interval (format: od-do): ";
                std::cin >> userIn;
                std::vector<RoutingTableEntry> processedTime = Algoritmus::process_data(processedIP.begin(), processedIP.end(), algoritmus.matchWithTime, userIn);
                std::cout << std::endl;
                std::cout << "Zdroj" << ";" << "prefix" << ";" << "metrika" << ";" << "next-hop" << ";" << "cas" << std::endl;
                for (const auto& entry : processedTime) {
                    std::cout << entry.source << ";" << entry.prefix.address << "/" << entry.prefix.netMask << ";" << entry.metric << ";" << entry.next_hop << ";" << entry.time << std::endl;
                }


            }
            else if (subChoice == 'b') {
                std::string userIn;
                std::cout << "Zadajte casovy interval (format: od-do): ";
                std::cin >> userIn;
                std::vector<RoutingTableEntry> processedTime = Algoritmus::process_data(routingTableData.begin(), routingTableData.end(), algoritmus.matchWithTime, userIn);
                std::cout << std::endl;
                std::cout << "Zdroj" << ";" << "prefix" << ";" << "metrika" << ";" << "next-hop" << ";" << "cas" << std::endl;
                for (const auto& entry : processedTime) {
                    std::cout << entry.source << ";" << entry.prefix.address << "/" << entry.prefix.netMask << ";" << entry.metric << ";" << entry.next_hop << ";" << entry.time << std::endl;
                }

                std::cout << "Zadajte IP adresu: ";
                std::cin >> userIn;
                std::vector<RoutingTableEntry> processedIP = Algoritmus::process_data(processedTime.begin(), processedTime.end(), algoritmus.matchWithAddress, userIn);
                std::cout << std::endl;
                std::cout << "Zdroj" << ";" << "prefix" << ";" << "metrika" << ";" << "next-hop" << ";" << "cas" << std::endl;
                for (const auto& entry : processedIP) {
                    std::cout << entry.source << ";" << entry.prefix.address << "/" << entry.prefix.netMask << ";" << entry.metric << ";" << entry.next_hop << ";" << entry.time << std::endl;
                }
            }
            else {
                std::cout << "Neplatna moznost!\n";
            }
            break;
        }
        case 'q':
            std::cout << "Ukoncujem program...\n";
            break;
        default:
            std::cout << "Neplatna moznost!\n";
            break;
        }
    } while (choice != 'q');


    return 0;
}


#include <libds/amt/implicit_sequence.h>
#include "csvLoader.h"



int main()
{
    ds::amt::ImplicitSequence<int> is;

    CsvLoader loader("RT.csv");
    const std::vector<Line>& lines = loader.getLines();


    for (const auto& line : lines) {
        std::cout << "Prefix: " << line.prefix << std::endl;
        std::cout << "Next-hop: " << line.next_hop << std::endl;
        std::cout << "Time: " << line.time << " sec" << std::endl;
        std::cout << std::endl;
    }

    return 0;
}
#include <mcpp/mcpp.h>
#include "plots.h"
#include "task_a.h"
#include "village.h"
#include <vector>
#include <iostream>
#include <random>
#include <algorithm>


int main(int argc, char* argv[]) {
    bool testmode = false;
    unsigned int seed = 42;

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--testmode") {
            testmode = true;
        } else if (arg.find("--seed=") == 0) {
            seed = std::stoi(arg.substr(7));
        }
    }

    if (!testmode) {
        std::cout << "This test program requires --testmode" << std::endl;
        return 1;
    }

    std::cout << "EDGE CASE 1: NO BLOCKS TO CLEAR" << std::endl;
    std::cout << "Seed is " << seed << std::endl << std::endl;

    mcpp::MinecraftConnection mc;

    int loc_x;
    int loc_z;
    int village_size = 100; // change for testing purposes

    std::cin >> loc_x >> loc_z;

    int half = village_size / 2;
    int minX = loc_x - half;
    int minZ = loc_z - half;
    int maxX = loc_x + half;
    int maxZ = loc_z + half;

    std::cout << "Village bounds: (" << minX << ", " << minZ << ") to (" << maxX << ", " << maxZ << ")\n"; //DEBUGGING

    setVillageBounds(minX, maxX, minZ, maxZ);

    clearUnwantedBlocks(minX, maxX, minZ, maxZ, mc);

    return 0;
}
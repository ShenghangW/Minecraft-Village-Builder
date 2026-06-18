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

    std::cout << "COMPONENT 5: GET WAYPOINTS" << std::endl;
    std::cout << "Seed is " << seed << std::endl << std::endl;

    mcpp::MinecraftConnection mc;

    int loc_x;
    int loc_z;
    int village_size = 100; // change for testing purposes
    int plot_border = 10;

    std::cin >> loc_x >> loc_z;

    int half = village_size / 2;
    int minX = loc_x - half;
    int minZ = loc_z - half;
    int maxX = loc_x + half;
    int maxZ = loc_z + half;

    std::cout << "Village bounds: (" << minX << ", " << minZ << ") to (" << maxX << ", " << maxZ << ")\n";

    setVillageBounds(minX, maxX, minZ, maxZ);

    clearUnwantedBlocks(minX, maxX, minZ, maxZ, mc);

    std::cout << "\t Finding plots..." << std::endl ;

    std::vector<Plot> plots = find_plots(loc_x, loc_z, village_size, plot_border, seed, testmode) ;

    std::cout << "\t Doing some landscaping..." << std::endl ;

    // Insert terraforming here.
    for (size_t i = 0; i < plots.size(); i++){
        int plotSize = plots[i].bound.x - plots[i].origin.x + 1;  // get plot size
        terraform(plots[i], plotSize, plot_border, mc, plots);
    }

    // Insert Placing a wall here.
        std::cout << "\t Placing a cool wall around the village..." << std::endl;
        place_wall(loc_x, loc_z, village_size, mc);

    std::cout << "\t Finding waypoints..." << std::endl;
    std::vector<mcpp::Coordinate> waypoints = find_waypoints(plots, loc_x, loc_z, village_size);

    return 0;
}
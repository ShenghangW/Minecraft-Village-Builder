#include <mcpp/mcpp.h>
#include <vector>
#include <iostream>
#include "task_b.h"
#include "plots.h"


int main(int argc, char* argv[]) {
//  Parsing --testmode for the argument
    bool testMode = false;
 // Fixed seed for testing
unsigned int seed = 42;

for (int i = 1; i < argc; i++) {
 std::string arg = argv[i];
        if (arg == "--testmode") {
            testMode = true;
        } else if (arg.find("--seed=") == 0) {
            seed = std::stoi(arg.substr(7));
        }
}
if (!testMode) {
        std::cout << "This test program requires --testmode" << std::endl;
        return 1;
    }
std::cout << "COMPONENT 3: Placing Doors" << std::endl;
    std::cout << "Seed is " << seed << std::endl << std::endl;

    std::vector<Plot> plots;

    int originX, originY, originZ, boundX, boundY, boundZ, entranceX, entranceY, entranceZ;

    while (std::cin >> originX >> originY >> originZ) {
        if (!(std::cin >> boundX >> boundY >> boundZ)) {   
            std::cout << "ERROR missing bound coordinates" << std::endl;
            return 1;
        }
        if (!(std::cin >> entranceX >> entranceY >> entranceZ)) {
            std::cout << "ERROR missing entrance coordinates" << std::endl;
            return 1;
        }

        Plot plot;
        plot.origin = mcpp::Coordinate(originX, originY, originZ);
        plot.bound = mcpp::Coordinate(boundX, boundY, boundZ);
        plot.entrance = mcpp::Coordinate(entranceX, entranceY, entranceZ);
        plots.push_back(plot);
    }

    std::cout << "Total plots: " << plots.size() << std::endl << std::endl;
    
    
    mcpp::MinecraftConnection mc;
    Building building(seed);
    
    for (Plot& plot : plots) {
         int x = plot.origin.x;
        int y = plot.origin.y;
        int z = plot.origin.z;
        int width = plot.bound.x - plot.origin.x + 1;
        int length = plot.bound.z - plot.origin.z + 1;
        int height = building.getNextHeight(testMode);
     
     std::cout << "\tBuilding " << width << "x" << length << " house with height " << height << std::endl;
    
    themedFloor(mc, x + 1, z + 1, width - 2, length - 2, y, "Living Room");
    buildWalls(mc, x, z, width, length, y - 1, height + 1);
    building.resetDoors();
    building.resetTheme();
    
    std::vector<Room> rooms = subdivideInterior(mc, plot, building, testMode);
        
    placeMainEntrance(mc, plot, building, rooms, testMode);
    placeDoorsForAllRooms(mc, rooms, building, testMode);
    std::cout << std::endl;
}
    return 0;
}





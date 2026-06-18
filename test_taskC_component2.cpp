#include <mcpp/mcpp.h>
#include <vector>
#include <iostream>
#include "task_c.h"
#include "task_a.h"
#include "task_b.h"
#include "plots.h"
#include "cache.h"

int main(int argc, char* argv[]) {
    bool testMode = false;
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

    std::cout << "COMPONENT 2: 3D WORLD PATHFINDING" << std::endl;
    std::cout << "Seed is " << seed << std::endl << std::endl;

    mcpp::MinecraftConnection mc;

    // Read plot data from stdin
    int numPlots;
    std::cin >> numPlots;
    
    std::vector<Plot> plots;
    for (int i = 0; i < numPlots; i++) {
        int ox, oy, oz, bx, by, bz, ex, ey, ez;
        std::cin >> ox >> oy >> oz >> bx >> by >> bz >> ex >> ey >> ez;
        
        mcpp::Coordinate origin(ox, oy, oz);
        mcpp::Coordinate bound(bx, by, bz);
        mcpp::Coordinate entrance(ex, ey, ez);
        
        plots.push_back(Plot(origin, bound, entrance));
    }
    
    // Read waypoints from stdin
    int numWaypoints;
    std::cin >> numWaypoints;
    
    std::vector<mcpp::Coordinate> waypoints;
    for (int i = 0; i < numWaypoints; i++) {
        int x, y, z;
        std::cin >> x >> y >> z;
        waypoints.push_back(mcpp::Coordinate(x, y, z));
    }

    std::cout << "Total plots: " << plots.size() << std::endl;
    std::cout << "Total waypoints: " << waypoints.size() << std::endl << std::endl;

    // Calculate village bounds
    int minX = plots[0].origin.x;
    int maxX = plots[0].bound.x;
    int minZ = plots[0].origin.z;
    int maxZ = plots[0].bound.z;

    for (const auto& plot : plots) {
        minX = std::min(minX, plot.origin.x);
        maxX = std::max(maxX, plot.bound.x);
        minZ = std::min(minZ, plot.origin.z);
        maxZ = std::max(maxZ, plot.bound.z);
    }

    // CRITICAL: Clear obstacles BEFORE pathfinding
    std::cout << "========================================" << std::endl;
    std::cout << "Step 1: Clearing Obstacles" << std::endl;
    std::cout << "========================================" << std::endl;
    clearUnwantedBlocks(minX, maxX, minZ, maxZ, mc);
    std::cout << "Obstacles cleared!\n" << std::endl;

    // Get proper height range for 3D terrain
    int minGroundY = 1000;
    int maxGroundY = 0;

    for (int x = minX; x <= maxX; x += 20) {
        for (int z = minZ; z <= maxZ; z += 20) {
            int h = mc.getHeight(mcpp::Coordinate2D(x, z));
            minGroundY = std::min(minGroundY, h);
            maxGroundY = std::max(maxGroundY, h);
        }
    }

    minGroundY -= 5;
    maxGroundY += 10;

    mcpp::Coordinate villageMin(minX - 50, minGroundY, minZ - 50);
    mcpp::Coordinate villageMax(maxX + 50, maxGroundY, maxZ + 50);

    std::cout << "========================================" << std::endl;
    std::cout << "Step 2: Preparing Block Cache" << std::endl;
    std::cout << "========================================" << std::endl;

    BlockCache cache;
    cache.init(villageMin, villageMax, mc);
    std::cout << "Cache ready!\n" << std::endl;

    std::cout << "========================================" << std::endl;
    std::cout << "Step 3: Executing Pathfinding" << std::endl;
    std::cout << "========================================\n" << std::endl;

    // Execute pathfinding
    executeTaskC(plots, waypoints, mc, villageMin, villageMax, cache);

    std::cout << "\n========================================" << std::endl;
    std::cout << "TEST COMPLETE!" << std::endl;
    std::cout << "========================================" << std::endl;

    return 0;
}
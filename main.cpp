
    #include <mcpp/mcpp.h>
    #include <vector>
    #include <iostream>
    #include <ctime>

    #include "plots.h"
    #include "village.h"
    #include "utils.h"
    #include "task_a.h"
    #include "task_b.h"
    #include "task_c.h"
    #include "cache.h"

    int main(const int argc, const char *const *const argv) {
        print_student_details() ;

        mcpp::MinecraftConnection mc;
        mcpp::Coordinate player = mc.getPlayerPosition();    // get coordinates of the player

        // Read command line arguments:

        int loc_x = player.x ;
        int loc_z = player.z ;
        int village_size = 200 ;
        int plot_border = 10 ;
        int seed = 0 ;
        bool testmode = false ;

        // // Demonstrating how to read command line arguments: REMOVE IN FINAL PROGRAM
        // // std::cout << "There are " << argc << " command line arguments." << std::endl ;
        // // for (unsigned int i = 0 ; i < argc ; i++) {
        // //     std::cout << argv[i] << std::endl ;
        // // }

        // // print_command_line_arguments(loc_x, loc_z, village_size, plot_border) ;

        // // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // Task A
        std::cout << "Task A: Finding Plots and Terraforming" << std::endl ;

        int half = village_size / 2;
        int minX = loc_x - half;
        int minZ = loc_z - half;
        int maxX = loc_x + half;
        int maxZ = loc_z + half;

        setVillageBounds(minX, maxX, minZ, maxZ);

        clearUnwantedBlocks(minX, maxX, minZ, maxZ, mc);

        std::cout << "\t Finding plots..." << std::endl ;

        // Insert finding plots here.
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
        
        // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // Task B
        std::cout << "Task B: Building Houses" << std::endl;
        build_buildings(plots, testmode, seed);

        // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        std::cout << "Task C: Pathfinding" << std::endl;

        // Calculate village bounds

        const std::vector<int>& villageMinMax = getVillageBounds();

        if (villageMinMax.size() >= 4) {
        minX = villageMinMax[0];
        maxX = villageMinMax[1];
        minZ = villageMinMax[2];
        maxZ = villageMinMax[3];
        
        std::cout << "Village bounds from task A: (" << minX << "," << minZ 
                << ") to (" << maxX << "," << maxZ << ")" << std::endl;
        } else {
            // Fallback to calculating from plots - REMOVE THE DUPLICATE DECLARATIONS
            minX = plots[0].origin.x;  // No 'int' here!
            maxX = plots[0].bound.x;
            minZ = plots[0].origin.z;
            maxZ = plots[0].bound.z;

            for (const auto& plot : plots) {
                minX = std::min(minX, plot.origin.x);
                maxX = std::max(maxX, plot.bound.x);
                minZ = std::min(minZ, plot.origin.z);
                maxZ = std::max(maxZ, plot.bound.z);
            }
        }

        std::cout << "Village actual bounds: (" << minX << "," << minZ 
                << ") to (" << maxX << "," << maxZ << ")" << std::endl;

        // Get proper height range for the area
        int minGroundY = 1000;
        int maxGroundY = 0;

        // Sample multiple points to find actual terrain height range
        for (int x = minX; x <= maxX; x += 20) {
            for (int z = minZ; z <= maxZ; z += 20) {
                int h = mc.getHeight(mcpp::Coordinate2D(x, z));
                minGroundY = std::min(minGroundY, h);
                maxGroundY = std::max(maxGroundY, h);
            }
        }

        // Add buffer for hills/valleys
        minGroundY -= 5;
        maxGroundY += 10;

        std::cout << "Detected ground height range: " << minGroundY << " to " << maxGroundY << std::endl;

        mcpp::Coordinate villageMin(minX - 50, minGroundY, minZ - 50);
        mcpp::Coordinate villageMax(maxX + 50, maxGroundY, maxZ + 50);

        // ADD THIS DEBUG OUTPUT:
        std::cout << "Cache Y range: " << villageMin.y << " to " << villageMax.y << std::endl;
        std::cout << "Cache dimensions: " 
                << (villageMax.x - villageMin.x + 1) << " x " 
                << (villageMax.y - villageMin.y + 1) << " x " 
                << (villageMax.z - villageMin.z + 1) << " blocks" << std::endl;

        std::cout << "\n========================================" << std::endl;
        std::cout << "Preparing Block Cache" << std::endl;
        std::cout << "========================================" << std::endl;

        BlockCache blockCache;
        blockCache.init(villageMin, villageMax, mc);

        std::cout << "\n========================================" << std::endl;
        std::cout << "Cache Complete - Starting Pathfinding" << std::endl;
        std::cout << "========================================\n" << std::endl;

        std::cout << "Test Configuration:" << std::endl;
        std::cout << "  Waypoints: " << waypoints.size() << std::endl;

        // Print all waypoint coordinates
        for (size_t i = 0; i < waypoints.size(); i++) {
            std::cout << "  Waypoint " << i << ": (" << waypoints[i].x << "," 
                    << waypoints[i].y << "," << waypoints[i].z << ")" << std::endl;
        }

        std::cout << "  Village bounds: (" << villageMin.x << "," << villageMin.z 
                << ") to (" << villageMax.x << "," << villageMax.z << ")" << std::endl;

        // RUN TASK C with cache!
        executeTaskC(plots, waypoints, mc, villageMin, villageMax, blockCache);

        std::cout << "Task C Complete!" << std::endl;

        return 0;
    }
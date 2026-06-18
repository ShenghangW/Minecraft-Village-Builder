#ifndef TASK_A
#define TASK_A
#include "plots.h"
#include <vector>

std::vector<Plot> find_plots(int loc_x, int loc_z, int village_size, int plot_border, int seed, bool testmode) ;

std::vector<mcpp::Coordinate> find_waypoints(const std::vector<Plot>& plots, int village_center_x, int village_center_z, int village_size) ;

// std::vector<mcpp::Coordinate> find_waypoints(const std::vector<Plot> plots, int village_center_x, int village_center_z, int village_size) ;

void terraform(const Plot& plot, int size, int border, mcpp::MinecraftConnection& mc, const std::vector<Plot>& plots) ;

void place_wall(int loc_x, int loc_z, int village_size, mcpp::MinecraftConnection& mc) ;

void clearUnwantedBlocks(int minX, int maxX, int minZ, int maxZ, mcpp::MinecraftConnection& mc) ;

namespace village_data {
    extern std::vector<int> minMax;
}

std::vector<mcpp::Coordinate> find_waypoints(const std::vector<Plot>& plots, int village_center_x, int village_center_z, int village_size) ;

#endif
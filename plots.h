#ifndef PLOTS_H
#define PLOTS_H

#include <mcpp/mcpp.h>

class Plot {
    public:
        mcpp::Coordinate origin; // minimum/north-west (-x, -z) corner
        mcpp::Coordinate bound; // maximum/south-east (x, z) corner
        mcpp::Coordinate entrance; // for paths and house generation
        int size;

        Plot() : origin(0, 0, 0), bound(0, 0, 0), entrance(0, 0, 0), size(0) {}
        Plot(mcpp::Coordinate origin, mcpp::Coordinate bound, mcpp::Coordinate entrance);
        // static int getHighestNonAirY(int x, int z, mcpp::MinecraftConnection &mc) ;
        static bool validOrNot(const mcpp::Coordinate& origin, int size, int border, const std::vector<Plot>& plots, mcpp::MinecraftConnection& mc) ;
        // additional fields and methods as needed

        
} ;

#endif
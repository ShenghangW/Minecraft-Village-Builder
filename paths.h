#ifndef PATHS_H
#define PATHS_H

#include <mcpp/mcpp.h>

class Path {
    public:
        mcpp::Coordinate start ;
        mcpp::Coordinate end ;
} ;

void breadth_first_search() ;

#endif
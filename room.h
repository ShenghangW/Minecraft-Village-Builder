#ifndef ROOM_H
#define ROOM_H
#include <mcpp/mcpp.h>
#include <vector>
#include <string>

class Room {
    public:
    // room constructors
    Room(const std::string& theme, int x, int y, int z, int width, int depth);
    Room(mcpp::Coordinate origin, mcpp::Coordinate bound, mcpp::Coordinate door);
    //getters
    mcpp::Coordinate getOrigin() const { return origin; }
    mcpp::Coordinate getBound() const { return bound; }
    mcpp::Coordinate getDoor() const { return door; }
    std::string getTheme() const { return theme; }
     void setTheme(const std::string& newTheme) { theme = newTheme; }
    //to sort the rooms
    int getWidth() const { 
        return bound.x - origin.x;
    }
     int getDepth() const { 
        return bound.z - origin.z;
    }
    int getArea() const {
        return getWidth() * getDepth();
    }
    

    std::vector<mcpp::Coordinate> getFurnituresLocation() const { return furnituresLocation; }
    std::vector <int> getFurnitures() const { return furnitures; }
bool hasFurnitureAtCoord(const mcpp::Coordinate& position) const;
void addFurnitures(mcpp::Coordinate loc, int id);


private:
mcpp::Coordinate origin;
mcpp::Coordinate bound;
mcpp::Coordinate door;
std::string theme;
std::vector<mcpp::Coordinate> furnituresLocation;
std::vector<int> furnitures;



};
#endif


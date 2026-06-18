#include "room.h"

// Constructor from theme and dimensions
Room::Room(const std::string& theme, int x, int y, int z, int width, int depth) : origin(mcpp::Coordinate(x, y, z)),  bound(mcpp::Coordinate(x + width, 0, z + depth)),  door(mcpp::Coordinate(0, 0, 0)), theme(theme) {}

// Constructor from coordinates
Room::Room(mcpp::Coordinate origin, mcpp::Coordinate bound, mcpp::Coordinate door) : origin(origin),bound(bound), door(door), theme("") {}

void Room::addFurnitures(mcpp::Coordinate loc, int id) {
    furnituresLocation.push_back(loc);
    furnitures.push_back(id);
}
bool Room::hasFurnitureAtCoord(const mcpp::Coordinate& position) const {
    for (size_t i = 0; i < furnituresLocation.size(); i++) {
         if (furnituresLocation[i].x == position.x &&  furnituresLocation[i].y == position.y && furnituresLocation[i].z == position.z) {
            return true;
        }
}
return false;
}
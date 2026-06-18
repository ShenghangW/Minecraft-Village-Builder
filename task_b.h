#ifndef TASK_B
#define TASK_B
#include <vector>
#include <set>
#include <random>
#include <iostream>
#include "plots.h"
#include "room.h"

// Building dimensions
const int EXTERIOR_HEIGHT = 6;
const int DOOR_HEIGHT = 2;
const int MIN_ROOM_SIZE = 4;
const int MAX_ROOM_SIZE = 8;
const int DOOR_OFFSET = 1;
// Block ID
const int AIR_BLOCK_ID = 0;
const int OAK_DOOR_ID = 64;
const int TORCH_ID = 50;
const int TORCH_FACING_EAST = 2;
const int TORCH_FACING_SOUTH = 4;
// The class manages the house construction with how the door and room themes should be placed.
// It tracks the palced doors to prevent the duplicate ones while cycling through each room theme.
class Building {

private:
std::set<std::pair<int, int>> placedDoors;
int themeIndex;
std::vector<std::string> themes;
std::mt19937 rng;
int buildingCount;
 int furnitureIndex;

public: 
//constructor 
Building(unsigned int seed) {
    themeIndex = 0;
    buildingCount = 0;
    furnitureIndex = 0;
    themes = {"Bedroom", "Kitchen", "Bathroom", "Living Room"};
    rng.seed(seed);
}
// destructor/cleanup
void resetDoors() { placedDoors.clear(); }
 void resetTheme() { themeIndex = 0; }
void resetBuildingCount() { buildingCount = 0; }
void resetFurnitureIndex() { furnitureIndex = 0; }
int getNextFurnitureIndex() { return furnitureIndex++; }
bool isDoorPlaced(int x, int z) const {

//-----------
 return placedDoors.find({x, z}) != placedDoors.end();

    }

void addDoor(int x, int z) {
     placedDoors.insert({x, z});
    }

std::mt19937& getRng() { return rng; }

std::string getNextTheme() {
 if (themeIndex >= static_cast<int>(themes.size())) {
            themeIndex = 0;
        }
        return themes[themeIndex++];
    }
    
 const std::set<std::pair<int,int>>& getPlacedDoors() const {
  return placedDoors;
    }

int getNextHeight(bool testMode) {
        int height = EXTERIOR_HEIGHT;  
        
        if (testMode) {
            height = 6 + buildingCount;
            if (height > 20) {
                height = 20;
            }
            buildingCount++;
        }
        
        return height;  
    }



};

//main function
void build_buildings(std::vector<Plot>& plots, bool testMode, unsigned int seed);

// Exterior
void buildExterior(mcpp::MinecraftConnection& mc, Plot& plot, Building& building, bool testMode);
void buildWalls(mcpp::MinecraftConnection& mc, int x, int z, int width, int depth, int y, int height);
void buildRoof(mcpp::MinecraftConnection& mc, int x, int z, int width, int depth, int y);
void themedFloor( mcpp::MinecraftConnection& mc, int x, int z, int width, int depth, int y, const std::string& theme);
mcpp::Coordinate placeMainEntrance(mcpp::MinecraftConnection& mc, const Plot& plot, Building& building,  const std::vector<Room>& rooms, bool testMode);


// Interior
// Structure to hold the area
struct SubdivisionArea {
    int minX, minZ;
    int maxX, maxZ;
    int floorY;
    
    int width() const { return maxX - minX + 1; }
    int depth() const { return maxZ - minZ + 1; }
};


// Sub division declarations
std::vector<Room> subdivideInterior(mcpp::MinecraftConnection& mc, const Plot& plot, Building& building, bool testMode);
void placeSingleDoor(mcpp::MinecraftConnection& mc, int x, int y, int z, bool isHorizontal, Building& building);
int getDoorPosition(int overlapStart, int overlapEnd, Building& building, bool testMode);
void subdivideRecursively(mcpp::MinecraftConnection& mc, const SubdivisionArea& area, std::vector<Room>& rooms, int& largeRoomCount, Building& building, bool testMode);
void placeDoorsForAllRooms(mcpp::MinecraftConnection& mc, std::vector<Room>& rooms, Building& building, bool testMode);
bool roomsOverlapInZ(const Room& a, const Room& b);
bool roomsOverlapInX(const Room& a, const Room& b);
void interiorWall(mcpp::MinecraftConnection& mc, int x1, int z1, int x2, int z2, int y, int height, bool isHorizontal);
int splitThePosition(int min, int max, int size, Building& building, bool testMode);
bool canSubdivideVertically(const SubdivisionArea& area);
bool canSubdivideHorizontally(const SubdivisionArea& area);


//Furniture decoration functions in decorate.h





#endif
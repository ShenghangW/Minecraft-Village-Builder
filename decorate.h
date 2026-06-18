#ifndef DECORATE_H
#define DECORATE_H
#include <mcpp/mcpp.h>
#include "room.h"
#include "plots.h"
// Block IDs for every piece of furniture
const int BED_ID = 26;
const int IRON_BLOCK_ID = 42;
const int BOOKSHELF_ID = 47;
const int CHEST_ID = 54;
const int FURNACE_ID = 61;
const int LEVER_ID = 69;
const int STONE_PRESSURE_PLATE_ID = 70;
const int IRON_DOOR_ID = 71;
const int OAK_FENCE_ID = 85;
const int CAULDRON_ID = 118;
const int REDSTONE_LAMP_ID = 124;
const int HOPPER_ID = 154;
const int QUARTZ_BLOCK_ID = 155;
const int QUARTZ_STAIRS_ID = 156;
const int CARPET_ID = 171;
const int WATER_ID = 9;
const int SMOKER_ID = 453;
const int OAK_STAIRS = 53;
// For every bed, i designed to face west only
const int WEST_BED_HEAD = 9;
const int WEST_BED_FOOT = 1;
const int IRON_DOOR_UPPER = 9;
const int IRON_DOOR_LOWER = 1;
const int LEVER_FACING_WEST = 2;
const int QUARTZ_STAIRS_UPSIDE_DOWN = 6;
const int PRESSURE_PLATE_DATA = 0;

// Carpet color that are going to be used
const int BLUE_CARPET = 11;
const int LIGHT_BLUE_CARPET = 3;
const int RED_CARPET = 14;

// Room thresholds for lamp placements
const int SMALL_ROOM = 20;
const int MEDIUM_ROOM = 30;
const int ONE_LAMP = 1;
const int TWO_LAMPS = 2;
const int THREE_LAMPS = 3;

// Furniture and Decoration
// i got to add functions for each theme to place the furniture in game
void addKitchenFurniture(mcpp::MinecraftConnection& mc, Room& room);
void addBedroomFurniture(mcpp::MinecraftConnection& mc, Room& room);
void addLivingRoomFurniture(mcpp::MinecraftConnection& mc, Room& room);
void addBathRoomFurniture(mcpp::MinecraftConnection& mc, Room& room);
void assignFurniture(mcpp::MinecraftConnection& mc, Room& room, bool testMode);
void addRoomLamps(mcpp::MinecraftConnection& mc, const Room& room);
// Decorating exterior wall
void addWindows(mcpp::MinecraftConnection& mc, Room& room, const mcpp::Coordinate& mainEntranceCoord, const Plot& plot);
#endif 
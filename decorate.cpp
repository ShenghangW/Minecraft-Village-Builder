#include "decorate.h"
#include "task_b.h"
#include <iostream>


// Adding lights to each room
void addRoomLamps(mcpp::MinecraftConnection& mc, Room& room) {
    // declare and assign the room dimension 
    int width = room.getWidth();
    int depth = room.getDepth();
    int area = width * depth;
    int y = room.getOrigin().y;

    int minX = room.getOrigin().x;
    int maxX = room.getBound().x;
    int minZ = room.getOrigin().z;
    int maxZ = room.getBound().z;
    // if the room is small sized 1 lamp will be placed
     // if the room is medium sized 2 lamps will be placed
      // if the room is large 3 lamps will be placed

    int numLamps = 1;
    if (area >= MEDIUM_ROOM) {
        numLamps = 3;
    } else if (area >= SMALL_ROOM) {
        numLamps = 2;
    }
    

    if (numLamps == 1) {
        // Place one lamp in the center of the room
        int lampX = (minX + maxX) / 2;
        int lampZ = (minZ + maxZ) / 2;
        int ceilingY = y + EXTERIOR_HEIGHT - 1;
         // Check if any of the lamp positions conflict with existing furniture
        mcpp::Coordinate ceilingCoord(lampX, ceilingY, lampZ);
        mcpp::Coordinate fence1Coord(lampX, ceilingY - 1, lampZ);
        mcpp::Coordinate lampCoord(lampX, ceilingY - 2, lampZ);
        
        // Only place lamp if no furniture exists at any of these positions
        if (!room.hasFurnitureAtCoord(ceilingCoord) &&  !room.hasFurnitureAtCoord(fence1Coord) &&  !room.hasFurnitureAtCoord(lampCoord)) {
      // Fence attached to ceiling
      mc.setBlock(mcpp::Coordinate(lampX, ceilingY, lampZ), mcpp::Blocks::OAK_FENCE);  
      // Second fence    
mc.setBlock(mcpp::Coordinate(lampX, ceilingY - 1, lampZ), mcpp::Blocks::OAK_FENCE); 
mc.doCommand("setblock " + std::to_string(lampX) + " " + std::to_string(ceilingY - 2) + " " + std::to_string(lampZ) + " redstone_lamp[lit=true]");
              room.addFurnitures(lampCoord, 124);
        
    } 
}else if (numLamps == 2) {
        // Place lamps in opposite corners 
        // 1 block away from wall
        int offsetX = 1; 
        int offsetZ = 1;
        
        // First lamp near one corner
        int lamp1X = minX + offsetX;
        int lamp1Z = minZ + offsetZ;
        
        mc.setBlock(mcpp::Coordinate(lamp1X, y, lamp1Z), mcpp::Blocks::OAK_FENCE);
        mc.setBlock(mcpp::Coordinate(lamp1X, y + 1, lamp1Z), mcpp::Blocks::OAK_FENCE);
        mc.doCommand("setblock " + std::to_string(lamp1X) + " " + std::to_string(y + 2) + " " + std::to_string(lamp1Z) + " redstone_lamp[lit=true]");
       // Adding furnitures to the furniture vector with its id and coordinates
        room.addFurnitures(mcpp::Coordinate(lamp1X, y, lamp1Z), 124);
        
        // Second lamp wil place the opposite corner
        int lamp2X = maxX - offsetX;
        int lamp2Z = maxZ - offsetZ;
        
        mc.setBlock(mcpp::Coordinate(lamp2X, y, lamp2Z), mcpp::Blocks::OAK_FENCE);
        mc.setBlock(mcpp::Coordinate(lamp2X, y + 1, lamp2Z), mcpp::Blocks::OAK_FENCE);
        mc.doCommand("setblock " + std::to_string(lamp2X) + " " + std::to_string(y + 2) + " " + std::to_string(lamp2Z) + " redstone_lamp[lit=true]");
       // Adding furnitures to the furniture vector with its id and coordinates
        room.addFurnitures(mcpp::Coordinate(lamp2X, y, lamp2Z), 124);
        
    } else if (numLamps == 3) {
        // Place three lamps two in opposite corners and one in the center
        int offsetX = 1;
        int offsetZ = 1;
        
        // First lamp one corner
        int lamp1_X = minX + offsetX;
        int lamp1_Z = minZ + offsetZ;
        
        mc.setBlock(mcpp::Coordinate(lamp1_X, y, lamp1_Z), mcpp::Blocks::OAK_FENCE);
        mc.setBlock(mcpp::Coordinate(lamp1_X, y + 1, lamp1_Z), mcpp::Blocks::OAK_FENCE);
        mc.doCommand("setblock " + std::to_string(lamp1_X) + " " + std::to_string(y + 2) + " " + std::to_string(lamp1_Z) + " redstone_lamp[lit=true]");
       // Adding furnitures to the furniture vector with its id and coordinates
        room.addFurnitures(mcpp::Coordinate(lamp1_X, y, lamp1_Z), 124);
        
        // Second lamp in the center
        int lamp2_X = (minX + maxX) / 2;
int lamp2_Z = (minZ + maxZ) / 2;

if (width >= depth) {
    // Wider room to shift sideways
    lamp2_X += 1; 
} 
else {
    // Deeper room to shift front or back
    lamp2_Z += 1; 
}
        
        mc.setBlock(mcpp::Coordinate(lamp2_X, y, lamp2_Z), mcpp::Blocks::OAK_FENCE);
        mc.setBlock(mcpp::Coordinate(lamp2_X, y + 1, lamp2_Z), mcpp::Blocks::OAK_FENCE);
        mc.doCommand("setblock " + std::to_string(lamp2_X) + " " + std::to_string(y + 2) + " " + std::to_string(lamp2_Z) + " redstone_lamp[lit=true]");
        room.addFurnitures(mcpp::Coordinate(lamp2_X, y, lamp2_Z), 124);
        
        // Third lamp opposite corner
        int lamp3_X = maxX - offsetX;
        int lamp3_Z = maxZ - offsetZ;
        
        mc.setBlock(mcpp::Coordinate(lamp3_X, y, lamp3_Z), mcpp::Blocks::OAK_FENCE);
        mc.setBlock(mcpp::Coordinate(lamp3_X, y + 1, lamp3_Z), mcpp::Blocks::OAK_FENCE);
        mc.doCommand("setblock " + std::to_string(lamp3_X) + " " + std::to_string(y + 2) + " " + std::to_string(lamp3_Z) + " redstone_lamp[lit=true]");
        room.addFurnitures(mcpp::Coordinate(lamp3_X, y, lamp3_Z), 124);
    }
}

void addKitchenFurniture(mcpp::MinecraftConnection& mc, Room& room) {
int maxX = room.getBound().x;
 int minZ = room.getOrigin().z;
 int maxZ = room.getBound().z;
 int y = room.getOrigin().y;

 int fridgeX = maxX - 2;
 int fridgeZ = (minZ + maxZ) / 2;
 

// iron door upper and lower half
 mc.setBlock(mcpp::Coordinate(fridgeX, y + 1, fridgeZ), mcpp::BlockType(IRON_DOOR_ID, IRON_DOOR_UPPER));
 mc.setBlock(mcpp::Coordinate(fridgeX, y, fridgeZ), mcpp::BlockType(IRON_DOOR_ID, IRON_DOOR_LOWER)); 

 // Iron blocks
mc.setBlock(mcpp::Coordinate(fridgeX, y, fridgeZ - 1), mcpp::BlockType(IRON_BLOCK_ID, 0));
    mc.setBlock(mcpp::Coordinate(fridgeX, y + 1, fridgeZ - 1), mcpp::BlockType(IRON_BLOCK_ID, 0));
// pressure palte to open the fridge
int plateX = fridgeX;
int plateY = y; 
 int plateZ = fridgeZ + 1;
 mc.setBlock(mcpp::Coordinate(plateX, plateY, plateZ), mcpp::BlockType(STONE_PRESSURE_PLATE_ID, PRESSURE_PLATE_DATA));
 
room.addFurnitures(mcpp::Coordinate(fridgeX, y, fridgeZ), STONE_PRESSURE_PLATE_ID);

    int stoveX = fridgeX + 1; 
    int stoveZ = fridgeZ;

    // Smoker as a stove
   mc.doCommand("setblock " + std::to_string(stoveX) + " " + std::to_string(y) + " " + std::to_string(stoveZ) + " minecraft:smoker");
    
    // Hopper above 
    mc.setBlock(mcpp::Coordinate(stoveX, y + 2, stoveZ), mcpp::Blocks::HOPPER);
    // brick wall from the ceiling
    int ceilingY = y + EXTERIOR_HEIGHT - 1;
    mc.doCommand("setblock " + std::to_string(stoveX) + " " + std::to_string(ceilingY) + " " + std::to_string(stoveZ) + " minecraft:stone_brick_wall");
    mc.doCommand("setblock " + std::to_string(stoveX) + " " + std::to_string(ceilingY - 1) + " " + std::to_string(stoveZ) + " minecraft:stone_brick_wall");
    mc.doCommand("setblock " + std::to_string(stoveX) + " " + std::to_string(ceilingY - 2) + " " + std::to_string(stoveZ) + " minecraft:stone_brick_wall");


    // Adding all the furniture
    // Smoker
    room.addFurnitures(mcpp::Coordinate(stoveX, y, stoveZ), SMOKER_ID); 
 // Hopper
    room.addFurnitures(mcpp::Coordinate(stoveX, y + 2, stoveZ), HOPPER_ID); 
   
}

void addBedroomFurniture(mcpp::MinecraftConnection& mc, Room& room) {
    int minX = room.getOrigin().x;
    int maxX = room.getBound().x;
    int minZ = room.getOrigin().z;
    int maxZ = room.getBound().z;
    int y = room.getOrigin().y;

    int width = room.getWidth();
    int depth = room.getDepth();

    // Check if room is big enough for beds
    if (width <= 4 && depth <= 4) {
     int bedZ1 = (minZ + maxZ) / 2 - 1;
    int bedFootX = maxX - 2;
    int bedHeadX = bedFootX - 1;
    mcpp::Coordinate foot(bedFootX, y, bedZ1);
    mcpp::Coordinate head(bedHeadX, y, bedZ1);
        mc.setBlock(head, mcpp::BlockType(26, 9)); 
        mc.setBlock(foot, mcpp::BlockType(26, 1)); 

            room.addFurnitures(head, 26);
            room.addFurnitures(foot, 26);
        for (int x = minX; x <= maxX; x++) {
        for (int z = minZ; z <= maxZ; z++) {
            mcpp::Coordinate floorCoord(x, y, z);
            if (!room.hasFurnitureAtCoord(floorCoord)) {
            int dx = x - minX;
            int dz = z - minZ;
            mcpp::BlockType carpetType;
             if ((dx + dz) % 2 == 0) {
                // Blue carpet
                carpetType = mcpp::BlockType(CARPET_ID, BLUE_CARPET); 
            } else {
                // Light blue carpet
                carpetType = mcpp::BlockType(CARPET_ID, LIGHT_BLUE_CARPET);  
            }
                mc.setBlock(floorCoord, carpetType); 
                room.addFurnitures(floorCoord, CARPET_ID);
            }
        }
    }
        return; 
    }

    
    int bedZ1 = (minZ + maxZ) / 2 - 1;
    int bedZ2 = (minZ + maxZ) / 2 + 1;
    int bedFootX = maxX - 2;
    int bedHeadX = bedFootX - 1;
    
    // First bed 
    {
        mcpp::Coordinate foot(bedFootX, y, bedZ1);
        mcpp::Coordinate head(bedHeadX, y, bedZ1);

        if (!room.hasFurnitureAtCoord(foot) && !room.hasFurnitureAtCoord(head)) {
            mc.setBlock(foot, mcpp::Blocks::AIR);
            mc.setBlock(head, mcpp::Blocks::AIR);

            // Bed 
           mc.setBlock(head, mcpp::BlockType(BED_ID, WEST_BED_HEAD)); 
           mc.setBlock(foot, mcpp::BlockType(BED_ID, WEST_BED_FOOT)); 

            room.addFurnitures(head, BED_ID);
            room.addFurnitures(foot, BED_ID);
        } 
    }

    // Second bed for larger rooms 5x5
    if (width > 5 && depth > 5) {
        mcpp::Coordinate foot2(bedFootX, y, bedZ2);
        mcpp::Coordinate head2(bedHeadX, y, bedZ2);

        if (!room.hasFurnitureAtCoord(foot2) && !room.hasFurnitureAtCoord(head2)) {
            mc.setBlock(foot2, mcpp::Blocks::AIR);
            mc.setBlock(head2, mcpp::Blocks::AIR);

            // Bed
            // Head 
            mc.setBlock(head2, mcpp::BlockType(BED_ID, WEST_BED_HEAD)); 
            // Foot 
            mc.setBlock(foot2, mcpp::BlockType(BED_ID, WEST_BED_FOOT)); 

            room.addFurnitures(head2, BED_ID);
            room.addFurnitures(foot2, BED_ID);
        } 
    }

    // Blue carpet after beds are placed
    for (int x = minX; x <= maxX; x++) {
        for (int z = minZ; z <= maxZ; z++) {
            mcpp::Coordinate floorCoord(x, y, z);
            if (!room.hasFurnitureAtCoord(floorCoord)) {
            int dx = x - minX;
            int dz = z - minZ;
            mcpp::BlockType carpetType;
             if ((dx + dz) % 2 == 0) {
                // Blue carpet
                carpetType = mcpp::BlockType(CARPET_ID, BLUE_CARPET); 
            } else {
                // Light blue carpet
                carpetType = mcpp::BlockType(CARPET_ID, LIGHT_BLUE_CARPET);  
            }
                mc.setBlock(floorCoord, carpetType); 
                room.addFurnitures(floorCoord, CARPET_ID);
            }
        }
    }
}

void addLivingRoomFurniture(mcpp::MinecraftConnection& mc, Room& room) {
    int minX = room.getOrigin().x;
    int maxX = room.getBound().x;
    int minZ = room.getOrigin().z;
    int maxZ = room.getBound().z;
    int y = room.getOrigin().y;

    int centerX = (minX + maxX) / 2;
    int centerZ = (minZ + maxZ) / 2;

    // To check if the wall in north is clear
    bool northWallClear = true;
    for (int x = centerX; x < centerX + 2; x++) {
        mcpp::BlockType blockAtWall = mc.getBlock(mcpp::Coordinate(x, y, minZ - 1));
        mcpp::BlockType blockAboveWall = mc.getBlock(mcpp::Coordinate(x, y + 1, minZ - 1));

        if (blockAtWall.id == AIR_BLOCK_ID || blockAboveWall.id == AIR_BLOCK_ID || blockAtWall.id == OAK_DOOR_ID || blockAboveWall.id == OAK_DOOR_ID) {
            northWallClear = false;
        }
    }

    if (northWallClear) {
        for (int x = centerX; x < centerX + 2; x++) {
            mc.setBlock(mcpp::Coordinate(x, y, minZ), mcpp::Blocks::BOOKSHELF);
            mc.setBlock(mcpp::Coordinate(x, y + 1, minZ), mcpp::Blocks::BOOKSHELF);
            mc.setBlock(mcpp::Coordinate(x, y + 2, minZ), mcpp::Blocks::OAK_WOOD_SLAB);
            room.addFurnitures(mcpp::Coordinate(x, y, minZ), BOOKSHELF_ID);
        }
    }

    // To check if the wall in south is clear
    bool southWallClear = true;
    for (int x = centerX; x < centerX + 2; x++) {
        mcpp::BlockType blockAtWall = mc.getBlock(mcpp::Coordinate(x, y, maxZ + 1));
        mcpp::BlockType blockAboveWall = mc.getBlock(mcpp::Coordinate(x, y + 1, maxZ + 1));

        if (blockAtWall.id == 0 || blockAboveWall.id == 0 ||   blockAtWall.id == 64 || blockAboveWall.id == 64) {
            southWallClear = false;
        }
    }

    if (southWallClear) {
        for (int x = centerX; x < centerX + 2; x++) {
            mc.setBlock(mcpp::Coordinate(x, y, maxZ), mcpp::Blocks::BOOKSHELF);
            mc.setBlock(mcpp::Coordinate(x, y + 1, maxZ), mcpp::Blocks::BOOKSHELF);
            mc.setBlock(mcpp::Coordinate(x, y + 2, maxZ), mcpp::Blocks::OAK_WOOD_SLAB);
            room.addFurnitures(mcpp::Coordinate(x, y, maxZ), BOOKSHELF_ID);
        }
    }

    // To check if the wall in west is clear
    bool westWallClear = true;
    for (int z = centerZ; z < centerZ + 2; z++) {
        mcpp::BlockType blockAtWall = mc.getBlock(mcpp::Coordinate(minX - 1, y, z));
        mcpp::BlockType blockAboveWall = mc.getBlock(mcpp::Coordinate(minX - 1, y + 1, z));

        if (blockAtWall.id == 0 || blockAboveWall.id == 0 ||  blockAtWall.id == 64 || blockAboveWall.id == 64) {
            westWallClear = false;
        }
    }

    if (westWallClear) {
        for (int z = centerZ; z < centerZ + 2; z++) {
            mc.setBlock(mcpp::Coordinate(minX, y, z), mcpp::Blocks::BOOKSHELF);
            mc.setBlock(mcpp::Coordinate(minX, y + 1, z), mcpp::Blocks::BOOKSHELF);
            mc.setBlock(mcpp::Coordinate(minX, y + 2, z), mcpp::Blocks::OAK_WOOD_SLAB);
            room.addFurnitures(mcpp::Coordinate(minX, y, z), BOOKSHELF_ID);
        }
    }

    // To check if the wall in east is clear
    bool eastWallClear = true;
    for (int z = centerZ; z < centerZ + 2; z++) {
        mcpp::BlockType blockAtWall = mc.getBlock(mcpp::Coordinate(maxX + 1, y, z));
        mcpp::BlockType blockAboveWall = mc.getBlock(mcpp::Coordinate(maxX + 1, y + 1, z));

        if (blockAtWall.id == 0 || blockAboveWall.id == 0 || blockAtWall.id == 64 || blockAboveWall.id == 64) {
            eastWallClear = false;
        }
    }

    if (eastWallClear) {
        for (int z = centerZ; z < centerZ + 2; z++) {
            mc.setBlock(mcpp::Coordinate(maxX, y, z), mcpp::Blocks::BOOKSHELF);
            mc.setBlock(mcpp::Coordinate(maxX, y + 1, z), mcpp::Blocks::BOOKSHELF);
            mc.setBlock(mcpp::Coordinate(maxX, y + 2, z), mcpp::Blocks::OAK_WOOD_SLAB);
            room.addFurnitures(mcpp::Coordinate(maxX, y, z), BOOKSHELF_ID);
        }
    }

 // Red carpet for each block for the whole room
 // Loop through every tile in the room
    for (int x = minX; x <= maxX; x++) {
        for (int z = minZ; z <= maxZ; z++) {
            mcpp::Coordinate floorCoord(x, y, z);
            if (!room.hasFurnitureAtCoord(floorCoord)) {
                mc.setBlock(floorCoord, mcpp::BlockType(CARPET_ID, RED_CARPET));  
                room.addFurnitures(floorCoord, CARPET_ID);
            }
        }
    }
}



void addBathRoomFurniture(mcpp::MinecraftConnection& mc, Room& room) {
    int minX = room.getOrigin().x;
    int maxX = room.getBound().x;
    int minZ = room.getOrigin().z;
    int maxZ = room.getBound().z;
    int y = room.getOrigin().y;
// Adding a toilet in the bathroom
 int toiletX = maxX - 2;
 int toiletZ = (minZ + maxZ) / 2;

 mc.setBlock(mcpp::Coordinate(toiletX, y, toiletZ), mcpp::Blocks::QUARTZ_BLOCK);
mc.setBlock(mcpp::Coordinate(toiletX, y + 1, toiletZ), mcpp::Blocks::QUARTZ_BLOCK);

 int toiletBowl_Z = toiletZ - 1;
 mc.setBlock( mcpp::Coordinate(toiletX, y, toiletBowl_Z), mcpp::Blocks::QUARTZ_STAIRS.with_mod(6));
 // bowl
mc.setBlock( mcpp::Coordinate(toiletX, y + 1, toiletBowl_Z), mcpp::Blocks::	IRON_TRAPDOOR);
// Lever
mc.setBlock(mcpp::Coordinate(toiletX - 1, y + 1, toiletZ), mcpp::BlockType(LEVER_ID, LEVER_FACING_WEST));
// Quartz 
room.addFurnitures(mcpp::Coordinate(toiletX, y, toiletZ), QUARTZ_BLOCK_ID);
// Tank    
room.addFurnitures(mcpp::Coordinate(toiletX, y + 1, toiletZ), QUARTZ_BLOCK_ID);
    // Stair bowl
    room.addFurnitures(mcpp::Coordinate(toiletX, y, toiletBowl_Z), QUARTZ_STAIRS_ID);   
     // Lever
room.addFurnitures(mcpp::Coordinate(toiletX - 1, y + 1, toiletZ - 1), LEVER_ID);
    // Bathtub
    int bathtubX = minX + 1;
int bathtubZ = minZ + 2; 
for (int dx = 0; dx < 2; dx++) {
    mcpp::Coordinate tubFloor(bathtubX + dx, y, bathtubZ);
    if (!room.hasFurnitureAtCoord(tubFloor)) {
        // Replacing floor as water
        mc.doCommand("setblock " + std::to_string(tubFloor.x) + " " +  std::to_string(y - 1) + " " + std::to_string(tubFloor.z) + " water[level=0]");
        room.addFurnitures(tubFloor, 9);
    }
}
    
}


 

void assignFurniture(mcpp::MinecraftConnection& mc, Room& room, bool
 testMode) {
    std::string theme = room.getTheme();
    //start testmode
    if (testMode) {
        // Every furniture will be placed top left corner
         int topLeftX = room.getOrigin().x + 1;
        int topLeftZ = room.getOrigin().z + 1;
        int y = room.getOrigin().y;

        if (theme == "Kitchen") {
            // A simple sink in top left position
           mc.doCommand("setblock " + std::to_string(topLeftX) + " " + std::to_string(y) + " " + std::to_string(topLeftZ) + " minecraft:water_cauldron[level=3]");
          std::cout << "  Furniture 1 Sink placed at (" << topLeftX << "," << y << "," << topLeftZ << ")" << std::endl;
           
          // Item 2 counter next to sink
            if (topLeftX + 1 <= room.getBound().x) {
                mc.doCommand("setblock " + std::to_string(topLeftX + 1) + " " + std::to_string(y) + " " + std::to_string(topLeftZ) + " minecraft:deepslate_brick_wall");
                mc.doCommand("setblock " + std::to_string(topLeftX + 1) + " " + std::to_string(y + 1) + " " + std::to_string(topLeftZ) + " minecraft:gray_carpet");
                room.addFurnitures(mcpp::Coordinate(topLeftX + 1, y, topLeftZ), CAULDRON_ID);
                std::cout << "  Furniture 2 Counter placed at (" << (topLeftX + 1) << "," << y << "," << topLeftZ << ")" << std::endl;
            }
             // Item 3 Furnance
            if (topLeftX + 2 <= room.getBound().x) {
                mc.setBlock(mcpp::Coordinate(topLeftX + 2, y, topLeftZ), mcpp::Blocks::FURNACE);
                room.addFurnitures(mcpp::Coordinate(topLeftX + 2, y, topLeftZ), FURNACE_ID);
                std::cout << "  Furniture 3 Furnance placed at (" << (topLeftX + 2) << "," << y << "," << topLeftZ << ")" << std::endl;
            }
        
           
            } else if (theme == "Bedroom") {
            // Place bed foot at top left facing west
            mc.setBlock(mcpp::Coordinate(topLeftX, y, topLeftZ), mcpp::BlockType(BED_ID, WEST_BED_HEAD));
            mc.setBlock(mcpp::Coordinate(topLeftX + 1, y, topLeftZ), mcpp::BlockType(BED_ID, WEST_BED_FOOT));
           
            room.addFurnitures(mcpp::Coordinate(topLeftX, y, topLeftZ), BED_ID);
            std::cout << "  Furniture 1 Bed placed at (" << topLeftX << "," << y << "," << topLeftZ << ")" << std::endl;
        
            // Item 2 Chest next to bed
            if (topLeftZ + 1 <= room.getBound().z) {
                mc.setBlock(mcpp::Coordinate(topLeftX, y, topLeftZ + 1), mcpp::Blocks::CHEST);
                room.addFurnitures(mcpp::Coordinate(topLeftX, y, topLeftZ + 1), CHEST_ID);
                std::cout << "  Furniture 2 Chest placed at (" << topLeftX << "," << y << "," << (topLeftZ + 1) << ")" << std::endl;
            }
            

        // Item 3 Bedside table 
            if (topLeftX + 2 <= room.getBound().x) {
                mc.setBlock(mcpp::Coordinate(topLeftX + 2, y, topLeftZ), mcpp::Blocks::OAK_FENCE);
                mc.setBlock(mcpp::Coordinate(topLeftX + 2, y + 1, topLeftZ), mcpp::Blocks::BLACK_CARPET);
                room.addFurnitures(mcpp::Coordinate(topLeftX + 2, y, topLeftZ), OAK_FENCE_ID);
               
                std::cout << "  Furniture 3 Bedside table placed at (" << (topLeftX + 2) << "," << y << "," << topLeftZ << ")" << std::endl;
            }
        
        
        } else if (theme == "Bathroom") {
            // Place toilet in top-left
            mc.setBlock(mcpp::Coordinate(topLeftX, y, topLeftZ), mcpp::Blocks::QUARTZ_BLOCK);
            room.addFurnitures(mcpp::Coordinate(topLeftX, y, topLeftZ), 155);
        std::cout << "  Furniture 1 Toilet placed at (" << topLeftX << "," << y << "," << topLeftZ << ")" << std::endl;
       
        // Item 2 Sink
        if (topLeftX + 1 <= room.getBound().x) {
               mc.doCommand("setblock " + std::to_string(topLeftX + 1) + " " + std::to_string(y) + " " + std::to_string(topLeftZ) + " minecraft:water_cauldron[level=3]");
                room.addFurnitures(mcpp::Coordinate(topLeftX + 1, y, topLeftZ), CAULDRON_ID);
                std::cout << "  Furniture 2 Sink placed at (" << (topLeftX + 1) << "," << y << "," << topLeftZ << ")" << std::endl;
            }

            // Item 3 Bathtub
            if (topLeftZ + 1 <= room.getBound().z) {
                mc.doCommand("setblock " + std::to_string(topLeftX) + " " + std::to_string(y - 1) + " " + std::to_string(topLeftZ + 1) + " water[level=0]");
                room.addFurnitures(mcpp::Coordinate(topLeftX, y, topLeftZ + 1), 9);
                std::cout << "  Furniture 3 Small Bathtub placed at (" << topLeftX << "," << y << "," << (topLeftZ + 1) << ")" << std::endl;
            }
            
        } else if (theme == "Living Room") {
            // Place bookshelf in top-left
            mc.setBlock(mcpp::Coordinate(topLeftX, y, topLeftZ), mcpp::Blocks::BOOKSHELF);
            mc.setBlock(mcpp::Coordinate(topLeftX, y + 1, topLeftZ), mcpp::Blocks::BOOKSHELF);
            room.addFurnitures(mcpp::Coordinate(topLeftX, y, topLeftZ), BOOKSHELF_ID);
            std::cout << "  Furniture 1 Bookshelf placed at (" << topLeftX << "," << y << "," << topLeftZ << ")" << std::endl;

            // Item 2 Table 
            if (topLeftX + 1 <= room.getBound().x) {
                mc.setBlock(mcpp::Coordinate(topLeftX + 1, y, topLeftZ), mcpp::Blocks::OAK_FENCE);
                mc.setBlock(mcpp::Coordinate(topLeftX + 1, y + 1, topLeftZ), mcpp::Blocks::BLACK_CARPET);
                room.addFurnitures(mcpp::Coordinate(topLeftX + 1, y, topLeftZ), OAK_FENCE_ID);
                std::cout << "  Furniture 2 Table placed at (" << (topLeftX + 1) << "," << y << "," << topLeftZ << ")" << std::endl;
            }

             // Item 3 Chair 
            if (topLeftX + 2 <= room.getBound().x) {
                mc.setBlock(mcpp::Coordinate(topLeftX + 2, y, topLeftZ), mcpp::Blocks::OAK_WOOD_STAIRS);
                room.addFurnitures(mcpp::Coordinate(topLeftX + 2, y, topLeftZ), OAK_STAIRS);
                std::cout << "  Furniture 3 Chair placed at (" << (topLeftX + 2) << "," << y << "," << topLeftZ << ")" << std::endl;
            }
        }

    } else {
   
    
    // Adding furniture to whatever theme the room is
    //     // i got to make the add the specefic theme function and call it from here
    if (theme == "Kitchen") {
    addKitchenFurniture(mc, room);
    } else if (theme == "Bedroom") {
        addBedroomFurniture(mc,room);
    } else if (theme == "Bathroom") {
        addBathRoomFurniture(mc, room);    
    } else if (theme == "Living Room") {
        addLivingRoomFurniture(mc, room); 
    }

    // Adding lamps to every room
    addRoomLamps(mc, room);
}
    std::cout << "Finished decorating " << theme << std::endl;
}

void addWindows(mcpp::MinecraftConnection& mc, Room& room, const mcpp::Coordinate& mainEntranceCoord, const Plot& plot) {
    int minX = room.getOrigin().x;
    int maxX = room.getBound().x;
    int minZ = room.getOrigin().z;
    int maxZ = room.getBound().z;
    int y = room.getOrigin().y;
    int roomWidth = maxX - minX + 1;
    int roomDepth = maxZ - minZ + 1;
const int MIN_WIDTH_FOR_WINDOW = 5;
    // Checking if the walls are exterior through the plot origin and bound
   
    bool northIsExterior = (minZ == plot.origin.z + 1);
    bool southIsExterior = (maxZ == plot.bound.z - 1);
    bool westIsExterior = (minX == plot.origin.x + 1);
    bool eastIsExterior = (maxX == plot.bound.x - 1);
    
    int northWallZ = minZ - 1;
    int southWallZ = maxZ + 1;
    int westWallX = minX - 1;
    int eastWallX = maxX + 1;
    
    // Checking if the main entrance is on the exterior wall
    bool entranceOnNorthWall = (mainEntranceCoord.z == plot.origin.z) &&  (mainEntranceCoord.x >= minX && mainEntranceCoord.x <= maxX);
    
    bool entranceOnSouthWall = (mainEntranceCoord.z == plot.bound.z) && (mainEntranceCoord.x >= minX && mainEntranceCoord.x <= maxX);
    
    bool entranceOnWestWall = (mainEntranceCoord.x == plot.origin.x) && (mainEntranceCoord.z >= minZ && mainEntranceCoord.z <= maxZ);

    bool entranceOnEastWall = (mainEntranceCoord.x == plot.bound.x) && (mainEntranceCoord.z >= minZ && mainEntranceCoord.z <= maxZ);
    
    // North wall only place the window if the entrance door is not there
    if (northIsExterior && roomWidth >= MIN_WIDTH_FOR_WINDOW && !entranceOnNorthWall) {
        int windowCenterX = (minX + maxX) / 2;
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = 1; dy <= 4; dy++) {
                mc.setBlock(mcpp::Coordinate(windowCenterX + dx, y + dy, northWallZ), mcpp::Blocks::GLASS);
            }
        }
    }
    
    // South wall only place the window if the entrance door is not there
    
if (southIsExterior && roomWidth >= MIN_WIDTH_FOR_WINDOW && !entranceOnSouthWall) {
        int windowCenterX = (minX + maxX) / 2;
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = 1; dy <= 4; dy++) {
                mc.setBlock(mcpp::Coordinate(windowCenterX + dx, y + dy, southWallZ), mcpp::Blocks::GLASS);
            }
        }
    }
    
    // West wall only place the window if the entrance door is not there
    if (westIsExterior && roomDepth >= MIN_WIDTH_FOR_WINDOW && !entranceOnWestWall) {
        int windowCenterZ = (minZ + maxZ) / 2;
        for (int dz = -1; dz <= 1; dz++) {
            for (int dy = 1; dy <= 4; dy++) {
                mc.setBlock(mcpp::Coordinate(westWallX, y + dy, windowCenterZ + dz), mcpp::Blocks::GLASS);
            }
        }
    }
    
    // East wall only place the window if the entrance door is not there
    if (eastIsExterior && roomDepth >= MIN_WIDTH_FOR_WINDOW && !entranceOnEastWall) {
        int windowCenterZ = (minZ + maxZ) / 2;
        for (int dz = -1; dz <= 1; dz++) {
            for (int dy = 1; dy <= 4; dy++) {
                mc.setBlock(mcpp::Coordinate(eastWallX, y + dy, windowCenterZ + dz), mcpp::Blocks::GLASS);
            }
        }
    }
}
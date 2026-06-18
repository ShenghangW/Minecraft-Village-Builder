    #include "task_b.h"
    #include "roomManager.h"
    #include "decorate.h" 
    // build exterior helper functions
    mcpp::Coordinate placeMainEntrance(mcpp::MinecraftConnection& mc, const Plot& plot, Building& building,  const std::vector<Room>& rooms, bool testMode) {
        // Placing main entrance door where the plot entrance was generated
        int doorX = plot.entrance.x;
        int doorY = plot.entrance.y;
        int doorZ = plot.entrance.z;
        
    std::cout << "\t Placing main entrance at generated plot (" 
                << doorX << ", " << doorY << ", " << doorZ << ")\n";
        
        // Clear door space
        for (int changeInY = 0; changeInY < DOOR_HEIGHT; changeInY++) {
            mc.setBlock(mcpp::Coordinate(doorX, doorY + changeInY, doorZ), mcpp::Blocks::AIR);
        }
    bool isHorizontal;
        if (doorZ == plot.bound.z || doorZ == plot.origin.z) {
        
            isHorizontal = true;  
        } else {
            isHorizontal = false; 
        }

    mcpp::BlockType lowerDoor, upperDoor;
        if (isHorizontal) {
    // South or North wall
            lowerDoor = mcpp::BlockType(OAK_DOOR_ID, 1); 
            upperDoor = mcpp::BlockType(OAK_DOOR_ID, 9);
        } else {
        // East or West wall
            lowerDoor = mcpp::BlockType(OAK_DOOR_ID, 0);
            upperDoor = mcpp::BlockType(OAK_DOOR_ID, 8);
        }
    int frontX = doorX;
    int frontZ = doorZ;
    int behindX = doorX;
    int behindZ = doorZ;
    // south
    if (doorZ == plot.bound.z) { 
            frontZ += 1; behindZ -= 1;
        } 
        // north
        else if (doorZ == plot.origin.z) { 
            frontZ -= 1; behindZ += 1;
        } 
        // east
        else if (doorX == plot.bound.x) { 
            frontX += 1; behindX -= 1;
        } 
        // west
        else if (doorX == plot.origin.x) { 
            frontX -= 1; behindX += 1;
        }
        // to check if in front of the door is blocked by the interior wall
    mcpp::BlockType frontLower = mc.getBlock(mcpp::Coordinate(frontX, doorY, frontZ));
    mcpp::BlockType frontUpper = mc.getBlock(mcpp::Coordinate(frontX, doorY + 1, frontZ));
    bool frontBlocked = !(frontLower.id == AIR_BLOCK_ID && frontUpper.id == AIR_BLOCK_ID);

    mcpp::BlockType behindLower = mc.getBlock(mcpp::Coordinate(behindX, doorY, behindZ));
    mcpp::BlockType behindUpper = mc.getBlock(mcpp::Coordinate(behindX, doorY + 1, behindZ));
    bool behindBlocked = !(behindLower.id == AIR_BLOCK_ID && behindUpper.id == AIR_BLOCK_ID);
    // If front is blocked but space behind is open, shift door sideways
    if (frontBlocked && !behindBlocked) {
        if (isHorizontal) {
            // shift sideways x
            int newDoorX = doorX + 1;
            mcpp::BlockType testLower = mc.getBlock(mcpp::Coordinate(newDoorX, doorY, doorZ));
            mcpp::BlockType testUpper = mc.getBlock(mcpp::Coordinate(newDoorX, doorY + 1, doorZ));
            if (testLower.id == AIR_BLOCK_ID && testUpper.id == AIR_BLOCK_ID) {
                doorX = newDoorX;
            }
            else { 
            // shifting left
            newDoorX = doorX - 1;
            testLower = mc.getBlock(mcpp::Coordinate(newDoorX, doorY, doorZ));
            testUpper = mc.getBlock(mcpp::Coordinate(newDoorX, doorY + 1, doorZ));
            if (testLower.id == AIR_BLOCK_ID && testUpper.id == AIR_BLOCK_ID) {
                doorX = newDoorX;
            }
        
            }
        } else {
            // shift Z axis
            int newDoorZ = doorZ + 1;
            mcpp::BlockType testLower = mc.getBlock(mcpp::Coordinate(doorX, doorY, newDoorZ));
            mcpp::BlockType testUpper = mc.getBlock(mcpp::Coordinate(doorX, doorY + 1, newDoorZ));
            if (testLower.id == AIR_BLOCK_ID && testUpper.id == AIR_BLOCK_ID) {
                doorZ = newDoorZ;
            }
             else {
        // Try shifting in the other direction 
        newDoorZ = doorZ - 1;
        testLower = mc.getBlock(mcpp::Coordinate(doorX, doorY, newDoorZ));
        testUpper = mc.getBlock(mcpp::Coordinate(doorX, doorY + 1, newDoorZ));
        if (testLower.id == AIR_BLOCK_ID && testUpper.id == AIR_BLOCK_ID) {
            doorZ = newDoorZ;
        }
    }
        }

    }

        mc.setBlock(mcpp::Coordinate(doorX, doorY + 1, doorZ), upperDoor);
        mc.setBlock(mcpp::Coordinate(doorX, doorY, doorZ), lowerDoor);
    building.addDoor(doorX, doorZ);

        return mcpp::Coordinate(doorX, doorY, doorZ);
    }


    void buildWalls(mcpp::MinecraftConnection& mc, int x, int z, int width, int depth, int y, int height) {
                        
        mcpp::BlockType wallBlock = mcpp::Blocks::SPRUCE_WOOD_PLANK;

        for (int dx = 0; dx < width; dx++) {
            for (int dy = 0; dy < height; dy++) {
                // outer wall for North
                mc.setBlock(mcpp::Coordinate(x + dx, y + dy, z), wallBlock); 
                // outer wall for South
                mc.setBlock(mcpp::Coordinate(x + dx, y + dy, z + depth - 1), wallBlock); 
            }
        }
    // Build west and east walls
        for (int dz = 0; dz < depth; dz++) {
            for (int dy = 0; dy < height; dy++) {
                // outer wall for West
                mc.setBlock(mcpp::Coordinate(x, y + dy, z + dz), wallBlock); 
                // outer wall for East
                mc.setBlock(mcpp::Coordinate(x + width - 1, y + dy, z + dz), wallBlock); 
            }
        }
    }


    void buildRoof(mcpp::MinecraftConnection& mc, int x, int z, int width, int depth, int y, const Plot& plot) {
        // to determine if the entrance runs north or south so the roof faces correctly
        bool runsNorthSouth = false;
        if (plot.entrance.z == plot.origin.z || plot.entrance.z == plot.bound.z) {
        // Roof slopes along X axis
            runsNorthSouth = false; 
        } 
        else if (plot.entrance.x == plot.origin.x || plot.entrance.x == plot.bound.x) {
        // Roof slopes along Z axis
            runsNorthSouth = true; 
        }
        int peakHeight;

        if (runsNorthSouth) {
            // Roof runs N or S, slopes in Z direction
            peakHeight = (depth + 2) / 2;
            
        } else {
            // Roof runs E or W, slopes in X direction  
            peakHeight = (width + 2) / 2;
        
        }
        uint8_t leftFacing, rightFacing;

        if (runsNorthSouth) {
            // Stairs face along Z axis when roof runs N or S
            // South stairs face south on north side
            leftFacing = 2;  
            // North stairs face north on south side
            rightFacing = 3; 
        } else {
            // Stairs face along X axis when roof runs E or W
            // East stairs face east on west side
            leftFacing = 0;  
            // West stairs face west on east side
            rightFacing = 1; 
        }

        int currentSize;

        for (int layer = 0; layer < peakHeight; layer++) {
            if (runsNorthSouth) {
                currentSize = depth + 2 - (2 * layer);
            } else {
                currentSize = width + 2 - (2 * layer);
            }
            if (runsNorthSouth) {
                int startZ = z - 1 + layer;
                for (int dx = 0; dx < width; dx++) {
                    if (currentSize > 0) {
                        // Left side north side, stairs face south
                        mc.setBlock(mcpp::Coordinate(x + dx, y + layer, startZ),  mcpp::Blocks::BRICK_STAIRS.with_mod(leftFacing));

                        // Right side south side, stairs face north
                        mc.setBlock(mcpp::Coordinate(x + dx, y + layer, startZ + currentSize - 1),  mcpp::Blocks::BRICK_STAIRS.with_mod(rightFacing));
            
                        // The middle to fill the gap
                        for (int dz = 1; dz < currentSize - 1; dz++) {
                            mc.setBlock(mcpp::Coordinate(x + dx, y + layer, startZ + dz), mcpp::Blocks::SPRUCE_WOOD_PLANK);
                        }
                    }
                }
                // Front overhang west side
                if (currentSize > 0) {
                    mc.setBlock(mcpp::Coordinate(x - 1, y + layer, startZ), mcpp::Blocks::BRICK_STAIRS.with_mod(leftFacing));
                    mc.setBlock(mcpp::Coordinate(x - 1, y + layer, startZ + currentSize - 1),   mcpp::Blocks::BRICK_STAIRS.with_mod(rightFacing));
                }

                // Back overhang east side
                if (currentSize > 0) {
                    mc.setBlock(mcpp::Coordinate(x + width, y + layer, startZ), mcpp::Blocks::BRICK_STAIRS.with_mod(leftFacing));
                    mc.setBlock(mcpp::Coordinate(x + width, y + layer, startZ + currentSize - 1), mcpp::Blocks::BRICK_STAIRS.with_mod(rightFacing));
                }
                
            } 
            else {
                int startX = x - 1 + layer;
                
                for (int dz = 0; dz < depth; dz++) {
                    if (currentSize > 0) {
                        // Left side facing east
                        mc.setBlock(mcpp::Coordinate(startX, y + layer, z + dz), 
                            mcpp::Blocks::BRICK_STAIRS.with_mod(leftFacing));
                        
                        // Right side facing west
                        mc.setBlock(mcpp::Coordinate(startX + currentSize - 1, y + layer, z + dz), 
                            mcpp::Blocks::BRICK_STAIRS.with_mod(rightFacing));
                        
                        // middle
                        for (int dx = 1; dx < currentSize - 1; dx++) {
                            mc.setBlock(mcpp::Coordinate(startX + dx, y + layer, z + dz), mcpp::Blocks::SPRUCE_WOOD_PLANK);
                        }
                    }
                }
                
                // front 
                if (currentSize > 0) {
                    mc.setBlock(mcpp::Coordinate(startX, y + layer, z - 1),  mcpp::Blocks::BRICK_STAIRS.with_mod(leftFacing));
                    mc.setBlock(mcpp::Coordinate(startX + currentSize - 1, y + layer, z - 1),  mcpp::Blocks::BRICK_STAIRS.with_mod(rightFacing));
                }
                
                // back overhang
                if (currentSize > 0) {
                    mc.setBlock(mcpp::Coordinate(startX, y + layer, z + depth), mcpp::Blocks::BRICK_STAIRS.with_mod(leftFacing));
                    mc.setBlock(mcpp::Coordinate(startX + currentSize - 1, y + layer, z + depth), mcpp::Blocks::BRICK_STAIRS.with_mod(rightFacing));
                }
            }
        }
        
        // Peak ridge
        int finalLayer = peakHeight - 1;
        int peakWidth; 

        if (runsNorthSouth) {
            peakWidth = depth + 2 - (2 * finalLayer);
        } else {
            peakWidth = width + 2 - (2 * finalLayer);
        }
        
        if (runsNorthSouth) {
            int peakZ = z - 1 + finalLayer;
            for (int dx = 0; dx < width; dx++) {
            
                if (peakWidth == 2) {
                    mc.setBlock(mcpp::Coordinate(x + dx, y + finalLayer, peakZ), mcpp::Blocks::BRICK_STAIRS.with_mod(leftFacing));
                    mc.setBlock(mcpp::Coordinate(x + dx, y + finalLayer, peakZ + 1),  mcpp::Blocks::BRICK_STAIRS.with_mod(rightFacing));
                } else if (peakWidth == 1) {
                    mc.setBlock(mcpp::Coordinate(x + dx, y + finalLayer, peakZ), mcpp::Blocks::BRICKS);
                }
            }
        
            // Peak front overhang
            if (peakWidth == 2) {
                mc.setBlock(mcpp::Coordinate(x - 1, y + finalLayer, peakZ), mcpp::Blocks::BRICK_STAIRS.with_mod(leftFacing));
                mc.setBlock(mcpp::Coordinate(x - 1, y + finalLayer, peakZ + 1), mcpp::Blocks::BRICK_STAIRS.with_mod(rightFacing));
            }
            
            // Peak back overhang
            if (peakWidth == 2) {
                mc.setBlock(mcpp::Coordinate(x + width, y + finalLayer, peakZ), mcpp::Blocks::BRICK_STAIRS.with_mod(leftFacing));
                mc.setBlock(mcpp::Coordinate(x + width, y + finalLayer, peakZ + 1), mcpp::Blocks::BRICK_STAIRS.with_mod(rightFacing));
            }
            
            // if the plot depth is odd
            if (depth % 2 != 0) {
                // middle point of the roof
                int centerZ = z + depth / 2;
                // one block above the final layer 
                int slabY = y + finalLayer + 1;  

                for (int dx = -1; dx <= width; dx++) {
                std::string slabCmd = "setblock " + std::to_string(x + dx) + " " + std::to_string(slabY) + " "  + std::to_string(centerZ) + " minecraft:brick_slab[type=bottom] replace";
                    mc.doCommand(slabCmd);
                }
            }
        } else {
            int peakX = x - 1 + finalLayer;
            
            for (int dz = 0; dz < depth; dz++) {
                if (peakWidth == 2) {
                    mc.setBlock(mcpp::Coordinate(peakX, y + finalLayer, z + dz), mcpp::Blocks::BRICK_STAIRS.with_mod(leftFacing));
                    mc.setBlock(mcpp::Coordinate(peakX + 1, y + finalLayer, z + dz), mcpp::Blocks::BRICK_STAIRS.with_mod(rightFacing));
                } else if (peakWidth == 1) {
                    mc.setBlock(mcpp::Coordinate(peakX, y + finalLayer, z + dz),  mcpp::Blocks::BRICKS);
                }
            }
            
            // Peak front overhang
            if (peakWidth == 2) {
                mc.setBlock(mcpp::Coordinate(peakX, y + finalLayer, z - 1), mcpp::Blocks::BRICK_STAIRS.with_mod(leftFacing));
                mc.setBlock(mcpp::Coordinate(peakX + 1, y + finalLayer, z - 1), mcpp::Blocks::BRICK_STAIRS.with_mod(rightFacing));
            }
            
            // Peak back overhang
            if (peakWidth == 2) {
                mc.setBlock(mcpp::Coordinate(peakX, y + finalLayer, z + depth), mcpp::Blocks::BRICK_STAIRS.with_mod(leftFacing));
                mc.setBlock(mcpp::Coordinate(peakX + 1, y + finalLayer, z + depth), mcpp::Blocks::BRICK_STAIRS.with_mod(rightFacing));
            }
            
            // Slab for odd width
            if (width % 2 != 0) {
                int centerX = x + width / 2;
                int slabY = y + finalLayer + 1;
                
                for (int dz = -1; dz <= depth; dz++) {
                    std::string slabCmd = "setblock " + std::to_string(centerX) + " " + std::to_string(slabY) + " " + std::to_string(z + dz) + " minecraft:brick_slab[type=bottom] replace";
                    mc.doCommand(slabCmd);
                }
            }
        }
    }

    void themedFloor( mcpp::MinecraftConnection& mc, int x, int z, int width, int depth, int y, const std::string& theme) {
        mcpp::BlockType floorBlock;
        if (theme == "Bathroom") {
            floorBlock = mcpp::Blocks::	CHISELED_QUARTZ_BLOCK;
        } else if (theme == "Kitchen") {
            // checkered pattern for kitchen 
            for (int dx = 0; dx < width; dx++) {
            for (int dz = 0; dz < depth; dz++) {
                mcpp::BlockType tile;

                if ((dx + dz) % 2 == 0) {
                    tile = mcpp::Blocks::WHITE_CONCRETE;
                } else {
                    tile = mcpp::Blocks::BLACK_CONCRETE;
                }

                mc.setBlock(mcpp::Coordinate(x + dx, y - 1, z + dz), tile);
            }
        }
        return;
        } 
        else if (theme == "Bedroom") {
            floorBlock = mcpp::Blocks::SPRUCE_WOOD_PLANK;
        } else {
            floorBlock = mcpp::Blocks::JUNGLE_WOOD_PLANK;
        }
        
        for (int dx = 0; dx < width; dx++) {
            for (int dz = 0; dz < depth; dz++) {
                mc.setBlock(mcpp::Coordinate(x + dx, y - 1, z + dz), floorBlock);
            }
        }
    }


    // main function for build exterior
    void buildExterior(mcpp::MinecraftConnection& mc, Plot& plot, Building& building, bool testMode) {
        
    int x = plot.origin.x;
        int y = plot.origin.y;
        int z = plot.origin.z;
        int width = plot.bound.x - plot.origin.x + 1;
        int length = plot.bound.z - plot.origin.z + 1;
        int height = building.getNextHeight(testMode);
    
        std::cout << "\t Building " << width << "x" << length << " house with height " << height << std::endl;
        // Building the floor then walls, and roof
        themedFloor(mc, x + 1, z + 1, width - 2, length - 2, y, "Living Room");
        buildWalls(mc, x, z, width, length, y - 1, height + 1);
        buildRoof(mc, x, z, width, length, y + height, plot);
        
        
        
        
    }
    //____________________________________________________
    // interior helper functions for subdividing
    bool canSubdivideHorizontally(const SubdivisionArea& area) {
        return area.width() >= (MIN_ROOM_SIZE * 2 + 1); 
    }
    bool canSubdivideVertically(const SubdivisionArea& area) {
        return area.depth() >= (MIN_ROOM_SIZE * 2 + 1);
    }

    int splitThePosition(int min, int max, int size, Building& building, bool testMode) {
    
        int result = 0;
        if (testMode) {
            result = min + (size / 2);
        } else {
            int minSplit = min + MIN_ROOM_SIZE;
            int maxSplit = max - MIN_ROOM_SIZE; 
            
            if (minSplit > maxSplit) {
                result = min + (size / 2);
            } else {
                std::uniform_int_distribution<int> dist(minSplit, maxSplit);
                result = dist(building.getRng());
            }
        }
        
        return result;  
    }

    void interiorWall(mcpp::MinecraftConnection& mc, int x1, int z1, int x2, int z2, 
                        int y, int height, bool isHorizontal) {
        mcpp::BlockType wallBlock = mcpp::Blocks::SPRUCE_WOOD;
        
        if (isHorizontal) {
            // horizontal wall 
            for (int x = x1; x <= x2; x++) {
                for (int dy = 0; dy < height; dy++) {
                    mc.setBlock(mcpp::Coordinate(x, y + dy, z1), wallBlock);
                }
            }
        } else {
            // vertical wall 
            for (int z = z1; z <= z2; z++) {
                for (int dy = 0; dy < height; dy++) {
                    mc.setBlock(mcpp::Coordinate(x1, y + dy, z), wallBlock);
                }
            }
        }
    }

    // Sub division algorithm
    // Recursively subdivides area into rooms of valid size
    // Splits along longest axis in testmode
    void subdivideRecursively(mcpp::MinecraftConnection& mc,  const SubdivisionArea& area,  std::vector<Room>& rooms, int& largeRoomCount,
    Building& building, bool testMode) {

        int width = area.width();
        int depth = area.depth();
        
    
        
        bool shouldCreateRoom = false;
        bool splitSucceeded = false;

        // If the area is too small to be split further (both dims < required split threshold),
        // create a room (stop subdividing).
        if (width < MIN_ROOM_SIZE * 2 + 1 && depth < MIN_ROOM_SIZE * 2 + 1) {
            shouldCreateRoom = true;
        } else {
            // Decide whether we must force a split because the area is too large and we already
            // have a large room.
            bool mustSplitBecauseOfLargeRoom = false;
            if ((width > MAX_ROOM_SIZE || depth > MAX_ROOM_SIZE) && largeRoomCount > 0) {
                mustSplitBecauseOfLargeRoom = true;
            }

            // Decide splitting axis
            bool splitVertically = false; // vertical split -> along X (create a vertical wall at X)
            if (testMode) {
                splitVertically = (width >= depth);
            } else if (mustSplitBecauseOfLargeRoom) {
                // Force a split that reduces the larger dimension.
                splitVertically = (width >= depth);
            } else {
                // Normal random 
                if (!canSubdivideHorizontally(area)) {
                    splitVertically = false;
                } else if (!canSubdivideVertically(area)) {
                    splitVertically = true;
                } else {
                    std::uniform_int_distribution<int> dir(0, 1);
                    splitVertically = (dir(building.getRng()) == 0);
                }
            }

            // it will attempt the split. If it fails no valid split position, fall back to creating a room.
            if (splitVertically) {
                int splitX = splitThePosition(area.minX, area.maxX, width, building, testMode);
                // Ensure both sides after split are at least the minimum room size
                if ((splitX - area.minX) >= MIN_ROOM_SIZE && (area.maxX - splitX) >= MIN_ROOM_SIZE) {
                    // placing a vertical interior wall at splitX 
                    interiorWall(mc, splitX, area.minZ, splitX, area.maxZ, area.floorY, EXTERIOR_HEIGHT, false);

                    SubdivisionArea left  = { area.minX, area.minZ, splitX - 1, area.maxZ, area.floorY };
                    SubdivisionArea right = { splitX + 1, area.minZ, area.maxX, area.maxZ, area.floorY };

                    // recurse on both halves
                    subdivideRecursively(mc, left,  rooms, largeRoomCount, building, testMode);
                    subdivideRecursively(mc, right, rooms, largeRoomCount, building, testMode);
                    splitSucceeded = true;
                }
            } else {
                int splitZ = splitThePosition(area.minZ, area.maxZ, depth, building, testMode);
                if ((splitZ - area.minZ) >= MIN_ROOM_SIZE && (area.maxZ - splitZ) >= MIN_ROOM_SIZE) {
                    // place horizontal interior wall at splitZ (z = splitZ)
                    interiorWall(mc, area.minX, splitZ, area.maxX, splitZ, area.floorY, EXTERIOR_HEIGHT, true);

                    SubdivisionArea front = { area.minX, area.minZ, area.maxX, splitZ - 1, area.floorY };
                    SubdivisionArea back  = { area.minX, splitZ + 1, area.maxX, area.maxZ, area.floorY };

                    subdivideRecursively(mc, front, rooms, largeRoomCount, building, testMode);
                    subdivideRecursively(mc, back,  rooms, largeRoomCount, building, testMode);
                    splitSucceeded = true;
                }
            }

            // If the split has failed, the option is to create a room as fallback
            if (!splitSucceeded) {
                shouldCreateRoom = true;
            }
        }

        // Create final room if needed if its either too small to split, or the splitting has failed
        if (shouldCreateRoom) {
            std::string theme = building.getNextTheme();
            themedFloor(mc, area.minX, area.minZ, width, depth, area.floorY, theme);

            Room room(

                mcpp::Coordinate(area.minX, area.floorY, area.minZ),
                mcpp::Coordinate(area.maxX, area.floorY, area.maxZ),
                mcpp::Coordinate((area.minX + area.maxX) / 2, area.floorY, (area.minZ + area.maxZ) / 2)
            );
            room.setTheme(theme);
            // DEBUG info for each created room
            int roomArea = width * depth;
        std::cout << "\tCreated " << theme << " at ("    << area.minX << "," << area.minZ << ") to (" 
        << area.maxX << "," << area.maxZ << ") "  << " | Size is " << width << "x" << depth
    << " | Area is " << roomArea << std::endl;

        if (width > MAX_ROOM_SIZE && depth > MAX_ROOM_SIZE) {
        largeRoomCount++;
    }

            rooms.push_back(room);
        }
    }



    bool roomsOverlapInX(const Room& a, const Room& b) {
        int aMinX = a.getOrigin().x;
        int aMaxX = a.getBound().x;
        int bMinX = b.getOrigin().x;
        int bMaxX = b.getBound().x;

        return !(aMaxX < bMinX || bMaxX < aMinX);
    }

    bool roomsOverlapInZ(const Room& a, const Room& b) {
        int aMinZ = a.getOrigin().z;
        int aMaxZ = a.getBound().z;
        int bMinZ = b.getOrigin().z;
        int bMaxZ = b.getBound().z;

        return !(aMaxZ < bMinZ || bMaxZ < aMinZ);
    }

    void placeDoorOnWall(mcpp::MinecraftConnection& mc, int startCoord, int endCoord,  int wallCoord, int y, bool alongX,
    Building& building, bool testMode) {

        int doorCoordinate = 0;
        
        if (testMode) {
            doorCoordinate = (startCoord + endCoord) / 2;
        } 
        else {
            int middle = (startCoord + endCoord) / 2;
            std::uniform_int_distribution<int> dist(middle - 1, middle + 1);
            doorCoordinate = dist(building.getRng());
        }

        doorCoordinate = std::max(startCoord + 1, std::min(endCoord - 1, doorCoordinate));

        int doorX = 0;
        int doorZ = 0;

        if (alongX) {
            doorX = doorCoordinate;
            doorZ = wallCoord;
        } 
        else {
            doorX = wallCoord;
            doorZ = doorCoordinate;
        }
        
        if (!building.isDoorPlaced(doorX, doorZ)) {
            placeSingleDoor(mc, doorX, y, doorZ, alongX, building);
            std::cout << "\t  Placed door at (" << doorX << "," << y << "," << doorZ << ")\n";
        }
    }

    void placeDoorsForAllRooms(mcpp::MinecraftConnection& mc, std::vector<Room>& rooms, Building& building, bool testMode) {
        std::cout << "\t Placing doors on all room walls..." << std::endl;
        int doorsPlaced = 0;
        // Connected walls that already have a door to another room
        std::set<std::string> connectedWalls;

        // Check every pair of rooms
        for (size_t i = 0; i < rooms.size(); ++i) {
            for (size_t j = i + 1; j < rooms.size(); ++j) {
                Room& roomA = rooms[i];
                Room& roomB = rooms[j];
                int y = roomA.getOrigin().y;

                // Horizontal wall for roomB to south of roomA
                if (roomA.getBound().z + 2 == roomB.getOrigin().z && roomsOverlapInX(roomA, roomB)) {
                    int wallZ = roomA.getBound().z + 1;
                    int startX = std::max(roomA.getOrigin().x, roomB.getOrigin().x);
                    int endX = std::min(roomA.getBound().x, roomB.getBound().x);
                    
                    if (endX - startX >= 2) {
                        // Wall key to avoid duplicate doors
                        std::string wallKey = std::to_string(wallZ) + "_H_" + std::to_string(startX) + "_" + std::to_string(endX);
                        
                        if (connectedWalls.find(wallKey) == connectedWalls.end()) {
                            int doorX = getDoorPosition(startX, endX, building, testMode);
                            
                            if (!building.isDoorPlaced(doorX, wallZ)) {
                                placeSingleDoor(mc, doorX, y, wallZ, true, building);
                                // Debug message to know where the doors are placed
                                std::cout << "\t  Placed door at (" << doorX << "," << y << "," << wallZ << ")\n";
                                connectedWalls.insert(wallKey);
                                doorsPlaced++;
                            }
                        }
                    }
                }
                
                // Vertical wall for roomB to east of roomA
                if (roomA.getBound().x + 2 == roomB.getOrigin().x && roomsOverlapInZ(roomA, roomB)) {
                    int wallX = roomA.getBound().x + 1;
                    int startZ = std::max(roomA.getOrigin().z, roomB.getOrigin().z);
                    int endZ = std::min(roomA.getBound().z, roomB.getBound().z);
                    
                    if (endZ - startZ >= 2) {
                        // Wall key to avoid duplicate doors
                        std::string wallKey = std::to_string(wallX) + "_V_" + std::to_string(startZ) + "_" + std::to_string(endZ);
                        
                        if (connectedWalls.find(wallKey) == connectedWalls.end()) {
                            int doorZ = getDoorPosition(startZ, endZ, building, testMode);
                            
                            if (!building.isDoorPlaced(wallX, doorZ)) {
                                placeSingleDoor(mc, wallX, y, doorZ, false, building);
                                std::cout << "\t  Placed door at " << wallX << ", " << y << ", " << doorZ << "\n";
                                connectedWalls.insert(wallKey);
                                doorsPlaced++;
                            }
                        }
                    }
                }
            }
        }
        

        std::cout << "\t Total doors placed: " << doorsPlaced << std::endl;
    }
    // Calculate the door position of the wall from the function above
    int getDoorPosition(int overlapStart, int overlapEnd, Building& building, bool testMode) {
        int middle = (overlapStart + overlapEnd) / 2;
        
        if (testMode) {
            return middle;
        }
        
        int minPos = std::max(overlapStart + 1, middle - 1);
        int maxPos = std::min(overlapEnd - 1, middle + 1);
        
        if (minPos > maxPos) {
            return middle;
        }
        
        std::uniform_int_distribution<int> dist(minPos, maxPos);
        return dist(building.getRng());
    }

    // subdividing the interior using the subdivision function
    std::vector<Room> subdivideInterior(mcpp::MinecraftConnection& mc, const Plot& plot, Building& building, bool testMode) {
        std::vector<Room> rooms;
    //calculate the interior space which is the inside the walls
    SubdivisionArea area;
        area.minX = plot.origin.x + 1;
        area.minZ = plot.origin.z + 1;
        area.maxX = plot.bound.x - 1;
        area.maxZ = plot.bound.z - 1;
        area.floorY = plot.origin.y;
        
    int largeRoomCount = 0;

    std::cout << "Starting subdivision for area (" << area.minX << "," << area.minZ 
                << ") to (" << area.maxX << "," << area.maxZ << ")" << std::endl;
        std::cout << "Interior dimensions: " << area.width() << "x" << area.depth() << std::endl;
        
        
        subdivideRecursively(mc, area, rooms, largeRoomCount, building, testMode);
        
        std::cout << "Total rooms created " << rooms.size() << std::endl;
        return rooms;
    }

    void placeSingleDoor(mcpp::MinecraftConnection& mc, int x, int y, int z, bool isHorizontal, Building& building) {
    
        
        if (!building.isDoorPlaced(x, z)) {
            // clear door space
            for (int dy = 0; dy < DOOR_HEIGHT; dy++) {
                mc.setBlock(mcpp::Coordinate(x, y + dy, z), mcpp::Blocks::AIR);
            }
        

        // upper and lower half
    // Facing
    mcpp::BlockType lowerDoor(OAK_DOOR_ID, 0); 

    if (isHorizontal) {
        lowerDoor = mcpp::BlockType(OAK_DOOR_ID, 1);
    } else {
        lowerDoor = mcpp::BlockType(OAK_DOOR_ID, 0);
    }
    mcpp::BlockType upperDoor(OAK_DOOR_ID, 8); 

    if (isHorizontal) {
        upperDoor = mcpp::BlockType(OAK_DOOR_ID, 9);
    } else {
        upperDoor = mcpp::BlockType(OAK_DOOR_ID, 8);
    }

    int temp = y + 1;
    mc.setBlock(mcpp::Coordinate(x, temp, z), upperDoor);
    mc.setBlock(mcpp::Coordinate(x, y, z), lowerDoor);

            building.addDoor(x, z);
        // One block above the door
    int torchY = y + 2;  
            
            if (isHorizontal) {
                // Door runs along X axis 
                // torch places on the wall
                // Torch facing south
                mc.setBlock(mcpp::Coordinate(x, torchY, z - 1), mcpp::BlockType(TORCH_ID, TORCH_FACING_SOUTH)); 
            } else {
            
                // Place torch on the wall block to the west
                // Torch facing east
                mc.setBlock(mcpp::Coordinate(x - 1, torchY, z), mcpp::BlockType(TORCH_ID, TORCH_FACING_EAST)); 
            }
        }


    }



    //main house function
    void build_buildings(std::vector<Plot>& plots, bool testMode, unsigned int seed) {
        mcpp::MinecraftConnection mc;
    
        // first is exterior
    Building building(seed);
        std::cout << "\nStarting Task 1 of Task B" << std::endl;
        std::cout << "\t Building the exterior of the buildings..." << std::endl ;
        
        
    for (Plot& plot : plots) {
        // Reset the doors from the previous plot or a new plot
    building.resetDoors();
        building.resetTheme();
        RoomManager roomManager;

        buildExterior(mc, plot, building, testMode);

        
        
    //second interior
    std::cout << "\nStarting Task 2 of Task B" << std::endl;
        std::cout << "\t Subdividing the interior..." << std::endl ;

            
        std::vector<Room> rooms = subdivideInterior(mc, plot, building, testMode);

        // store entrance coordinate in the plot
    mcpp::Coordinate entranceCoordinate = placeMainEntrance(mc, plot, building, rooms, testMode);
    plot.entrance = entranceCoordinate;
        
    placeDoorsForAllRooms(mc, rooms, building, testMode);
            for (const Room& room : rooms) {
                roomManager.addRoom(room);
            }
        
        //interior decorating left to do in decorate.cpp
        std::cout << "\nStarting Task 3 of Task B" << std::endl;
        std::cout << "\t Doing some interior decorating..." << std::endl ;
        for (Room& room : rooms) {
        addWindows(mc, room, entranceCoordinate, plot);
    }
        
    // assign furniture
    for (Room& room : rooms) {
        assignFurniture(mc, room, testMode);
    
    }
    
    }

    std::cout << "\n All Buildings Complete " << std::endl;
    }


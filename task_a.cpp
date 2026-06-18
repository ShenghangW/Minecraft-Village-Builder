#include "plots.h"
#include "village.h"
#include "task_a.h"
#include <vector>
#include <iostream>
#include <random>
#include <algorithm>

// plot constructor
Plot::Plot(mcpp::Coordinate origin, mcpp::Coordinate bound, mcpp::Coordinate entrance) 
    : origin(origin), bound(bound), entrance(entrance) {}

// check conditions for valid plot
bool Plot::validOrNot(const mcpp::Coordinate& origin, int size, int border, const std::vector<Plot>& plots, mcpp::MinecraftConnection& mc){
    std::cout << "  [VALIDATION START] Checking " << size << "x" << size << " plot...\n"; // DEBUGGING

    mcpp::Coordinate2D corner1(origin.x, origin.z);
    mcpp::Coordinate2D corner2(origin.x + size - 1, origin.z + size - 1);
    mcpp::HeightMap heights = mc.getHeights(corner1, corner2);  // heightmap gets the 2d area with the highest non air y value at each x, z value

    int maxY = -9999;
    int minY = 9999;
    int waterCount = 0;
    const int buildingHeight = 6;
    
    for (int i = 0; i < size; i++) {
        if (i % 5 == 0) {
            std::cout << "    Scanning row " << i << "/" << size << "...\n"; // DEBUGGING
        }
        
        for (int j = 0; j < size; j++) {
            int x = origin.x + i;
            int z = origin.z + j;
            int y = heights.get(i, j);  // get height from the height map
            mcpp::BlockType block = mc.getBlock({x, y, z});

            maxY = std::max(maxY, y);
            minY = std::min(minY, y);
            if (block == mcpp::Blocks::STILL_WATER || block == mcpp::Blocks::FLOWING_WATER) { 
                waterCount++; 
            }
        }
    }

    std::cout << "  [TERRAIN DONE] minY=" << minY << ", maxY=" << maxY << ", water=" << waterCount << "\n"; // DEBUGGING

    int slopeDelta = maxY - minY;
    int totalBlocks = size * size;
    double waterPercent = (double)waterCount / totalBlocks;
    
    if (waterPercent > 0.15) {
        std::cout << "  [REJECTED] Too much water: " << (waterPercent * 100) << "%\n"; // DEBUGGING
        return false;
    }
    
    if (slopeDelta > 15) {
        std::cout << "  [REJECTED] Slope too steep: " << slopeDelta << "\n"; // DEBUGGING
        return false;
    }

    std::cout << "  [CHECKING BORDER]...\n"; // DEBUGGING
    
    int baseHeight = minY + 1;

    // get border corner coordinates for area
    mcpp::Coordinate2D borderCorner1(origin.x - border, origin.z - border);
    mcpp::Coordinate2D borderCorner2(origin.x + size - 1 + border, origin.z + size - 1 + border);

    // height map for the entire border area
    mcpp::HeightMap borderHeights = mc.getHeights(borderCorner1, borderCorner2);

    // exclude plot interior
    for (int i = -border; i < size + border; i++) {
        for (int j = -border; j < size + border; j++) {
            bool notBorder = (i >= 0 && i < size && j >= 0 && j < size);
            if (!notBorder) {
                int borderX = i + border;  // adjust for border offset in heightmap
                int borderZ = j + border;
                int y = borderHeights.get(borderX, borderZ);
                
                // only check if the height is within our building range
                if (y >= baseHeight && y < baseHeight + buildingHeight) {
                    int x = origin.x + i;
                    int z = origin.z + j;
                    mcpp::BlockType block = mc.getBlock({x, y, z});
                    
                    if (block != mcpp::Blocks::AIR &&
                        block != mcpp::Blocks::OAK_LEAVES &&
                        block != mcpp::Blocks::DARK_OAK_LEAVES &&
                        block != mcpp::Blocks::BIRCH_LEAVES &&
                        block != mcpp::Blocks::SPRUCE_LEAVES &&
                        block != mcpp::Blocks::JUNGLE_LEAVES &&
                        block != mcpp::Blocks::ACACIA_LEAVES &&
                        block != mcpp::Blocks::GRASS &&
                        block != mcpp::Blocks::TALL_GRASS &&
                        block != mcpp::Blocks::DOUBLE_TALLGRASS &&
                        block != mcpp::Blocks::DIRT &&
                        block != mcpp::Blocks::COARSE_DIRT &&
                        block != mcpp::Blocks::GRAVEL &&
                        block != mcpp::Blocks::STONE &&
                        block != mcpp::Blocks::SAND &&
                        block != mcpp::Blocks::PODZOL &&
                        block != mcpp::Blocks::RED_SAND &&
                        block != mcpp::Blocks::TALL_GRASS) { 
                        std::cout << "  [REJECTED] Border collision at (" << x << "," << y << "," << z << ")\n"; // DEBUGGING
                        return false;
                    }
                }
            }
        }
    }

    std::cout << "  [BORDER OK, CHECKING INTERSECTIONS]\n"; // DEBUGGING

    for (const Plot& p : plots){
        int thisMinX = origin.x;
        int thisMaxX = origin.x + size - 1;
        int thisMinZ = origin.z;
        int thisMaxZ = origin.z + size - 1;
        
        int existMinX = p.origin.x;
        int existMaxX = p.bound.x;
        int existMinZ = p.origin.z;
        int existMaxZ = p.bound.z;
        
        bool xOverlap = !(thisMaxX < existMinX || thisMinX > existMaxX);
        bool zOverlap = !(thisMaxZ < existMinZ || thisMinZ > existMaxZ);
        
        if (xOverlap && zOverlap) {
            std::cout << "  [REJECTED] Plot intersection\n"; // DEBUGGING
            return false;
        }
    }

    std::cout << "  [ACCEPTED] VALID PLOT!\n"; // DEBUGGING
    return true;
}


void clearUnwantedBlocks(int minX, int maxX, int minZ, int maxZ, mcpp::MinecraftConnection& mc) {
    std::cout << "  [CLEARING TREES] Using /fill command...\n";
    
    // clear unwanted blocks like trees, mushrooms, vines, bamboo...
    std::vector<std::string> treeBlocks = {
        "oak_leaves", "dark_oak_leaves", "birch_leaves", 
        "spruce_leaves", "jungle_leaves", "acacia_leaves",
        "mangrove_leaves", "azalea_leaves", "flowering_azalea_leaves",
        "oak_log", "dark_oak_log", "birch_log",
        "spruce_log", "jungle_log", "acacia_log",
        "mangrove_log", "azalea", "flowering_azalea",
        "vine", "bamboo", "mushroom_stem",
        "cactus", "dead_bush", "red_mushroom_block",
        "brown_mushroom_block", "grass", "tall_grass",
        "allium", "azure_bluet", "blue_orchid",
        "cornflower", "dandelion", "lilac",
        "lily_of_the_valley", "oxeye_daisy",
        "peony", "poppy", "rose_bush",
        "sunflower", "orange_tulip", "red_tulip", "white_tulip", "pink_tulip",
        "wither_rose", "melon", "pumpkin"
    };
    
    // split the area into smaller chunks to use /fill
    const int chunkSize = 16;
    int totalChunks = 0;

    int clearBottom = -55;
    int clearTop = 32;

    for (int chunkX = minX; chunkX <= maxX; chunkX += chunkSize) {
        for (int chunkZ = minZ; chunkZ <= maxZ; chunkZ += chunkSize) {
            int x1 = chunkX;
            int x2 = std::min(chunkX + chunkSize - 1, maxX);
            int z1 = chunkZ;
            int z2 = std::min(chunkZ + chunkSize - 1, maxZ);

            totalChunks ++;
            
            // clear all tree types in this chunk
            for (const std::string& blockType : treeBlocks) {
                std::string fillCmd = "fill " + 
                    std::to_string(x1) + " " + std::to_string(clearBottom) + " " + std::to_string(z1) + " " +
                    std::to_string(x2) + " " + std::to_string(clearTop) + " " + std::to_string(z2) + 
                    " air replace " + blockType;
                mc.doCommand(fillCmd);
            }
        }
    }

    clearBottom = 33;
    clearTop = 130;

    for (int chunkX = minX; chunkX <= maxX; chunkX += chunkSize) {
        for (int chunkZ = minZ; chunkZ <= maxZ; chunkZ += chunkSize) {
            int x1 = chunkX;
            int x2 = std::min(chunkX + chunkSize - 1, maxX);
            int z1 = chunkZ;
            int z2 = std::min(chunkZ + chunkSize - 1, maxZ);

            totalChunks ++;
            
            // clear all tree types in this chunk
            for (const std::string& blockType : treeBlocks) {
                std::string fillCmd = "fill " + 
                    std::to_string(x1) + " " + std::to_string(clearBottom) + " " + std::to_string(z1) + " " +
                    std::to_string(x2) + " " + std::to_string(clearTop) + " " + std::to_string(z2) + 
                    " air replace " + blockType;
                mc.doCommand(fillCmd);
            }
        }
    }

    clearBottom = 131;
    clearTop = 200;

    for (int chunkX = minX; chunkX <= maxX; chunkX += chunkSize) {
        for (int chunkZ = minZ; chunkZ <= maxZ; chunkZ += chunkSize) {
            int x1 = chunkX;
            int x2 = std::min(chunkX + chunkSize - 1, maxX);
            int z1 = chunkZ;
            int z2 = std::min(chunkZ + chunkSize - 1, maxZ);

            totalChunks ++;
            
            // clear all tree types in this chunk
            for (const std::string& blockType : treeBlocks) {
                std::string fillCmd = "fill " + 
                    std::to_string(x1) + " " + std::to_string(clearBottom) + " " + std::to_string(z1) + " " +
                    std::to_string(x2) + " " + std::to_string(clearTop) + " " + std::to_string(z2) + 
                    " air replace " + blockType;
                mc.doCommand(fillCmd);
            }
        }
    }

    std::cout << "  [CLEARED " << totalChunks << " chunks]\n";
}


std::vector<Plot> find_plots(int loc_x, int loc_z, int village_size, int plot_border, int seed, bool testmode) {
    mcpp::MinecraftConnection mc;
    std::vector<Plot> plots ;

    int half = village_size / 2;
    int minX = loc_x - half;
    int minZ = loc_z - half;
    int maxX = loc_x + half;
    int maxZ = loc_z + half;

    setVillageBounds(minX, maxX, minZ, maxZ);

    std::mt19937 gen(seed);
    std::uniform_int_distribution<> plotSize(14, 20);

    size_t targetPlots = std::max(1, village_size / 50);

    if(testmode) {
        int size = 14;
        for(int z = minZ + 5; z <= maxZ - 5; z += 5) {
            for(int x = minX + 5; x <= maxX - 5; x += 5) {

                mcpp::Coordinate2D plotCorner1(x, z);
                mcpp::Coordinate2D plotCorner2(x + size - 1, z + size - 1);
                mcpp::HeightMap plotHeights = mc.getHeights(plotCorner1, plotCorner2);

                int maxY = -9999;
                for (int i = 0; i < size; i++) {
                    for (int j = 0; j < size; j++) {
                        int heightAtPoint = plotHeights.get(i, j);
                        maxY = std::max(maxY, heightAtPoint);
                    }
                }

                int y = maxY;
                mcpp::Coordinate origin(x, y + 1, z);

                if (Plot::validOrNot(origin, size, plot_border, plots, mc)) {
                    mcpp::Coordinate bound(x + size - 1, y + 1, z + size - 1);
                    int plotCX = x + size / 2;
                    int plotCZ = z + size / 2;

                    int toVillageX = loc_x - plotCX;
                    int toVillageZ = loc_z - plotCZ;
                    std::uniform_int_distribution<> edgePos(1, size - 2);
                    int offset = edgePos(gen);

                    mcpp::Coordinate entrance;
                    if (std::abs(toVillageX) > std::abs(toVillageZ)) {
                        if (toVillageX > 0) entrance = mcpp::Coordinate(x + size - 1, y + 1, z + offset);
                        else entrance = mcpp::Coordinate(x, y + 1, z + offset);
                    } else {
                        if (toVillageZ > 0) entrance = mcpp::Coordinate(x + offset, y + 1, z + size - 1);
                        else entrance = mcpp::Coordinate(x + offset, y + 1, z);
                    }

                    Plot newPlot(origin, bound, entrance);
                    plots.push_back(newPlot);
                    std::cout << "Plot " << plots.size() << " found at (" << x << ", " << y << ", " << z << ") with size " << size << "x" << size << "\n";

                    size++;
                    if (size > 20) size = 14;

                    if(plots.size() >= targetPlots) return plots;
                }
            }
        }
        if (plots.size() < targetPlots) std::cerr << "Could not find enough valid plots. Go try again somewhere else.\n";
        return plots;
    }

    int attempts = 0;
    int maxAttempts = 1000;

    while (attempts < maxAttempts && plots.size() < targetPlots){
        attempts++;
        int size = plotSize(gen);

        std::uniform_int_distribution<> posX(minX, maxX - 20);
        std::uniform_int_distribution<> posZ(minZ, maxZ - 20);

        int x = posX(gen);
        int z = posZ(gen);
        std::cout << "Checking random plot attempt " << attempts << "...\n";

        mcpp::Coordinate2D plotCorner1(x, z);
        mcpp::Coordinate2D plotCorner2(x + size - 1, z + size - 1);
        mcpp::HeightMap plotHeights = mc.getHeights(plotCorner1, plotCorner2);

        int maxY = -9999;
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                int heightAtPoint = plotHeights.get(i, j);
                maxY = std::max(maxY, heightAtPoint);
            }
        }

        int y = maxY;
        std::cout << "Got max ground height: " << y << " at plot starting (" << x << ", " << z << ")\n";

        mcpp::Coordinate origin(x, y + 1, z);

        if (Plot::validOrNot(origin, size, plot_border, plots, mc)){
            mcpp::Coordinate bound(x + size - 1, y + 1, z + size - 1);
            int plotCX = x + size / 2;
            int plotCZ = z + size / 2;
            int toVillageX = loc_x - plotCX;
            int toVillageZ = loc_z - plotCZ;

            mcpp::Coordinate entrance;
            std::uniform_int_distribution<> edgePos(1, size - 2);
            int offset = edgePos(gen);

            if (std::abs(toVillageX) > std::abs(toVillageZ)) {
                if (toVillageX > 0) entrance = mcpp::Coordinate(x + size - 1, y + 1, z + offset);
                else entrance = mcpp::Coordinate(x, y + 1, z + offset);
            } else {
                if (toVillageZ > 0) entrance = mcpp::Coordinate(x + offset, y + 1, z + size - 1);
                else entrance = mcpp::Coordinate(x + offset, y + 1, z);
            }

            Plot newPlot(origin, bound, entrance);
            plots.push_back(newPlot);

            std::cout << "Plot " << plots.size() << " found at (" << x << ", " << y << ", " << z << ") with size " << size << "x" << size << "\n";
        }
    }

    if (plots.size() < targetPlots) std::cerr << "Could not find enough valid plots. Go try again somewhere else.\n";

    return plots;
}

void terraform(const Plot& plot, int size, int border, mcpp::MinecraftConnection& mc, const std::vector<Plot>& plots) {
    
    int plotHeight = plot.origin.y - 1;
    
    // calculate the entire terraform area
    int minX = plot.origin.x - border;
    int maxX = plot.bound.x + border;
    int minZ = plot.origin.z - border;
    int maxZ = plot.bound.z + border;

    // get village boundaries
    const std::vector<int>& villageBounds = getVillageBounds();
    int villageMinX = villageBounds[0];
    int villageMaxX = villageBounds[1];
    int villageMinZ = villageBounds[2];
    int villageMaxZ = villageBounds[3];
    
    // clamp terraform area to village boundaries
    int clampedMinX = std::max(minX, villageMinX);
    int clampedMaxX = std::min(maxX, villageMaxX);
    int clampedMinZ = std::max(minZ, villageMinZ);
    int clampedMaxZ = std::min(maxZ, villageMaxZ);
    
    // get heights for the entire area
    mcpp::Coordinate2D corner1(clampedMinX, clampedMinZ);
    mcpp::Coordinate2D corner2(clampedMaxX, clampedMaxZ);
    mcpp::HeightMap heights = mc.getHeights(corner1, corner2);
    
    // first pass: calculate target heights and clear air space
    std::vector<std::tuple<int, int, int, int, mcpp::BlockType>> operations; // x, z, currentHeight, targetHeight, blockType
    
    for (int i = -border; i < size + border; i++) {
        for (int j = -border; j < size + border; j++) {
            int x = plot.origin.x + i;
            int z = plot.origin.z + j;

            bool insideVillageBounds = (x >= villageMinX && x <= villageMaxX && z >= villageMinZ && z <= villageMaxZ);

            if (insideVillageBounds) {
            
                // check if inside other plots
                bool insideOtherPlot = false;
                    for (const Plot& otherPlot : plots) {
                        bool notCurrentPlot = (&otherPlot != &plot);
                        bool overlaps = (x >= otherPlot.origin.x && x <= otherPlot.bound.x && z >= otherPlot.origin.z && z <= otherPlot.bound.z);
                        if (notCurrentPlot && overlaps) {
                            insideOtherPlot = true;
                        }
                    }
                
                if (!insideOtherPlot) {
                    // calculate distance to plot edge
                    int distX = std::max(0 - i, i - (size - 1));
                    int distZ = std::max(0 - j, j - (size - 1));
                    int distance = std::max(distX, distZ);
                    
                    // get current height
                    int heightMapX = x - clampedMinX;
                    int heightMapZ = z - clampedMinZ;
                    int currentHeight = heights.get(heightMapX, heightMapZ);
                    
                    // get the block type at current ground level
                    mcpp::BlockType groundBlock = mc.getBlock({x, currentHeight, z});
                    
                    // calculate target height
                    int targetHeight;
                    if (distance <= 0) {
                        // inside plot - use plot height
                        targetHeight = plotHeight;
                    } else {
                        // in border - interpolate
                        double ratio = (double)distance / border;
                        targetHeight = plotHeight + (int)((currentHeight - plotHeight) * ratio);
                    }
                    
                    operations.emplace_back(x, z, currentHeight, targetHeight, groundBlock);
                }
            }
        }
    }
    
    std::cout << "  [TERRAFORMING] Processing " << operations.size() << " columns...\n"; // DEBUGGING
    
    // clear air space in bulk for all columns that need raising
    for (const std::tuple<int, int, int, int, mcpp::BlockType>& op : operations) {
        int x = std::get<0>(op);
        int z = std::get<1>(op);
        int currentHeight = std::get<2>(op);
        int targetHeight = std::get<3>(op);
        
        if (currentHeight < targetHeight) {
            // need to fill upward - clear space above current terrain
            int clearTop = std::max(currentHeight, targetHeight) + 5;
            mc.setBlocks({x, currentHeight + 1, z}, {x, clearTop, z}, mcpp::Blocks::AIR);
        } else if (currentHeight > targetHeight) {
            // need to lower - clear space above target height
            mc.setBlocks({x, targetHeight + 1, z}, {x, currentHeight + 5, z}, mcpp::Blocks::AIR);
        }
    }
    
    // place blocks in bulk
    for (const std::tuple<int, int, int, int, mcpp::BlockType>& op : operations) {
        int x = std::get<0>(op);
        int z = std::get<1>(op);
        int currentHeight = std::get<2>(op);
        int targetHeight = std::get<3>(op);
        mcpp::BlockType groundBlock = std::get<4>(op);
        
        if (currentHeight < targetHeight) {
            // fill upward with the same block type
            for (int y = currentHeight + 1; y <= targetHeight; y++) {
                mc.setBlock({x, y, z}, groundBlock);
            }
        } else if (currentHeight > targetHeight) {
            // just place the same block type on top
            mc.setBlock({x, targetHeight, z}, groundBlock);
        } else {
            // same height - just ensure it's the same block type
            mc.setBlock({x, targetHeight, z}, groundBlock);
        }
    }
    
    std::cout << "  [TERRAFORMING COMPLETE]\n"; // DEBUGGING
}

void place_wall(int loc_x, int loc_z, int village_size, mcpp::MinecraftConnection& mc) {
    std::cout << "Building village wall...\n"; // DEBUGGING
    
    int half = village_size / 2;
    int minX = loc_x - half;
    int minZ = loc_z - half;
    int maxX = loc_x + half;
    int maxZ = loc_z + half;
    
    const int wallHeight = 5;   // wall height above ground
    const int wallDepth = 7;    // wall depth below ground
    const int maxStepDown = 1;  // maximum step down between adjacent columns
    const int maxStepUp = 5;    // maximum step up between adjacent columns
    const int minDropFromPeak = 3;  // don't drop more than this from highest point
    
    // north wall (minZ, from minX to maxX, excluding corners)
    std::cout << "  Building north wall...\n"; // DEBUGGING
    int prevTopY = -1;
    int peakTopY = -1;  // track the highest point
    for (int x = minX + 1; x < maxX; x++) {
        mcpp::HeightMap height = mc.getHeights(mcpp::Coordinate2D(x, minZ), mcpp::Coordinate2D(x, minZ));
        int groundY = height.get(0, 0);
        int topY = groundY + wallHeight;
        
        // smooth transition, limit height difference
        if (prevTopY != -1) {
            if (topY < prevTopY - maxStepDown) {
                topY = prevTopY - maxStepDown;  // maintain consistent drop
            } else if (topY > prevTopY + maxStepUp) {
                topY = prevTopY + maxStepUp;  // maintain consistent rise
            }
        }
        
        // track peak and prevent excessive drops
        if (peakTopY == -1 || topY > peakTopY) {
            peakTopY = topY;
        }
        if (topY < peakTopY - minDropFromPeak) {
            topY = peakTopY - minDropFromPeak;
        }
        
        // build from bottom of terrain
        int baseY = groundY - wallDepth;
        for (int y = baseY; y <= topY; y++) {
            mc.setBlock({x, y, minZ}, mcpp::Blocks::STONE_BRICKS);
        }
        
        mc.setBlock({x, topY + 1, minZ}, mcpp::Blocks::STONE_BRICK_SLAB);
        
        prevTopY = topY;
    }
    
    // south wall (maxZ, from minX to maxX, excluding corners)
    std::cout << "  Building south wall...\n"; // DEBUGGING
    prevTopY = -1;
    peakTopY = -1;
    for (int x = minX + 1; x < maxX; x++) {
        mcpp::HeightMap height = mc.getHeights(mcpp::Coordinate2D(x, maxZ), mcpp::Coordinate2D(x, maxZ));
        int groundY = height.get(0, 0);
        int topY = groundY + wallHeight;
        
        if (prevTopY != -1) {
            if (topY < prevTopY - maxStepDown) {
                topY = prevTopY - maxStepDown;
            } else if (topY > prevTopY + maxStepUp) {
                topY = prevTopY + maxStepUp;
            }
        }
        
        if (peakTopY == -1 || topY > peakTopY) {
            peakTopY = topY;
        }
        if (topY < peakTopY - minDropFromPeak) {
            topY = peakTopY - minDropFromPeak;
        }
        
        int baseY = groundY - wallDepth;
        for (int y = baseY; y <= topY; y++) {
            mc.setBlock({x, y, maxZ}, mcpp::Blocks::STONE_BRICKS);
        }
        
        mc.setBlock({x, topY + 1, maxZ}, mcpp::Blocks::STONE_BRICK_SLAB);
        
        prevTopY = topY;
    }
    
    // west wall (minX, from minZ to maxZ, including corners)
    std::cout << "  Building west wall...\n"; // DEBUGGING
    prevTopY = -1;
    peakTopY = -1;
    for (int z = minZ; z <= maxZ; z++) {
        mcpp::HeightMap height = mc.getHeights(mcpp::Coordinate2D(minX, z), mcpp::Coordinate2D(minX, z));
        int groundY = height.get(0, 0);
        int topY = groundY + wallHeight;
        
        if (prevTopY != -1) {
            if (topY < prevTopY - maxStepDown) {
                topY = prevTopY - maxStepDown;
            } else if (topY > prevTopY + maxStepUp) {
                topY = prevTopY + maxStepUp;
            }
        }
        
        if (peakTopY == -1 || topY > peakTopY) {
            peakTopY = topY;
        }
        if (topY < peakTopY - minDropFromPeak) {
            topY = peakTopY - minDropFromPeak;
        }
        
        int baseY = groundY - wallDepth;
        for (int y = baseY; y <= topY; y++) {
            mc.setBlock({minX, y, z}, mcpp::Blocks::STONE_BRICKS);
        }
        
        mc.setBlock({minX, topY + 1, z}, mcpp::Blocks::STONE_BRICK_SLAB);
        
        prevTopY = topY;
    }
    
    // east wall (maxX, from minZ to maxZ, including corners)
    std::cout << "  Building east wall...\n"; // DEBUGGING
    prevTopY = -1;
    peakTopY = -1;
    for (int z = minZ; z <= maxZ; z++) {
        mcpp::HeightMap height = mc.getHeights(mcpp::Coordinate2D(maxX, z), mcpp::Coordinate2D(maxX, z));
        int groundY = height.get(0, 0);
        int topY = groundY + wallHeight;
        
        if (prevTopY != -1) {
            if (topY < prevTopY - maxStepDown) {
                topY = prevTopY - maxStepDown;
            } else if (topY > prevTopY + maxStepUp) {
                topY = prevTopY + maxStepUp;
            }
        }
        
        if (peakTopY == -1 || topY > peakTopY) {
            peakTopY = topY;
        }
        if (topY < peakTopY - minDropFromPeak) {
            topY = peakTopY - minDropFromPeak;
        }
        
        int baseY = groundY - wallDepth;
        for (int y = baseY; y <= topY; y++) {
            mc.setBlock({maxX, y, z}, mcpp::Blocks::STONE_BRICKS);
        }
        
        mc.setBlock({maxX, topY + 1, z}, mcpp::Blocks::STONE_BRICK_SLAB);
        
        prevTopY = topY;
    }
    
    std::cout << "Village wall complete!\n"; // DEBUGGING
}


// helper function - minimal validation for waypoint placement
bool isValidWaypointPosition(int x, int z, int y,
                             const std::vector<Plot>& plots,
                             int villageMinX, int villageMaxX,
                             int villageMinZ, int villageMaxZ,
                             mcpp::MinecraftConnection& mc) {
    
    // check if within 5-block margin from village edges
    if (x < villageMinX + 5 || x > villageMaxX - 5 ||  
        z < villageMinZ + 5 || z > villageMaxZ - 5) {
        std::cout << "Outside margin (" << x << "," << z << ")\n"; // DEBUGGING
        return false;
    }
    
    // check if not inside plot interior (allow near entrance)
    int buffer = 2;
    for (const Plot& plot : plots) {
        if (x > plot.origin.x - buffer && x < plot.bound.x + buffer &&
            z > plot.origin.z - buffer && z < plot.bound.z + buffer) {
            
            int distToEntrance = std::abs(x - plot.entrance.x) +
                                std::abs(z - plot.entrance.z);
            
            if (distToEntrance > 5) {
                std::cout << "Inside plot, far from entrance\n"; // DEBUGGING
                return false;
            }
        }
    }
    
    // check if has solid ground below
    mcpp::BlockType blockBelow = mc.getBlock({x, y - 1, z});
    
    if (blockBelow == mcpp::Blocks::AIR ||
        blockBelow == mcpp::Blocks::STILL_WATER ||
        blockBelow == mcpp::Blocks::FLOWING_WATER) {
        std::cout << "No solid ground below\n"; // DEBUGGING
        return false;
    }

    std::cout << "Valid position!\n"; // DEBUGGING
    return true;
}

std::vector<mcpp::Coordinate> find_waypoints(
    const std::vector<Plot>& plots,
    int village_center_x,
    int village_center_z,
    int village_size
) {
    std::cout << "Finding waypoints...\n"; // DEBUGGING

    std::vector<mcpp::Coordinate> waypoints;
    if (plots.empty()) {
        std::cerr << "No plots. Skipping waypoint generation.\n"; // DEBUGGING
        return waypoints;
    }

    mcpp::MinecraftConnection mc;

    // calculate village boundaries
    int half = village_size / 2;
    int villageMinX = village_center_x - half;
    int villageMaxX = village_center_x + half;
    int villageMinZ = village_center_z - half;
    int villageMaxZ = village_center_z + half;

    std::cout << "Village boundaries: X[" << villageMinX << " to " << villageMaxX << "], Z[" << villageMinZ << " to " << villageMaxZ << "]\n"; // DEBUGGING
    std::cout << "Pathfinding safe zone: X[" << (villageMinX + 5) << " to " << (villageMaxX - 5) << "], Z[" << (villageMinZ + 5) << " to " << (villageMaxZ - 5) << "]\n"; // DEBUGGING

    // find walkable ground height
    std::function<int(int, int)> findWalkableY = [&](int x, int z) -> int {
        // ask heightmap for the topmost non-air block (may be leaves/water)
        mcpp::Coordinate2D pos(x, z);
        mcpp::HeightMap height = mc.getHeights(pos, pos);
        int y = height.get(0, 0);

        // safety caps
        const int WORLD_MIN_Y = -64;
        const int MAX_DESCEND = 300;

        int descends = 0;
        while (y > WORLD_MIN_Y && descends++ < MAX_DESCEND) {
            mcpp::BlockType blockAt = mc.getBlock({x, y, z});
            mcpp::BlockType blockAbove = mc.getBlock({x, y + 1, z});
            mcpp::BlockType blockBelow = mc.getBlock({x, y - 1, z});

            std::function<bool(const mcpp::BlockType&)> isSupport = [&](const mcpp::BlockType &b)->bool {
                if (b == mcpp::Blocks::AIR) return false;
                if (b == mcpp::Blocks::STILL_WATER || b == mcpp::Blocks::FLOWING_WATER) return false;
                if (b == mcpp::Blocks::OAK_LEAVES || b == mcpp::Blocks::DARK_OAK_LEAVES ||
                    b == mcpp::Blocks::BIRCH_LEAVES || b == mcpp::Blocks::SPRUCE_LEAVES ||
                    b == mcpp::Blocks::JUNGLE_LEAVES || b == mcpp::Blocks::ACACIA_LEAVES) return false;
                if (b == mcpp::Blocks::OAK_WOOD || b == mcpp::Blocks::DARK_OAK_WOOD ||
                    b == mcpp::Blocks::BIRCH_WOOD || b == mcpp::Blocks::SPRUCE_WOOD ||
                    b == mcpp::Blocks::JUNGLE_WOOD || b == mcpp::Blocks::ACACIA_WOOD ||
                    b == mcpp::Blocks::BRICK_STAIRS || b == mcpp::Blocks::BRICKS) return false;
                if (b == mcpp::Blocks::DANDELION || b == mcpp::Blocks::POPPY) return false;
                return true;
            };

            if (isSupport(blockAt) && blockAbove == mcpp::Blocks::AIR) {
                if (isSupport(blockBelow) || blockBelow == mcpp::Blocks::AIR) {
                    return y + 1; // walkable surface
                }
            }

            y--;
        }

        // fallback
        std::cout << "findWalkableY: no solid support found at (" << x << "," << z << "), falling back\n"; // DEBUGGING
        return height.get(0,0) + 1;
    };

    // calculate distance to village center
    std::function<double(int, int)> distToCenter = [&](int x, int z) -> double {
        int dx = x - village_center_x;
        int dz = z - village_center_z;
        return std::sqrt(dx * dx + dz * dz);
    };

    // generate waypoint per plot - PRIORITIZE CLOSEST TO CENTER
    for (size_t plotIdx = 0; plotIdx < plots.size(); plotIdx++) {
        const Plot& plot = plots[plotIdx];
        int centerX = (plot.origin.x + plot.bound.x) / 2;
        int centerZ = (plot.origin.z + plot.bound.z) / 2;

        std::cout << "Finding waypoint for plot " << plotIdx << " at (" << centerX << ", " << centerZ << ")...\n"; // DEBUGGING

        // generate all candidates first
        std::vector<std::tuple<int, int, double>> candidates; // x, z, distance to village center

        for (int distance = 2; distance <= 10; distance++) {
            // cardinal directions
            std::vector<std::pair<int,int>> positions = {
                {plot.origin.x - distance, centerZ},
                {plot.bound.x + distance, centerZ},
                {centerX, plot.origin.z - distance},
                {centerX, plot.bound.z + distance},
                // diagonals
                {plot.origin.x - distance, plot.origin.z - distance},
                {plot.origin.x - distance, plot.bound.z + distance},
                {plot.bound.x + distance, plot.origin.z - distance},
                {plot.bound.x + distance, plot.bound.z + distance}
            };

            for (std::pair<int, int> pos : positions) {
                int x = pos.first;
                int z = pos.second;
                double dist = distToCenter(x, z);
                candidates.emplace_back(x, z, dist);
            }
        }

        // sort by distance to village center (closest first)
        std::sort(candidates.begin(), candidates.end(), 
            [](const std::tuple<int, int, double>& a, const std::tuple<int, int, double>& b) {
                return std::get<2>(a) < std::get<2>(b);
            });

        bool placed = false;
        int testedCount = 0;
        
        for (std::tuple<int, int, double> candidate : candidates) {
            int x = std::get<0>(candidate);
            int z = std::get<1>(candidate);
            double dist = std::get<2>(candidate);
            testedCount++;
            
            std::cout << "Testing position (" << x << ", " << z << ") - dist to center: " << dist << "\n"; // DEBUGGING
            
            int y = findWalkableY(x, z);
            
            if (!placed && isValidWaypointPosition(x, z, y, plots, villageMinX, villageMaxX, 
                                       villageMinZ, villageMaxZ, mc)) {
                waypoints.emplace_back(x, y, z);
                std::cout << "Waypoint placed at (" << x << ", " << y << ", " << z << ") - distance to center: " << dist << "\n"; // DEBUGGING
                placed = true;
            }
        }

        if (!placed) {
            std::cout << "WARNING: No valid waypoint found for plot " << plotIdx << " after testing " << testedCount << " positions!\n"; // DEBUGGING
        }
    }

    // add central waypoint, try positions in expanding rings
    std::cout << "Finding central waypoint...\n"; // DEBUGGING

    std::vector<std::pair<int,int>> centerCandidates;
    centerCandidates.emplace_back(village_center_x, village_center_z);

    for (int radius = 1; radius <= 15; radius++) {
        for (int dx = -radius; dx <= radius; dx++) {
            for (int dz = -radius; dz <= radius; dz++) {
                // only add positions on the perimeter of this radius
                if (std::abs(dx) == radius || std::abs(dz) == radius) {
                    centerCandidates.emplace_back(village_center_x + dx, village_center_z + dz);
                }
            }
        }
    }

    bool centerPlaced = false;
    int centerTested = 0;

    for (std::pair<int, int> centerPos : centerCandidates) {
        if (!centerPlaced) {  // Add this check at the start
            int x = centerPos.first;
            int z = centerPos.second;
            centerTested++;
            
            if (centerTested <= 10 || centerTested % 50 == 0) {
                std::cout << "Testing center position (" << x << ", " << z << ")\n"; // DEBUGGING
            }
            
            int y = findWalkableY(x, z);
            
            if (isValidWaypointPosition(x, z, y, plots, villageMinX, villageMaxX, 
                                        villageMinZ, villageMaxZ, mc)) {
                waypoints.emplace_back(x, y, z);
                std::cout << "Center waypoint at (" << x << ", " << y << ", " << z << ")\n"; // DEBUGGING
                centerPlaced = true;
            }
        }
    }

    if (!centerPlaced) {
        std::cout << "WARNING: Could not place center waypoint after " 
                << centerTested << " attempts!\n";
    }

    std::cout << "\n========================================\n";
    std::cout << "Generated " << waypoints.size() << " total waypoints\n"; // DEBUGGING
    std::cout << "========================================\n";
    
    return waypoints;
}

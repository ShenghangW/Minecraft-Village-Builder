#ifndef BLOCK_CACHE_H  
#define BLOCK_CACHE_H

#include <mcpp/mcpp.h>
#include <iostream>

class BlockCache {
private:
    mcpp::BlockType*** blocks;  // 3D Array 
    int minX, minY, minZ;
    int sizeX, sizeY, sizeZ;
    bool initialised;

public:
    BlockCache() : blocks(nullptr), initialised(false) {}  

    ~BlockCache() {
        cleanup();
    }

    void cleanup() {
        if (blocks != nullptr) {
            for (int i = 0; i < sizeX; i++) {
                for (int j = 0; j < sizeY; j++) {  
                    delete[] blocks[i][j];
                }
                delete[] blocks[i];
            }
            delete[] blocks;
            blocks = nullptr;
        }
        initialised = false;
    }

    // Initialize cache for village area - OPTIMIZED VERSION
    void init(const mcpp::Coordinate& villageMin,
              const mcpp::Coordinate& villageMax,
              mcpp::MinecraftConnection& mc) {

        std::cout << "  [CACHE] Initialising block cache..." << std::endl;
        std::cout << "  [CACHE] Area: (" << villageMin.x << ", " << villageMin.y << ", " << villageMin.z  
                  << ") to (" << villageMax.x << ", " << villageMax.y << ", " << villageMax.z << ")" << std::endl;
        
        cleanup();  // Clean up any existing cache

        minX = villageMin.x;
        minY = villageMin.y;
        minZ = villageMin.z;

        sizeX = villageMax.x - villageMin.x + 1;
        sizeY = villageMax.y - villageMin.y + 1;  
        sizeZ = villageMax.z - villageMin.z + 1;  

        std::cout << "  [CACHE] Allocating 3D array: " << sizeX << "x" << sizeY << "x" << sizeZ
                  << " = " << (sizeX * sizeY * sizeZ) << " blocks" << std::endl;

        // Allocate 3D array
        blocks = new mcpp::BlockType**[sizeX];
        for (int i = 0; i < sizeX; i++) {
            blocks[i] = new mcpp::BlockType*[sizeY];
            for (int j = 0; j < sizeY; j++) {
                blocks[i][j] = new mcpp::BlockType[sizeZ];
            }
        }

       
        std::cout << "  [CACHE] Fetching blocks in bulk..." << std::flush;
        
        mcpp::Chunk chunk = mc.getBlocks(villageMin, villageMax);
        
        std::cout << " Done!" << std::endl;
        std::cout << "  [CACHE] Populating cache array..." << std::flush;

        
        // Chunk indexing: chunk.get(x, y, z) where coordinates are relative to villageMin
        for (int i = 0; i < sizeX; i++) {
            for (int j = 0; j < sizeY; j++) {
                for (int k = 0; k < sizeZ; k++) {
                    blocks[i][j][k] = chunk.get(i, j, k);
                }
            }
        }

        std::cout << " Done!" << std::endl;
        std::cout << "  [CACHE] Cache ready with " << (sizeX * sizeY * sizeZ) << " blocks!" << std::endl;  

        initialised = true;
    }

    // Get block from cache (returns AIR if out of bounds)
    mcpp::BlockType getBlock(int x, int y, int z) const {
        if (!initialised) {
            std::cerr << "ERROR: Cache not initialized!" << std::endl;
            return mcpp::Blocks::AIR; 
        }

        int ix = x - minX;
        int iy = y - minY;
        int iz = z - minZ;

        // Check bounds
        if (ix < 0 || ix >= sizeX ||
            iy < 0 || iy >= sizeY ||
            iz < 0 || iz >= sizeZ) {
            return mcpp::Blocks::AIR;  
        }
        
        return blocks[ix][iy][iz];
    }

    // Get block from coordinate
    mcpp::BlockType getBlock(const mcpp::Coordinate& coord) const {
        return getBlock(coord.x, coord.y, coord.z);
    }

    bool isInitialised() const {  
        return initialised; 
    }
};

#endif
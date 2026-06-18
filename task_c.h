#ifndef TASKC_H
#define TASKC_H

#include <mcpp/mcpp.h>
#include <vector>
#include <iostream>
#include "plots.h"
#include "cache.h"

// ============================================================================
// CUSTOM DATA STRUCTURES (No STL containers allowed for BFS!)
// ============================================================================

// A single node in our path - like a breadcrumb!
struct PathNode {
    mcpp::Coordinate coord;     // Where this node is located
    PathNode* prev;             // Pointer to previous node (for backtracking)
    int depth;                  // How many steps from start
    double cost;                // Total cost to reach this node
    PathNode* next;             // Pointer to next node in our list
    
    // Constructor to create a new node
    PathNode(mcpp::Coordinate c, PathNode* p = nullptr, int d = 0, double cost_val = 0.0);
};

// ============================================================================
// CUSTOM QUEUE - Like a line at a store (first in, first out)
// ============================================================================
class PathQueue {
private:
    PathNode* head;  // Front of the line
    PathNode* tail;  // Back of the line
    int size;        // How many items in line
    
public:
    PathQueue();
    ~PathQueue();
    
    // Add someone to the back of the line
    void push(mcpp::Coordinate coord, PathNode* prev, int depth, double cost = 0.0);
    
    // Remove someone from the front of the line
    PathNode* pop();
    
    // Check if line is empty
    bool isEmpty() const;
    
    // How many in line?
    int getSize() const;
};

// ============================================================================
// CUSTOM VISITED LIST - Keep track of places we've been
// ============================================================================
class VisitedList {
private:
    PathNode* head;  // Start of our list
    int size;        // How many places visited
    
public:
    VisitedList();
    ~VisitedList();
    
    // Mark a place as visited
    void add(mcpp::Coordinate coord, PathNode* prev, int depth, double cost = 0.0);
    
    // Have we been here before?
    bool contains(mcpp::Coordinate coord) const;
    
    // Find a specific location we visited
    PathNode* find(mcpp::Coordinate coord) const;
    
    // Get the start of the list
    PathNode* getHead() const;
};

// Return PathList as answer for BFS;
class PathList {
private:
    PathNode* head;
public:
    PathList();
    ~PathList();
    void add(mcpp::Coordinate coord);
    void reverse();
    int size() const;
    mcpp::Coordinate getCoord(int index) const;
    std::vector<mcpp::Coordinate> toVector() const;  // Add this line
};

// ============================================================================
// HELPER FUNCTIONS
// ============================================================================

// Calculate distance between two points (2D - ignoring height)
double distance2D(const mcpp::Coordinate& a, const mcpp::Coordinate& b);

// Check if we can put a path here
bool isValidPathNode(const mcpp::Coordinate& coord, 
                     const BlockCache& cache,
                     const std::vector<Plot>& plots,
                     const mcpp::Coordinate& villageMin,
                     const mcpp::Coordinate& villageMax);

bool isWalkableCoordinate(const mcpp::Coordinate& coord,
                         const BlockCache& cache);

// Calculate cost for going to a location (higher = worse)
double calculateNodeCost(const mcpp::Coordinate& from, 
                        const mcpp::Coordinate& to,
                        const BlockCache& cache);

int getGroundHeightFromCache(int x, int z, const BlockCache& cache, 
                              const mcpp::Coordinate& villageMin,
                              const mcpp::Coordinate& villageMax);

void exploreNeighbours(PathNode* current,
                        PathQueue& queue,
                        VisitedList& visited,
                        const std::vector<Plot>& plots,
                        const mcpp::Coordinate& villageMin,
                        const mcpp::Coordinate& villageMax,
                        const BlockCache& cache);

// ============================================================================
// MAIN PATHFINDING FUNCTIONS
// ============================================================================

// Find a path from start to goal using BFS
PathList* BFSFindPath(const mcpp::Coordinate& start,
                                       const mcpp::Coordinate& goal,
                                       mcpp::MinecraftConnection& mc,
                                       const std::vector<Plot>& plots,
                                       const mcpp::Coordinate& villageMin,
                                       const mcpp::Coordinate& villageMax,
                                        const BlockCache& cache);

// Connect all waypoints together (Prim's Algorithm)
void connectWaypoints(std::vector<mcpp::Coordinate>& waypoints,
                     mcpp::MinecraftConnection& mc,
                     const std::vector<Plot>& plots,
                     const mcpp::Coordinate& villageMin,
                     const mcpp::Coordinate& villageMax,
                        const BlockCache& cache);

// Connect each house to nearest waypoint
void connectHousesToWaypoints(const std::vector<Plot>& plots,
                              const std::vector<mcpp::Coordinate>& waypoints,
                              mcpp::MinecraftConnection& mc,
                              const mcpp::Coordinate& villageMin,
                              const mcpp::Coordinate& villageMax,
                                const BlockCache& cache);

// Actually place gravel blocks in Minecraft
void buildPath(const std::vector<mcpp::Coordinate>& path,
              mcpp::MinecraftConnection& mc,
              const std::vector<Plot>& plots);

// Build a lamp post or other structure at waypoint
void buildWaypointStructure(const mcpp::Coordinate& waypoint,
                           mcpp::MinecraftConnection& mc);

// ============================================================================
// MAIN ENTRY POINT FOR TASK C
// ============================================================================
void executeTaskC(const std::vector<Plot>& plots,
                 std::vector<mcpp::Coordinate>& waypoints,
                 mcpp::MinecraftConnection& mc,
                 const mcpp::Coordinate& villageMin,
                 const mcpp::Coordinate& villageMax,
                    const BlockCache& cache);

#endif // TASKC_H
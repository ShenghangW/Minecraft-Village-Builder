#include "task_c.h"
#include <iostream>
#include <cmath>
#include <limits>
#include <algorithm>

// ============================================================================
// PART 1: PathNode Constructor
// ============================================================================

PathNode::PathNode(mcpp::Coordinate c, PathNode* p, int d, double cost_val)
    : coord(c), prev(p), depth(d), cost(cost_val), next(nullptr) {
}

// ============================================================================
// PART 2: PathQueue Implementation
// ============================================================================

PathQueue::PathQueue() : head(nullptr), tail(nullptr), size(0) {}

PathQueue::~PathQueue() {
    while (!isEmpty()) {
        PathNode* temp = pop();
        delete temp;
    }
}

void PathQueue::push(mcpp::Coordinate coord, PathNode* prev, int depth, double cost) {
    PathNode* newNode = new PathNode(coord, prev, depth, cost);

    if (tail == nullptr) { 
        head = newNode;
        tail = newNode;
    } else {
        tail->next = newNode;
        tail = newNode;
    }
    size++;
}

PathNode* PathQueue::pop() {
    if (isEmpty()) {
        return nullptr;
    }

    PathNode* temp = head;
    head = head->next;

    if (head == nullptr) {
        tail = nullptr;
    }

    size--;
    return temp;
}

bool PathQueue::isEmpty() const {  
    return head == nullptr;
}

int PathQueue::getSize() const {
    return size;
}

// ============================================================================
// PART 3: VisitedList Implementation
// ============================================================================

VisitedList::VisitedList() : head(nullptr), size(0) {}

VisitedList::~VisitedList() {  
    PathNode* current = head;  
    while (current != nullptr) {
        PathNode* next = current->next;
        delete current;
        current = next;
    }
}

void VisitedList::add(mcpp::Coordinate coord, PathNode* prev, int depth, double cost) {
    PathNode* newNode = new PathNode(coord, prev, depth, cost);
    newNode->next = head;
    head = newNode;
    size++;
}

bool VisitedList::contains(mcpp::Coordinate coord) const {
    PathNode* current = head;
    bool found = false;
    while (current != nullptr && !found) {
        if (current->coord.x == coord.x &&  
            current->coord.y == coord.y &&  
            current->coord.z == coord.z) {  
            found = true;
        } else {
            current = current->next;
        }
    }
    return found;
}

PathNode* VisitedList::find(mcpp::Coordinate coord) const { 
    PathNode* current = head;
    PathNode* result = nullptr;
    while (current != nullptr && result == nullptr) {  
        if (current->coord.x == coord.x &&
            current->coord.y == coord.y && 
            current->coord.z == coord.z) {
            result = current;
        } else {
            current = current->next;
        }
    }
    return result;
}

PathNode* VisitedList::getHead() const {
    return head;
}

// ============================================================================
// PART 4: Helper Functions
// ============================================================================

double distance2D(const mcpp::Coordinate& a, const mcpp::Coordinate& b) { 
    int dx = a.x - b.x;
    int dz = a.z - b.z;
    return std::sqrt(dx * dx + dz * dz); 
}

bool isValidPathNode(const mcpp::Coordinate& coord,
                    const BlockCache& cache,
                    const std::vector<Plot>& plots,
                    const mcpp::Coordinate& villageMin,
                    const mcpp::Coordinate& villageMax) {
    
    bool valid = true;
    
    // Check bounds (with margin for safety)
    if (coord.x < villageMin.x + 5 || coord.x > villageMax.x - 5 ||  
        coord.z < villageMin.z + 5 || coord.z > villageMax.z - 5 ||
        coord.y < villageMin.y || coord.y > villageMax.y) {  
        valid = false;
    }
    
    if (valid) {
        int buffer = 3;

        // Check if inside plot (with entrance exception)
        for (size_t p = 0; p < plots.size() && valid; p++) {
            const auto& plot = plots[p];
            if (coord.x >= plot.origin.x - buffer && coord.x <= plot.bound.x + buffer &&
                coord.z >= plot.origin.z - buffer && coord.z <= plot.bound.z + buffer) {  
                
                // Calculate 2D distance to entrance (more accurate)
                int dx = coord.x - plot.entrance.x;
                int dz = coord.z - plot.entrance.z;
                double distToEntrance = std::sqrt(dx*dx + dz*dz);
                
                // Allow much closer to entrance for connection (increased from 8 to 12)
                if (distToEntrance > 12) {  
                    valid = false;
                }
            }
        }
    }

    if (valid) {
        // Check current position is walkable
        mcpp::BlockType blockAtCoord = cache.getBlock(coord);
        if (blockAtCoord != mcpp::Blocks::AIR && 
            blockAtCoord != mcpp::Blocks::GRAVEL) {
            valid = false;
        }
    }

    if (valid) {
        // Check space above (2 blocks headroom)
        mcpp::BlockType blockAbove = cache.getBlock(coord.x, coord.y + 1, coord.z);
        if (blockAbove != mcpp::Blocks::AIR) {
            valid = false;
        }
    }

    if (valid) {
        // Check solid ground below
        mcpp::Coordinate below(coord.x, coord.y - 1, coord.z);
        mcpp::BlockType blockBelow = cache.getBlock(below);
        
        // RELAXED: Allow more ground types
        if (blockBelow == mcpp::Blocks::AIR ||
            blockBelow == mcpp::Blocks::STILL_WATER ||
            blockBelow == mcpp::Blocks::FLOWING_WATER) {
            valid = false;
        }
    }

    return valid;
}

double calculateNodeCost(const mcpp::Coordinate& from,  
                        const mcpp::Coordinate& to,
                        const BlockCache& cache) {
    
    double cost = 1.0;

    int heightDiff = std::abs(to.y - from.y);  
    
    // Heavily penalize ANY height change to prefer flat paths
    if (heightDiff > 0) {
        cost += heightDiff * heightDiff * 15;
    }

    if (to.y > from.y) {
        cost += heightDiff * 8.0;
    }

    mcpp::Coordinate below(to.x, to.y - 1, to.z);
    mcpp::BlockType blockBelow = cache.getBlock(below);

    if (blockBelow == mcpp::Blocks::STILL_WATER || blockBelow == mcpp::Blocks::FLOWING_WATER) {
        cost += 30.0;
    }

    if (blockBelow.id == 12 || blockBelow.id == 13) {
        cost += 5.0;
    }

    return cost;
}

// ============================================================================
// PathList Implementation
// ============================================================================

PathList::PathList() : head(nullptr) {}

PathList::~PathList() {
    PathNode* current = head;
    while (current != nullptr) {
        PathNode* next = current->next;
        delete current;
        current = next;
    }
}

void PathList::add(mcpp::Coordinate coord) {
    PathNode* newNode = new PathNode(coord, nullptr, 0, 0.0);
    newNode->next = head;
    head = newNode;
}

void PathList::reverse() {
    PathNode* current = head;
    PathNode* prev = nullptr;
    PathNode* next = nullptr;

    while (current != nullptr) {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    
    head = prev;
}

int PathList::size() const {
    int count = 0;
    PathNode* current = head;
    while (current != nullptr) {
        count++;
        current = current->next;
    }
    return count;
}

mcpp::Coordinate PathList::getCoord(int index) const {
    PathNode* current = head;
    int count = 0;

    while (current != nullptr && count < index) {
        current = current->next;
        count++;
    }

    if (current != nullptr) {
        return current->coord;
    }

    return mcpp::Coordinate(0,0,0);
}

std::vector<mcpp::Coordinate> PathList::toVector() const {
    PathNode* current = head;
    std::vector<mcpp::Coordinate> rs;

    while (current != nullptr) {
        rs.push_back(current->coord);
        current = current->next;
    }
    return rs;
}

int getGroundHeightFromCache(int x, int z, const BlockCache& cache, 
                              const mcpp::Coordinate& villageMin,
                              const mcpp::Coordinate& villageMax) {
    int startY = villageMax.y;
    int resultY = villageMin.y;
    bool found = false;
    
    for (int y = startY; y >= villageMin.y && !found; y--) {
        mcpp::BlockType block = cache.getBlock(x, y, z);
        
        bool isSolid = (block != mcpp::Blocks::AIR && 
                       block != mcpp::Blocks::STILL_WATER &&
                       block != mcpp::Blocks::FLOWING_WATER);
        
        if (isSolid) {
            // Found solid block - check if we can walk on it
            mcpp::BlockType blockAbove = cache.getBlock(x, y + 1, z);
            mcpp::BlockType TwoBlockAbove = cache.getBlock(x, y + 2, z);

            if (blockAbove == mcpp::Blocks::AIR && TwoBlockAbove == mcpp::Blocks::AIR) {
                resultY = y;
                found = true;
            }
        }
    }
    
    return resultY;
}

void exploreNeighbours(PathNode* current,
                        PathQueue& queue,
                        VisitedList& visited,
                        const std::vector<Plot>& plots,
                        const mcpp::Coordinate& villageMin,
                        const mcpp::Coordinate& villageMax,
                        const BlockCache& cache) {
                
    int dx[] = {1, -1, 0, 0};
    int dz[] = {0, 0, 1, -1};

    for (int i = 0; i < 4; i++) {
        int newX = current->coord.x + dx[i];
        int newZ = current->coord.z + dz[i];

        // Get ground height at new position
        int groundY = getGroundHeightFromCache(newX, newZ, cache, villageMin, villageMax);
        
        // Calculate height difference from current position's GROUND level
        int currentGroundY = current->coord.y - 1;
        int heightDiff = std::abs(currentGroundY - groundY);

        // ADAPTIVE HEIGHT: Be more lenient for house connections
        bool connectingHouses = (current->depth < 10);
        int maxHeightDiff = connectingHouses ? 8 : 5;

        bool validHeight = (heightDiff <= maxHeightDiff);
        
        if (validHeight) {
            // Create neighbor coordinate (walking on ground level)
            mcpp::Coordinate neighbour(newX, groundY + 1, newZ);

            // Check bounds
            bool inBounds = (neighbour.x >= villageMin.x + 5 && neighbour.x <= villageMax.x - 5 &&  
                            neighbour.z >= villageMin.z + 5 && neighbour.z <= villageMax.z - 5);

            if (inBounds) {
                // Check if already visited
                bool alreadyVisited = visited.contains(neighbour);

                if (!alreadyVisited) {
                    // Plot check with generous entrance allowance
                    bool insidePlot = false;
                    for (size_t p = 0; p < plots.size() && !insidePlot; p++) {
                        const auto& plot = plots[p];
                        int buffer = 3;
                        if (neighbour.x > plot.origin.x + buffer && neighbour.x < plot.bound.x - buffer &&
                            neighbour.z > plot.origin.z + buffer && neighbour.z < plot.bound.z - buffer) {
                            
                            // Allow very close to entrance
                            int distToEntrance = std::abs(neighbour.x - plot.entrance.x) +
                                                std::abs(neighbour.z - plot.entrance.z);
                            if (distToEntrance > 8) {
                                insidePlot = true;
                            }
                        }
                    }
                    
                    if (!insidePlot) {
                        // Calculate cost and add to queue
                        double newCost = current->cost + calculateNodeCost(current->coord, neighbour, cache);
                        PathNode* currentAsPrev = visited.find(current->coord);

                        queue.push(neighbour, currentAsPrev, current->depth + 1, newCost);
                        visited.add(neighbour, currentAsPrev, current->depth + 1, newCost);
                    }
                }
            }
        }
    }
}

// ============================================================================
// PART 5: BREADTH FIRST SEARCH
// ============================================================================

PathList* BFSFindPath(const mcpp::Coordinate& start, 
                    const mcpp::Coordinate& goal,
                    mcpp::MinecraftConnection& mc, 
                    const std::vector<Plot>& plots,
                    const mcpp::Coordinate& villageMin,
                    const mcpp::Coordinate& villageMax,
                    const BlockCache& cache) {
    
    std::cout << "Finding path from (" << start.x << "," << start.y << "," << start.z << ") to ("
                << goal.x << "," << goal.y << "," << goal.z << ")" << std::endl;

    PathQueue frontQueue;
    PathQueue backQueue;
    VisitedList frontVisited;
    VisitedList backVisited;

    bool startValid = isValidPathNode(start, cache, plots, villageMin, villageMax);
    bool goalValid = isValidPathNode(goal, cache, plots, villageMin, villageMax);

    std::cout << "  Start valid? " << (startValid ? "Yes" : "No") << std::endl;
    std::cout << "  Goal valid? " << (goalValid ? "Yes" : "No") << std::endl;

    PathList* result = new PathList();
    
    if (goalValid && startValid) {
        frontQueue.push(start, nullptr, 0, 0.0);
        backQueue.push(goal, nullptr, 0, 0.0);

        frontVisited.add(start, nullptr, 0, 0.0);
        backVisited.add(goal, nullptr, 0, 0.0);

        std::cout << "  Starting Bidirectional BFS loop..." << std::endl;

        mcpp::Coordinate meetPoint(0, 0, 0);
        bool pathFound = false;
        int count = 0;
        int maxLoop = 50000;

        while (!frontQueue.isEmpty() && !backQueue.isEmpty() && !pathFound && count < maxLoop) { 
            count++;

            if (count <= 10 || count % 100 == 0) {
                std::cout << "  Iteration " << count 
                          << " | Forward: " << frontQueue.getSize()
                          << " | Backward: " << backQueue.getSize() << std::endl;
            }
            
            if (!frontQueue.isEmpty() && !pathFound) {
                PathNode* current = frontQueue.pop();

                if (backVisited.contains(current->coord)) {
                    meetPoint = current->coord;
                    pathFound = true;
                    std::cout << "  Paths met at (" << meetPoint.x << ", " 
                              << meetPoint.y << ", " << meetPoint.z << ")!" << std::endl;
                } else {
                    exploreNeighbours(current, frontQueue, frontVisited, plots, villageMin, villageMax, cache);
                }
                
                delete current;
            }

            if (!backQueue.isEmpty() && !pathFound) {
                PathNode* current = backQueue.pop();

                if (frontVisited.contains(current->coord)) {
                    pathFound = true;
                    meetPoint = current->coord;
                    std::cout << "  Paths met at (" << meetPoint.x << "," 
                              << meetPoint.y << "," << meetPoint.z << ")!" << std::endl;
                } else {
                    exploreNeighbours(current, backQueue, backVisited, plots, villageMin, villageMax, cache);
                }
                
                delete current;
            }
        }
        
        std::cout << "  BFS ended after " << count << " iterations" << std::endl;

        if (pathFound) {
            PathNode* frontNode = frontVisited.find(meetPoint);
            std::vector<mcpp::Coordinate> forwardPath;

            while (frontNode != nullptr) {
                forwardPath.push_back(frontNode->coord);  
                frontNode = frontNode->prev;
            }

            std::reverse(forwardPath.begin(), forwardPath.end());

            PathNode* backNode = backVisited.find(meetPoint);
            std::vector<mcpp::Coordinate> backwardPath;

            if (backNode != nullptr) {
                backNode = backNode->prev;
            }

            while (backNode != nullptr) {
                backwardPath.push_back(backNode->coord);
                backNode = backNode->prev;
            }

            for (const auto& coord : forwardPath) {
                result->add(coord);
            }

            for (const auto& coord : backwardPath) {
                result->add(coord);
            }

            result->reverse();

            std::cout << "  Path found with " << result->size() << " steps" << std::endl;
        } else {
            std::cout << "  No path found" << std::endl;
        }
    } else {
        std::cout << "  Start or goal invalid" << std::endl;
    }

    return result;
}

// ============================================================================
// PART 6: Connect Waypoints
// ============================================================================

void connectWaypoints(std::vector<mcpp::Coordinate>& waypoints,
                     mcpp::MinecraftConnection& mc,
                     const std::vector<Plot>& plots,
                     const mcpp::Coordinate& villageMin,
                     const mcpp::Coordinate& villageMax,
                     const BlockCache& cache) {

    std::cout << "Connecting " << waypoints.size() << " waypoints" << std::endl; 

    if (waypoints.empty()) {
        return;
    }

    // Fix waypoint heights and validate
    for (size_t i = 0; i < waypoints.size(); i++) {
        int groundY = getGroundHeightFromCache(waypoints[i].x, waypoints[i].z, cache, villageMin, villageMax);
        waypoints[i].y = groundY + 1;
        
        // Verify waypoint is valid
        bool valid = isValidPathNode(waypoints[i], cache, plots, villageMin, villageMax);
        std::cout << "  Waypoint " << i << " at (" << waypoints[i].x << "," 
                  << waypoints[i].y << "," << waypoints[i].z << ") - " 
                  << (valid ? "VALID" : "INVALID") << std::endl;
        
        // If invalid, try to move it slightly
        if (!valid) {
            std::cout << "    Attempting to fix waypoint " << i << "..." << std::endl;
            bool fixed = false;
            
            // Try nearby positions
            for (int dx = -5; dx <= 5 && !fixed; dx++) {
                for (int dz = -5; dz <= 5 && !fixed; dz++) {
                    if (dx != 0 || dz != 0) {
                        int testX = waypoints[i].x + dx;
                        int testZ = waypoints[i].z + dz;
                        int testY = getGroundHeightFromCache(testX, testZ, cache, villageMin, villageMax) + 1;
                        
                        mcpp::Coordinate testCoord(testX, testY, testZ);
                        if (isValidPathNode(testCoord, cache, plots, villageMin, villageMax)) {
                            waypoints[i] = testCoord;
                            std::cout << "    Fixed! New position: (" << testX << "," << testY << "," << testZ << ")" << std::endl;
                            fixed = true;
                        }
                    }
                }
            }
            
            if (!fixed) {
                std::cout << "    WARNING: Could not fix waypoint " << i << std::endl;
            }
        }
    }

    std::vector<bool> connected(waypoints.size(), false);
    std::vector<std::vector<mcpp::Coordinate>> allPaths;

    connected[0] = true;
    int numConnected = 1;

    while (numConnected < (int)waypoints.size()) {  
        double minDist = std::numeric_limits<double>::max();
        int bestConnected = -1;
        int bestUnconnected = -1;

        for (size_t i = 0; i < waypoints.size(); i++) { 
            if (!connected[i]) {
                for (size_t j = 0; j < waypoints.size(); j++) { 
                    if (connected[j] && i != j) {
                        double dist = distance2D(waypoints[i], waypoints[j]);  

                        if (dist < minDist) {
                            minDist = dist;
                            bestConnected = j;
                            bestUnconnected = i;
                        }
                    }
                }
            }
        }

        if (bestUnconnected != -1) {
            std::cout << "Connecting Waypoints " << bestConnected << " to "
                    << bestUnconnected << std::endl;
            connected[bestUnconnected] = true;
            numConnected++;

            PathList* pathList = BFSFindPath(
                waypoints[bestConnected],
                waypoints[bestUnconnected],
                mc, plots, villageMin, villageMax, cache  
            );

            std::vector<mcpp::Coordinate> path = pathList->toVector();
            allPaths.push_back(path);
            delete pathList;
        }
    }

    for (const auto& path : allPaths) {
        buildPath(path, mc, plots);
    }

    std::cout << "Waypoints connected and Path build!\n";
}

// ============================================================================
// PART 7: Connect Houses to Waypoints
// ============================================================================

void connectHousesToWaypoints(const std::vector<Plot>& plots,  
                    const std::vector<mcpp::Coordinate>& waypoints,  
                    mcpp::MinecraftConnection& mc,  
                    const mcpp::Coordinate& villageMin,
                    const mcpp::Coordinate& villageMax,
                    const BlockCache& cache) {

    for (size_t i = 0; i < plots.size(); i++) {  
        const auto& plot = plots[i];

        std::cout << "Connecting house " << i << " to nearest waypoint..." << std::endl;
        std::cout << "  House entrance at (" << plot.entrance.x << ","
                        << plot.entrance.y << "," << plot.entrance.z << ")" << std::endl;

        // Start from 2 blocks away from the entrance
        mcpp::Coordinate startPoint = plot.entrance;

        // Move exactly 2 blocks away from house entrance based on which side the entrance is on
        if (plot.entrance.z == plot.bound.z) { // South side
            startPoint.z += 2;
        } else if (plot.entrance.z == plot.origin.z) { // North side  
            startPoint.z -= 2;
        } else if (plot.entrance.x == plot.bound.x) { // East side
            startPoint.x += 2;
        } else if (plot.entrance.x == plot.origin.x) { // West side
            startPoint.x -= 2;
        }

        // Get proper ground height from cache
        int groundHeight = getGroundHeightFromCache(startPoint.x, startPoint.z, cache, villageMin, villageMax);
        startPoint.y = groundHeight + 1;

        std::cout << "  Start point (2 blocks from entrance): (" << startPoint.x << "," 
                  << startPoint.y << "," << startPoint.z << ")" << std::endl;

        // Find nearest waypoint
        double minDist = std::numeric_limits<double>::max();  
        int nearestWaypoint = 0;  

        for (size_t j = 0; j < waypoints.size(); j++) {  
            double dist = distance2D(startPoint, waypoints[j]);  
            if (dist < minDist) {
                minDist = dist;
                nearestWaypoint = j;
            }
        }

        std::cout << "  Connecting to waypoint " << nearestWaypoint 
                  << " at (" << waypoints[nearestWaypoint].x << "," << waypoints[nearestWaypoint].y << "," << waypoints[nearestWaypoint].z << ")" << std::endl;

        PathList* pathList = BFSFindPath(
            startPoint,
            waypoints[nearestWaypoint],  
            mc, plots, villageMin, villageMax, cache
        );

        std::vector<mcpp::Coordinate> path = pathList->toVector();
        
        if (!path.empty()) {
            buildPath(path, mc, plots);
            std::cout << "  ✓ Path built successfully!" << std::endl;
        } else {
            std::cout << "  ✗ No path found from house " << i << " to waypoint " << nearestWaypoint << std::endl;
        }
        
        delete pathList;
    }

    std::cout << "Houses connected!" << std::endl;
}

// ============================================================================
// PART 8: Build Paths in Minecraft
// ============================================================================

void buildPath(const std::vector<mcpp::Coordinate>& path, mcpp::MinecraftConnection& mc, const std::vector<Plot>& plots) {  
    if (path.empty()) return;

    std::cout << "  Building path with " << path.size() << " segments..." << std::endl;

    for (size_t i = 0; i < path.size(); i++) {
        const auto& coord = path[i];
        int targetY = coord.y - 1;

        // Check each block in the 3x3 area individually
        for (int dx = -1; dx <= 1; dx++) {
            for (int dz = -1; dz <= 1; dz++) {
                int pathX = coord.x + dx;
                int pathZ = coord.z + dz;
                
                bool skip = false;
                
                for (size_t p = 0; p < plots.size() && !skip; p++) {
                    const auto& plot = plots[p];
                    // Check if on plot perimeter (walls)
                    if ((pathX == plot.origin.x || pathX == plot.bound.x) &&
                        pathZ >= plot.origin.z && pathZ <= plot.bound.z) {
                        skip = true;
                    } else if ((pathZ == plot.origin.z || pathZ == plot.bound.z) &&
                        pathX >= plot.origin.x && pathX <= plot.bound.x) {
                        skip = true;
                    } else if (pathX == plot.entrance.x && pathZ == plot.entrance.z) {
                        // Skip only the exact entrance block (protect the door)
                        skip = true;
                    }
                }
                
                if (!skip) {
                    // Clear space above first
                    mc.setBlocks(
                        mcpp::Coordinate(pathX, targetY + 1, pathZ),
                        mcpp::Coordinate(pathX, targetY + 3, pathZ),
                        mcpp::Blocks::AIR
                    );
                    
                    // Place gravel at surface level
                    mc.setBlock(mcpp::Coordinate(pathX, targetY, pathZ), mcpp::Blocks::GRAVEL);
                }
            }
        }
    }

    std::cout << "  Path built!" << std::endl;
}

// ============================================================================
// PART 9: Build Waypoint Structures
// ============================================================================

void buildWaypointStructure(const mcpp::Coordinate& waypoint, mcpp::MinecraftConnection& mc) {
    int height = 5;

    for (int i = 0; i < height; i++) {
        mc.setBlock(  
            mcpp::Coordinate(waypoint.x, waypoint.y + i, waypoint.z),  
            mcpp::Blocks::OAK_FENCE
        );
    }

    mc.setBlock(  
        mcpp::Coordinate(waypoint.x, waypoint.y + height, waypoint.z),
        mcpp::Blocks::GLOWSTONE  
    );
}

// ============================================================================
// PART 10: Main Entry Point for Task C
// ============================================================================

void executeTaskC(const std::vector<Plot>& plots,
                std::vector<mcpp::Coordinate>& waypoints,
                mcpp::MinecraftConnection& mc,
                const mcpp::Coordinate& villageMin,
                const mcpp::Coordinate& villageMax,
                const BlockCache& cache) {

    std::cout << "\n========================================" << std::endl;
    std::cout << "Starting Task C: PathFinding" << std::endl;
    std::cout << "========================================\n" << std::endl;

    std::cout << "Step 1: Connecting waypoints..." << std::endl;
    connectWaypoints(waypoints, mc, plots, villageMin, villageMax, cache);

    std::cout << "\nStep 2: Connecting houses to waypoints..." << std::endl;
    connectHousesToWaypoints(plots, waypoints, mc, villageMin, villageMax, cache);

    std::cout << "\nStep 3: Building waypoint structures(lamps)..." << std::endl;
    for (const auto& waypoint: waypoints) {
        buildWaypointStructure(waypoint, mc);
    }

    std::cout << "\n========================================" << std::endl;
    std::cout << "Task C Complete" << std::endl;
    std::cout << "========================================\n" << std::endl;
}
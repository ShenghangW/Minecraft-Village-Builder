#ifndef VILLAGE_H
#define VILLAGE_H

#include <vector>

// helper functions to access/modify the village bounds
void setVillageBounds(int minX, int maxX, int minZ, int maxZ);
const std::vector<int>& getVillageBounds();
void clearVillageBounds();

#endif
#include "village.h"

static std::vector<int> villageMinMax;

void setVillageBounds(int minX, int maxX, int minZ, int maxZ) {
    villageMinMax.clear();
    villageMinMax.push_back(minX);
    villageMinMax.push_back(maxX);
    villageMinMax.push_back(minZ);
    villageMinMax.push_back(maxZ);
}

const std::vector<int>& getVillageBounds() {
    return villageMinMax;
}

void clearVillageBounds() {
    villageMinMax.clear();
}

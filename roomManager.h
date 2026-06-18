#ifndef ROOMMANAGER_H
#define ROOMMANAGER_H
#include "room.h"  
#include <vector>

class RoomManager {
public:
    RoomManager();
    void addRoom(const Room& room);
    const std::vector<Room>& getRooms() const;

private:
    std::vector<Room> rooms;
};

#endif
#include "roomManager.h"

RoomManager::RoomManager() {}

//adding a room
void RoomManager::addRoom(const Room& room) {
    rooms.push_back(room);
}
// returning rooms
const std::vector<Room>& RoomManager::getRooms() const {
    return rooms;
}


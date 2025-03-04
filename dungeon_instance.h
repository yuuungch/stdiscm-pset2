#ifndef DUNGEON_INSTANCE_H
#define DUNGEON_INSTANCE_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include "party.h"

class DungeonInstance {
public:
    DungeonInstance(int id, int t1, int t2);
    void run(std::queue<Party>& partyQueue, std::mutex& queueMutex, std::condition_variable& cv);
    int getId() const;
    int getPartiesServed() const;
    int getTotalTimeServed() const;
    void stop();
    bool isActive() const;

private:
    int id;
    int t1;
    int t2;
    bool active;
    int parties_served;
    int total_time_served;
};

#endif // DUNGEON_INSTANCE_H
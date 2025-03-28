#ifndef DUNGEON_INSTANCE_H
#define DUNGEON_INSTANCE_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include <vector>
#include "party.h"

using namespace std;

class DungeonInstance {
public:
    DungeonInstance(unsigned int id, unsigned int t1, unsigned int t2);
    void run(queue<Party>& partyQueue, mutex& queueMutex, condition_variable& cv, const vector<DungeonInstance>& allDungeons, mutex& printMutex);
    unsigned int getId() const;
    unsigned int getPartiesServed() const;
    unsigned int getTotalTimeServed() const;
    void stop();
    bool isActive() const;

private:
    unsigned int id;
    unsigned int t1;
    unsigned int t2;
    bool active;
    unsigned int parties_served;
    unsigned int total_time_served;
};

#endif // DUNGEON_INSTANCE_H
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
    DungeonInstance(int id, int t1, int t2);
    void run(queue<Party>& partyQueue, mutex& queueMutex, condition_variable& cv, const vector<DungeonInstance>& allDungeons, mutex& printMutex);
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
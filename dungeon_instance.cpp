#include "dungeon_instance.h"
#include "party.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <random>
#include <vector>

using namespace std;

DungeonInstance::DungeonInstance(int id, int t1, int t2)
    : id(id), t1(t1), t2(t2), active(false), parties_served(0), total_time_served(0) {
}

void DungeonInstance::run(queue<Party>& partyQueue, mutex& queueMutex, condition_variable& cv, const vector<DungeonInstance>& allDungeons, mutex& printMutex) {
    // Random number generator setup
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(t1, t2);

    while (true) {
        unique_lock<mutex> lock(queueMutex);
        cv.wait(lock, [&partyQueue, this] { return !partyQueue.empty() || !active; });

        if (!partyQueue.empty()) {
            Party party = partyQueue.front();
            partyQueue.pop();
            active = true;
            lock.unlock();

            // Simulate serving the party
            int serve_time = dis(gen); // Generate a random serve time between t1 and t2
            this_thread::sleep_for(chrono::seconds(serve_time));

            parties_served++;
            total_time_served += serve_time;
            bool justBecameInactive = active; // Store the transition state
            active = false;

            // Lock the printing mutex before output
            lock_guard<mutex> printLock(printMutex);
            cout << "Dungeon " << id << " served party " << party.getId() << " in " << serve_time << " seconds." << endl << endl;
            
            // Show status of all dungeon instances
            cout << "Current Dungeon Instances Status:" << endl;
            for (const auto& dungeon : allDungeons) {
                cout << "Dungeon " << dungeon.getId() << ": " 
                     << (dungeon.isActive() ? "Active" : "Inactive")
                     << ((&dungeon == this && !dungeon.isActive() && justBecameInactive) ? " <-- Just finished!" : "")
                     << endl;
            }
            cout << "----------------------------------------" << endl << endl;
        }
        else if (partyQueue.empty()) {
            break;
        }
    }
}

int DungeonInstance::getId() const {
    return id;
}

int DungeonInstance::getPartiesServed() const {
    return parties_served;
}

int DungeonInstance::getTotalTimeServed() const {
    return total_time_served;
}

void DungeonInstance::stop() {
    active = false;
}

bool DungeonInstance::isActive() const {
    return active;
}
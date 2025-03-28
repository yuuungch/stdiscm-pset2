#include "../header_files/dungeon_instance.h"
#include "../header_files/party.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <random>
#include <vector>

using namespace std;

DungeonInstance::DungeonInstance(unsigned int id, unsigned int t1, unsigned int t2, unsigned int timeout)
    : id(id), t1(t1), t2(t2), timeout(timeout), active(false), parties_served(0), total_time_served(0) {
}

void DungeonInstance::run(queue<Party>& partyQueue, mutex& queueMutex, condition_variable& cv, const vector<DungeonInstance>& allDungeons, mutex& printMutex) {
    // Random number generator setup
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<unsigned int> dis(t1, t2);

    while (true) {
        unique_lock<mutex> lock(queueMutex);
        cv.wait(lock, [&partyQueue, this] { return !partyQueue.empty() || !active; });

        if (!partyQueue.empty()) {
            Party party = partyQueue.front();
            partyQueue.pop();
            active = true;
            lock.unlock();

            // Simulate serving the party
            unsigned int serve_time = dis(gen); // Generate a random serve time between t1 and t2
            this_thread::sleep_for(chrono::seconds(serve_time));

            parties_served++;
            total_time_served += serve_time;
            bool justBecameInactive = active; // Store the transition state
            active = false;

            // Check active dungeons before taking the printLock
            bool otherDungeonsActive = false;
            {
                lock_guard<mutex> checkLock(printMutex);
                for (const auto& dungeon : allDungeons) {
                    if (&dungeon != this && dungeon.isActive()) {
                        otherDungeonsActive = true;
                        break;
                    }
                }
            }

            {  // Scope for printLock
                // Lock the printing mutex before output
                lock_guard<mutex> printLock(printMutex);
                cout << "Dungeon " << id << " served party " << party.getId() << " in " << serve_time << " seconds." << endl;
                
                if (otherDungeonsActive) {
                    cout << "Dungeon " << id << " is now on cooldown for " << timeout << " seconds." << endl << endl;
                }
                
                // Show status of all dungeon instances
                cout << "Current Dungeon Instances Status:" << endl;
                for (const auto& dungeon : allDungeons) {
                    cout << "Dungeon " << dungeon.getId() << ": " 
                         << (dungeon.isActive() ? "Active" : "Inactive")
                         << ((&dungeon == this && !dungeon.isActive() && justBecameInactive) ? " <-- Just finished!" : "")
                         << endl;
                }
                cout << "----------------------------------------" << endl << endl;
            }  // Release printLock

            // Apply timeout after releasing printLock if needed
            if (otherDungeonsActive) {
                this_thread::sleep_for(chrono::seconds(timeout));
            }
        }
        else if (partyQueue.empty()) {
            break;
        }
    }
}

unsigned int DungeonInstance::getId() const {
    return id;
}

unsigned int DungeonInstance::getPartiesServed() const {
    return parties_served;
}

unsigned int DungeonInstance::getTotalTimeServed() const {
    return total_time_served;
}

void DungeonInstance::stop() {
    active = false;
}

bool DungeonInstance::isActive() const {
    return active;
}
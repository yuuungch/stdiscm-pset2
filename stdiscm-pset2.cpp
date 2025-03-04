#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <cstdlib>
#include <ctime>
#include "config_reader.h"
#include "dungeon_instance.h"
#include "party.h"

using namespace std;

void create_parties(int& tanks, int& healers, int& dps, queue<Party>& partyQueue) {
    int partyId = 1;
    while (tanks > 0 && healers > 0 && dps >= 3) {
        partyQueue.push(Party(partyId++, 1, 1, 1, 1, 1)); // Create a new party
        tanks--;
        healers--;
        dps -= 3;
    }

    cout << "Created " << partyQueue.size() << " parties." << endl << endl;
}

void run_dungeon(DungeonInstance& instance, queue<Party>& partyQueue, mutex& queueMutex, condition_variable& cv) {
    instance.run(partyQueue, queueMutex, cv);
}

DungeonInstance* getDungeonInstance(vector<DungeonInstance>& dungeons) {
    for (auto& dungeon : dungeons) {
        if (!dungeon.isActive()) {
            return &dungeon;
        }
    }
    return nullptr;
}

int main() {
    auto config = read_config("config.txt");
    if (config.empty()) {
        return -1;
    }

    int n = config["n"];
    int t1 = config["t1"];
    int t2 = config["t2"];
    if (t2 > 15) {
        t2 = 15;
    }

    int tanks, healers, dps;
    cout << "Enter the number of tank players in the queue: ";
    cin >> tanks;
    cout << "Enter the number of healer players in the queue: ";
    cin >> healers;
    cout << "Enter the number of DPS players in the queue: ";
    cin >> dps;

    system("cls");

    srand(time_t(NULL));
    vector<DungeonInstance> dungeons;
    vector<thread> threads;
    queue<Party> partyQueue;
    mutex queueMutex;
    condition_variable cv;

    create_parties(tanks, healers, dps, partyQueue);

	cout << "---------- Dungeon Raiding Simulation ----------" << endl << endl;

    // Instantiate dungeon instances
    for (int i = 0; i < n; ++i) {
        dungeons.emplace_back(i, t1, t2);
    }

    // Attach parties to dungeon instances and run them in separate threads
    for (auto& dungeon : dungeons) {
        threads.emplace_back(run_dungeon, ref(dungeon), ref(partyQueue), ref(queueMutex), ref(cv));
    }

    // Notify all threads to start processing parties
    cv.notify_all();

    // Wait for all threads to finish
    for (auto& thread : threads) {
        thread.join();
    }

    // Stop all instances
    for (auto& dungeon : dungeons) {
        dungeon.stop();
    }

    // Notify all threads to exit
    cv.notify_all();

    cout << endl << "----------- Summary of LFG Dungeon ----------" << endl << endl;

    // Show summary
    for (const auto& dungeon : dungeons) {
        cout << "Dungeon " << dungeon.getId() << " served " << dungeon.getPartiesServed() << " parties for a total of "
            << dungeon.getTotalTimeServed() << " seconds." << endl;
    }

    cout << endl <<"---------- Unused Roles ----------" << endl << endl;

    // Display unused players
    cout << "Unused tanks: " << tanks << endl;
    cout << "Unused healers: " << healers << endl;
    cout << "Unused DPS: " << dps << endl;

    return 0;
}
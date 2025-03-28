#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <cstdlib>
#include <ctime>
#include "../header_files/config_reader.h"
#include "../header_files/dungeon_instance.h"
#include "../header_files/party.h"

using namespace std;

void create_parties(unsigned int& tanks, unsigned int& healers, unsigned int& dps, queue<Party>& partyQueue) {
    unsigned int partyId = 1;
    while (tanks > 0 && healers > 0 && dps >= 3) {
        partyQueue.push(Party(partyId++, 1, 1, 1, 1, 1)); // Create a new party
        tanks--;
        healers--;
        dps -= 3;
    }

    cout << "Created " << partyQueue.size() << " parties." << endl << endl;
}

void run_dungeon(DungeonInstance& instance, queue<Party>& partyQueue, mutex& queueMutex, condition_variable& cv, const vector<DungeonInstance>& allDungeons, mutex& printMutex) {
    instance.run(partyQueue, queueMutex, cv, allDungeons, printMutex);
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
    int choice;
    do {
        cout << "Choose a test configuration:" << endl;
        cout << "1. All valid values" << endl;
        cout << "2. Invalid number of dungeons (n > 10)" << endl;
        cout << "3. Invalid minimum service time (t1 > t2)" << endl;
        cout << "4. Invalid maximum service time (t2 > 15)" << endl;
        cout << "5. Overflow tanks value" << endl;
        cout << "6. Overflow healers value" << endl;
        cout << "7. Overflow DPS value" << endl;
        cout << "8. Negative tanks value" << endl;
        cout << "9. Negative healers value" << endl;
        cout << "10. Negative DPS value" << endl;
        cout << "\nEnter your choice (1-10): ";

        cin >> choice;

        if (choice < 1 || choice > 10) {
            cout << "\nInvalid choice. Please try again." << endl << endl;
        }
    } while (choice < 1 || choice > 10);

    string config_file;
    switch(choice) {
        case 1: config_file = "test_configs/all_valid.txt"; break;
        case 2: config_file = "test_configs/invalid_n.txt"; break;
        case 3: config_file = "test_configs/invalid_t1.txt"; break;
        case 4: config_file = "test_configs/invalid_t2.txt"; break;
        case 5: config_file = "test_configs/overflow_tanks.txt"; break;
        case 6: config_file = "test_configs/overflow_healers.txt"; break;
        case 7: config_file = "test_configs/overflow_dps.txt"; break;
        case 8: config_file = "test_configs/negative_tanks.txt"; break;
        case 9: config_file = "test_configs/negative_healers.txt"; break;
        case 10: config_file = "test_configs/negative_dps.txt"; break;
    }

    cout << "\nUsing configuration file: " << config_file << endl << endl;

    auto config = read_config(config_file);
    if (config.empty()) {
        cerr << "\nFailed to read configuration. Please check the config file and ensure all values are valid." << endl;
        return -1;
    }

    unsigned int n = config["n"];
    unsigned int t1 = config["t1"];
    unsigned int t2 = config["t2"];
    if (t2 > 15) {
        t2 = 15;
    }

    unsigned int tanks = config["tanks"];
    unsigned int healers = config["healers"];
    unsigned int dps = config["dps"];

    system("cls");

    srand(time_t(NULL));
    vector<DungeonInstance> dungeons;
    vector<thread> threads;
    queue<Party> partyQueue;
    mutex queueMutex;
    mutex printMutex;
    condition_variable cv;

    cout << "Configuration loaded successfully:" << endl;
    cout << "Number of dungeon instances: " << n << endl;
    cout << "Service time range: " << t1 << " - " << t2 << " seconds" << endl;
    cout << "Players in queue: " << tanks << " tanks, " << healers << " healers, " << dps << " DPS" << endl << endl;

    create_parties(tanks, healers, dps, partyQueue);

    cout << "---------- Dungeon Raiding Simulation ----------" << endl << endl;

    // Instantiate dungeon instances
    for (unsigned int i = 0; i < n; ++i) {
        dungeons.emplace_back(i, t1, t2);
    }

    // Attach parties to dungeon instances and run them in separate threads
    for (auto& dungeon : dungeons) {
        threads.emplace_back(run_dungeon, ref(dungeon), ref(partyQueue), ref(queueMutex), ref(cv), ref(dungeons), ref(printMutex));
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

    unsigned int total_parties = 0;
    unsigned int total_time = 0;
    for (const auto& dungeon : dungeons) {
        cout << "Dungeon " << dungeon.getId() << " served " << dungeon.getPartiesServed() 
             << " parties in " << dungeon.getTotalTimeServed() << " seconds" << endl;
        total_parties += dungeon.getPartiesServed();
        total_time += dungeon.getTotalTimeServed();
    }

    cout << endl << "Total parties served: " << total_parties << endl;
    cout << "Total service time: " << total_time << " seconds" << endl;
    cout << "Average service time per party: " << (total_parties > 0 ? static_cast<float>(total_time) / total_parties : 0) << " seconds" << endl;

    cout << endl << "---------- Unused Roles ----------" << endl << endl;

    // Display unused players
    cout << "Unused tanks: " << tanks << endl;
    cout << "Unused healers: " << healers << endl;
    cout << "Unused DPS: " << dps << endl;

    return 0;
}
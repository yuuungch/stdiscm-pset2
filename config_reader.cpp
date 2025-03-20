#include "config_reader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

using namespace std;

unordered_map<string, int> read_config(const string& filename) {
    unordered_map<string, int> config;
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error: Could not open config file." << endl;
        return config;
    }

    // Required configuration keys
    vector<string> required_keys = {"n", "t1", "t2", "tanks", "healers", "dps"};
    unordered_map<string, string> key_descriptions = {
        {"n", "number of dungeon instances"},
        {"t1", "minimum service time"},
        {"t2", "maximum service time"},
        {"tanks", "number of tank players"},
        {"healers", "number of healer players"},
        {"dps", "number of DPS players"}
    };

    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string key;
        int value;
        if (!(iss >> key >> value)) {
            cerr << "Error: Invalid line in config file: " << line << endl;
            continue;
        }
        
        // Validate values based on key
        bool valid = true;
        if (value <= 0) {
            cerr << "Error: Invalid value for " << key << ". Must be a positive integer." << endl;
            valid = false;
        }
        else if (key == "n" && value > 10) {
            cerr << "Error: Number of dungeon instances (n) cannot exceed 10." << endl;
            valid = false;
        }
        else if (key == "t2" && value > 15) {
            cerr << "Warning: Maximum service time (t2) capped at 15 seconds." << endl;
            value = 15;
        }
        
        if (valid) {
            config[key] = value;
        }
    }

    file.close();

    // Check for missing required keys
    bool missing_keys = false;
    for (const auto& key : required_keys) {
        if (config.find(key) == config.end()) {
            cerr << "Error: Missing required configuration for " << key_descriptions[key] << " (" << key << ")" << endl;
            missing_keys = true;
        }
    }

    // Additional validation
    if (!missing_keys) {
        if (config["t1"] > config["t2"]) {
            cerr << "Error: Minimum service time (t1) cannot be greater than maximum service time (t2)" << endl;
            config.clear();
        }
    }

    if (missing_keys) {
        config.clear();
    }

    return config;
}
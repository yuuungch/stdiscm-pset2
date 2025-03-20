#include "config_reader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <limits>

using namespace std;

unordered_map<string, unsigned int> read_config(const string& filename) {
    unordered_map<string, unsigned int> config;
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
        unsigned long long value; // Using larger type to check for overflow
        if (!(iss >> key >> value)) {
            cerr << "Error: Invalid line in config file: " << line << endl;
            continue;
        }
        
        // Check for overflow
        if (value > numeric_limits<unsigned int>::max()) {
            cerr << "Error: Value for " << key << " (" << value << ") exceeds maximum allowed value (" 
                 << numeric_limits<unsigned int>::max() << ")" << endl;
            continue;
        }

        // Validate values based on key
        bool valid = true;
        if (value == 0) {
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
            config[key] = static_cast<unsigned int>(value);
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
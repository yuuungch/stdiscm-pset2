#include "config_reader.h"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

unordered_map<string, int> read_config(const string& filename) {
    unordered_map<string, int> config;
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error: Could not open config file." << endl;
        return config;
    }

    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string key;
        int value;
        if (!(iss >> key >> value)) {
            cerr << "Error: Invalid line in config file: " << line << endl;
            continue;
        }
        if (value <= 0) {
            cerr << "Error: Invalid value for " << key << ". Must be a positive integer." << endl;
            continue;
        }
        config[key] = value;
    }

    file.close();
    return config;
}
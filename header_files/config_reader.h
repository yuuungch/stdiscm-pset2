#ifndef CONFIG_READER_H
#define CONFIG_READER_H

#include <unordered_map>
#include <string>

using namespace std;

unordered_map<string, unsigned int> read_config(const string& filename);

#endif // CONFIG_READER_H
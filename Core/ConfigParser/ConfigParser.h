#pragma once

#include <cctype>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <functional>
#include <unordered_map>

using namespace std;

class ConfigParser
{
private:
    unordered_map<string, string> configData_;

public:
    bool ParseConfig(const string &fileName);

    inline void Trim(string &str);

    bool GetStringValue(const string &name, string &value);
    bool GetIntValue(const string &name, int &value);
};
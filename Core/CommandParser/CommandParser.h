#pragma once

#include <string>
#include <iostream>
#include <unordered_map>

using namespace std;

class CommandParser
{
private:
    unordered_map<string, pair<string, string>> commands_;
    void ShowError();

public:
    void AddOption(const string &option, const string &defaults = "", const string &hint = "");
    void ParseArgs(int &argc, const char **argv);
    const string &GetStringOption(const string &option);
};
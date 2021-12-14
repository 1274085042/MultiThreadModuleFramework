#pragma once

#include "../Core/CommandParser/CommandParser.h"

struct CmdParameters
{
    string config;
};

void ParseCommandLine(int argc, const char **argv, CmdParameters &cmdParameters)
{
    cout<< "Begin to parse command line..."<<endl;
    CommandParser commandParser;
    commandParser.AddOption("--config", "../config.txt", "The config file using for pipeline.");

    commandParser.ParseArgs(argc, argv);

    cmdParameters.config = commandParser.GetStringOption("--config");
}
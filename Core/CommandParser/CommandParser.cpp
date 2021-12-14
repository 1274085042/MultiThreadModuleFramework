#include "CommandParser.h"

const int MOD2 = 2;  

void CommandParser::ShowError()
{
    cerr << "Wrong parameter!" << endl;
    exit(0);
}

/*
* @brief：添加固定的选项 
*/ 
void CommandParser::AddOption(const string &option, const string &defaults, const string &hint)
{
    commands_[option] = make_pair(defaults, hint);
}

/*
* @brief：检查命令行并解析
*         (1) 检查参数个数是否正确
*         (2) 检查选项名称和格式
*/
void CommandParser::ParseArgs(int &argc, const char **argv)
{
    if(argc % MOD2 == 0)    //确保main后面的参数是成对的
    {
        ShowError();
    }

    for(int i = 1; i < argc; ++i)   //检查选项和参数格式
    {
        if(i + 1 < argc && argv[i][0] == '-' && argv[i][1] == '-' && argv[i + 1][0] != '-' )
        {
            ++i;
            continue;
        }
        else
        {
            ShowError();
        }
    }

    for(int i = 1; i < argc; ++i) //检查长选项的名称是否正确
    {
        if(commands_.find(argv[i]) == commands_.end())
        {
            ShowError();
        }
        ++i;
    }

    for(int i = 1; i < argc; ++i)
    {
        commands_[argv[i]].first = argv[i + 1];
        ++i;
    }
}

const string &CommandParser::GetStringOption(const string &option)
{
    if(commands_.find(option) == commands_.end())
    {
        cerr << "Get option fail" << endl;
        ShowError();
    }

    return commands_[option].first;
}

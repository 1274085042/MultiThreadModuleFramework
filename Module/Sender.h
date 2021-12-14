#pragma once
#include "../Core/ModuleManager/ModuleBase.h"
#include "../Core/ModuleManager/ModuleFactory.h"

class Sender:public ModuleBase
{
public:
    ~Sender()
    {
        isStop_ = true;
        if(processThread_.joinable())
        {
            processThread_.join();
        }
    }

    bool Init(ConfigParser &configParser, ModuleInitArguments &initArgs)
    {
        
    }

    bool Process(shared_ptr<void> input)
    {

    }

};

MODULE_REGISTER(Sender)   

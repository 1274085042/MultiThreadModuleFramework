#pragma once
#include "../Core/ModuleManager/ModuleBase.h"
#include "../Core/ModuleManager/ModuleFactory.h"

class Receiver:public ModuleBase
{
public:
    ~Receiver()
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

MODULE_REGISTER(Receiver)   

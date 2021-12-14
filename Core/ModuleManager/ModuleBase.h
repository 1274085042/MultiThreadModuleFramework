#pragma once

#include "../ConfigParser/ConfigParser.h"
#include "../ThreadSafeQueue/ThreadSafeQueue.h"
#include <atomic>
#include <thread>

using namespace std;

struct ModuleInitArguments
{
    string moduleName;
    int instanceId;
};

struct ModuleOutputInformation
{
    string moduleName;
    vector<shared_ptr<ThreadSafeQueue<shared_ptr<void>>>> outputQueueVector;
    int outputQueueVectorSize;
};

class ModuleBase
{
protected:
    int instanceId_ = -1;
    string moduleName_ = {};

    bool withoutInputQueue_ = false;
    atomic_bool isStop_ = {};

    shared_ptr<ThreadSafeQueue<shared_ptr<void>>> inputQueue_ = nullptr;
    unordered_map<string, ModuleOutputInformation> outputInformationMap_ = {};

    thread processThread_ = {};

    void AssignInitArgs(ModuleInitArguments &initArgs);
    
    void ProcessThread();
    virtual bool Process(shared_ptr<void> input) = 0;

public:
    virtual ~ModuleBase() {};
    virtual bool Init(ConfigParser &configParser, ModuleInitArguments &initArgs) = 0;
    void SetInputQueue(shared_ptr<ThreadSafeQueue<shared_ptr<void>>> inputQueue);
    void SetOutputInfo(string moduleName, vector<shared_ptr<ThreadSafeQueue<shared_ptr<void>>>> outputQueueVector);

    bool Run();
    void SendToNextModule(string moduleNext, shared_ptr<void> output, int channelId = 0);
};
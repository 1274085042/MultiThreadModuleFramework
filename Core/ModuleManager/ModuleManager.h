#pragma once

#include "ModuleBase.h"
#include "ModuleFactory.h"
#include <vector>
#include <memory>
#include <unordered_map>

using namespace std;

struct ModuleDescription
{
    string moduleName;
    int moduleCount;
};  

struct ModuleConnectDescription
{
    string moduleSend;
    string moduleReceive;
};

struct ModulesInformation
{
    vector<shared_ptr<ModuleBase>> moduleVector;
    vector<shared_ptr<ThreadSafeQueue<shared_ptr<void>>>> inputQueueVector;
};

/*
* @brief：用于管理module实例
*/
class ModuleManager
{
private:
    unordered_map<string, ModulesInformation> pipelineMap_ = {}; 
    ConfigParser configParser_ = {};

    bool InitModuleInstance(shared_ptr<ModuleBase> moduleInstance, string moduleName, int instanceId);
    bool RegisterModuleInputConnect(string moduleName, vector<shared_ptr<ThreadSafeQueue<shared_ptr<void>>>> inputQueueVector);
    bool RegisterModuleOutputConnect(string moduleSend, string moduleRecv, vector<shared_ptr<ThreadSafeQueue<shared_ptr<void>>>> outputQueueVector);

public:
    bool Init(const string &configPath);
    bool RegisterModules(ModuleDescription *moduleDescription, int moduleTypeCount, int channelCount);
    bool RegisterModuleConnects(ModuleConnectDescription *moduleConnectDescription, int moduleConnectCount);
    bool RunPipeline();

};
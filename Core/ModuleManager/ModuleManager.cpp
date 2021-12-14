#include "ModuleManager.h"

bool ModuleManager::Init(const string &configPath)
{
    cout << "ModuleManager: begin to init."<<endl;
    bool ret = configParser_.ParseConfig(configPath);
    if(ret != true)
    {
        cerr << "ModuleManager: cannot parse file." << endl;
        return ret;
    }

    return ret;
}

bool ModuleManager::InitModuleInstance(shared_ptr<ModuleBase> moduleInstance, string moduleName, int instanceId)
{
    cout << "ModuleManager: begin to init " << moduleName <<" instance." << endl;
    
    ModuleInitArguments initArgs;
    initArgs.moduleName = moduleName;
    initArgs.instanceId = instanceId;

    bool ret = moduleInstance->Init(configParser_, initArgs);

    if(ret != true)
    {
        cerr << "ModuleManager: fail to init " << moduleName << " , instance id = " << instanceId << "." << endl;
        return ret;
    }

    return ret;
}

bool ModuleManager::RegisterModules(ModuleDescription *modulesDescription, int moduleTypeCount, int channelCount)
{

    shared_ptr<ModuleBase> moduleInstance = nullptr;

    for(int i = 0; i < moduleTypeCount; ++i)
    {
        ModuleDescription moduleDescription = modulesDescription[i];
        int moduleCount = (moduleDescription.moduleCount == -1) ? channelCount : moduleDescription.moduleCount;

        ModulesInformation modulesInformation;
        for(int j = 0; j < moduleCount; ++j)
        {
            moduleInstance.reset(static_cast<ModuleBase *>(ModuleFactory::MakeModule(moduleDescription.moduleName)));

            //Module实例初始化
            bool ret = InitModuleInstance(moduleInstance, modulesDescription->moduleName, j);

            if (ret != true)
            {
                return ret;
            }

            modulesInformation.moduleVector.push_back(moduleInstance);
        }

        pipelineMap_[moduleDescription.moduleName] = modulesInformation;
    }

    return true;
}

bool ModuleManager::RegisterModuleConnects(ModuleConnectDescription *moduleConnectDescription, int moduleConnectCount)
{
    shared_ptr<ThreadSafeQueue<shared_ptr<void>>> dataQueue = nullptr;

    for(int i = 0; i<moduleConnectCount; ++i)
    {
        ModuleConnectDescription connectDescription = moduleConnectDescription[i];
        cout << "Add connect " << connectDescription.moduleSend << "-" <<connectDescription.moduleReceive << endl;

        auto itSend = pipelineMap_.find(connectDescription.moduleSend);
        auto itRecv = pipelineMap_.find(connectDescription.moduleReceive);

        if(itSend == pipelineMap_.end() || itRecv == pipelineMap_.end())
        {
            cerr << "Cannot find module!" << endl;
            return false;
        }

        //如果有n个接受module实例，则搭建n个管道
        ModulesInformation moduleInfoRecv = itRecv->second;
        for(int j = 0; j < moduleInfoRecv.moduleVector.size(); ++j)
        {
            dataQueue = make_shared<ThreadSafeQueue<shared_ptr<void>>>();
            moduleInfoRecv.inputQueueVector.push_back(dataQueue);
        }

        RegisterModuleInputConnect(connectDescription.moduleReceive, moduleInfoRecv.inputQueueVector);
        RegisterModuleOutputConnect(connectDescription.moduleSend, connectDescription.moduleReceive, moduleInfoRecv.inputQueueVector);
    }

    return true;
}

bool ModuleManager::RegisterModuleInputConnect(string moduleName, vector<shared_ptr<ThreadSafeQueue<shared_ptr<void>>>> inputQueueVector)
{
    auto it = pipelineMap_.find(moduleName);
    if(it != pipelineMap_.end())
    {
        ModulesInformation moduleInformation = it->second;

        if(moduleInformation.moduleVector.size() != inputQueueVector.size())
        {
            return false;
        }

        shared_ptr<ThreadSafeQueue<shared_ptr<void>>> inputQueue = nullptr;

        for(int i = 0; i < moduleInformation.moduleVector.size(); ++i)
        {
            inputQueue = inputQueueVector[i];
            shared_ptr<ModuleBase> moduleInstance = moduleInformation.moduleVector[i];
            moduleInstance->SetInputQueue(inputQueue);
        }
        return true;
    }
}

bool ModuleManager::RegisterModuleOutputConnect(string moduleSend, string moduleRecv, vector<shared_ptr<ThreadSafeQueue<shared_ptr<void>>>> outputQueueVector)
{
    auto it = pipelineMap_.find(moduleSend);
    if(it != pipelineMap_.end())
    {
        ModulesInformation moduleInformation = it->second;
        for(int i = 0; i < moduleInformation.moduleVector.size(); ++i)
        {
            shared_ptr<ModuleBase> moduleInstance = moduleInformation.moduleVector[i];
            moduleInstance->SetOutputInfo(moduleRecv, outputQueueVector);
        }

        return true;
    }
}

bool ModuleManager::RunPipeline()
{
    cout << "ModuleManager: run pipeline." << endl;

    shared_ptr<ModuleBase> moduleInstance;

    for(auto it = pipelineMap_.begin(); it != pipelineMap_.end(); ++it)
    {
        ModulesInformation modulesInfo = it->second;
        for(int i = 0; i < modulesInfo.moduleVector.size(); ++i)
        {
            moduleInstance = modulesInfo.moduleVector[i];
            bool ret = moduleInstance->Run();
            if(ret != true)
            {
                cerr << "ModuleManager: fail to run module!" << endl;
                return ret;
            }
        }
    }

    cout << "ModuleManager: run pipeline success." << endl;
    return true;
} 

#include "ModuleBase.h"

void ModuleBase::AssignInitArgs(ModuleInitArguments &initArgs)
{
    moduleName_ = initArgs.moduleName;
    instanceId_ = initArgs.instanceId;
    isStop_ = false;
}

void ModuleBase::SetInputQueue(shared_ptr<ThreadSafeQueue<shared_ptr<void>>> inputQueue)
{
    inputQueue_ = inputQueue;
}

void ModuleBase::SetOutputInfo(string moduleName, vector<shared_ptr<ThreadSafeQueue<shared_ptr<void>>>> outputQueueVector)
{
    if(outputQueueVector.size() == 0)
    {
        cout << "outputQueueVector is empty!" << endl;
        return;
    }

    ModuleOutputInformation outputInformation;
    outputInformation.moduleName = moduleName;
    outputInformation.outputQueueVector = outputQueueVector;
    outputInformation.outputQueueVectorSize = outputQueueVector.size();
    outputInformationMap_[moduleName] = outputInformation;
}

bool ModuleBase::Run()
{
    cout << moduleName_ << "[" << instanceId_ << "] run."<<endl;
    /*
    * 线程对象在基类中，如果在派生类对象析构前，没有对该线程对象join()，程序报错
    */
    processThread_ = thread(&ModuleBase::ProcessThread, this);
    return true;
}

void ModuleBase::ProcessThread()
{
    bool ret;

    //没有输入queue，直接调用Process函数
    if(withoutInputQueue_ == true)
    {
        bool ret = Process(nullptr);
        if (ret != true)
        {
            cerr << "Fail to process data for " << moduleName_ << "[" << instanceId_ << "]!" << endl;
        }
        return;
    }

    if(inputQueue_ == nullptr);
    {
        cerr << "Invalid input queue of " << moduleName_ << "[" << instanceId_ << "]!" << endl;
        return;
    }

    //不断从输入queue弹出数据，并调用Process()
    while(!isStop_)
    {
        shared_ptr<void> input = nullptr;
        inputQueue_->Pop(input);
        if(input == nullptr)
        {
            cerr << "Fail to get data from input queue for " << moduleName_ << "[" << instanceId_ << "]!" << endl;
            continue;
        }
        
        bool ret = Process(input);
        if(ret != true)
        {
            cerr << "Fail to process data for " << moduleName_ << "[" << instanceId_ << "]!" << endl;
        }
    }
    
    cout << moduleName_ << "[" << instanceId_ << "] process thread end." << endl;
}

void ModuleBase::SendToNextModule(string moduleNext, shared_ptr<void> output, int channelId)
{
    if(outputInformationMap_.find(moduleNext) == outputInformationMap_.end())
    {
        cerr << "No next module!" << endl;
        return;
    }

    ModuleOutputInformation moduleOutputInformantion = (outputInformationMap_.find(moduleNext))->second;

    unsigned ch = channelId % moduleOutputInformantion.outputQueueVectorSize;
    if(ch >= moduleOutputInformantion.outputQueueVectorSize)
    {
        cerr << "No next module!" << endl;
        return;
    }

    moduleOutputInformantion.outputQueueVector[ch]->Push(output);
}

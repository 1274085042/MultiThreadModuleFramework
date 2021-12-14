#pragma once

#include <string>
#include <functional>
#include <unordered_map>

using namespace std;

using Constructor = function<void * ()>;

#define MODULE_REGISTER(class_name)                                                 \
class class_name##Helper                                                            \
{                                                                                   \
public:                                                                             \
    static void* CreatObj()                                                         \
    {                                                                               \
        return new class_name;                                                      \
    }                                                                               \
    class_name##Helper()                                                            \
    {                                                                               \
        ModuleFactory::RegisterModule(#class_name, class_name##Helper::CreatObj);   \
    }                                                                               \
};                                                                                  \
static class_name##Helper class_name##helper;

class ModuleFactory
{
private:
    inline static unordered_map<string, Constructor> &Constructors()
    {
        static unordered_map<string, Constructor> constructorMap;
        return constructorMap;
    }

public:
    static void RegisterModule(const string &className, Constructor constructor)
    {
        Constructors()[className] = constructor;
    }

    static void* MakeModule(const string &className)    
    {
        auto it = Constructors().find(className);
        if(it == Constructors().end())
        {
            return nullptr;
        }
        return (it->second)();
    }
};
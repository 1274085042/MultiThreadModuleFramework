#include "Module/Sender.h"
#include "Module/Receiver.h"
#include "Facilities/CommandLine.h"
#include "Core/ModuleManager/ModuleManager.h"

const int MODULE_TYPE_COUNT = 2;
const int MODULE_CONNECT_COUNT = 1; 

ModuleDescription moduleDescription[MODULE_TYPE_COUNT] = {
                                                            {"Receiver" , -1},
                                                            {"Sender" , -1}
                                                         };

ModuleConnectDescription moduleConnectDescription[MODULE_CONNECT_COUNT] = {
                                                                            {"Receiver", "Sender"}
                                                                          };

bool InitModuleManager(ModuleManager &moduleManager, string &configPath)
{
  cout << "InitModuleManager() begin "<< endl;

  ConfigParser configParser;

  bool ret = configParser.ParseConfig(configPath);
  if(ret != true)
  {
    cerr << "Cannot parse file!" << endl;
    return ret;
  }

  int channelCount = 0;

  ret = configParser.GetIntValue("SystemConfig.channelCount", channelCount);

  if(ret !=  true)
  {
    return ret;
  }

  if(channelCount <= 0)
  {
    cerr << "Invalid channel count!" << endl;
    return false;
  }

  cout << "ModuleManager: begin to init" << endl;
  ret = moduleManager.Init(configPath);
  if(ret != true)
  {
    cerr << "Fail to init system manager!" <<endl;
    return ret;
  }

  ret = moduleManager.RegisterModules(moduleDescription, MODULE_TYPE_COUNT, channelCount);
  if(ret != true)
  {
    return ret;
  }

  ret = moduleManager.RegisterModuleConnects(moduleConnectDescription, MODULE_CONNECT_COUNT);
  if(ret != true)
  {
    return ret;
  }

  return true;
}

/*
* 1 解析命令行，得到配置文件路径
* 2 使用ModuleManager管理各个模块
*/
int main(int argc, const char *argv[])
{
    CmdParameters cmdParams;
    ParseCommandLine(argc, argv, cmdParams);
    //cout<<cmdParams.config_<<endl;

    ModuleManager moduleManager;
    InitModuleManager(moduleManager, cmdParams.config);
    moduleManager.RunPipeline();

    /*
    TODO:接受用户触发的外部中断，注意用户触发的外部中断不会使程序的变量正常析构
    */
    return 0;
}

框架提供的功能：  
1 接收Module的输入源是唯一的  
&ensp; 发送Module的输出源不是唯一的    
&ensp; 这里的唯一指的是发送数据的Module类型只能是一种  
&ensp; 不唯一指的是接受数据的Module类型可以是多种
![][image1]  

2 多线程  
&ensp;一个线程对应一个Module的处理过程  
3 命令行解析  
4 配置文件解析  
5 工厂方法
  
**用户只需写配置文件（config.txt）以及自己业务需要的模块（Module），剩余的工作交给ModuleManager类处理，包括：**  
- Module的实例化
- Module之间的连接  
- 各个Module的运行

[//]:#(reference)
[image1]:./Explanation/ModulesConnect.png
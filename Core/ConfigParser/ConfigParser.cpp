#include "ConfigParser.h"

const char COMMENT_CHAR = '#';

inline void ConfigParser::Trim(string &str)
{
    str.erase(str.begin(), find_if(str.begin(), str.end(), not_fn(::isspace)));    //not_fn定义于C++17
    //base()返回底层迭代器，该迭代器指向的是reverse_iterator当前所指元素的下一个位置
    str.erase(find_if(str.rbegin(), str.rend(), not_fn(::isspace)).base(), str.end());    

    //str.erase(str.begin(), find_if(str.begin(), str.end(), not1(ptr_fun(isspace))));
    //str.erase(find_if(str.rbegin(), str.rend(), not1(ptr_fun(isspace))).base(), str.end());
}

bool ConfigParser::ParseConfig(const string &fileName)
{
    ifstream inFile(fileName);
    if(!inFile.is_open())
    {
       cout<<"Cannot read "<<fileName<<" file!"<<endl;
       return false; 
    }

    string line, newLine;
    int pos, startPos, endPos;

    while(getline(inFile, line))
    {
        if(line.empty())
        {
            continue;
        }
        else
        {
            startPos = 0;
            endPos = line.size() - 1;
            pos = line.find(COMMENT_CHAR);    //查找注释位置
            if(pos != -1)
            {
                if(pos == 0)
                {
                    continue;
                }

                endPos = pos -1;
            }

            newLine = line.substr(startPos, (endPos-startPos + 1));  //删除注释
            pos = newLine.find('=');

            if (pos = -1)
            {
                continue;
            }

            string sf = newLine.substr(0, pos);
            Trim(sf);   //删除空格，得到key

            string sb = newLine.substr(pos + 1, (endPos + 1) - (pos + 1));
            Trim(sb);   //删除空格，得到value

            configData_.insert(make_pair(sf, sb));  //插入key-value pair
        }

    }

    return true;
}

bool ConfigParser::GetStringValue(const string &name, string &value)
{
    if(configData_.count(name) == 0)
    {
        return false;
    }

    value = configData_.find(name)->second;
    return true;
}

bool ConfigParser::GetIntValue(const string &name, int &value)
{
    if(configData_.count(name) == 0)
    {
        return false;
    }

    string str = configData_.find(name)->second;
    if(!(stringstream(str) >> value))
    {
        return false;
    }

    return true;
}
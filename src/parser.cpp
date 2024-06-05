#include "parser.h"
#include <fstream>
#include <iostream>

std::string parser::returnFilename()
{
    return filename;
}

std::string parser::returnInfo()
{
    return erroinfo;
}

std::vector<std::vector<int>> &parser::getGraph()
{
    return Graph;
}

int parser::parse()
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        erroinfo = "file not open";
        return 1;
    }
    else
    {
        std::string numstr;
        std::string line;
        getline(file, numstr);
        int num = stoi(numstr);
        Graph.resize(num);
        while (getline(file, line))
        {
            std::string tmp;
            bool isIndex = true;
            int index = -1;
            for (int i = 0; line[i] != '\0'; i++)
            {
                if (line[i] != ' ')
                    tmp += line[i];
                else
                {
                    if (isIndex)
                    {
                        index = stoi(tmp);
                        tmp = "";
                        isIndex = false;
                    }
                    else
                    {
                        if (index >= num)
                        {
                            erroinfo = "node overflow";
                            return 1;
                            break;
                        }
                        else
                        {
                            Graph[index].push_back(stoi(tmp));
                            tmp = "";
                        }
                    }
                }
            }
            if (index >= 0)
                Graph[index].push_back(stoi(tmp));
        }
    }
    return 0;
}
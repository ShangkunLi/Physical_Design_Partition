#pragma once
#include <vector>
#include <fstream>
#include <string>

class parser
{
public:
    parser(std::string filename)
    {
        this->filename = filename;
    }
    int parse();
    std::vector<std::vector<int>> &getGraph();
    std::string returnInfo();
    std::string returnFilename();

private:
    std::string filename;
    std::string erroinfo;
    std::vector<std::vector<int>> Graph;
};

#pragma once
#include "app/Steps.h"
#include <string>
#include <vector>

struct CliConfig
{
    std::string inputPath;
    std::string outputPath;
    std::vector<Step> inputSteps;  
    std::vector<Step> outputSteps; 
};

CliConfig ParseCli(int argc, char** argv);
void PrintUsage();

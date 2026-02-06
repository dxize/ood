#pragma once
#include "app/Steps.h"
#include <string>
#include <vector>

struct CliConfig
{
    std::string inputPath;
    std::string outputPath;
    std::vector<Step> inputSteps;   // decrypt/decompress
    std::vector<Step> outputSteps;  // encrypt/compress
};

CliConfig ParseCli(int argc, char** argv);
void PrintUsage();

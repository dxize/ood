#include "app/Cli.h"
#include "app/TransformPipeline.h"
#include <iostream>

int main(int argc, char** argv)
{
    try 
    {
        CliConfig cfg = ParseCli(argc, argv);
        return RunTransform(cfg);
    }
    catch (const std::exception& e) 
    {
        std::cerr << "Error: " << e.what() << "\n\n";
        PrintUsage();
        return 2;
    }
}

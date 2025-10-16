#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include "CommandProcessor.h"

std::vector<std::string> SplitCommand(const std::string& input) 
{
    std::vector<std::string> args;
    std::istringstream iss(input);
    std::string token;
    while (iss >> token)
    {
        args.push_back(token);
    }
    return args;
}

void MainAlg(std::string& input, CommandProcessor& processor)
{
    while (true)
    {
        std::cout << "> ";
        std::getline(std::cin, input);
        if (input.empty())
        {
            continue;
        }

        auto args = SplitCommand(input);

        if (!args.empty() && (args[0] == "exit" || args[0] == "quit"))
        {
            break;
        }

        processor.ProcessCommand(args);
    }
}

int main()
{
    try 
    {
        shapes::CPicture picture;
        CCanvas canvas("output.svg");
        CommandProcessor processor(picture, canvas);

        std::cout << "Write \"Help\" to get started\n";

        std::string input;

        MainAlg(input, processor);

        canvas.Finish();

    }
    catch (const std::exception& e) 
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
//повторить отношения
//понерфить график
#include "app/Cli.h"
#include <iostream>
#include <stdexcept>

void PrintUsage() 
{
    std::cerr <<
        "Usage:\n"
        "  transform [options] <input-file> <output-file>\n\n"
        "Options:\n"
        "  --encrypt <key>     Encrypt on write (can repeat)\n"
        "  --decrypt <key>     Decrypt on read (can repeat)\n"
        "  --compress          RLE compress on write\n"
        "  --decompress        RLE decompress on read\n\n"
        "Examples:\n"
        "  transform --encrypt 3 --encrypt 100500 --compress orig1.gif output.dat\n"
        "  transform --decompress --decrypt 100500 --decrypt 3 output.dat restoredOrig1.gif\n";
}

static uint32_t ParseKey(const std::string& s) 
{
    unsigned long long v = 0;

    try 
    {
        std::size_t pos = 0;
        v = std::stoull(s, &pos, 10);
        if (pos != s.size())
        {
            throw std::invalid_argument("bad");
        }
    }
    catch (...) 
    {
        throw std::invalid_argument("Invalid key: " + s);
    }

    if (v > 0xFFFFFFFFull)//v > uint32_t
    {
        throw std::invalid_argument("Key out of range: " + s);
    }

    return static_cast<uint32_t>(v);
}

CliConfig ParseCli(int argc, char** argv) 
{
    if (argc < 3) 
    {
        throw std::invalid_argument("Not enough arguments");
    }

    CliConfig cfg;
    cfg.inputPath = argv[argc - 2];
    cfg.outputPath = argv[argc - 1];

    for (int i = 1; i <= argc - 3; ++i) 
    {
        std::string a = argv[i];

        if (a == "--encrypt") 
        {
            if (i + 1 > argc - 3)
            {
                throw std::invalid_argument("--encrypt needs <key>");
            }

            uint32_t key = ParseKey(argv[++i]);
            cfg.outputSteps.push_back({ StepKind::Encrypt, key });
        }
        else if (a == "--decrypt") 
        {
            if (i + 1 > argc - 3)
            {
                throw std::invalid_argument("--decrypt needs <key>");
            }

            uint32_t key = ParseKey(argv[++i]);
            cfg.inputSteps.push_back({ StepKind::Decrypt, key });
        }
        else if (a == "--compress") 
        {
            cfg.outputSteps.push_back({ StepKind::Compress, 0 });
        }
        else if (a == "--decompress") 
        {
            cfg.inputSteps.push_back({ StepKind::Decompress, 0 });
        }
        else 
        {
            throw std::invalid_argument("Unknown option: " + a);
        }
    }

    return cfg;
}

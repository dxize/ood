#include "../../include/util/StringUtil.h"

#include <cctype>

namespace util
{
    std::string Trim(const std::string& s)
    {
        size_t start = 0;
        while (start < s.size() && std::isspace(static_cast<unsigned char>(s[start])))
        {
            ++start;
        }

        size_t end = s.size();
        while (end > start && std::isspace(static_cast<unsigned char>(s[end - 1])))
        {
            --end;
        }

        return s.substr(start, end - start);
    }

    bool IsBlankOrComment(const std::string& line)
    {
        const std::string t = Trim(line);
        return t.empty() || (!t.empty() && t[0] == '#');
    }

    std::string ToLowerAscii(std::string s)
    {
        for (char& ch : s)
        {
            ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
        }
        return s;
    }
} 

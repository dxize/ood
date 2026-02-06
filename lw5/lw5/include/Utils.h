#pragma once
#include <string>
#include <cctype>
#include <stdexcept>

inline std::string TrimLeft(std::string s)
{
    while (!s.empty() && std::isspace((unsigned char)s[0]))
    {
        s.erase(s.begin());
    }
    return s;
}

inline std::string HtmlEscape(const std::string& s)
{
    std::string out;
    out.reserve(s.size());

    for (char ch : s)
    {
        switch (ch)
        {
        case '&': out += "&amp;"; break;
        case '<': out += "&lt;"; break;
        case '>': out += "&gt;"; break;
        case '"': out += "&quot;"; break;
        case '\'': out += "&#39;"; break;
        default: out += ch; break;
        }
    }
    return out;
}

inline size_t ParseIndex1Based(const std::string& token)
{
    long long x = std::stoll(token);
    if (x <= 0)
    {
        throw std::runtime_error("Index must be >= 1");
    }
    return static_cast<size_t>(x);
}

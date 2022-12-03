#pragma once

#include <string>

class Report
{
private:
    static std::string _style;

public:
    Report(/* args */);
    ~Report();

    static const std::string &getStyle();
    // static const std::string &getHeader();
    // static const std::string &getBody();
    // static const std::string &geFooter();
    static std::string styleFromFile();
};

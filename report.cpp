#include <fstream>
#include <sstream>

#include "report.h"

std::string Report::_style;

Report::Report(/* args */)
{
}

Report::~Report()
{
}

const std::string &Report::getStyle()
{
    if (_style.empty())
    {
        _style = styleFromFile();
    }

    return _style;
}


std::string Report::styleFromFile()
{
    auto path = "./css/style.css";

    std::ifstream file;

    file.open(path);

    if (!file.is_open())
    {
        return "";
    }

    std::ostringstream os;
    os << file.rdbuf();
    file.close();

    return os.str();
}

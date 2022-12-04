#include <iostream>
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

void Report::open(std::string fileUrl)
{
    if (system(NULL))
    {
        std::cout << "Command Processor is present\n";

        // std::string originUrl{"file:///Users/goke/Library/CloudStorage/OneDrive-Personal/zs/schoolcpp/"};
        std::string url{""};
        int rcmd = 0;

#if defined(__APPLE__)
        url = "open " + fileUrl;
        rcmd = system(url.c_str());

        //  url={"open -a \"Google Chrome\" " + fileUrl};
        //  rcmd = system(url.c_str());
        //  url={"open -a \"Firefox\" " + fileUrl};
        //  rcmd = system(url.c_str());
        // url = {"open -a \"Edge\" " + fileUrl};
        // rcmd = system(url.c_str());

#endif // __APPLE__

#if defined(_WIN32)
        url = "start " + fileUrl;
        rcmd = system(url.c_str());
#endif // _WIN32

#if defined(__unix__)
        url = "xdg-open " + fileUrl;
        rcmd = system(url.c_str());
        if (rcmd > 0)
        {
            url = "wslview " + fileUrl;
            rcmd = system(url.c_str());
        }

#endif // __unix__

        // rcmd = system(url.c_str());
        // std::cout << rcmd;

        std::cout << rcmd << std::endl;
    }
    else
    {
        std::cout << "Command processor is not present\n";
    }
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

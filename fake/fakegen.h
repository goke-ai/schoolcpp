#pragma once

#include <sstream>
#include <iomanip>
#include <vector>
#include <string>

namespace gcore
{
    class FakeGen
    {
    private:
        /* data */
        static std::vector<std::string> SURNAMES;
        static std::vector<std::string> NAMES;
        static char GENDERS[3];

    public:
        FakeGen(/* args */);
        ~FakeGen();

        static std::string getSURNAME();
        static std::string getNAME();
        static char getGENDER();
        static std::string BIRTHDATE(int minYear = 1900, int maxYear = 2022);
    };
} // namespace gcore

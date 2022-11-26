#include <sstream>
#include <iomanip>
#include "utilities.h"
#include "datetime.h"
#include "fakegen.h"

namespace gcore
{
    FakeGen::FakeGen(/* args */)
    {
    }

    FakeGen::~FakeGen()
    {
    }

    std::string FakeGen::getSURNAME()
    {
        gcore::setTrueRandSeed();

        int i = gcore::randint(0, gcore::FakeGen::SURNAMES.size() - 1);
        return gcore::FakeGen::SURNAMES[i];
    }

    std::string FakeGen::getNAME()
    {
        gcore::setTrueRandSeed();

        int i = gcore::randint(0, gcore::FakeGen::NAMES.size() - 1);
        return gcore::FakeGen::NAMES[i];
    }

    char FakeGen::getGENDER()
    {
        gcore::setTrueRandSeed();

        int i = gcore::randint(0, 2);
        return gcore::FakeGen::GENDERS[i];
    }

    std::vector<std::string> FakeGen::SURNAMES = {
        "Oladokun", "Bridgwater", "Adeyemi", "Agbolade", "Williams",
        "Obi", "Ayeni", "Abu", "Abdulahi", "Adams",
        "Oni", "Babalola", "Ayoola", "Fatoki", "Efevbokhan"};

    std::vector<std::string> FakeGen::NAMES = {
        "Tope", "Tim", "Yemi", "Lola", "Tom",
        "Uche", "Yomi", "Zara", "Musa", "Eve",
        "Bimbo", "Kate", "Karen", "Ruby", "Lolade",
        "John", "Judas", "Solomon", "James", "Matthew",
        "Paul", "Zach", "Kole", "Kola", "Max",
        "Tunde", "Bola", "Ola", "Goke", "Efe"};

    char FakeGen::GENDERS[3] = {'M', 'F', 'O'};

    std::string FakeGen::BIRTHDATE(int minYear, int maxYear)
    {
        auto utcNow = gcore::DateTime::utcNow();
        auto localNow = gcore::DateTime::now();

        gcore::setTrueRandSeed();

        int y = gcore::randint(minYear, maxYear);
        int M = gcore::randint(1, 12);
        int d = gcore::randint(1, 28);

        std::ostringstream os;

        os << std::setfill('0') << std::setw(4) << y
           << "-" << std::setfill('0') << std::setw(2) << M
           << "-" << std::setfill('0') << std::setw(2) << d;

        return os.str();
    }

} // namespace gcore

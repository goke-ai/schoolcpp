#include <iostream>
#include <fstream>
#include <sstream>
#include "gstring.h"
#include "utilities.h"
#include "query.h"
#include "fakegen.h"
#include "ux.h"
#include "grade.h"

namespace school
{
    Grade::Grade()
    {
    }

    Grade::Grade(int id,
                 std::string name,
                 int sytemType,
                 float beginScore,
                 float nextScore,
                 float point)

    {
        _id = id;
        _name = name;
        _sytemType = sytemType;
        _beginScore = beginScore;
        _nextScore = nextScore;
        _point = point;
    }

    Grade::~Grade()
    {
    }

    std::string Grade::toString()
    {
        std::ostringstream os;

        os << "{";
        os << "id:" << _id;
        os << ", name:" << _name;
        os << ", sytemType:" << _sytemType;
        os << ", beginScore:" << _beginScore;
        os << ", nextScore:" << _nextScore;
        os << ", point:" << _point;
        os << "}\n";

        return os.str();
    }

    std::string Grade::toCsv()
    {
        std::ostringstream os;

        os << _id;
        os << "," << _name;
        os << "," << _sytemType;
        os << "," << _beginScore;
        os << "," << _nextScore;
        os << "," << _point;
        os << "\n";

        return os.str();
    }

    std::string Grade::toJson()
    {
        std::ostringstream os;

        os << "{";
        os << "\"id\":" << _id;
        os << "\"name\":\"" << _name << "\"";
        os << "\"sytemType\":" << _sytemType;
        os << "\"beginScore\":\"" << _beginScore;
        os << "\"nextScore\":\"" << _nextScore;
        os << "\"point\":" << _point;
        os << "}";

        return os.str();
    }

    // static

    std::string Grade::_path = "./data/grade.sch";
    std::vector<Grade> Grade::_data;

    std::vector<Grade> &Grade::getData()
    {
        return _data;
    }

    bool Grade::hasData()
    {
        auto &grades = getData();

        if (grades.size() == 0)
        {
            load();
        }

        return grades.size() > 0;
    }

    bool Grade::isExisting(Grade grade)
    {
        if (hasData())
        {
            auto &grades = getData();

            bool v = gquery::any(grades,
                                 [&grade](Grade p)
                                 {
                                     return (p.getName() == grade.getName()) && (p.getSytemType() == grade.getSytemType());
                                 });
            return v;
        }
        return false;
    }

    void Grade::load()
    {
        auto &grades = getData();

        grades.clear();

        auto path = _path; // "grade.sch";

        std::ifstream file;

        file.open(path);

        if (!file.is_open())
        {
            if (grades.empty())
            {
                generate();
            }
            return;
        }

        std::string line;

        while (std::getline(file, line))
        {
            auto words = gcore::split(line, ",");

            int id = std::stoi(words[0]);
            std::string name = words[1];
            int sytemType = std::stoi(words[2]);
            float beginScore = std::stof(words[3]);
            float nextScore = std::stof(words[4]);
            auto point = std::stof(words[5]);

            Grade p(id, name, sytemType, beginScore, nextScore, point);

            grades.push_back(p);
        }

        file.close();

        if (grades.empty())
        {
            generate();
        }
    }

    void Grade::write()
    {
        auto path = _path; // "grade.sch";

        std::ofstream file(path);

        if (!file.is_open())
        {
            return;
        }

        auto &grades = getData();

        for (auto &&v : grades)
        {
            file << v.toCsv();
        }

        file.close();
    }

    void Grade::clean()
    {
        auto path = _path; // "grade.sch";

        std::ofstream file;
        file.open(path, std::ofstream::out | std::ofstream::trunc);
        file.close();
    }

    Grade Grade::input(int &maxID)
    {
        char text[100]; std::string emp{""};

        int id = 0;
        // std::cout << "Enter ID (0, " << ++maxID << " or any number): ";
        // std::cin.getline(text, 99);
        // id = std::stoi(text);

        std::string name;
        std::cout << "Enter Name: ";
        std::cin.getline(text, 99);
        name = text;

        int sytemType;
        std::cout << "Enter sytemType: ";
        std::cin.getline(text, 99);
        sytemType = std::stoi(text);

        float beginScore;
        std::cout << "Enter beginScore: ";
        std::cin.getline(text, 99);
        beginScore = std::stof(text);

        float nextScore;
        std::cout << "Enter nextScore (YYYY-MM-DD): ";
        std::cin.getline(text, 99);
        nextScore = std::stof(text);

        float point;
        std::cout << "Enter point: ";
        std::cin.getline(text, 99);
        point = std::stof(text);

        Grade g(id, name, sytemType, beginScore, nextScore, point);
        return g;
    }

    bool Grade::generate(int count)
    {
        auto &grades = getData();

        if (grades.empty())
        {
            Grade p1(1, "A", 0, 70, 101, 5.0);
            grades.push_back(p1);
            Grade p2(2, "B", 0, 60, 70, 4.0);
            grades.push_back(p2);
            Grade p3(3, "C", 0, 50, 60, 3.0);
            grades.push_back(p3);
            Grade p4(4, "D", 0, 45, 50, 2.0);
            grades.push_back(p4);
            Grade p5(5, "F", 0, 0, 45, 0);
            grades.push_back(p5);
        }
        else
        {
            Grade p1(1, "A", 0, 70, 101, 5.0);
            add(p1);
            Grade p2(2, "B", 0, 60, 70, 4.0);
            add(p2);
            Grade p3(3, "C", 0, 50, 60, 3.0);
            add(p3);
            Grade p4(4, "D", 0, 45, 50, 2.0);
            add(p4);
            Grade p5(5, "F", 0, 0, 45, 0);
            add(p5);
        }

        return true;
    }

    void Grade::list()
    {
        auto &grades = getData();

        for (auto &&v : grades)
        {
            std::cout << v.toString();
        }
    }

    bool Grade::create()
    {
        auto &grades = getData();
        int maxID = grades.size();
        Grade p = Grade::input(maxID);

        auto b = add(p);
        if (b)
        {
            std::cout << "Created successfully!\n";
        }
        else
        {
            std::cout << "Create failed!\n";
        }
        return b;
    }

    bool Grade::edit()
    {
        // display list
        list();

        // choose ID to edit
        std::cout << "Enter the Grade ID to Edit: ";
        char text[100]; std::string emp{""};
        std::cin.getline(text, 99);

        int id = std::stoi(text);

        auto &grades = getData();
        auto p = gquery::first(grades, [&id](Grade x)
                               { return x.getId() == id; });

        // dislay grade to edit
        std::cout << "\n"
                  << p.toString();
        // ask for confirmation
        auto message = "Confirm editing of Grade ID:" + std::to_string(p.getId());
        auto yesorno = gcore::Ux::confirm(message);
        if (yesorno == 'N')
        {
            std::cout << "Update nextScorencelled.\n";
            return false;
        }

        // int id = 0;
        //  std::cout << "Enter ID (0, " << ++maxID << " or any number): ";
        //  std::cin.getline(text, 99);
        //  id = std::stoi(text);

        std::cout << "Edit Name [" << p.getName() << "]: ";
        std::cin.getline(text, 99);
        if (emp.compare(text) != 0)
        {
            p.setName(text);
        }

        std::cout << "Edit sytemType [" << p.getSytemType() << "]: ";
        std::cin.getline(text, 99);
        if (emp.compare(text) != 0)
        {
            p.setSytemType(std::stoi(text));
        }

        std::cout << "Edit beginScore [" << p.getBeginScore() << "]: ";
        std::cin.getline(text, 99);
        if (emp.compare(text) != 0)
        {
            p.setBeginScore(std::stof(text));
        }

        std::cout << "Edit nextScore [" << p.getNextScore() << "]: ";
        std::cin.getline(text, 99);
        if (emp.compare(text) != 0)
        {
            p.setNextScore(std::stof(text));
        }

        std::cout << "Edit point [" << p.getPoint() << "]: ";
        std::cin.getline(text, 99);
        if (emp.compare(text) != 0)
        {
            p.setPoint(std::stof(text));
        }

        auto b = update(p);
        if (b)
        {
            std::cout << "Updated successfully!\n";
        }
        else
        {
            std::cout << "Update failed!\n";
        }
        return b;
    }

    bool Grade::remove()
    {
        // display list
        list();

        // choose ID to edit
        std::cout << "Enter the Grade ID to Delete: ";
        char text[100]; std::string emp{""};
        std::cin.getline(text, 99);

        int id = std::stoi(text);

        auto &grades = getData();
        auto p = gquery::first(grades, [&id](Grade x)
                               { return x.getId() == id; });

        // dislay grade to delete
        std::cout << "\n"
                  << p.toString();
        // ask for confirmation
        auto message = "Confirm delete of Grade ID:" + std::to_string(p.getId());
        auto yesorno = gcore::Ux::confirm(message);
        if (yesorno == 'N')
        {
            std::cout << "Delete nextScorencelled.\n";
            return false;
        }

        auto b = deletee(p.getId());

        if (b)
        {
            std::cout << "Deleted successfully!\n";
        }
        else
        {
            std::cout << "Delete failed!\n";
        }
        return b;
    }

    bool Grade::add(Grade grade)
    {
        auto &grades = getData();

        if (!isExisting(grade))
        {
            if (grade.getId() == 0)
            {
                int maxID = 0;

                if (grades.size() > 0)
                {
                    auto p = gquery::max(grades, [](Grade x, Grade y)
                                         { return x.getId() < y.getId(); });
                    maxID = p.getId();
                }
                grade.setId(++maxID);
            }

            grades.push_back(grade);
            return true;
        }
        return false;
    }

    bool Grade::update(Grade grade)
    {
        auto &grades = getData();

        if (hasData())
        {
            gquery::replace(grades, grade, [&grade](Grade x)
                            { return x.getId() == grade.getId(); });

            return true;
        }
        return false;
    }

    bool Grade::deletee(int id)
    {
        if (hasData())
        {
            auto &grades = getData();

            gquery::remove(grades, [&id](Grade x)
                           { return x.getId() == id; });
            return true;
        }
        return false;
    }

} // namespace school

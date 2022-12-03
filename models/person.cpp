#include <iostream>
#include <fstream>
#include <sstream>
#include "gstring.h"
#include "utilities.h"
#include "query.h"
#include "fakegen.h"
#include "ux.h"
#include "person.h"

namespace school
{
    Person::Person()
    {
    }

    Person::Person(int id,
                   std::string surname,
                   std::string firstname,
                   std::string email,
                   std::string birthDate,
                   gcore::Gender gender) : Base(id)

    {
        //_id = id;
        _surname = surname;
        _firstname = firstname;
        _email = email;
        _birthDate = birthDate;
        _gender = gender;

        auto ymd = gcore::split(_birthDate, "-");
        _year = std::stoi(ymd[0]);
        _month = std::stoi(ymd[1]);
        _day = std::stoi(ymd[2]);
    }

    Person::~Person()
    {
    }

    std::string Person::toString()
    {
        std::ostringstream os;

        os << "{";
        os << "id:" << getId();
        os << ", surname:" << _surname;
        os << ", firstname:" << _firstname;
        os << ", email:" << _email;
        os << ", birthDate:" << _birthDate;
        os << ", gender:" << gcore::genderToString(_gender);
        os << "}\n";

        return os.str();
    }

    std::string Person::toCsv()
    {
        std::ostringstream os;

        os << getId();
        os << "," << _surname;
        os << "," << _firstname;
        os << "," << _email;
        os << "," << _birthDate;
        os << "," << _gender;
        os << "\n";

        return os.str();
    }

    std::string Person::toJson()
    {
        std::ostringstream os;

        os << "{";
        os << "\"id\":" << getId();
        os << "\"surname\":\"" << _surname << "\"";
        os << "\"firstname\":\"" << _firstname << "\"";
        os << "\"email\":\"" << _email << "\"";
        os << "\"birthDate\":\"" << _birthDate << "\"";
        os << "\"gender\":" << _gender;
        os << "}";

        return os.str();
    }

    // static

    std::vector<Person> Person::_data;
    std::string Person::_path = "./data/person.sch";

    std::vector<Person> &Person::getData()
    {
        return _data;
    }

    bool Person::hasData()
    {
        auto &persons = getData();

        if (persons.size() == 0)
        {
            load();
        }

        return persons.size() > 0;
    }

    bool Person::isExisting(Person person)
    {
        if (hasData())
        {
            auto &persons = getData();

            bool v = gquery::any(persons, [&person](Person p)
                                 { return p.getEmail() == person.getEmail(); });
            return v;
        }
        return false;
    }

    void Person::load()
    {

        auto &persons = getData();

        persons.clear();

        auto path = _path;

        std::ifstream file;

        file.open(path);

        if (!file.is_open())
        {
            return;
        }

        std::string line;

        while (std::getline(file, line))
        {
            auto words = gcore::split(line, ",");

            int id = std::stoi(words[0]);
            std::string surname = words[1];
            std::string firstname = words[2];
            std::string email = words[3];
            std::string birthDate = words[4];
            auto gender = static_cast<gcore::Gender>(std::stoi(words[5]));

            Person p(id, surname, firstname, email, birthDate, gender);

            persons.push_back(p);
        }

        file.close();
    }

    void Person::write()
    {
        // auto path = "person.sch";
        auto path = _path;

        std::ofstream file(path);

        if (!file.is_open())
        {
            return;
        }

        auto &persons = getData();

        for (auto &&v : persons)
        {
            file << v.toCsv();
        }

        file.close();
    }

    void Person::clean()
    {
        auto path = _path; // "person.sch";

        std::ofstream file;
        file.open(path, std::ofstream::out | std::ofstream::trunc);
        file.close();
    }

    Person Person::input(int &maxID)
    {
        char text[100];

        int id = 0;
        // std::cout << "Enter ID (0, " << ++maxID << " or any number): ";
        // std::cin.getline(text, 99);
        // id = std::stoi(text);

        std::string surname;
        std::cout << "Enter Surname: ";
        std::cin.getline(text, 99);
        surname = text;

        std::string firstname;
        std::cout << "Enter firstname: ";
        std::cin.getline(text, 99);
        firstname = text;

        std::string email;
        std::cout << "Enter email: ";
        std::cin.getline(text, 99);
        email = text;

        std::string birthDate;
        std::cout << "Enter birthDate (YYYY-MM-DD): ";
        std::cin.getline(text, 99);
        birthDate = text;

        gcore::Gender gender;
        std::cout << "Enter gender (M or F): ";
        char key;
        std::cin.get(key);
        std::cin.ignore(10000, '\n');
        if (std::toupper(key) == 'M')
        {
            gender = gcore::Gender::Male;
        }
        else if (std::toupper(key) == 'F')
        {
            gender = gcore::Gender::Female;
        }
        else
        {
            gender = gcore::Gender::None;
        }

        Person p(id, surname, firstname, email, birthDate, gender);
        return p;
    }

    bool Person::generate(int count)
    {
        auto &persons = getData();

        int id = 0;
        std::string surname;
        std::string firstname;
        std::string email;
        std::string birthDate;
        char genderChar;
        gcore::Gender gender;

        for (int i = 0; count > 0; i++)
        {
            // id = 0;
            surname = gcore::FakeGen::getSURNAME();
            firstname = gcore::FakeGen::getNAME();
            email = gcore::FakeGen::getEMAIL(firstname, surname);
            birthDate = gcore::FakeGen::BIRTHDATE();
            genderChar = gcore::FakeGen::getGENDER();

            switch (genderChar)
            {
            case 'M':
                gender = gcore::Gender::Male;
                break;
            case 'F':
                gender = gcore::Gender::Female;
                break;
            default:
                gender = gcore::Gender::None;
                break;
            }

            Person p(id, surname, firstname, email, birthDate, gender);
            if (add(p))
            {
                --count;
            }
        }
        return true;
    }

    void Person::list()
    {
        auto &persons = getData();

        for (auto &&v : persons)
        {
            std::cout << v.toString();
        }
    }

    bool Person::create()
    {
        auto &persons = getData();
        int maxID = persons.size();
        Person p = Person::input(maxID);

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

    bool Person::edit()
    {
        // display list
        list();

        // choose ID to edit
        std::cout << "Enter the Person ID to Edit: ";
        char text[100];
        std::cin.getline(text, 99);

        int id = std::stoi(text);

        auto &persons = getData();
        auto p = gquery::first(persons, [&id](Person x)
                               { return x.getId() == id; });

        // dislay person to edit
        std::cout << "\n"
                  << p.toString();
        // ask for confirmation
        auto message = "Confirm editing of Person ID:" + std::to_string(p.getId());
        auto yesorno = gcore::Ux::confirm(message);
        if (yesorno == 'N')
        {
            std::cout << "Update cancelled.\n";
            return false;
        }

        // int id = 0;
        //  std::cout << "Enter ID (0, " << ++maxID << " or any number): ";
        //  std::cin.getline(text, 99);
        //  id = std::stoi(text);

        std::cout << "Edit Surname [" << p.getSurname() << "]: ";
        std::cin.getline(text, 99);
        if (std::strcmp(text, "") != 0)
        {
            p.setSurname(text);
        }

        std::cout << "Edit firstname [" << p.getFirstname() << "]: ";
        std::cin.getline(text, 99);
        if (std::strcmp(text, "") != 0)
        {
            p.setFirstname(text);
        }

        std::cout << "Edit email [" << p.getEmail() << "]: ";
        std::cin.getline(text, 99);
        if (std::strcmp(text, "") != 0)
        {
            p.setEmail(text);
        }

        std::cout << "Edit birthDate [" << p.getBirthDate() << "] (YYYY-MM-DD): ";
        std::cin.getline(text, 99);
        if (std::strcmp(text, "") != 0)
        {
            p.setBirthDate(text);
        }

        std::cout << "Edit gender [" << gcore::genderToString(p.getGender()) << "] (M or F) : ";
        char key;
        key = '\0';
        key = std::cin.peek();
        std::cin.ignore(10000, '\n');
        if (key != '\n')
        {
            if (std::toupper(key) == 'M')
            {
                p.setGender(gcore::Gender::Male);
            }
            else if (std::toupper(key) == 'F')
            {
                p.setGender(gcore::Gender::Female);
            }
            else
            {
                p.setGender(gcore::Gender::None);
            }
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

    bool Person::remove()
    {
        // display list
        list();

        // choose ID to edit
        std::cout << "Enter the Person ID to Delete: ";
        char text[100];
        std::cin.getline(text, 99);

        int id = std::stoi(text);

        auto &persons = getData();
        auto p = gquery::first(persons, [&id](Person x)
                               { return x.getId() == id; });

        // dislay person to delete
        std::cout << "\n"
                  << p.toString();
        // ask for confirmation
        auto message = "Confirm delete of Person ID:" + std::to_string(p.getId());
        auto yesorno = gcore::Ux::confirm(message);
        if (yesorno == 'N')
        {
            std::cout << "Delete cancelled.\n";
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

    void Person::listReport()
    {
    }

    void Person::detailReports()
    {
    }

    bool Person::add(Person person)
    {
        auto &persons = getData();

        if (!isExisting(person))
        {
            if (person.getId() == 0)
            {
                int maxID = 0;

                if (persons.size() > 0)
                {
                    auto p = gquery::max(persons, [](Person x, Person y)
                                         { return x.getId() < y.getId(); });
                    maxID = p.getId();
                }
                person.setId(++maxID);
            }

            persons.push_back(person);
            return true;
        }
        return false;
    }

    bool Person::update(Person person)
    {
        auto &persons = getData();

        if (hasData())
        {
            gquery::replace(persons, person, [&person](Person x)
                            { return x.getId() == person.getId(); });

            return true;
        }
        return false;
    }

    bool Person::deletee(int id)
    {
        if (hasData())
        {
            auto &persons = getData();

            gquery::remove(persons, [&id](Person x)
                           { return x.getId() == id; });
            return true;
        }
        return false;
    }

} // namespace school

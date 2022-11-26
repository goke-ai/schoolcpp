#include <iostream>
#include <fstream>
#include <sstream>
#include "utilities.h"
#include "../fake/fakegen.h"
#include "person.h"

namespace school
{
    Person::Person(int id,
                   std::string surname,
                   std::string firstname,
                   std::string birthDate,
                   gender gender)

    {
        _id = id;
        _surname = surname;
        _firstname = firstname;
        _birthDate = birthDate;
        _gender = gender;
    }

    Person::~Person()
    {
    }

    std::string Person::toString()
    {
        std::ostringstream os;

        os << "{";
        os << "id:" << _id;
        os << ", surname:" << _surname;
        os << ", firstname:" << _firstname;
        os << ", birthDate:" << _birthDate;
        os << ", gender:" << _gender;
        os << "}";

        return os.str();
    }

    std::string Person::toCsv()
    {
        std::ostringstream os;

        os << _id;
        os << "," << _surname;
        os << "," << _firstname;
        os << "," << _birthDate;
        os << "," << _gender;

        return os.str();
    }

    std::string Person::toJson()
    {
        std::ostringstream os;

        os << "{";
        os << "\"id\":" << _id;
        os << "\"surname\":\"" << _surname << "\"";
        os << "\"firstname\":\"" << _firstname << "\"";
        os << "\"birthDate\":\"" << _birthDate << "\"";
        os << "\"gender\":" << _gender;
        os << "}";

        return os.str();
    }

    // static
    void Person::create(std::vector<Person> &persons)
    {
        int maxID = persons.size();
        Person p = Person::input(maxID);

        persons.push_back(p);
    }

    Person Person::input(int &maxID)
    {
        char text[100];

        int id = ++maxID;
        // std::cout << "Enter ID: ";
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

        std::string birthDate;
        std::cout << "Enter birthDate (YYYY-MM-DD): ";
        std::cin.getline(text, 99);
        birthDate = text;

        gender gender;
        std::cout << "Enter gender (M, F or O): ";
        char key = std::cin.get();
        std::cin.ignore();
        if (std::toupper(key) == 'M')
        {
            gender = gender::Male;
        }
        else if (std::toupper(key) == 'F')
        {
            gender = gender::Male;
        }
        else
        {
            gender = gender::None;
        }

        Person p(id, surname, firstname, birthDate, gender);
        return p;
    }

    void Person::generate(std::vector<Person> &persons, int count)
    {

        int maxID = persons.size();
        int id = 0;
        std::string surname;   // = gcore::FakeGen::getSURNAME();
        std::string firstname; // = gcore::FakeGen::getNAME();
        std::string birthDate; // = gcore::FakeGen::BIRTHDATE();
        char genderChar;       // = gcore::FakeGen::getGENDER();

        gender gender;

        for (int i = 0; i < count; i++)
        {

            int id = ++maxID;

            surname = gcore::FakeGen::getSURNAME();
            firstname = gcore::FakeGen::getNAME();
            birthDate = gcore::FakeGen::BIRTHDATE();
            genderChar = gcore::FakeGen::getGENDER();

            switch (genderChar)
            {
            case 'M':
                gender = gender::Male;
                break;
            case 'F':
                gender = gender::Female;
                break;
            default:
                gender = gender::None;
                break;
            }

            Person p(id, surname, firstname, birthDate, gender);

            persons.push_back(p);
        }
    }

    void Person::list(std::vector<Person> &persons)
    {
        for (auto &&v : persons)
        {
            std::cout << v.toString() << std::endl;
        }
    }

    void Person::add(std::vector<Person> &persons, Person person)
    {
        int n = 0;
    }

    void Person::edit(std::vector<Person> &persons, Person person)
    {
        int n = 0;
    }

    void Person::remove(std::vector<Person> &persons, int id)
    {
    }

} // namespace school

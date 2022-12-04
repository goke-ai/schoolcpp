#pragma once

#include <vector>
#include <string>
#include "core.h"
#include "base.h"

namespace school
{
    class Person : Base
    {
    private:
        // int _id = 0;
        std::string _surname;
        std::string _firstname;
        std::string _birthDate;
        gcore::Gender _gender;
        std::string _email;

        int _year;
        int _month;
        int _day;

        static std::string _path;
        static std::vector<Person> _data;

    public:
        Person();
        Person(int id,
               std::string surname,
               std::string firstname,
               std::string email,
               std::string birthDate,
               gcore::Gender gender);

        ~Person();

        const int &getId() const { return Base::getId(); }
        void setId(int id) { Base::setId(id); }

        const std::string &getSurname() const { return _surname; }
        void setSurname(std::string surname) { _surname = surname; }

        const std::string &getFirstname() const { return _firstname; }
        void setFirstname(std::string firstname) { _firstname = firstname; }

        const std::string &getEmail() const { return _email; }
        void setEmail(std::string email) { _email = email; }

        const std::string &getBirthDate() const { return _birthDate; }
        void setBirthDate(std::string birthDate) { _birthDate = birthDate; }

        const gcore::Gender &getGender() const { return _gender; }
        void setGender(gcore::Gender gender) { _gender = gender; }

        const std::string getUrlName() const;
        

        std::string toString();
        std::string toCsv();
        std::string toJson();

        std::string detailReport();

        // statics
        static std::vector<Person> &getData();

        static bool hasData();
        static bool isExisting(Person person);

        static void load();
        static void write();
        static void clean();

        static Person input(int &maxID);
        static bool generate(int count = 20);
        static void list();
        static bool create();
        static bool edit();
        static bool remove();
        static void listReport();
        static void detailReports();

        static bool add(Person person);
        static bool update(Person person);
        static bool deletee(int id);
    };
} // namespace school

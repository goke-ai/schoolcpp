#if !defined(_PERSON_H_)
#define _PERSON_H_

#include <vector>
#include <string>

namespace school
{
    enum gender
    {
        None,
        Male,
        Female
    };

    class Person
    {
    private:
        int _id;
        std::string _surname;
        std::string _firstname;
        std::string _birthDate;
        gender _gender;

    public:
        Person(int id,
               std::string surname,
               std::string firstname,
               std::string birthDate,
               gender gender);

        ~Person();

        std::string toString();
        std::string toCsv();
        std::string toJson();

        static void create(std::vector<Person> &persons);
        static Person input(int &maxID);
        static void generate(std::vector<Person> &persons, int count = 20);

        static void list(std::vector<Person> &persons);
        static void add(std::vector<Person> &persons, Person person);
        static void edit(std::vector<Person> &persons, Person person);
        static void remove(std::vector<Person> &persons, int id);
    };
} // namespace school

#endif // _PERSON_H_

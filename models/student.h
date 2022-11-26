#if !defined(_STUDENT_H_)
#define _STUDENT_H_

#include <vector>
#include <string>

namespace school
{
    class Student
    {
    private:
        /* data */
    public:
        Student(/* args */);
        ~Student();

        std::string toString();
        std::string toCsv();
        std::string toJson();

        static void create(std::vector<Student> &students);
        static Student input(int &maxID);
        static void generate(std::vector<Student> &students);

        static void list(std::vector<Student> &students);
        static void add(std::vector<Student> &students, Student student);
        static void edit(std::vector<Student> &students, Student student);
        static void remove(std::vector<Student> &students, int id);
    };
} // namespace school

#endif // _STUDENT_H_

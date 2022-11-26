#if !defined(_COURSE_H_)
#define _COURSE_H_

#include <vector>
#include <string>

namespace school
{
    class Course
    {
    private:
        /* data */
    public:
        Course(/* args */);
        ~Course();

        std::string toString();
        std::string toCsv();
        std::string toJson();

        static void create(std::vector<Course> &courses);
        static Course input(int &maxID);
        static void generate(std::vector<Course> &courses);

        static void list(std::vector<Course> &courses);
        static void add(std::vector<Course> &courses, Course course);
        static void edit(std::vector<Course> &courses, Course course);
        static void remove(std::vector<Course> &courses, int id);
    };
} // namespace school

#endif // _COURSE_H_

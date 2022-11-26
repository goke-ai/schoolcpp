#if !defined(_STUDENT_COURSE_H_)
#define _STUDENT_COURSE_H_

#include <vector>
#include <string>

namespace school
{
    class StudentCourse
    {
    private:
        /* data */
    public:
        StudentCourse(/* args */);
        ~StudentCourse();

        std::string toString();
        std::string toCsv();
        std::string toJson();

        static void create(std::vector<StudentCourse> &studentCourses);
        static StudentCourse input(int &maxID);
        static void generate(std::vector<StudentCourse> &studentCourses);

        static void list(std::vector<StudentCourse> &studentCourses);
        static void add(std::vector<StudentCourse> &studentCourses, StudentCourse studentCourse);
        static void edit(std::vector<StudentCourse> &studentCourses, StudentCourse studentCourse);
        static void remove(std::vector<StudentCourse> &studentCourses, int id);
    };

} // namespace school

#endif // _STUDENT_COURSE_H_

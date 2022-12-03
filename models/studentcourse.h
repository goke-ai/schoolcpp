#pragma once

#include <vector>
#include <string>
#include "utilities.h"
#include "grade.h"

namespace school
{
    class StudentCourse
    {
    private:
        int _id = 0;
        int _studentId;
        int _courseId;
        float _ca;
        float _exam;

        static std::string _path;
        static std::vector<StudentCourse> _data;

    public:
        StudentCourse();
        StudentCourse(int id,
                      int studentId,
                      int courseId,
                      float ca,
                      float exam);

        ~StudentCourse();

        const int &getId() const { return _id; }
        void setId(int id) { _id = id; }

        const int &getStudentId() const { return _studentId; }
        void setStudentId(int studentId) { _studentId = studentId; }

        const int &getCourseId() const { return _courseId; }
        void setCourseId(int courseId) { _courseId = courseId; }

        const float &getCA() const { return _ca; }
        void setCA(float ca) { _ca = ca; }

        const float &getExam() const { return _exam; }
        void setExam(float exam) { _exam = exam; }

        const float getTotal() const { return gcore::approximate((_ca + _exam),0); }
        const std::string getGrade() const;
        const float getGradePoint() const;
        const std::string getCode() const;
        const std::string getTitle() const;
        const float getUnit() const;

        std::string toString();
        std::string toCsv();
        std::string toJson();

        // statics
        static std::vector<StudentCourse> &getData();

        static bool hasData();
        static bool isExisting(StudentCourse studentCourse);

        static void load();
        static void write();
        static void clean();

        static StudentCourse input(int &maxID);
        static bool generate(int count = 20, int studentCount = 20,
                             int courseCount = 20);
        static void list();
        static bool create();
        static bool edit();
        static bool remove();

        static bool add(StudentCourse studentCourse);
        static bool update(StudentCourse studentCourse);
        static bool deletee(int id);
    };
} // namespace school

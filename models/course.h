#pragma once

#include <vector>
#include <string>

namespace school
{
    class Course
    {
    private:
        int _id = 0;
        std::string _code;
        std::string _title;
        float _unit;
        float _ca;
        float _exam;

        static std::string _path;
        static std::vector<Course> _data;

    public:
        Course();
        Course(int id,
               std::string code,
               std::string title,
               float unit,
               float ca,
               float exam);

        ~Course();

        const int &getId() const { return _id; }
        void setId(int id) { _id = id; }

        const std::string &getCode() const { return _code; }
        void setCode(std::string code) { _code = code; }

        const std::string &getTitle() const { return _title; }
        void setTitle(std::string title) { _title = title; }

        const float &getUnit() const { return _unit; }
        void setUnit(float unit) { _unit = unit; }

        const float &getCA() const { return _ca; }
        void setCA(float ca) { _ca = ca; }

        const float &getExam() const { return _exam; }
        void setExam(float exam) { _exam = exam; }

        std::string toString();
        std::string toCsv();
        std::string toJson();

        void resultSheetReport();

        // statics
        static std::vector<Course> &getData();

        static bool hasData();
        static bool isExisting(Course course);

        static void load();
        static void write();
        static void clean();

        static Course input(int &maxID);
        static bool generate(int count = 20);
        static void list();
        static bool create();
        static bool edit();
        static bool remove();
        static bool resultSheetsRports();

        static bool add(Course course);
        static bool update(Course course);
        static bool deletee(int id);
    };
} // namespace school

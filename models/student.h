#pragma once

#include <vector>
#include <string>

namespace school
{
    class Student
    {
    private:
        int _id = 0;
        std::string _studentNo;
        std::string _department;
        int _personId;

        static std::string _path;
        static std::vector<Student> _data;

    public:
        Student();
        Student(int id,
                std::string studentNo,
                std::string department,
                int personId);

        ~Student();

        const int &getId() const { return _id; }
        void setId(int id) { _id = id; }

        const std::string &getStudentNo() const { return _studentNo; }
        void setStudentNo(std::string studentNo) { _studentNo = studentNo; }

        const std::string &getDepartment() const { return _department; }
        void setDepartment(std::string department) { _department = department; }

        const int &getPersonId() const { return _personId; }
        void setPersonId(int personId) { _personId = personId; }

        const std::string getPersonName() const;
        const std::string getGender() const;
        const std::string getBirthDate() const;
        const std::string getUrlName() const;

        std::string toString();
        std::string toCsv();
        std::string toJson();

        std::string detailReport();
        void transcriptReport();
        void openTranscript();

        // statics
        static std::vector<Student> &getData();

        static bool hasData();
        static bool isExisting(Student student);

        static void load();
        static void write();
        static void clean();

        static Student input(int &maxID);
        static bool generate(int count = 20);
        static void list();
        static bool create();
        static bool edit();
        static bool remove();
        static void listReport();
        static void detailReports();
        static bool transcriptReports();

        static bool add(Student student);
        static bool update(Student student);
        static bool deletee(int id);
    };
} // namespace school

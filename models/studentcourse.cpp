#include <iostream>
#include <fstream>
#include <sstream>
#include "gstring.h"
#include "utilities.h"
#include "query.h"
#include "fakegen.h"
#include "ux.h"
#include "html.h"
#include "../report.h"
#include "student.h"
#include "course.h"
#include "studentcourse.h"

namespace school
{
    StudentCourse::StudentCourse()
    {
    }

    StudentCourse::StudentCourse(int id,
                                 int studentId,
                                 int courseId,
                                 float ca,
                                 float exam)

    {
        _id = id;
        _studentId = studentId;
        _courseId = courseId;
        _ca = ca;
        _exam = exam;
    }

    StudentCourse::~StudentCourse()
    {
    }

    const std::string StudentCourse::getGrade() const
    {
        auto &grades = Grade::getData();
        auto total = getTotal();
        auto g = gquery::first(grades,
                               [&total](Grade x)
                               {
                                   return x.getBeginScore() <= total && total < x.getNextScore();
                               });

        return g.getName();
    }

    const float StudentCourse::getGradePoint() const
    {
        auto &grades = Grade::getData();
        auto total = getTotal();
        auto g = gquery::first(grades,
                               [&total](Grade x)
                               {
                                   return x.getBeginScore() <= total && total < x.getNextScore();
                               });

        return g.getPoint();
    }

    const std::string StudentCourse::getCode() const
    {
        auto &courses = Course::getData();

        auto c = gquery::first(courses, [this](Course x)
                               { return x.getId() == getCourseId(); });
        return c.getCode();
    }

    const std::string StudentCourse::getTitle() const
    {
        auto &courses = Course::getData();

        auto c = gquery::first(courses, [this](Course x)
                               { return x.getId() == getCourseId(); });
        return c.getTitle();
    }

    const float StudentCourse::getUnit() const
    {
        auto &courses = Course::getData();

        auto c = gquery::first(courses, [this](Course x)
                               { return x.getId() == getCourseId(); });
        return c.getUnit();
    }

    const std::string StudentCourse::getStudentName() const
    {
        auto &students = Student::getData();

        auto s = gquery::first(students, [this](Student x)
                               { return x.getId() == getStudentId(); });
        return s.getStudentNo() + " | " + s.getPersonName();
    }

    const std::string StudentCourse::getCourseName() const
    {
        auto &courses = Course::getData();

        auto c = gquery::first(courses, [this](Course x)
                               { return x.getId() == getCourseId(); });
        return c.getCode() + " | " + c.getTitle() + " | " + gcore::toWidth(c.getUnit(), 1);
    }

    const std::string StudentCourse::getUrlName() const
    {
        auto &students = Student::getData();
        auto s = gquery::first(students, [this](Student x)
                               { return x.getId() == getStudentId(); });

        auto &courses = Course::getData();
        auto c = gquery::first(courses, [this](Course x)
                               { return x.getId() == getCourseId(); });

        auto text = s.getStudentNo() + "_" + c.getCode();

        return gcore::validUrl(text);
    }

    std::string StudentCourse::toString()
    {
        std::ostringstream os;

        os << "{";
        os << "id:" << _id;
        os << ", studentId:" << _studentId;
        os << ", courseId:" << _courseId;
        os << ", ca:" << _ca;
        os << ", exam:" << _exam;
        os << "}\n";

        return os.str();
    }

    std::string StudentCourse::toCsv()
    {
        std::ostringstream os;

        os << _id;
        os << "," << _studentId;
        os << "," << _courseId;
        os << "," << _ca;
        os << "," << _exam;
        os << "\n";

        return os.str();
    }

    std::string StudentCourse::toJson()
    {
        std::ostringstream os;

        os << "{";
        os << "\"id\":" << _id;
        os << "\"studentId\":\"" << _studentId << "\"";
        os << "\"courseId\":\"" << _courseId << "\"";
        os << "\"ca\":\"" << _ca;
        os << "\"exam\":" << _exam;
        os << "}";

        return os.str();
    }

    std::string StudentCourse::detailReport()
    {
        std::ostringstream os;

        os << Html::html({
            Html::head({
                Html::style({
                    Report::getStyle(),
                }),
            }),

            Html::body({
                Html::div({
                              Html::img({
                                  "src='../../img/logo.png'",
                                  "width='200px'",
                              }),
                              Html::tag("h1", "COVENANT UNIVERSITY"),
                              Html::tag("h2", "CHEMICAL ENGINEERING DEPARTMENT"),
                              Html::tag("h2", "STUDENT-COURSE DETAILS", {
                                                                            "class='p-1'",
                                                                        }),
                              Html::hr(),
                          },
                          "class='m-1 p-1 text-center'"),

                Html::div({
                              Html::table({
                                  Html::tr({
                                      Html::th("ID"),
                                      Html::td(std::to_string(getId())),
                                  }),
                                  Html::tr({
                                      Html::th("StudentName"),
                                      Html::td(getStudentName()),
                                  }),
                                  Html::tr({
                                      Html::th("CourseName"),
                                      Html::td(getCourseName()),
                                  }),
                                  Html::tr({
                                      Html::th("CA"),
                                      Html::td(gcore::toWidth(getCA(), 2)),
                                  }),
                                  Html::tr({
                                      Html::th("Exam"),
                                      Html::td(gcore::toWidth(getExam(), 2)),
                                  }),
                              }),
                          },
                          "class='m-1 p-1'"),
            }),
        });

        std::string path = "reports/sc/" + this->getUrlName() + ".html";

        std::ofstream file(path);

        if (!file.is_open())
        {
            return NULL;
        }

        file << os.str();

        file.close();

        auto fileUrl = path; //.substr(2);
        return fileUrl;
    }

    // static

    std::string StudentCourse::_path = "./data/studentcourse.sch";
    std::vector<StudentCourse> StudentCourse::_data;

    std::vector<StudentCourse> &StudentCourse::getData()
    {
        return _data;
    }

    bool StudentCourse::hasData()
    {
        auto &studentCourses = getData();

        if (studentCourses.size() == 0)
        {
            load();
        }

        return studentCourses.size() > 0;
    }

    bool StudentCourse::isExisting(StudentCourse studentCourse)
    {
        if (hasData())
        {
            auto &studentCourses = getData();

            bool v = gquery::any(studentCourses, [&studentCourse](StudentCourse p)
                                 { return (p.getStudentId() == studentCourse.getStudentId()) && (p.getCourseId() == studentCourse.getCourseId()); });
            return v;
        }
        return false;
    }

    void StudentCourse::load()
    {
        auto &studentCourses = getData();

        studentCourses.clear();

        auto path = _path; // "studentCourse.sch";

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
            int studentId = std::stoi(words[1]);
            int courseId = std::stoi(words[2]);
            float ca = std::stof(words[3]);
            auto exam = std::stof(words[4]);

            StudentCourse p(id, studentId, courseId, ca, exam);

            studentCourses.push_back(p);
        }

        file.close();
    }

    void StudentCourse::write()
    {
        auto path = _path; // "studentCourse.sch";

        std::ofstream file(path);

        if (!file.is_open())
        {
            return;
        }

        auto &studentCourses = getData();

        for (auto &&v : studentCourses)
        {
            file << v.toCsv();
        }

        file.close();
    }

    void StudentCourse::clean()
    {
        auto path = _path; // "studentCourse.sch";

        std::ofstream file;
        file.open(path, std::ofstream::out | std::ofstream::trunc);
        file.close();
    }

    StudentCourse StudentCourse::input(int &maxID)
    {
        char text[100];
        std::string emp{""};

        int id = 0;
        // std::cout << "Enter ID (0, " << ++maxID << " or any number): ";
        // std::cin.getline(text, 99);
        // id = std::stoi(text);

        int studentId;
        std::cout << "Enter StudentId: ";
        std::cin.getline(text, 99);
        studentId = std::stoi(text);

        int courseId;
        std::cout << "Enter courseId: ";
        std::cin.getline(text, 99);
        courseId = std::stoi(text);

        float unit;
        std::cout << "Enter unit: ";
        std::cin.getline(text, 99);
        unit = std::stof(text);

        float ca;
        std::cout << "Enter ca (YYYY-MM-DD): ";
        std::cin.getline(text, 99);
        ca = std::stof(text);

        float exam;
        std::cout << "Enter exam: ";
        std::cin.getline(text, 99);
        exam = std::stof(text);

        StudentCourse p(id, studentId, courseId, ca, exam);
        return p;
    }

    bool StudentCourse::generate(int count, int studentCount,
                                 int courseCount)
    {
        auto &studentCourses = getData();
        auto &students = Student::getData();
        auto &courses = Course::getData();

        int id = 0;
        int studentId;
        int courseId;
        float unit;
        float ca;
        float exam;

        int k = 0, l = 0;

        std::vector<Student> availStudents;
        std::vector<Course> availCourses;

        auto cc = courseCount;
        availStudents = gquery::where(students,
                                      [&studentCourses](Student s)
                                      {
                                          return gquery::any(studentCourses,
                                                             [&s](StudentCourse sc)
                                                             {
                                                                 return s.getId() == sc.getStudentId();
                                                             }) == false;
                                      });

        for (int i = 0; studentCount > 0; i++)
        {
            // id = 0;

            if (availStudents.size() < 1)
            {
                break;
            }
            k = gcore::randint(0, availStudents.size() - 1);
            auto student = availStudents[k];
            studentId = student.getId();

            cc = courseCount;

            availCourses = gquery::where(courses,
                                         [&studentCourses, &student](Course c)
                                         {
                                             return gquery::any(studentCourses,
                                                                [&c, &student](StudentCourse sc)
                                                                {
                                                                    return (student.getId() == sc.getStudentId()) && (c.getId() == sc.getCourseId());
                                                                }) == false;
                                         });

            for (int j = 0; cc > 0; j++)
            {

                if (availCourses.size() < 1)
                {
                    break;
                }
                l = gcore::randint(0, availCourses.size() - 1);
                auto course = availCourses[l];
                courseId = course.getId();

                unit = gcore::FakeGen::getNUMBER(0, 5);
                ca = gcore::FakeGen::getNUMBER(0, 15);
                exam = gcore::FakeGen::getDECNUMBER(0, 70, 1);

                StudentCourse p(id, studentId, courseId, ca, exam);
                if (add(p))
                {
                    --cc;
                    //
                    availCourses.erase(availCourses.begin() + l, availCourses.begin() + l + 1);
                }
            }
            --studentCount;
            availStudents.erase(availStudents.begin() + k, availStudents.begin() + k + 1);
        }
        return true;
    }

    void StudentCourse::list()
    {
        auto &studentCourses = getData();

        for (auto &&v : studentCourses)
        {
            std::cout << v.toString();
        }
    }

    bool StudentCourse::create()
    {
        auto &studentCourses = getData();
        int maxID = studentCourses.size();
        StudentCourse p = StudentCourse::input(maxID);

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

    bool StudentCourse::edit()
    {
        // display list
        list();

        // choose ID to edit
        std::cout << "Enter the StudentCourse ID to Edit: ";
        char text[100];
        std::string emp{""};
        std::cin.getline(text, 99);

        int id = std::stoi(text);

        auto &studentCourses = getData();
        auto p = gquery::first(studentCourses, [&id](StudentCourse x)
                               { return x.getId() == id; });

        // dislay studentCourse to edit
        std::cout << "\n"
                  << p.toString();
        // ask for confirmation
        auto message = "Confirm editing of Student ID:" + std::to_string(p.getId());
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

        std::cout << "Edit StudentId [" << p.getStudentId() << "]: ";
        std::cin.getline(text, 99);
        if (emp.compare(text) != 0)
        {
            p.setStudentId(std::stoi(text));
        }

        std::cout << "Edit courseId [" << p.getCourseId() << "]: ";
        std::cin.getline(text, 99);
        if (emp.compare(text) != 0)
        {
            p.setCourseId(std::stoi(text));
        }

        std::cout << "Edit ca [" << p.getCA() << "]: ";
        std::cin.getline(text, 99);
        if (emp.compare(text) != 0)
        {
            p.setCA(std::stof(text));
        }

        std::cout << "Edit exam [" << p.getExam() << "]: ";
        std::cin.getline(text, 99);
        if (emp.compare(text) != 0)
        {
            p.setExam(std::stof(text));
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

    bool StudentCourse::remove()
    {
        // display list
        list();

        // choose ID to edit
        std::cout << "Enter the StudentCourse ID to Delete: ";
        char text[100];
        std::string emp{""};
        std::cin.getline(text, 99);

        int id = std::stoi(text);

        auto &studentCourses = getData();
        auto p = gquery::first(studentCourses, [&id](StudentCourse x)
                               { return x.getId() == id; });

        // dislay studentCourse to delete
        std::cout << "\n"
                  << p.toString();
        // ask for confirmation
        auto message = "Confirm delete of Course ID:" + std::to_string(p.getId());
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

    void StudentCourse::listReport()
    {
        auto &studentCourses = getData();

        auto oStudentCourses = gquery::orderBy(studentCourses, [](StudentCourse x, StudentCourse y)
                                               { return (x.getStudentName() < y.getStudentName()); });

        std::ostringstream os;

        os << Html::html({
            Html::head({
                Html::style({Report::getStyle()}),
            }),

            Html::body({
                Html::div({
                              Html::img({
                                  "src='../../img/logo.png'",
                                  "width='200px'",
                              }),
                              Html::tag("h1", "COVENANT UNIVERSITY"),
                              Html::tag("h2", "CHEMICAL ENGINEERING DEPARTMENT"),
                              Html::tag("h2", "STUDENT-COURSE LIST", {
                                                                         "class='p-1'",
                                                                     }),
                              Html::hr(),
                          },
                          "class='m-1 p-1 text-center'"),

                Html::div({
                              Html::table({Html::tr({
                                               Html::th("SN"),
                                               Html::th("Id"),
                                               Html::th("StudentName"),
                                               Html::th("CourseName"),
                                               Html::th("CA"),
                                               Html::th("Exam"),
                                           }),

                                           Html::forTag([&]()
                                                        {
                                                std::string rt{" "};
                                                int n=0;

                                                for (auto &&v : oStudentCourses)
                                                {
                                                    rt += Html::tr({
                                                        Html::td([&n](){ return ++n; }, [](){ return ""; }),                                                                
                                                        Html::td([&v](){ return v.getId(); }, [](){ return ""; }),                                                                
                                                        Html::td([&v](){ return v.getStudentName(); }, [](){ return ""; }),                                                                
                                                        Html::td([&v](){ return v.getCourseName(); }, [](){ return ""; }),                                                                
                                                        Html::td([&v](){ return v.getCA(); }, [](){ return ""; }),                                                                
                                                        Html::td([&v](){ return v.getExam(); }, [](){ return ""; }),                                                                
                                                    });
                                                }
                                                return rt; }),

                                           Html::tr({
                                               Html::th(),
                                               Html::th(),
                                               Html::th(),
                                               Html::th(),
                                               Html::th(),
                                               Html::th(),
                                           })}),

                          },
                          "class='m-1 p-1'"),
            }),
        });

        std::string path = "reports/sc/list.html";
        std::ofstream file(path);
        if (!file.is_open())
        {
            return;
        }
        file << os.str();
        file.close();

        std::string fileUrl = "reports/sc/list.html";

        Report::open(fileUrl);
    }

    void StudentCourse::detailReports()
    {
        //
        list();

        // choose ID to edit
        std::cout << "Enter the StudentCourse ID to create detail report: ";
        char text[100];
        std::string emp{""};
        std::cin.getline(text, 99);

        int id = std::stoi(text);

        auto &studentCourses = getData();
        auto s = gquery::first(studentCourses, [&id](StudentCourse x)
                               { return x.getId() == id; });

        // dislay studentCourse to edit
        std::cout << "\n"
                  << s.toString();
        // ask for confirmation
        auto message = "Confirm creating detail report for StudentCourse ID:" + std::to_string(s.getId());
        auto yesorno = gcore::Ux::confirm(message);
        if (yesorno == 'N')
        {
            std::cout << "Detail report cancelled.\n";
            return;
        }

        std::string fileUrl = s.detailReport();
        Report::open(fileUrl);
    }

    bool StudentCourse::add(StudentCourse studentCourse)
    {
        auto &studentCourses = getData();

        if (!isExisting(studentCourse))
        {
            if (studentCourse.getId() == 0)
            {
                int maxID = 0;

                if (studentCourses.size() > 0)
                {
                    auto p = gquery::max(studentCourses, [](StudentCourse x, StudentCourse y)
                                         { return x.getId() < y.getId(); });
                    maxID = p.getId();
                }
                studentCourse.setId(++maxID);
            }

            studentCourses.push_back(studentCourse);
            return true;
        }
        return false;
    }

    bool StudentCourse::update(StudentCourse studentCourse)
    {
        auto &studentCourses = getData();

        if (hasData())
        {
            gquery::replace(studentCourses, studentCourse, [&studentCourse](StudentCourse x)
                            { return x.getId() == studentCourse.getId(); });

            return true;
        }
        return false;
    }

    bool StudentCourse::deletee(int id)
    {
        if (hasData())
        {
            auto &studentCourses = getData();

            gquery::remove(studentCourses, [&id](StudentCourse x)
                           { return x.getId() == id; });
            return true;
        }
        return false;
    }

} // namespace school

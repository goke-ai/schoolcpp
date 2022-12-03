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
#include "query.h"
#include "student.h"
#include "studentcourse.h"
#include "course.h"

namespace school
{
    Course::Course()
    {
    }

    Course::Course(int id,
                   std::string code,
                   std::string title,
                   float unit,
                   float ca,
                   float exam)

    {
        _id = id;
        _code = code;
        _title = title;
        _unit = unit;
        _ca = ca;
        _exam = exam;
    }

    Course::~Course()
    {
    }

    std::string Course::toString()
    {
        std::ostringstream os;

        os << "{";
        os << "id:" << _id;
        os << ", code:" << _code;
        os << ", title:" << _title;
        os << ", unit:" << _unit;
        os << ", ca:" << _ca;
        os << ", exam:" << _exam;
        os << "}\n";

        return os.str();
    }

    std::string Course::toCsv()
    {
        std::ostringstream os;

        os << _id;
        os << "," << _code;
        os << "," << _title;
        os << "," << _unit;
        os << "," << _ca;
        os << "," << _exam;
        os << "\n";

        return os.str();
    }

    std::string Course::toJson()
    {
        std::ostringstream os;

        os << "{";
        os << "\"id\":" << _id;
        os << "\"code\":\"" << _code << "\"";
        os << "\"title\":\"" << _title << "\"";
        os << "\"unit\":\"" << _unit;
        os << "\"ca\":\"" << _ca;
        os << "\"exam\":" << _exam;
        os << "}";

        return os.str();
    }

    void Course::resultSheetReport()
    {
        auto &studentCourses = StudentCourse::getData();
        auto &students = Student::getData();

        auto vals = gquery::where(studentCourses, [this](StudentCourse x)
                                  { return x.getCourseId() == this->getId(); });
        auto qq = gquery::orderBy(vals,
                                  [&students](StudentCourse x, StudentCourse y)
                                  {
                                      auto cx = gquery::first(students, [&x](Student c1)
                                                              { return c1.getId() == x.getStudentId(); });
                                      auto cy = gquery::first(students, [&y](Student c1)
                                                              { return c1.getId() == y.getStudentId(); });
                                      return cx.getStudentNo() < cy.getStudentNo();
                                  });

        std::string textCenter{"class='text-center'"};

        std::ostringstream os;

        os << Html::htmlStart()
           << Html::headStart()
           << Html::styleStart()
           << Report::getStyle()
           << Html::styleClose()
           << Html::headClose();

        os << Html::bodyStart();

        os << Html::div({
                            Html::img({
                                "src='../../img/logo.png'",
                                "width='200px'",
                                textCenter,
                            }),
                            Html::tag("h1", "COVENANT UNIVERSITY"),
                            Html::tag("h2", "CHEMICAL ENGINEERING DEPARTMENT"),
                            Html::tag("h2", "COURSE RESULT SHEET", {
                                                                      "class='p-1'",
                                                                  }),
                            Html::hr(),
                        },
                        "class='m-1 p-1 text-center'");

        os << Html::divStart("class='m-1 p-1'")
           << Html::tableStart()

           << Html::tr({Html::tag("th", "Code"),
                        Html::tag("td", getCode())})
           << Html::tr({Html::tag("th", "Title"),
                        Html::tag("td", getTitle())})
           << Html::tr({
                  Html::tag("th", "Unit"),
                  Html::tag(
                      "td", [this]()
                      { return getUnit(); },
                      []()
                      { return ""; }),
              })

           << Html::tableClose()
           << Html::divClose();

        os << Html::divStart("class='m-1 p-1'")
           << Html::tableStart();

        os << Html::tr({
            Html::tag("th", "S/N"),
            Html::tag("th", "Id"),
            Html::tag("th", "Student No."),
            Html::tag("th", "Names"),
            Html::tag("th", "CA", {textCenter}),
            Html::th("Exam", {textCenter}),
            Html::th("Total", {textCenter}),
            Html::th("Grade", {textCenter}),
            Html::th("Grade Point", {textCenter}),
        });

        int n = 0;
        for (auto &&v : qq)
        {
            auto student = gquery::first(students, [&v](Student x)
                                         { return x.getId() == v.getStudentId(); });

            os << Html::tr({
                Html::tag(
                    "td", [&v, &n]()
                    { return ++n; },
                    []()
                    { return ""; }),
                Html::tag(
                    "td", [&v]()
                    { return v.getId(); },
                    []()
                    { return ""; }),
                Html::tag(
                    "td", [&student]()
                    { return student.getStudentNo(); },
                    []()
                    { return ""; }),
                Html::tag(
                    "td", [&student]()
                    { return student.getPersonName(); },
                    []()
                    { return ""; }),
                Html::tag(
                    "td", [&v]()
                    { return gcore::toWidth(v.getCA(), 2); },
                    [&]()
                    { return textCenter; }),
                Html::tag(
                    "td", [&v]()
                    { return v.getExam(); },
                    [&]()
                    { return textCenter; }),
                Html::tag(
                    "td", [&v]()
                    { return v.getTotal(); },
                    [&]()
                    { return textCenter; }),
                Html::tag(
                    "td", [&v]()
                    { return v.getGrade(); },
                    [&]()
                    { return textCenter; }),
                Html::tag(
                    "td", [&v]()
                    { return v.getGradePoint(); },
                    [&]()
                    { return textCenter; }),
            });
        }

        os << Html::tr({
            Html::th(),
            Html::th(),
            Html::th(),
            Html::th(),
            Html::th(),
            Html::th(),
            Html::th(),
            Html::th(),
            Html::th(),
        });

        os << Html::tableClose()
           << Html::divClose()
           << Html::bodyClose()
           << Html::htmlClose();

        std::string path = "./reports/rs/" + this->getCode() + ".html";

        std::ofstream file(path);

        if (!file.is_open())
        {
            return;
        }

        file << os.str();

        file.close();
    }

    // static

    std::string Course::_path = "./data/course.sch";
    std::vector<Course> Course::_data;

    std::vector<Course> &Course::getData()
    {
        return _data;
    }

    bool Course::hasData()
    {
        auto &courses = getData();

        if (courses.size() == 0)
        {
            load();
        }

        return courses.size() > 0;
    }

    bool Course::isExisting(Course course)
    {
        if (hasData())
        {
            auto &courses = getData();

            bool v = gquery::any(courses, [&course](Course p)
                                 { return p.getCode() == course.getCode(); });
            return v;
        }
        return false;
    }

    void Course::load()
    {
        auto &courses = getData();

        courses.clear();

        auto path = _path; // "course.sch";

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
            std::string code = words[1];
            std::string title = words[2];
            float unit = std::stof(words[3]);
            float ca = std::stof(words[4]);
            auto exam = std::stof(words[5]);

            Course p(id, code, title, unit, ca, exam);

            courses.push_back(p);
        }

        file.close();
    }

    void Course::write()
    {
        auto path = _path; // "course.sch";

        std::ofstream file(path);

        if (!file.is_open())
        {
            return;
        }

        auto &courses = getData();

        for (auto &&v : courses)
        {
            file << v.toCsv();
        }

        file.close();
    }

    void Course::clean()
    {
        auto path = _path; // "course.sch";

        std::ofstream file;
        file.open(path, std::ofstream::out | std::ofstream::trunc);
        file.close();
    }

    Course Course::input(int &maxID)
    {
        char text[100];

        int id = 0;
        // std::cout << "Enter ID (0, " << ++maxID << " or any number): ";
        // std::cin.getline(text, 99);
        // id = std::stoi(text);

        std::string code;
        std::cout << "Enter Code: ";
        std::cin.getline(text, 99);
        code = text;

        std::string title;
        std::cout << "Enter title: ";
        std::cin.getline(text, 99);
        title = text;

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

        Course p(id, code, title, unit, ca, exam);
        return p;
    }

    bool Course::generate(int count)
    {
        auto &courses = getData();

        int id = 0;
        std::string code;
        std::string title;
        float unit;
        float ca;
        float exam;

        for (int i = 0; count > 0; i++)
        {
            // id = 0;
            code = gcore::FakeGen::getCOURSECODE();
            title = gcore::FakeGen::getCOURSETITLE();
            unit = gcore::FakeGen::getNUMBER(0, 5);
            ca = gcore::FakeGen::getNUMBER({0, 30, 50, 70});
            exam = 100 - ca;

            Course p(id, code, title, unit, ca, exam);
            if (add(p))
            {
                --count;
            }
        }
        return true;
    }

    void Course::list()
    {
        auto &courses = getData();

        for (auto &&v : courses)
        {
            std::cout << v.toString();
        }
    }

    bool Course::create()
    {
        auto &courses = getData();
        int maxID = courses.size();
        Course p = Course::input(maxID);

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

    bool Course::edit()
    {
        // display list
        list();

        // choose ID to edit
        std::cout << "Enter the Course ID to Edit: ";
        char text[100];
        std::cin.getline(text, 99);

        int id = std::stoi(text);

        auto &courses = getData();
        auto p = gquery::first(courses, [&id](Course x)
                               { return x.getId() == id; });

        // dislay course to edit
        std::cout << "\n"
                  << p.toString();
        // ask for confirmation
        auto message = "Confirm editing of Course ID:" + std::to_string(p.getId());
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

        std::cout << "Edit Code [" << p.getCode() << "]: ";
        std::cin.getline(text, 99);
        if (std::strcmp(text, "") != 0)
        {
            p.setCode(text);
        }

        std::cout << "Edit title [" << p.getTitle() << "]: ";
        std::cin.getline(text, 99);
        if (std::strcmp(text, "") != 0)
        {
            p.setTitle(text);
        }

        std::cout << "Edit unit [" << p.getUnit() << "]: ";
        std::cin.getline(text, 99);
        if (std::strcmp(text, "") != 0)
        {
            p.setUnit(std::stof(text));
        }

        std::cout << "Edit ca [" << p.getCA() << "]: ";
        std::cin.getline(text, 99);
        if (std::strcmp(text, "") != 0)
        {
            p.setCA(std::stof(text));
        }

        std::cout << "Edit exam [" << p.getExam() << "]: ";
        std::cin.getline(text, 99);
        if (std::strcmp(text, "") != 0)
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

    bool Course::remove()
    {
        // display list
        list();

        // choose ID to edit
        std::cout << "Enter the Course ID to Delete: ";
        char text[100];
        std::cin.getline(text, 99);

        int id = std::stoi(text);

        auto &courses = getData();
        auto p = gquery::first(courses, [&id](Course x)
                               { return x.getId() == id; });

        // dislay course to delete
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
    
    void Course::listReport()
    {
        
    }
    
    void Course::detailReports()
    {
        
    }

    bool Course::resultSheetReports()
    {
        auto &courses = getData();

        for (auto &&v : courses)
        {
            v.resultSheetReport();
        }

        return true;
    }

    bool Course::add(Course course)
    {
        auto &courses = getData();

        if (!isExisting(course))
        {
            if (course.getId() == 0)
            {
                int maxID = 0;

                if (courses.size() > 0)
                {
                    auto p = gquery::max(courses, [](Course x, Course y)
                                         { return x.getId() < y.getId(); });
                    maxID = p.getId();
                }
                course.setId(++maxID);
            }

            courses.push_back(course);
            return true;
        }
        return false;
    }

    bool Course::update(Course course)
    {
        auto &courses = getData();

        if (hasData())
        {
            gquery::replace(courses, course, [&course](Course x)
                            { return x.getId() == course.getId(); });

            return true;
        }
        return false;
    }

    bool Course::deletee(int id)
    {
        if (hasData())
        {
            auto &courses = getData();

            gquery::remove(courses, [&id](Course x)
                           { return x.getId() == id; });
            return true;
        }
        return false;
    }

} // namespace school

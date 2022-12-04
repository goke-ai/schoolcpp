#include <stdlib.h>
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
#include "person.h"
#include "student.h"
#include "course.h"
#include "grade.h"
#include "studentcourse.h"

namespace school
{
    Student::Student()
    {
    }

    Student::Student(int id,
                     std::string studentNo,
                     std::string department,
                     int personId)

    {
        _id = id;
        _studentNo = studentNo;
        _department = department;
        _personId = personId;
    }

    Student::~Student()
    {
    }

    const std::string Student::getPersonName() const
    {
        auto &persons = Person::getData();

        auto p = gquery::first(persons, [this](Person x)
                               { return x.getId() == this->getPersonId(); });

        std::ostringstream os;
        os << gcore::toUpper(p.getSurname()) << ", " << p.getFirstname();
        return os.str();
    }

    const std::string Student::getGender() const
    {
        auto &persons = Person::getData();

        auto p = gquery::first(persons, [this](Person x)
                               { return x.getId() == this->getPersonId(); });

        std::ostringstream os;
        os << gcore::genderToString(p.getGender());
        return os.str();
    }

    const std::string Student::getBirthDate() const
    {
        auto &persons = Person::getData();

        auto p = gquery::first(persons, [this](Person x)
                               { return x.getId() == this->getPersonId(); });

        std::ostringstream os;
        os << p.getBirthDate();
        return os.str();
    }

    const std::string Student::getUrlName() const
    {
        return getStudentNo();
    }

    std::string Student::toString()
    {
        std::ostringstream os;

        os << "{";
        os << "id:" << _id;
        os << ", studentNo:" << _studentNo;
        os << ", department:" << _department;
        os << ", personId:" << _personId;
        os << "}\n";

        return os.str();
    }

    std::string Student::toCsv()
    {
        std::ostringstream os;

        os << _id;
        os << "," << _studentNo;
        os << "," << _department;
        os << "," << _personId;
        os << "\n";

        return os.str();
    }

    std::string Student::toJson()
    {
        std::ostringstream os;

        os << "{";
        os << "\"id\":" << _id;
        os << "\"studentNo\":\"" << _studentNo << "\"";
        os << "\"department\":\"" << _department << "\"";
        os << "\"personId\":" << _personId;
        os << "}";

        return os.str();
    }

    std::string Student::detailReport()
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
                              Html::tag("h2", "STUDENT PERSONAL DATE", {
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
                                      Html::th("Student No"),
                                      Html::td(getStudentNo()),
                                  }),
                                  Html::tr({
                                      Html::th("Person ID"),
                                      Html::td(std::to_string(getPersonId())),
                                  }),
                                  Html::tr({
                                      Html::th("Names"),
                                      Html::td(getPersonName()),
                                  }),
                                  Html::tr({
                                      Html::th("BirthDate"),
                                      Html::td(getBirthDate()),
                                  }),
                                  Html::tr({
                                      Html::th("Gender"),
                                      Html::td(getGender()),
                                  }),
                                  Html::tr({
                                      Html::th("Department"),
                                      Html::td(getDepartment()),
                                  }),
                              }),
                          },
                          "class='m-1 p-1'"),
            }),
        });

        std::string path = "reports/st/" + this->getUrlName() + ".html";

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

    std::string Student::transcriptReport()
    {
        auto &studentCourses = StudentCourse::getData();
        auto &courses = Course::getData();

        auto qStudentCourses = gquery::where(studentCourses, [this](StudentCourse x)
                                             { return x.getStudentId() == this->getId(); });
        auto oStudentCourses = gquery::orderBy(qStudentCourses,
                                               [&courses](StudentCourse x, StudentCourse y)
                                               {
                                                   auto cx = gquery::first(courses, [&x](Course c1)
                                                                           { return c1.getId() == x.getCourseId(); });
                                                   auto cy = gquery::first(courses, [&y](Course c1)
                                                                           { return c1.getId() == y.getCourseId(); });
                                                   return cx.getCode() < cy.getCode();
                                               });

        auto totalUnit = gquery::sum<float>(oStudentCourses,
                                            [](StudentCourse x)
                                            { return x.getUnit(); });
        auto totalGGP = gquery::sum<float>(oStudentCourses,
                                           [](StudentCourse x)
                                           { return x.getUnit() * x.getGradePoint(); });
        auto CGPA = totalGGP / totalUnit;

        auto textCenter = "class='text-center'";
        auto style = Report::getStyle();

        std::ostringstream os;

        os << Html::html({
            Html::head({
                Html::style({style}),
            }),

            Html::body({
                Html::div({
                              Html::img({
                                  "src='../../img/logo.png'",
                                  "width='200px'",
                                  textCenter,
                              }),
                              Html::tag("h1", "COVENANT UNIVERSITY"),
                              Html::tag("h2", "CHEMICAL ENGINEERING DEPARTMENT"),
                              Html::tag("h2", "STUDENT TRANSCRIPT", {
                                                                        "class='p-1'",
                                                                    }),
                              Html::hr(),
                          },
                          "class='m-1 p-1 text-center'"),

                Html::div({
                              Html::table({
                                  Html::tr({
                                      Html::th("Student No."),
                                      Html::td(getStudentNo()),
                                  }),
                                  Html::tr({
                                      Html::th("Names"),
                                      Html::td(getPersonName()),
                                  }),
                                  Html::tr({
                                      Html::th("Gender"),
                                      Html::td(getGender()),
                                  }),
                                  Html::tr({
                                      Html::th("BirthDate"),
                                      Html::td(getBirthDate()),
                                  }),
                              }),
                          },
                          "class='m-1 p-1'"),

                Html::div({
                              Html::table({Html::tr({
                                               Html::th("SN"),
                                               Html::th("Id"),
                                               Html::th("Code"),
                                               Html::th("Title"),
                                               Html::th("Unit", {textCenter}),
                                               Html::tag("th", "CA", {textCenter}),
                                               Html::th("Exam", {textCenter}),
                                               Html::th("Total", {textCenter}),
                                               Html::th("Grade", {textCenter}),
                                               Html::th("Grade Point", {textCenter}),
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
                                                        Html::td([&v](){ return v.getCode(); }, [](){ return ""; }),                                                                
                                                        Html::td([&v](){ return v.getTitle(); }, [](){ return ""; }),                                                                
                                                        Html::td([&v](){ return v.getUnit(); }, [](){ return "class='text-center'"; }),                                                                
                                                        Html::td([&v](){ return gcore::toWidth(v.getCA(),2); }, [](){ return "class='text-center'"; }),                                                                
                                                        Html::td([&v](){ return gcore::toWidth(v.getExam(),2); }, [](){ return "class='text-center'"; }),                                                                
                                                        Html::td([&v](){ return gcore::toWidth(v.getTotal(),2); }, [](){ return "class='text-center'"; }),                                                                
                                                        Html::td([&v](){ return v.getGrade(); }, [](){ return "class='text-center'"; }),                                                                
                                                        Html::td([&v](){ return v.getGradePoint(); }, [](){ return "class='text-center'"; }),                                                                
                                                    });
                                                }
                                                return rt; }),

                                           Html::tr({
                                               Html::th(),
                                               Html::th(),
                                               Html::th(),
                                               Html::th(),
                                               Html::th([&]()
                                                        { return totalUnit; },
                                                        [&]()
                                                        { return textCenter; }),
                                               Html::th([&]()
                                                        { return totalGGP; },
                                                        [&]()
                                                        { return textCenter; }),
                                               Html::th([&]()
                                                        { return gcore::approximate(CGPA); },
                                                        [&]()
                                                        { return textCenter; }),
                                               Html::th(),
                                               Html::th(),
                                               Html::th(),
                                           })}),

                          },
                          "class='m-1 p-1'"),
            }),
        });

        std::string path = "reports/ts/" + this->getStudentNo() + ".html";

        std::ofstream file(path);

        if (!file.is_open())
        {
            return NULL;
        }

        file << os.str();

        file.close();

        return path;
    }

    void Student::openTranscript()
    {
        auto url = "file:///Users/goke/Library/CloudStorage/OneDrive-Personal/zs/schoolcpp/reports/rs_CHE112.html";
        // ShellExecute(NULL, "open", url, NULL, NULL, SW_SHOWNORMAL);
    }

    // static

    std::string Student::_path = "./data/student.sch";
    std::vector<Student> Student::_data;

    std::vector<Student> &Student::getData()
    {
        return _data;
    }

    bool Student::hasData()
    {
        auto &students = getData();

        if (students.size() == 0)
        {
            load();
        }

        return students.size() > 0;
    }

    bool Student::isExisting(Student student)
    {
        if (hasData())
        {
            auto &students = getData();

            bool v = gquery::any(students, [&student](Student s)
                                 { return s.getStudentNo() == student.getStudentNo(); });
            return v;
        }
        return false;
    }

    void Student::load()
    {
        auto &students = getData();

        students.clear();

        auto path = _path; // "student.sch";

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
            std::string studentNo = words[1];
            std::string department = words[2];
            auto personId = std::stoi(words[3]);

            Student s(id, studentNo, department, personId);

            students.push_back(s);
        }

        file.close();
    }

    void Student::write()
    {
        auto path = _path; // "student.sch";

        std::ofstream file(path);

        if (!file.is_open())
        {
            return;
        }

        auto &students = getData();

        for (auto &&v : students)
        {
            file << v.toCsv();
        }

        file.close();
    }

    void Student::clean()
    {
        auto path = _path; // "student.sch";

        std::ofstream file;
        file.open(path, std::ofstream::out | std::ofstream::trunc);
        file.close();
    }

    Student Student::input(int &maxID)
    {
        char text[100];
        std::string emp{""};

        int id = 0;
        // std::cout << "Enter ID (0, " << ++maxID << " or any number): ";
        // std::cin.getline(text, 99);
        // id = std::stoi(text);

        std::string studentNo;
        std::cout << "Enter StudentNo: ";
        std::cin.getline(text, 99);
        studentNo = text;

        std::string department;
        std::cout << "Enter department: ";
        std::cin.getline(text, 99);
        department = text;

        int personId;
        std::cout << "Enter personId: ";
        std::cin.getline(text, 99);
        personId = std::stoi(text);

        Student s(id, studentNo, department, personId);
        return s;
    }

    bool Student::generate(int count)
    {
        auto &students = getData();
        // get available persons
        auto &persons = Person::getData();
        auto availPersons = gquery::where(persons,
                                          [&students](Person p)
                                          { return gquery::any(students,
                                                               [&p](Student s)
                                                               { return s.getPersonId() == p.getId(); }) == false; });

        int id = 0;
        std::string studentNo;
        std::string department;
        int personId;

        int maxk = 0;
        for (int i = 0; count > 0; i++)
        {
            maxk = availPersons.size() - 1;
            if (maxk < 0)
            {
                break;
            }

            int k = gcore::randint(0, maxk);
            auto p = availPersons[k];
            personId = p.getId();

            // id = 0;
            department = gcore::FakeGen::getDEPARTMENTCODE();
            studentNo = gcore::FakeGen::getSTUDENTNUMBER(2019, 2019, department);

            Student s(id, studentNo, department, personId);
            if (add(s))
            {
                --count;
                availPersons.erase(availPersons.begin() + k,
                                   availPersons.begin() + k + 1);
            }
        }
        return true;
    }

    void Student::list()
    {
        auto &students = getData();

        for (auto &&v : students)
        {
            std::cout << v.toString();
        }
    }

    bool Student::create()
    {
        auto &students = getData();
        int maxID = students.size();
        Student s = Student::input(maxID);

        auto b = add(s);
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

    bool Student::edit()
    {
        // display list
        list();

        // choose ID to edit
        std::cout << "Enter the Student ID to Edit: ";
        char text[100];
        std::string emp{""};
        std::cin.getline(text, 99);

        int id = std::stoi(text);

        auto &students = getData();
        auto s = gquery::first(students, [&id](Student x)
                               { return x.getId() == id; });

        // dislay student to edit
        std::cout << "\n"
                  << s.toString();
        // ask for confirmation
        auto message = "Confirm editing of Student ID:" + std::to_string(s.getId());
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

        std::cout << "Edit StudentNo [" << s.getStudentNo() << "]: ";
        std::cin.getline(text, 99);
        if (emp.compare(text) != 0)
        {
            s.setStudentNo(text);
        }

        std::cout << "Edit department [" << s.getDepartment() << "]: ";
        std::cin.getline(text, 99);
        if (emp.compare(text) != 0)
        {
            s.setDepartment(text);
        }

        std::cout << "Edit personId [" << s.getPersonId() << "] (M or F) : ";
        std::cin.getline(text, 99);
        if (emp.compare(text) != 0)
        {
            s.setPersonId(std::stoi(text));
        }

        auto b = update(s);
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

    bool Student::remove()
    {
        // display list
        list();

        // choose ID to edit
        std::cout << "Enter the Student ID to Delete: ";
        char text[100];
        std::string emp{""};
        std::cin.getline(text, 99);

        int id = std::stoi(text);

        auto &students = getData();
        auto s = gquery::first(students, [&id](Student x)
                               { return x.getId() == id; });

        // dislay student to delete
        std::cout << "\n"
                  << s.toString();
        // ask for confirmation
        auto message = "Confirm delete of Student ID:" + std::to_string(s.getId());
        auto yesorno = gcore::Ux::confirm(message);
        if (yesorno == 'N')
        {
            std::cout << "Delete cancelled.\n";
            return false;
        }

        auto b = deletee(s.getId());

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

    void Student::listReport()
    {
        auto &students = getData();

        auto oStudents = gquery::orderBy(students, [](Student x, Student y)
                                         { return (x.getStudentNo() < y.getStudentNo()); });

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
                              Html::tag("h2", "STUDENT PERSONAL DATA", {
                                                                           "class='p-1'",
                                                                       }),
                              Html::hr(),
                          },
                          "class='m-1 p-1 text-center'"),

                Html::div({
                              Html::table({Html::tr({
                                               Html::th("SN"),
                                               Html::th("Id"),
                                               Html::th("Student No."),
                                               Html::th("Person ID"),
                                               Html::th("Names"),
                                               Html::th("BirthDate"),
                                               Html::th("Gender"),
                                               Html::th("Department"),
                                           }),

                                           Html::forTag([&]()
                                                        {
                                                std::string rt{" "};
                                                int n=0;

                                                for (auto &&v : oStudents)
                                                {
                                                    rt += Html::tr({
                                                        Html::td([&n](){ return ++n; }, [](){ return ""; }),                                                                
                                                        Html::td([&v](){ return v.getId(); }, [](){ return ""; }),                                                                
                                                        Html::td([&v](){ return v.getStudentNo(); }, [](){ return ""; }),                                                                
                                                        Html::td([&v](){ return v.getPersonId(); }, [](){ return ""; }),                                                                
                                                        Html::td([&v](){ return v.getPersonName(); }, [](){ return ""; }),                                                                
                                                        Html::td([&v](){ return v.getBirthDate(); }, [](){ return ""; }),                                                                
                                                        Html::td([&v](){ return v.getGender(); }, [](){ return ""; }),                                                                
                                                        Html::td([&v](){ return v.getDepartment(); }, [](){ return ""; }),                                                                
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
                                               Html::th(),
                                               Html::th(),
                                           })}),

                          },
                          "class='m-1 p-1'"),
            }),
        });

        std::string path = "reports/st/list.html";
        std::ofstream file(path);
        if (!file.is_open())
        {
            return;
        }
        file << os.str();
        file.close();

        std::string fileUrl = "reports/st/list.html";

        Report::open(fileUrl);
    }

    void Student::detailReports()
    {
        //
        list();

        // choose ID to edit
        std::cout << "Enter the Student ID to create detail report: ";
        char text[100];
        std::string emp{""};
        std::cin.getline(text, 99);

        int id = std::stoi(text);

        auto &students = getData();
        auto s = gquery::first(students, [&id](Student x)
                               { return x.getId() == id; });

        // dislay student to edit
        std::cout << "\n"
                  << s.toString();
        // ask for confirmation
        auto message = "Confirm creating detail report for Student ID:" + std::to_string(s.getId());
        auto yesorno = gcore::Ux::confirm(message);
        if (yesorno == 'N')
        {
            std::cout << "Detail report cancelled.\n";
            return;
        }

        auto fileUrl = s.detailReport();

        Report::open(fileUrl);
    }

    bool Student::transcriptReports()
    {
        list();

        // choose ID to edit
        std::cout << "Enter the Student ID to create transcript: ";
        char text[100];
        std::string emp{""};
        std::cin.getline(text, 99);

        int id = std::stoi(text);

        auto &students = getData();
        auto s = gquery::first(students, [&id](Student x)
                               { return x.getId() == id; });

        // dislay student to edit
        std::cout << "\n"
                  << s.toString();
        // ask for confirmation
        auto message = "Confirm creating transcript for Student ID:" + std::to_string(s.getId());
        auto yesorno = gcore::Ux::confirm(message);
        if (yesorno == 'N')
        {
            std::cout << "Transcript cancelled.\n";
            return false;
        }

        std::string fileUrl = s.transcriptReport();

        // std::string fileUrl{"reports/ts/" + s.getStudentNo() + ".html"};

        Report::open(fileUrl);

        return true;
    }

    bool Student::add(Student student)
    {
        auto &students = getData();

        if (!isExisting(student))
        {
            if (student.getId() == 0)
            {
                int maxID = 0;

                if (students.size() > 0)
                {
                    auto s = gquery::max(students, [](Student x, Student y)
                                         { return x.getId() < y.getId(); });
                    maxID = s.getId();
                }
                student.setId(++maxID);
            }

            students.push_back(student);
            //
            return true;
        }
        return false;
    }

    bool Student::update(Student student)
    {
        auto &students = getData();

        if (hasData())
        {
            gquery::replace(students, student, [&student](Student x)
                            { return x.getId() == student.getId(); });
            //
            return true;
        }
        return false;
    }

    bool Student::deletee(int id)
    {
        if (hasData())
        {
            auto &students = getData();

            gquery::remove(students, [&id](Student x)
                           { return x.getId() == id; });
            return true;
        }
        return false;
    }

} // namespace school

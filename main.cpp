#include "main.h"

using namespace std;
using namespace school;

// enum MenuItem
// {
//     None,
//     AddPerson,
//     ViewPersons,
//     GenPersons
// };

char menu();
void load(vector<Person> &persons,
          vector<Student> &students,
          vector<Course> &courses,
          vector<StudentCourse> &studentCourses)
{
    Person::load(persons);
}
void save(vector<Person> &persons,
          vector<Student> &students,
          vector<Course> &courses,
          vector<StudentCourse> &studentCourses)
{
    Person::write(persons);
}

int main(int argc, char const *argv[])
{
    cout << "\n============================\n";
    cout << "Welcome to School Manager!!!\n";

    char key;
    // MenuItem menuItem;

    vector<Person> persons;
    vector<Student> students;
    vector<Course> courses;
    vector<StudentCourse> studentCourses;

    while (true)
    {
        key = menu();

        switch (key)
        {
        case 'p':
            Person::create(persons);
            break;
        case 'l':
            Person::list(persons);
            break;
        case 'g':
            Person::generate(persons);
            break;

        case 's':
            Student::create(students);
            break;
        case 'm':
            Student::list(students);
            break;

        case 'c':
            Course::create(courses);
            break;
        case 'n':
            Course::list(courses);
            break;

        case 't':
            StudentCourse::create(studentCourses);
            break;
        case 'o':
            StudentCourse::list(studentCourses);
            break;

        case 'f':
            load(persons, students, courses, studentCourses);
            break;
        case 'w':
            save(persons, students, courses, studentCourses);
            break;
        default:
            break;
        }

        if (key == 'q')
        {
            break;
        }
    }
    return 0;
}

char menu()
{
    cout << endl;
    cout << "Enter p, l to add, list Person: \n";
    cout << "Enter q to quit: \n";
    cout << "Enter p, or q: ";

    char key = cin.get();
    cin.ignore();

    return key;
}

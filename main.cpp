#include "main.h"

using namespace std;
using namespace school;

char menu();
void load();
void save();
void clean();

int main(int argc, char const *argv[])
{
    cout << "\n============================\n";
    cout << "Welcome to School Manager!!!\n";

    char key;

    while (true)
    {
        key = menu();

        switch (key)
        {
        case 'p':
        {
            cout << "start person create...\n";
            Person::create();
            cout << "...person create finished\n";
        }
        break;
        case 'l':
        {
            cout << "start person list...\n";
            Person::list();
            cout << "...person list finished\n";
        }
        break;
        case 'g':
        {
            cout << "start person generate...\n";
            Person::generate();
            cout << "...person generate finished\n";
        }
        break;
        case '1':
        {
            cout << "start person edit...\n";
            Person::edit();
            cout << "...person edit finished\n";
        }
        break;
        case '5':
        {
            cout << "start person remove...\n";
            Person::remove();
            cout << "...person remove finished\n";
        }
        break;
        case 'd':
        {
            cout << "start person detail reports...\n";
            Person::detailReports();
            cout << "...person detail reports finished\n";
        }
        break;

        case 'u':
        {
            cout << "start student create...\n";
            Student::create();
            cout << "...student create finished\n";
        }
        break;
        case 'm':
        {
            cout << "start student list...\n";
            Student::list();
            cout << "...student list finished\n";
        }
        break;
        case 'h':
        {
            cout << "start student generate...\n";
            Student::generate();
            cout << "...student generate finished\n";
        }
        break;
        case '2':
        {
            cout << "start student edit...\n";
            Student::edit();
            cout << "...student edit finished\n";
        }
        break;
        case '6':
        {
            cout << "start student remove...\n";
            Student::remove();
            cout << "...student remove finished\n";
        }
        break;
        case 'e':
        {
            cout << "start student detail reports...\n";
            Student::detailReports();
            cout << "...student detail reports finished\n";
        }
        break;

        case 'c':
        {
            cout << "start course create...\n";
            Course::create();
            cout << "...course create finished\n";
        }
        break;
        case 'n':
        {
            cout << "start course list...\n";
            Course::list();
            cout << "...course list finished\n";
        }
        break;
        case 'i':
        {
            cout << "start course generate...\n";
            Course::generate();
            cout << "...course generate finished\n";
        }
        break;
        case '3':
        {
            cout << "start course edit...\n";
            Course::edit();
            cout << "...course edit finished\n";
        }
        break;
        case '7':
        {
            cout << "start course remove...\n";
            Course::remove();
            cout << "...course remove finished\n";
        }
        break;
        case 'f':
        {
            cout << "start course detail reports...\n";
            Course::detailReports();
            cout << "...course detail reports finished\n";
        }
        break;

        case 't':
        {
            cout << "start studentcourse create...\n";
            StudentCourse::create();
            cout << "...studentcourse create finished\n";
        }
        break;
        case 'r':
        {
            cout << "start studentcourse list...\n";
            StudentCourse::list();
            cout << "...studentcourse list finished\n";
        }
        break;
        case 'j':
        {
            cout << "start studentcourse generate...\n";
            StudentCourse::generate();
            cout << "...studentcourse generate finished\n";
        }
        break;
        case '4':
        {
            cout << "start studentcourse edit...\n";
            StudentCourse::edit();
            cout << "...studentcourse edit finished\n";
        }
        break;
        case '8':
        {
            cout << "start studentcourse remove...\n";
            StudentCourse::remove();
            cout << "...studentcourse remove finished\n";
        }
        break;
        case 'k':
        {
            cout << "start studentcourse detail reports...\n";
            StudentCourse::detailReports();
            cout << "...studentcourse detail reports finished\n";
        }
        break;

        case 'a':
        {
            cout << "start student transcripts...\n";
            Student::transcriptReports();
            cout << "...student transcripts finished\n";
        }
        break;
        case 'b':
        {
            cout << "start course result-sheets...\n";
            Course::resultSheetReports();
            cout << "...course result-sheets finished\n";
        }
        break;
        case 'v':
        {
            cout << "start list reports...\n";
            Student::listReport();
            cout << "...list finished\n";
        }
        break;

        case 'o':
        {
            cout << "start load...\n";
            load();
            cout << "...load finished\n";
        }
        break;
        case 's':
        {
            cout << "start save...\n";
            save();
            cout << "...save finished\n";
        }
        break;
        case 'x':
        {
            cout << "start clean...\n";
            clean();
            cout << "...clean finished\n";
        }
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
    cout << "Enter p, l, g, 1, 5, d to add, list|generate|edit|delete|details Person: \n";
    cout << "Enter u, m, h, 2, 6, e to add, list|generate|edit|delete|details Student: \n";
    cout << "Enter c, n, i, 3, 7, f to add, list|generate|edit|delete|details Course: \n";
    cout << "Enter t, r, j, 4, 8, k to add, list|generate|edit|delete|details StudentCourse: \n";
    cout << "Enter a, b, v to transcipts, result-sheets, list-Report: \n";
    cout << "Enter o, s, x, q to open, save, clean, quit: \n";
    cout << "Enter p,u,c,t; l,m,n,r; g,h,i,j; 1,2,3,4; 5,6,7,8; d,e,f,k; a,b,v or o,s,x,q: ";

    char key = cin.get();
    cin.ignore();

    return key;
}

void load()
{
    Person::load();
    Student::load();
    Course::load();
    StudentCourse::load();
    Grade::load();
}

void save()
{
    Person::write();
    Student::write();
    Course::write();
    StudentCourse::write();
    Grade::write();
}

void clean()
{
    Person::clean();
    Student::clean();
    Course::clean();
    StudentCourse::clean();
    Grade::clean();
}
#pragma once

#include <vector>
#include <string>

namespace school
{
    class Grade
    {
    private:
        int _id = 0;
        std::string _name;
        int _sytemType;
        float _beginScore;
        float _nextScore;
        float _point;

        static std::string _path;
        static std::vector<Grade> _data;

    public:
        Grade();
        Grade(int id,
               std::string name,
               int sytemType,
               float beginScore,
               float nextScore,
               float point);

        ~Grade();

        const int &getId() const { return _id; }
        void setId(int id) { _id = id; }

        const std::string &getName() const { return _name; }
        void setName(std::string name) { _name = name; }

        const int &getSytemType() const { return _sytemType; }
        void setSytemType(int sytemType) { _sytemType = sytemType; }

        const float &getBeginScore() const { return _beginScore; }
        void setBeginScore(float beginScore) { _beginScore = beginScore; }

        const float &getNextScore() const { return _nextScore; }
        void setNextScore(float nextScore) { _nextScore = nextScore; }

        const float &getPoint() const { return _point; }
        void setPoint(float point) { _point = point; }

        std::string toString();
        std::string toCsv();
        std::string toJson();

        // statics
        static std::vector<Grade> &getData();

        static bool hasData();
        static bool isExisting(Grade grade);

        static void load();
        static void write();
        static void clean();

        static Grade input(int &maxID);
        static bool generate(int count = 20);
        static void list();
        static bool create();
        static bool edit();
        static bool remove();

        static bool add(Grade grade);
        static bool update(Grade grade);
        static bool deletee(int id);
    };
} // namespace school

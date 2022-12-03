#pragma once

namespace school
{

    class Base
    {
    private:
        int _id;

    public:
        Base();
        Base(int id);
        ~Base();

        const int &getId() const { return _id; }
        void setId(int id) { _id = id; }
    };

} // namespace school

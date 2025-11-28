#ifndef OBJECT_H
#define OBJECT_H
#include "shape.h"


class object
{
    public:
        object();
        ~object();

        int id;
        int x;
        int y;

        int xDir;
        int yDir;

        int travel;

        shape body;
        void move();
        bool is_collide(int x1,int y1);

    protected:

    private:
};

#endif // OBJECT_H

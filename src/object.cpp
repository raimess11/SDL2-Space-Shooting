#include "object.h"
#include <iostream>

object::object()
{
    //ctor


}

//object::move(){
//}

void object::move(){
    x+=xDir;
    y+=yDir;
}

bool object::is_collide(int x1,int y1) {
    int a = x1-x;
    int b = y1-y;
    if(body.Tab_pixel[(a+1)+(body.collom*(-y))]!=0x00){return true;std::cout<<"got hit"<<std::endl;}

    return false;
}

object::~object()
{
    //dtor
}

#include "enemy.h"

enemy::enemy()
{
    //ctor
}

void enemy::action()
{
    xDir = tab_post[action_num].x - x;
    yDir = tab_post[action_num].y - y;
    x += xDir/abs(xDir);
    y += yDir/abs(yDir);

    if(x == tab_post[action_num].x && y == tab_post[action_num].y ){
        action_num++;
        if(action_num<post_len){
            action_num = 0;
        }
    }
}

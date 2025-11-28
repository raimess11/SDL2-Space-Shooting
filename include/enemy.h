#ifndef ENEMY_H
#define ENEMY_H

#include "object.h"
#include <SDL.h>


class enemy : public object
{
    public:
        enemy();
        SDL_Point tab_post[100];
        int post_len;
        int action_num;
        Uint8 tag;

        void action();

    protected:

    private:
};

#endif // ENEMY_H

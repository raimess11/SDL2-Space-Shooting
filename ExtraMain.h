#ifndef EXTRAMAIN_H_INCLUDED
#define EXTRAMAIN_H_INCLUDED

#include <SDL.h>
#include "include\object.h"
#include "include\shape.h"

class tab_bit{
public:
    int arr[32*32];
    int len;
};

void draw_object(SDL_Renderer* renderer, object obj);
void build_body(shape* ship,tab_bit shipArr,int collom,int row);



#endif // EXTRAMAIN_H_INCLUDED

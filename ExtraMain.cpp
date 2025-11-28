#include "ExtraMain.h"
#include <object.h>

void draw_object(SDL_Renderer* renderer, object obj){
    for(int i=0; i < obj.body.collom * obj.body.row; i++){
        if(obj.body.Tab_pixel[i]!=0x00){
            SDL_RenderDrawPoint(renderer, i% obj.body.collom + obj.x - (obj.body.collom/2), i/obj.body.collom + obj.y - (obj.body.row/2));
        }
    }
}

void build_body(shape* ship,tab_bit shipArr,int collom,int row)
{
    for(int i=0; i<shipArr.len; i++){
        ship->Tab_pixel[i]=shipArr.arr[i];
    }
    ship->collom = collom;
    ship->row = row;
}



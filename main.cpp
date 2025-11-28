#include <iostream>
#include <cstdio>
#include <string>
#include <ctime>
#include <cmath>
#include <stdlib.h> //srand, rand
#include <time.h>

#include "include/shape.h"
#include "include/object.h"
#include "include/enemy.h"

#include "ExtraMain.h"

#include <SDL.h>
#include "C:\Program Support\Development\SDL2_ttf-2.20.1\x86_64-w64-mingw32\include\SDL2\SDL_ttf.h"

//window size
const int WIDTH = 1280, HEIGHT = 720;



//render scaling
int scale = 4;
int scale_x = scale;
int scale_y = scale;

//FPS
float AR;   //actual processing rate per second
float FPS;  // Frame per second
clock_t delta;       //time need to finish last iteration
clock_t deltaframe;  //time need to do next frame
clock_t sumdeltaProcess;   //cumulative of delta for process
clock_t sumdeltaFrame;     //cumulative of delta for frames

//input handle;
bool a;
bool s;
bool d;
bool w;


//create tab bit
tab_bit ship_bit;
tab_bit enemyship_bit;

//asset model in array data
const int shipArr[]={0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x01,0x00,0x00,0x00,0x00,0x01,0x01,0x00,0x00,0x00,0x01,0x01,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x01,0x00,0x00,0x00,0x01,0x00,0x00};
const int enemyship1Arr[]={0x00,0x00,0x01,0x00,0x01,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x01,0x00,0x01,0x00,0x00,0x00,0x00,0x01,0x00,0x01,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00};

//create shape
shape ship;
shape enemy1;

//create array of bullet
object tab_bullet[1000];
int tab_bullet_size;

//create array of enemybullet
object tab_enemy_bullet[1000];
int tab_enemy_bullet_size;

//create array of enemy
object tab_enemy[1000];
int tab_enemy_size;

char astring[10];

int main(int argc, char* argv[])
{
    //try to get init error
    if(SDL_Init(SDL_INIT_EVERYTHING)){
        std::cout<<"SDL could not initialize! Error: "<<SDL_GetError()<<std::endl;
    }

    //create window & renderer
    SDL_Window* window;
    SDL_Renderer* renderer;

    SDL_CreateWindowAndRenderer(WIDTH, HEIGHT,0,&window,&renderer);
    SDL_RenderSetScale(renderer,scale,scale);



    //get open window error
    if(window == NULL){
        std::cout<<"window could not open! Error: "<< SDL_GetError()<<std::endl;
        return EXIT_FAILURE;
    }

    if(TTF_Init()==-1){std::cout<<"SDL_ttf could not initialize! Error: "<<TTF_GetError()<<std::endl;}

    SDL_Surface* surfaceMessage;
    SDL_Texture* Message;

    //randomis
    srand(time(NULL));

    //declare event
    SDL_Event event;

    //convert array to tab_bit data type
    ship_bit.len = sizeof(shipArr)/sizeof(shipArr[0]);
    for(int i=0; i<ship_bit.len; i++){
            ship_bit.arr[i] = shipArr[i];
        }

    enemyship_bit.len = sizeof(enemyship1Arr)/sizeof(enemyship1Arr[0]);
    for(int i=0; i<enemyship_bit.len; i++){
            enemyship_bit.arr[i] = enemyship1Arr[i];
        }


    //create shape/body
    build_body(&ship,ship_bit,9,7);
    build_body(&enemy1,enemyship_bit,7,7);

    //player
    object player;
    player.body = ship;
    player.x = WIDTH/scale/2;
    player.y = -10 + HEIGHT/scale;

    //assign bullet
    //build bullet
    object bullet;
    bullet.id = 100;
    bullet.body.Tab_pixel[0]=1;
    bullet.body.Tab_pixel[1]=1;
    bullet.body.collom = 1;
    bullet.body.row = 2;

    bullet.x = -100;
    bullet.y = -100;

    tab_bullet_size = 10;
    for(int i=0; i<tab_bullet_size; i++){
        tab_bullet[i] = bullet;
        tab_bullet[i].travel = i*HEIGHT/scale/tab_bullet_size;
    }


    bullet.id = 101;
    bullet.body.Tab_pixel[0]=0;
    bullet.body.Tab_pixel[1]=1;
    bullet.body.Tab_pixel[3]=1;
    bullet.body.Tab_pixel[5]=1;
    bullet.body.Tab_pixel[7]=1;
    bullet.body.collom = 3;
    bullet.body.row = 3;

    tab_enemy_bullet_size = 5;
    for(int i=0; i<tab_enemy_bullet_size; i++){
        tab_enemy_bullet[i] = bullet;
        tab_enemy_bullet[i].travel = i*HEIGHT/scale/tab_enemy_bullet_size;
    }

    //assign enemy
    object enemyT1;
    enemyT1.body = enemy1;
    for(int i=0;i<4;i++){
        for(int j=0;j<10;j++){
            tab_enemy[10*i+j] = enemyT1;
            tab_enemy[10*i+j].x = ((10*j)-90/2+WIDTH/2/scale);
            tab_enemy[10*i+j].y = ((10*i)+HEIGHT/5/scale);
        }
    }
    tab_enemy[0].id =100;
    tab_enemy_size = 40;



    object xx = tab_enemy[0];
    std::cout<<"position of bit: "<<std::endl;
    for(int i=0; i < xx.body.collom * xx.body.row; i++){
        if(xx.body.Tab_pixel[i]!=0x00){
                std::cout<<"("<<i%xx.body.collom+xx.x<<","<<i/xx.body.collom + xx.y<<") . ";
        }
        }


    //program loop
    while (true)
        {
            AR++;
            //get time on beginning of loop
            clock_t begin_time = clock();

            //input
            if(SDL_PollEvent(&event)){
                //QUIT EVENT
                if(event.type == SDL_QUIT){
                    break;
                }

                //ASWD direction input
                if(event.type == SDL_KEYDOWN){
                    if(event.key.keysym.sym == SDLK_d){
                        d=true;
                    }
                    if(event.key.keysym.sym == SDLK_a){
                        a=true;
                    }
                    if(event.key.keysym.sym == SDLK_s){
                        s=true;
                    }
                    if(event.key.keysym.sym == SDLK_w){
                        w=true;
                    }
                }
                if(event.type == SDL_KEYUP){
                    if(event.key.keysym.sym == SDLK_d){
                        d=false;
                    }
                    if(event.key.keysym.sym == SDLK_a){
                        a=false;
                    }
                    if(event.key.keysym.sym == SDLK_s){
                        s=false;
                    }
                    if(event.key.keysym.sym == SDLK_w){
                        w=false;
                    }
                }

                player.xDir = (d) - (a);
                player.yDir = (s) - (w);
            }



            //FPS process
            if (sumdeltaFrame > 1000/60){
                //begin frame
                FPS++;
                clock_t frame_begin = clock();






                if(player.x + player.xDir > 0 && player.x + player.xDir<WIDTH/scale){
                    player.x += player.xDir;
                }
                if(player.y + player.yDir>0 && player.y + player.yDir<HEIGHT/scale){
                    player.y += player.yDir;
                }

                for(int i=0; i<tab_bullet_size; i++){
                    tab_bullet[i].y -= 1;
                    tab_bullet[i].travel +=1;

                    if(tab_bullet[i].travel >= HEIGHT/scale){
                        tab_bullet[i].x = player.x;
                        tab_bullet[i].y = player.y;
                        tab_bullet[i].travel = 0;
                    }
                }

                for(int i=0; i<tab_enemy_bullet_size; i++){
                    tab_enemy_bullet[i].y += 1;
                    tab_enemy_bullet[i].travel += 1;

                    if(tab_enemy_bullet[i].travel >= HEIGHT/scale) {
                        tab_enemy_bullet[i].x = tab_enemy[int(rand()%tab_enemy_size)].x;
                        tab_enemy_bullet[i].y = tab_enemy[int(rand()%tab_enemy_size)].y;
                        tab_enemy_bullet[i].travel = 0;
                    }
                }

                // clear frame before draw new frame
                SDL_SetRenderDrawColor(renderer,125,135,80,255);
                SDL_RenderClear(renderer);


                //draw new frame
                SDL_SetRenderDrawColor(renderer,70,70,80,255);
                draw_object(renderer, player);
                //draw bullet
                for(int i=0; i<tab_bullet_size; i++){
                    draw_object(renderer, tab_bullet[i]);
                }
                //draw enemy
                for(int i=0; i<tab_enemy_size; i++){
                    draw_object(renderer, tab_enemy[i]);
                }

                for(int i = 0; i<tab_enemy_bullet_size; i++) {
                    draw_object(renderer, tab_enemy_bullet[i]);
                }


                for(int i=0; i<tab_enemy_size; i++){
                    for(int j=0; j<tab_bullet_size; j++){
                        for(int z=0; z<tab_enemy[i].body.collom * tab_enemy[i].body.row; z++){
                                if (tab_enemy[i].body.Tab_pixel[z] != 0){
                                    if (tab_bullet[j].x== tab_enemy[i].x+z%tab_enemy[i].body.collom - (tab_enemy[i].body.collom/2) && tab_bullet[j].y == tab_enemy[i].y+z/tab_enemy[i].body.collom-(tab_enemy[i].body.row/2)) {
                                        tab_enemy[i].x = -100;
                                        tab_bullet[j].x = -1000;
                                    }
                                }
                            }
                        }
                    }

                for(int j=0; j<tab_enemy_bullet_size; j++){
                    for(int z=0; z<player.body.collom * player.body.row; z++){
                            if (player.body.Tab_pixel[z] != 0){
                                if (tab_enemy_bullet[j].x== player.x+z%player.body.collom - (player.body.collom/2) && tab_enemy_bullet[j].y == player.y+z/player.body.collom-(player.body.row/2)) {
                                    player.x = -100;
                                    tab_enemy_bullet[j].x = -1000;
                                }
                            }
                        }
                    }

                //this opens a font style and sets a size
                TTF_Font* Sans = TTF_OpenFont("dogicapixel.ttf", 14);
                if(Sans ==NULL){std::cout<<"font not found"<<std::endl;}
                // this is the color in rgb format,
                // maxing out all would give you the color white,
                // and it will be your text's color
                SDL_Color White = {255, 255, 255};

                // as TTF_RenderText_Solid could only be used on
                // SDL_Surface then you have to create the surface first

                surfaceMessage =
                    TTF_RenderText_Solid(Sans, astring, White);

                // now you can convert it into a texture
                Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

                SDL_Rect Message_rect; //create a rect
                Message_rect.x = WIDTH/2/scale;  //controls the rect's x coordinate
                Message_rect.y = HEIGHT/2/scale; // controls the rect's y coordinte
                Message_rect.w = 100; // controls the width of the rect
                Message_rect.h = 20; // controls the height of the rect

                // (0,0) is on the top left of the window/screen,
                // think a rect as the text's box,
                // that way it would be very simple to understand

                // Now since it's a texture, you have to put RenderCopy
                // in your game loop area, the area where the whole code executes

                // you put the renderer's name first, the Message,
                // the crop size (you can ignore this if you don't want
                // to dabble with cropping), and the rect which is the size
                // and coordinate of your texture

                SDL_RenderCopy(renderer, Message, NULL, &Message_rect);

                //render current frame
                SDL_RenderPresent(renderer);

                //get delta frame
                clock_t frame_end = clock();
                deltaframe = frame_end - frame_begin;

                //reset value
                sumdeltaFrame = 0;
            }


            //calculate delta time for FPS
            clock_t end_time = clock();
            delta = end_time - begin_time;
            sumdeltaProcess += delta;
            sumdeltaFrame += delta;

            //get FPS information each second
            if(sumdeltaProcess>=1000){
                //print value
                std::cout<<"sumdeltaProcess: "<<sumdeltaProcess<<std::endl;
                std::cout<<"FPS: "<<FPS<<std::endl;
                std::cout<<"AR: "<<AR<<std::endl;
                sprintf(astring, "%d", int(FPS) );


                //reset value
                sumdeltaProcess = 0;
                FPS = 0;
                AR = 0;
            }

        }
    // Don't forget to free your surface and texture
    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(Message);

    //QUIT PROSEDURE
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}

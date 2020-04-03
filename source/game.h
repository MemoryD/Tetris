#ifndef _GAME_H_ 
#define _GAME_H_

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "Timer.h"
#include <string>
#include <cstdlib>
#include <cstdio>
#include <ctime>

const int SCREEN_WIDTH = 620;
const int SCREEN_HEIGHT = 600;
const int SCREEN_BPP = 32;

const int FPS_SLOW = 2;
const int FPS_FAST = 50;

const int PRESS_TIME = 100;

const int START_SET = 6;

const int GAME_ROW = 20;
const int GAME_COL = 14;

const int BOX_SIZE = 30;
const int SMALL_SIZE = 20;

const int NUM_WIDTH = 30;
const int NUM_HEIGHT = 59;

const int RIGHT = 0;
const int LEFT = 1;
const int DOWN = 2;


SDL_Surface *screen;

SDL_Surface *load_image( std::string filename, bool color = false , int key = -1)
{
    SDL_Surface* loadedImage = NULL;

    SDL_Surface* optimizedImage = NULL;

    loadedImage = IMG_Load( filename.c_str() );

    if( loadedImage != NULL )
    {
        optimizedImage = SDL_DisplayFormat( loadedImage );

        SDL_FreeSurface( loadedImage );

        if( optimizedImage != NULL && color )
        {
        	if( key < 0 )
            	SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB( optimizedImage->format, 0xFF, 0xFF, 0xFF ) );
            else
            	SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, key );
        }
    }

    return optimizedImage;
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination , SDL_Rect *clip = NULL )
{
    SDL_Rect offset;

    offset.x = x;
    offset.y = y;

    SDL_BlitSurface( source, clip, destination, &offset );
}

bool init()
{
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return false;
    }
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );

    if( screen == NULL )
    {
        return false;
    }
    
    SDL_WM_SetCaption( "TETRIS", NULL );

    return true;
}

int random( int min, int max )
{
	return rand() % ( max - min + 1 ) + min;
}

#endif

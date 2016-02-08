#include <iostream>
#include <SDL2/SDL.h>
#include "cleanup.h"

const int SCREEN_WIDTH = 840;
const int SCREEN_HEIGHT = 840;

void assertptr(void *val, std::string ErrorText) {
    if (val == nullptr) {
        std::cout << ErrorText << SDL_GetError() << std::endl;
        SDL_Quit();
        exit(1);
    }
}

int main(int, char**){
	if (SDL_Init(SDL_INIT_VIDEO) != 0){
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}
    SDL_Window *win = SDL_CreateWindow("Perun", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 840, 840, 0);
    assertptr(win, "SDL_CreateWindow Error: ");

    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    assertptr(ren, "SDL_CreateRenderer Error: ");

    //std::string imagePath = getResourcePath("res") + "Sprites.png";
    SDL_Surface *bmp = SDL_LoadBMP("res/Sprites.bmp");
    assertptr(bmp, "SDL_LoadBMP Error: ");

    SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, bmp);
    SDL_FreeSurface(bmp);
    assertptr(tex, "SDL_CreateTextureFromSurface Error: ");

    //A sleepy rendering loop, wait for 3 seconds and render and present the screen each time
    for (int i = 0; i < 3; ++i){
    	//First clear the renderer
    	SDL_RenderClear(ren);
    	//Draw the texture
    	SDL_RenderCopy(ren, tex, NULL, NULL);
    	//Update the screen
    	SDL_RenderPresent(ren);
    	//Take a quick break after all that hard work
    	SDL_Delay(1000);
    }

    cleanup(ren, win, tex);
    SDL_Quit();
	return 0;
}

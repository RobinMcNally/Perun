#include <iostream>
#include <SDL2/SDL.h>
#include "cleanup.h"

const int SCREEN_WIDTH = 840;
const int SCREEN_HEIGHT = 840;

const int TILE_SIZE = 14;

void assertptr(void *val, std::string ErrorText) {
    if (val == nullptr) {
        std::cout << ErrorText << SDL_GetError() << std::endl;
        SDL_Quit();
        exit(1);
    }
}

void logSDLError(std::ostream &os, const std::string &msg){
	os << msg << " error: " << SDL_GetError() << std::endl;
}

SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren){

	SDL_Texture *texture = nullptr;

	SDL_Surface *loadedImage = SDL_LoadBMP(file.c_str());

	if (loadedImage != nullptr){
		texture = SDL_CreateTextureFromSurface(ren, loadedImage);

		SDL_FreeSurface(loadedImage);

		if (texture == nullptr){
			logSDLError(std::cout, "CreateTextureFromSurface");
		}
	}
	else {
		logSDLError(std::cout, "LoadBMP");
	}
	return texture;
}

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y){
	SDL_Rect destination;
	destination.x = x;
	destination.y = y;
	SDL_QueryTexture(tex, NULL, NULL, &destination.w, &destination.h);
	SDL_RenderCopy(ren, tex, NULL, &destination);
}

void renderSprite(SDL_Texture *tex, SDL_Renderer *ren, int srcx, int srcy, int x, int y) {

    SDL_Rect source;
    source.x = srcx;
    source.y = srcy;
    source.h = TILE_SIZE;
    source.w = TILE_SIZE;

	SDL_Rect destination;
	destination.x = x;
	destination.y = y;
    destination.h = TILE_SIZE;
    destination.w = TILE_SIZE;

	SDL_RenderCopy(ren, tex, &source, &destination);
}

//Write out a message to the ingame user in the text output
void write_player_message() {

}

int main(int, char**){
	if (SDL_Init(SDL_INIT_VIDEO) != 0){
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}
    SDL_Window *window = SDL_CreateWindow("Perun", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 840, 840, 0);
    assertptr(window, "SDL_CreateWindow Error: ");

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    assertptr(renderer, "SDL_CreateRenderer Error: ");

    SDL_Texture *background = loadTexture("res/RawTemplate.bmp", renderer);
    SDL_Texture *spritesheet = loadTexture("res/Sprites.bmp");

    //Main game loop
    for (int i = 0; i < 3; ++i){
    	//First clear the renderer
    	SDL_RenderClear(renderer);
    	//Draw the texture
    	renderTexture(background, renderer, 0, 0);
        renderSprite(background, renderer, 0, 0, 14, 14);
    	//Update the screen
    	SDL_RenderPresent(renderer);
    	//Take a quick break after all that hard work
    	SDL_Delay(1000);
    }

    cleanup(renderer, window, background);
    SDL_Quit();
	return 0;
}

#include <iostream>
#include <SDL2/SDL.h>
#include <string>
#include <unistd.h>
#include "../include/cleanup.h"
#include "../include/world.h"

using namespace std;

const int SCREEN_WIDTH = 840;
const int SCREEN_HEIGHT = 840;
//                           {x, y}
const int SCREEN_START[2] = {12, 1};
const int SCREEN_END[2] = {58, 34};
const int SCREEN_CENTER[2] = {(SCREEN_END[0] - SCREEN_START[0]) / 2, (SCREEN_END[1] - SCREEN_START[1]) / 2}
const int FOV_RADIUS = 5;

extern const int WORLD_WIDTH;
extern const int WORLD_HEIGHT;


const int TILE_SIZE = 14;

class Player {
public:
    bool hasmoved;
    int xlocation;
    int ylocation;
};

class vector2 {
public:
    double x;
    double y;
}

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
void write_player_message(string message) {

}

//Basic euclidean distance
double distance(int y1, int x1, int y2, int x2) {
    return sqrt(abs(pow((y2 - y1), 2) + pow(x2 - x1, 2)));
}

//Thanks to roguebasin for this simple FOV algorithm
void fov(Player player, world world){
    if (player.hasmoved) {
        for (int i = 0; i < WORLD_HEIGHT; i++) {
            for (int j = 0; j < WORLD_WIDTH; j++) {
                if (distance(i, j, player.ylocation, player.xlocation) <= FOV_RADIUS) {

                }
            }
        }
    }
}

vector2 get_vector(vector2 vect1, vector2 vect2) {
    vector2 delta;
    delta.x = abs(vect2.x - vect1.x);
    delta.y = abs(vect2.y - vect1.y);
    double dist = distance(vect1.y, vect1.x, vect2.y, vect2.x);
    if (dist == 0.0) {
        return vect1;
    } else {
        vector2 direction = delta/distance;
    }
}

int main(int, char**){

    usleep(4000);
	if (SDL_Init(SDL_INIT_VIDEO) != 0){
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}
    SDL_Window *window = SDL_CreateWindow("Perun", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 840, 840, 0);
    assertptr(window, "SDL_CreateWindow Error: ");

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    assertptr(renderer, "SDL_CreateRenderer Error: ");

    SDL_Texture *background = loadTexture("res/RawTemplate.bmp", renderer);
    SDL_Texture *spritesheet = loadTexture("res/Sprites.bmp", renderer);
    SDL_Event event;

    Player player;
    player.xlocation = 14;
    player.ylocation = 14;
    player.hasmoved = false;
    int quit = 0;
    //Main game loop

    cout << "hi\n";
    world gameworld;
    cout << "worked\n";
    gameworld.generate_world();

    for (size_t y = 0; y < WORLD_WIDTH; y++) {
        for (size_t x = 0; x < WORLD_HEIGHT; x++) {
            cout << gameworld.worldmap[x][y];
        }
        cout << "\n";
    }

    while(!quit) {
    	//First clear the renderer
    	SDL_RenderClear(renderer);
    	//Draw the texture
        while (!SDL_PollEvent(&event)) {};
        do {
            //Handle input
            switch(event.type) {
                case SDL_QUIT:
                    quit = 1;
                    break;
                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym) {
                        case SDLK_LEFT:
                        case SDLK_KP_4:
                            player.xlocation--;
                            break;
                        case SDLK_RIGHT:
                        case SDLK_KP_6:
                            player.xlocation++;
                            break;
                        case SDLK_UP:
                        case SDLK_KP_8:
                            player.ylocation--;
                            break;
                        case SDLK_DOWN:
                        case SDLK_KP_2:
                            player.ylocation++;
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
        } while (SDL_PollEvent(&event));
    	renderTexture(background, renderer, 0, 0);
        renderSprite(spritesheet, renderer, (5 * TILE_SIZE), (5 * TILE_SIZE), (player.xlocation * TILE_SIZE), (player.ylocation * TILE_SIZE));
    	//Update the screen
    	SDL_RenderPresent(renderer);
    	//Take a quick break after all that hard work
    }

    cleanup(renderer, window, background);
    SDL_Quit();
	return 0;
}

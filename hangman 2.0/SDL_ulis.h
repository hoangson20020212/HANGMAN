#ifndef SDL_ULIS_H_INCLUDED
#define SDL_ULIS_H_INCLUDED

#include <iostream>
#include <SDL.h>
#include <string>

void logSDLError(std::ostream& os,
const std::string &msg, bool fatal = false);


const int SCREEN_WIDTH = 644;
const int SCREEN_HEIGHT = 800;
const std::string WINDOW_TITLE = "HangMan";
void initSDL(SDL_Window* &window, SDL_Renderer* &renderer);
void quitSDL(SDL_Window* window, SDL_Renderer* renderer);



void waitUntilKeyPressed();
 SDL_Texture* loadTexture(SDL_Renderer* renderer, std::string path );

#endif // SDL_ULIS_H_INCLUDED

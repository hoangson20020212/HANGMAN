#include "SDL_ulis.h"

void logSDLError(std::ostream& os,const std::string &msg, bool fatal)
{
    os << msg << " Error: " << SDL_GetError() << std::endl;
    if (fatal) {
        SDL_Quit();
        exit(1);
    }
}



void initSDL(SDL_Window* &window, SDL_Renderer* &renderer)
{
if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
logSDLError(std::cout, "SDL_Init", true);
window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
//window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP);
if (window == nullptr) logSDLError(std::cout, "CreateWindow", true);
//Khi thông thường chạy với môi trường bình thường ở nhà
renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |
SDL_RENDERER_PRESENTVSYNC);
//Khi chạy ở máy thực hành WinXP ở trường (máy ảo)
//renderer = SDL_CreateSoftwareRenderer(SDL_GetWindowSurface(window));
if (renderer == nullptr) logSDLError(std::cout, "CreateRenderer", true);
SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
}




void quitSDL(SDL_Window* window, SDL_Renderer* renderer)
{
SDL_DestroyRenderer(renderer);
SDL_DestroyWindow(window);
SDL_Quit();
}



void waitUntilKeyPressed()
{
SDL_Event e;
while (true) {
if ( SDL_WaitEvent(&e) != 0 &&
(e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_QUIT) )
return;
SDL_Delay(100);
}
}


SDL_Texture* loadTexture(SDL_Renderer* renderer, std::string path )
{
 SDL_Texture* newTexture = NULL;
 SDL_Surface* loadedSurface = SDL_LoadBMP( path.c_str() );
 if ( loadedSurface == NULL )
std:: cout << "Unable to load image " << path << " SDL_image Error: "
 << SDL_GetError() << std::endl;
 else {
 newTexture = SDL_CreateTextureFromSurface( renderer, loadedSurface );
 if( newTexture == NULL )
 std::cout << "Unable to create texture from " << path << " SDL Error: "
 << SDL_GetError() << std::endl;
 SDL_FreeSurface( loadedSurface );
 }
 return newTexture;
}

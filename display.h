#include <SDL2/SDL.h>

class display {

    public:
    SDL_Window* screen;
    SDL_Renderer* render;
    SDL_Texture* texture;

    void displayInit();
    void displayDraw(uint32_t *disp);
    void displayDestroy();
};

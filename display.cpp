#include "display.h"

void display::displayInit(){

    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
		printf("Error! Unable to initialize the SDL!\n");

	// Creates the screen
	screen = SDL_CreateWindow("CHIP-8 Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1024, 512, SDL_WINDOW_SHOWN);
	if(screen == NULL)
		printf("Error! Unable to create the screen!\n");

	// Creates the render
	render = SDL_CreateRenderer(screen, -1, SDL_RENDERER_ACCELERATED);
	if(render == NULL)
		printf("Error! Unable to create the render!\n");

	SDL_SetRenderDrawColor(render, 0xFF, 0xFF, 0xFF, 0xFF);

	// Creates the Texture
	texture = SDL_CreateTexture(render, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, 64, 32);
	if(texture == NULL)
		printf("Error! Unable to create the texture!\n");

	// Delay to see possible errors
	SDL_Delay(1000);

}

void display::displayDraw(uint32_t *disp){
    // Saves the display pixels on the buffer
	uint32_t pixel;
    uint32_t pixels[32*64];

    for(int i = 0; i < 2048; i++){
        pixel = disp[i];
        pixels[i] = (0x00FFFFFF * pixel) | 0xFF000000;
    }

    // Updates the screen
    SDL_UpdateTexture(texture, NULL, pixels, 64 * sizeof(uint32_t));
    SDL_RenderCopy(render, texture, NULL, NULL);
    SDL_RenderPresent(render);
}

void display::displayDestroy(){
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(screen);
    SDL_Quit();
}

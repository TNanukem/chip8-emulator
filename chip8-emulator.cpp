/* An CHIP-8 emulator built in C++, work in progress
   Tiago Toledo Junior (a.k.a TNanukem) */

#include "chip8.h"

int main (void){

	chip8 Chip8;

	// Intializes the Chip8 system and loads the game into memory
	Chip8.initialize();
	Chip8.loadGame("pong.c8");

	// Graphical System Initialization
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
		printf("Error! Unable to initialize the SDL!\n");

	// Creates the screen
	SDL_Window* screen = SDL_CreateWindow("CHIP-8 Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 640, 0);
	if(screen == NULL)
		printf("Error! Unable to create the screen!\n");

	// Creates the render
	SDL_Renderer* render = SDL_CreateRenderer(screen, -1, 0);
	if(render == NULL)
		printf("Error! Unable to create the render!\n");
		SDL_RenderClear(render);

	// Creates the Texture
	SDL_Texture* texture = SDL_CreateTexture(render, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 640, 640);
	if(texture == NULL)
		printf("Error! Unable to create the texture!\n");

	// Delay to see possible errors
	SDL_Delay(3000);

	while(1){			// Emulation loop

		SDL_Event event;

		// Event loop
		while(SDL_PollEvent(&event)){
			if(event.type == SDL_QUIT)		// Closes the window when the user clicks on the close button
				return 0;
		}

		Chip8.nextCycle();

		if(Chip8.drawFlag){
			Chip8.drawFlag = 0;


		}
		std::this_thread::sleep_for(std::chrono::milliseconds(16));
		SDL_RenderClear(render);
	}

	SDL_DestroyRenderer(render);
	SDL_DestroyWindow(screen);
	SDL_Quit();

	return 0;
}

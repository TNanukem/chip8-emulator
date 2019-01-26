/* An CHIP-8 emulator built in C++, work in progress
   Tiago Toledo Junior (a.k.a TNanukem) */

#include "chip8.h"

int main (void){

	chip8 Chip8;
	uint32_t buffer[2048];
	uint8_t pixel;
	uint8_t slot = 0;

	// Intializes the Chip8 system and loads the game into memory
	Chip8.initialize();
	Chip8.loadGame("roms/PONG");

	// Graphical System Initialization
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
		printf("Error! Unable to initialize the SDL!\n");

	// Creates the screen
	SDL_Window* screen = SDL_CreateWindow("CHIP-8 Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 512, 0);
	if(screen == NULL)
		printf("Error! Unable to create the screen!\n");

	// Creates the render
	SDL_Renderer* render = SDL_CreateRenderer(screen, -1, 0);
	if(render == NULL)
		printf("Error! Unable to create the render!\n");
	SDL_SetRenderDrawColor(render,255,0,0,255);
	SDL_RenderClear(render);

	// Creates the Texture
	SDL_Texture* texture = SDL_CreateTexture(render, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 64, 32);
	if(texture == NULL)
		printf("Error! Unable to create the texture!\n");

	// Delay to see possible errors
	SDL_Delay(1000);

	while(1){			// Emulation loop

		SDL_Event event;

		// Event loop -> This is what controls the keyboard inputs
		while(SDL_PollEvent(&event)){
			if(event.type == SDL_QUIT)		// Closes the window when the user clicks on the close button
				return 0;

			// Structure that pauses the game when the user presses the spacebar
			if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE){
				printf("Game Paused\n");

				std::this_thread::sleep_for(std::chrono::milliseconds(800));
				while(1){
					SDL_PollEvent(&event);
					if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE)
						break;
				}
				printf("Unpausing Game\n");
			}

			// Selects the load/save state slot
			if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_F1){
				if(slot == 3)
					slot = 0;
				else
					slot++;
				printf("Selected slot %d\n", slot);
			}

			// Saves the game on the selected slot
			if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_F2){
				printf("Saving the game on slot %d\n", slot);
				// Chip8.saveGame(slot)
			}

			// Loads the game from the selected slot
			if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_F3){
				printf("Loading the game from slot %d\n", slot);
				// Chip8.loadGame(slot)
			}

			// All the pressed keys options
			if(event.type == SDL_KEYUP){
				if(event.key.keysym.sym == SDLK_1)	Chip8.keyboard[0] = 0;
				if(event.key.keysym.sym == SDLK_2)	Chip8.keyboard[1] = 0;
				if(event.key.keysym.sym == SDLK_3)	Chip8.keyboard[2] = 0;
				if(event.key.keysym.sym == SDLK_4)	Chip8.keyboard[3] = 0;
				if(event.key.keysym.sym == SDLK_q)	Chip8.keyboard[4] = 0;
				if(event.key.keysym.sym == SDLK_w)	Chip8.keyboard[5] = 0;
				if(event.key.keysym.sym == SDLK_e)	Chip8.keyboard[6] = 0;
				if(event.key.keysym.sym == SDLK_r)	Chip8.keyboard[7] = 0;
				if(event.key.keysym.sym == SDLK_a)	Chip8.keyboard[8] = 0;
				if(event.key.keysym.sym == SDLK_s)	Chip8.keyboard[9] = 0;
				if(event.key.keysym.sym == SDLK_d)	Chip8.keyboard[10] = 0;
				if(event.key.keysym.sym == SDLK_f)	Chip8.keyboard[11] = 0;
				if(event.key.keysym.sym == SDLK_z)	Chip8.keyboard[12] = 0;
				if(event.key.keysym.sym == SDLK_x)	Chip8.keyboard[13] = 0;
				if(event.key.keysym.sym == SDLK_c)	Chip8.keyboard[14] = 0;
				if(event.key.keysym.sym == SDLK_v)	Chip8.keyboard[15] = 0;
			}

			// All the release keys options
			if(event.type == SDL_KEYDOWN){
				if(event.key.keysym.sym == SDLK_1)	Chip8.keyboard[0] = 1;
				if(event.key.keysym.sym == SDLK_2)	Chip8.keyboard[1] = 1;
				if(event.key.keysym.sym == SDLK_3)	Chip8.keyboard[2] = 1;
				if(event.key.keysym.sym == SDLK_4)	Chip8.keyboard[3] = 1;
				if(event.key.keysym.sym == SDLK_q)	Chip8.keyboard[4] = 1;
				if(event.key.keysym.sym == SDLK_w)	Chip8.keyboard[5] = 1;
				if(event.key.keysym.sym == SDLK_e)	Chip8.keyboard[6] = 1;
				if(event.key.keysym.sym == SDLK_r)	Chip8.keyboard[7] = 1;
				if(event.key.keysym.sym == SDLK_a)	Chip8.keyboard[8] = 1;
				if(event.key.keysym.sym == SDLK_s)	Chip8.keyboard[9] = 1;
				if(event.key.keysym.sym == SDLK_d)	Chip8.keyboard[10] = 1;
				if(event.key.keysym.sym == SDLK_f)	Chip8.keyboard[11] = 1;
				if(event.key.keysym.sym == SDLK_z)	Chip8.keyboard[12] = 1;
				if(event.key.keysym.sym == SDLK_x)	Chip8.keyboard[13] = 1;
				if(event.key.keysym.sym == SDLK_c)	Chip8.keyboard[14] = 1;
				if(event.key.keysym.sym == SDLK_v)	Chip8.keyboard[15] = 1;
			}
		}

		// Calls the next cycle, fetching, decoding and executing the next instruction in the memory
		Chip8.nextCycle();

		if(Chip8.drawFlag){
			Chip8.drawFlag = 0;

			// Saves the display pixels on the buffer
			for(int i = 0; i < 2048; i++){
				pixel = Chip8.display[i];
				buffer[i] = (0x00FFFFFF * pixel) | 0xFF000000;
			}

			// Updates the screen
			SDL_UpdateTexture(texture, NULL, buffer, 64 * sizeof(int));
			SDL_RenderClear(render);
			SDL_RenderCopy(render, texture, NULL, NULL);
			SDL_RenderPresent(render);
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(25));
		SDL_RenderClear(render);
	}

	// Free every SDL structure allocated
	SDL_DestroyRenderer(render);
	SDL_DestroyWindow(screen);
	SDL_Quit();

	return 0;
}

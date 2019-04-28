#include "emulator.h"

void emulator::saveGame(uint8_t SaveStateSlot, chip8 *Chip8){
	FILE* saveGame;
	char game[] = "PONG";
	char filename[64];

	sprintf(filename, "roms/%s_%d.sav", game, SaveStateSlot);
	saveGame = fopen(filename,"wb");

	fwrite(Chip8, sizeof(chip8), 1, saveGame);
}

chip8 emulator::loadGame(uint8_t SaveStateSlot, chip8 *Chip8){
	FILE* loadGame;
	char game[] = "PONG";
	char filename[64];

	sprintf(filename, "roms/%s_%d.sav", game, SaveStateSlot);
	loadGame = fopen(filename,"rb");

	if(loadGame == NULL){
		return *Chip8;
	}

	chip8 aux;

	fread(&aux, sizeof(chip8), 1, loadGame);
	return aux;
}

void emulator::emulate(){
    chip8 Chip8;
	display Display;

	// Intializes the Chip8 system and loads the game into memory
	Chip8.initialize();
	Chip8.loadGame("roms/TETRIS");

	// Graphical System Initialization
	Display.displayInit();

	while(1){			// Emulation loop

		SDL_Event event;

		// Event loop -> This is what controls the keyboard inputs
		while(SDL_PollEvent(&event)){
			if(event.type == SDL_QUIT)		// Closes the window when the user clicks on the close button
				return;

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

			// Selects the load/save state SaveStateSlot
			if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_F1){
				if(SaveStateSlot == 3)
					SaveStateSlot = 0;
				else
					SaveStateSlot++;
				printf("Selected SaveStateSlot %d\n", SaveStateSlot);
			}

			// Saves the game on the selected SaveStateSlot
			if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_F2){
				printf("Saving the game on SaveStateSlot %d\n", SaveStateSlot);
				saveGame(SaveStateSlot, &Chip8);
			}

			// Loads the game from the selected SaveStateSlot
			if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_F3){
				printf("Loading the game from SaveStateSlot %d\n", SaveStateSlot);
				Chip8 = loadGame(SaveStateSlot, &Chip8);
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

			// SDL_SetRenderDrawColor(render, 0x00, 0x00, 0x00, 0xFF);
			// SDL_RenderClear(render);
			// int changeLine = 0;
			// int changeColumn = 0;
			//
			// for(int i = 0; i < 32*64; i++){
			// 	if((i % 64) == 0){
			// 		changeLine += 9;
			// 		changeColumn = 0;
			// 	}
			// 	if(Chip8.display[i] == 1){
			// 		SDL_Rect fillRect = {changeColumn, changeLine, 10, 10};
			// 		SDL_SetRenderDrawColor(render, 0x00, 0xFF, 0x00, 0xFF);
			// 		SDL_RenderFillRect(render, &fillRect);
			// 	}
			// 	changeColumn += 9;
			// }
			// SDL_RenderPresent(render);

			Chip8.drawFlag = false;
			Display.displayDraw(Chip8.display);
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	// Free every SDL structure allocated
	Display.displayDestroy();
}

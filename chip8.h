#include "masks_chip8.h"
#include <time.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <SDL2/SDL.h>

// CHIP-8 Simulator .h file
// Tiago Toledo Junior

class chip8{

	public:

	unsigned short opcode;			// All opcodes from CHIP-8 are 2 bytes long
	unsigned char memory[4096];		// We have 4KB of Memory
	unsigned char v[16];			// We have 16 8-bits data registers

	unsigned short I;				// Adress register is two-byte long
	unsigned short pc;				// Program counter is two-byte long

	unsigned char display[64 * 32];	// The 64x32 pixels display
	unsigned char delayTimer;
	unsigned char soundTimer;

	unsigned short stack[16];
	unsigned short sp;

	unsigned char keyboard[16];		// The keyboard has 16 keys

	bool	drawFlag;				// Flag to determine if we should draw on the screen or not.

	void nextCycle();
	void initialize();
	bool loadGame(char *game);
};

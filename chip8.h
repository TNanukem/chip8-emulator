#include "masks_chip8.h"
#include <time.h>
#include <iostream>
#include <stdint.h>
#include <chrono>
#include <thread>
#include <SDL2/SDL.h>

// CHIP-8 Simulator .h file
// Tiago Toledo Junior

class chip8{

	public:

	uint16_t opcode;			// All opcodes from CHIP-8 are 2 bytes long
	uint8_t memory[4096];		// We have 4KB of Memory
	uint8_t  v[16];			// We have 16 8-bits data registers

	uint16_t  I;				// Adress register is two-byte long
	uint16_t  pc;				// Program counter is two-byte long

	uint32_t  display[64 * 32];	// The 64x32 pixels display
	uint8_t  delayTimer;
	uint8_t  soundTimer;

	uint16_t stack[16];
	uint16_t sp;

	uint8_t  keyboard[16];		// The keyboard has 16 keys

	bool drawFlag = false;				// Flag to determine if we should draw on the screen or not.

	void nextCycle();
	void initialize();
	bool loadGame(char *game);
};

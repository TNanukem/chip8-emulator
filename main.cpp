/* An CHIP-8 emulator built in C++, work in progress
   Tiago Toledo Junior (a.k.a TNanukem) */

#include "emulator.h"

int main (int argc, char** argv){

	emulator Chip8Emulator;

	Chip8Emulator.emulate(argc, argv);

	return 0;
}

/* An CHIP-8 emulator built in C++, work in progress
   Tiago Toledo Junior (a.k.a TNanukem) */

#include "chip8.h"

int main (void){
	
	chip8 Chip8;

	while(1){			// Emulation loop

		Chip8.nextCycle();
	}

	return 0;
}
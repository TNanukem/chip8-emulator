#include "chip8.h"
// Chip-8 Class Methods Definition

void chip8::initialize(){

	// Clears the memory of the system
	for(int i = 0; i < 4096; i++){
		this->memory[i] = 0;
	}

	// Clears all registers and the stack
	for(int i = 0; i < 16; i++){
		this->v[i] = 0;
		this->stack[i] = 0;
	}

	this->opcode = 0;
	this->pc = 0x200;
	this->sp = 0;
	this->I = 0;

	this->soundTimer = 0;
	this->delayTimer = 0;
}

void chip8::nextCycle(){

	// Fetching the instruction
	this->opcode = this->memory[this->pc] << 8 | this->memory[this->pc+1];

	// Decoding the instruction
	switch(opcode & GET_4BITS){					// Decode the instruction based on the first 4 bits

		// Decodification of the 0x0 opcodes
		case 0x0000:
			switch(opcode & GET_4BIT_CONSTANT){

				case 0x0000:

					break;

				case 0x000E:

					break;
			}

		// Jumps to the address NNN
		case 0x1000:
			this->pc = (opcode & GET_ADDRESS);
			break;

		// Skips the next instruction if v[X] is equal to NN
		case 0x3000:
			if(v[(opcode & GET_X) >> 8] == (opcode & GET_8BIT_CONSTANT)){
					this->pc += 4;
			}
			else this->pc += 2;
			break;

		// Skips the next instruction if v[X] is not equal to NN
		case 0x4000:
			if(v[(opcode & GET_X) >> 8] != (opcode & GET_8BIT_CONSTANT)){
					this->pc += 4;
			}
			else this->pc += 2;
			break;

		// Skips the next instruction if v[X] is equal to v[Y]
		case 0x5000:
			if(v[(opcode & GET_X) >> 8] == v[(opcode & GET_Y) >> 4]){
				this->pc += 4;
			}
			else
				this->pc += 2;
			break;

		// Sets v[X] to NN
		case 0x6000:
			v[(opcode & GET_X) >> 8] = (opcode & GET_8BIT_CONSTANT);
			this->pc += 2;
			break;

		// Adds NN to v[X]
		case 0x7000:
			v[(opcode & GET_X) >> 8]  += (opcode & GET_8BIT_CONSTANT);
			this->pc += 2;
			break;

		// Skips the next instruction if v[X] is not equal to v[Y]
		case 0x9000:
			if(v[opcode & GET_X] != v[opcode & GET_Y]){
				this->pc += 4;
			}
			else this->pc += 2;
			break;

		case 0x8000:
			switch(opcode & GET_4BIT_CONSTANT){

				// Sets v[X] to v[Y]
				case 0x0000:
					v[opcode & GET_X] = v[opcode & GET_Y];
					break;

				// Sets v[X] to v[X] OR v[Y]
				case 0x0001:
					v[opcode & GET_X] = v[opcode & GET_X] | v[opcode & GET_Y];
					break;

				// Sets v[X] to v[X] AND v[Y]
				case 0x0002:
					v[opcode & GET_X] = v[opcode & GET_X] & v[opcode & GET_Y];
					break;

				// Sets v[X] to v[X] XOR v[Y]
				case 0x0003:

					break;

				case 0x0004:

					break;

				case 0x0005:

					break;

				case 0x0006:

					break;

				case 0x0007:

					break;

				case 0x000E:

					break;
			}

		// Sets the I address to NNN
		case 0xA000:
			this->I = opcode & GET_ADDRESS;
			this->pc += 2;
			break;

		// Jumps to the address V0 + NNN
		case 0xB000:
			this->pc = (opcode & GET_ADDRESS) + this->v[0];
			this->pc += 2;
			break;

		// Sets VX to the result of a bitwise and operation on a random number (Typically: 0 to 255) and NN.
		case 0xC000:
			//v[opcode & GET_X] = rand() & (opcode & GET_8BIT_CONSTANT);
			this->pc += 2;
			break;

		// Draws a sprite at (vx,vy) with size height of N pixels
		case 0xD000:
			//draw(v[opcode & GET_X],v[opcode & GET_Y], (opcode & GET_4BIT_CONSTANT));
			this->pc += 2;
			break;
	}

	// Decrement the timers if they are greater than zero
	if(this->delayTimer > 0)
		this->delayTimer--;

	if(this->soundTimer > 0)
		this->soundTimer--;
}

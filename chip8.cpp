#include "chip8.h"
// Chip-8 Class Methods Definition

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

		case 0x1000:
			this->pc = (opcode & GET_ADDRESS);
			break;

		case 0x3000:
			if(v[opcode & GET_X] == (opcode & GET_8BIT_CONSTANT)){
					this->pc += 4;
			}
			else this->pc += 2;
			break;

		case 0x4000:
			if(v[opcode & GET_X] != (opcode & GET_8BIT_CONSTANT)){
					this->pc += 4;
			}
			else this->pc += 2;
			break;

		case 0x5000:
			if(v[opcode & GET_X] == v[opcode & GET_Y]){
				this->pc += 4;
			}
			else this->pc += 2;
			break;

		case 0x6000:
			v[opcode & GET_X] = (opcode & GET_8BIT_CONSTANT);
			this->pc += 2;
			break;

		case 0x7000:
			v[opcode & GET_X] += (opcode & GET_8BIT_CONSTANT);
			this->pc += 2;
			break;

		case 0x9000:
			if(v[opcode & GET_X] != v[opcode & GET_Y]){
				this->pc += 4;
			}
			else this->pc += 2;
			break;

		case 0xA000:
			this->I = opcode & GET_ADDRESS;
			this->pc += 2;
			break;

		case 0xB000:
			this->pc = (opcode & GET_ADDRESS) + this->v[0];
			this->pc += 2;
			break;

		case 0xC000:
			//v[opcode & GET_X] = rand() & (opcode & GET_8BIT_CONSTANT);
			this->pc += 2;
			break;

		case 0xD000:
			//draw(v[opcode & GET_X],v[opcode & GET_Y], (opcode & GET_4BIT_CONSTANT));
			this->pc += 2;
			break;
	}
}

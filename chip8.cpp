#include "chip8.h"
// Chip-8 Class Methods Definition

bool pressedKey = false;
unsigned short pixel, xaux, yaux;
int height;

unsigned char chip8_fontset[80] = {
  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
  0x20, 0x60, 0x20, 0x20, 0x70, // 1
  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

void chip8::initialize(){

	// Clears the memory of the system
	for(int i = 0; i < 4096; i++){
		memory[i] = 0;
	}

	// Clears all registers and the stack
	for(int i = 0; i < 16; i++){
		v[i] = 0;
		stack[i] = 0;
        keyboard[i] = 0;
	}

	// Clears the display
	for(int i = 0; i < 2048; i++){
		display[i] = 0;
	}

	// Uploads the fontset to the Memory
	for(int i = 0; i < 80; i++){
		memory[i] = chip8_fontset[i];
	}

	opcode = 0;
	pc = 0x200;
	sp = 0;
	I = 0;

	soundTimer = 0;
	delayTimer = 0;

	drawFlag = 1;

}

bool chip8::loadGame(char *game){
    printf("Loading the game....\n");
	FILE *gameP = fopen(game, "rb");
	if(gameP == NULL){
        printf("ERROR! Game file not found!\n");
		return false;
	}

	// Finds the size of the game
	fseek(gameP, 0, SEEK_END);
	long int gameSize = ftell(gameP);
	fseek(gameP, 0, SEEK_SET);

	// Reads the data from the game into the buffer
	char *buffer = (char*) malloc(sizeof(char)*gameSize);
	if(buffer == NULL){
		return false;
	}
	fread(buffer, 1, gameSize, gameP);

	// Writes the data on the memory
	for(int i = 0; i < gameSize; i++){
		memory[i + 512] = buffer[i];
        printf("Memory[%d] = %d\n", i+512,memory[i+512]);
	}

	fclose(gameP);
	free(buffer);
    return true;
}

void chip8::nextCycle(){

	// Fetching the instruction
	opcode = memory[pc] << 8 | memory[pc+1];

	// Decoding the instruction
	switch(opcode & GET_4BITS){					// Decode the instruction based on the first 4 bits

		// Decodification of the 0x0 opcodes
		case 0x0000:
			switch(opcode & GET_8BIT_CONSTANT){

				// Clears the screen.
				case 0x00E0:
					for(int i = 0; i < 32*64; i++){
						display[i] = 0;
					}
					pc += 2;
					drawFlag = 1;
                    printf("CLS\n");
					break;

				// Returns from a subroutine.
				case 0x00EE:
					sp--;
					pc = stack[sp];
                    pc += 2;
                    printf("RET\n");
					break;
			}
			break;

		// Jumps to the address NNN
		case 0x1000:
			pc = (opcode & GET_ADDRESS);
            printf("JMP %d\n", opcode & GET_ADDRESS);
			break;

        // Calls subroutine at NNN.
		case 0x2000:
			stack[sp] = pc;
			sp++;
			pc = opcode & GET_ADDRESS;
            printf("CALL %d\n", opcode & GET_ADDRESS);
			break;

		// Skips the next instruction if v[X] is equal to NN
		case 0x3000:
			if(v[(opcode & GET_X) >> 8] == (opcode & GET_8BIT_CONSTANT))
					pc += 4;
			else
                pc += 2;
            printf("SE V%d,%d\n", (opcode & GET_X) >> 8, opcode & GET_8BIT_CONSTANT);
			break;

		// Skips the next instruction if v[X] is not equal to NN
		case 0x4000:
			if(v[(opcode & GET_X) >> 8] != (opcode & GET_8BIT_CONSTANT))
				pc += 4;
			else
                pc += 2;
            printf("SNE V%d,%d\n", (opcode & GET_X) >> 8, opcode & GET_8BIT_CONSTANT);
			break;

		// Skips the next instruction if v[X] is equal to v[Y]
		case 0x5000:
			if(v[(opcode & GET_X) >> 8] == v[(opcode & GET_Y) >> 4]){
				pc += 4;
			}
			else
				pc += 2;

            printf("SE V%d,V%d\n", (opcode & GET_X) >> 8, (opcode & GET_Y) >> 4);
			break;

		// Sets v[X] to NN
		case 0x6000:
			v[(opcode & GET_X) >> 8] = (opcode & GET_8BIT_CONSTANT);
			pc += 2;
            printf("LD V%d,%d\n", (opcode & GET_X) >> 8, opcode & GET_8BIT_CONSTANT);
			break;

		// Adds NN to v[X]
		case 0x7000:
			v[(opcode & GET_X) >> 8]  += (opcode & GET_8BIT_CONSTANT);
			pc += 2;
            printf("ADD V%d,%d\n", (opcode & GET_X) >> 8, opcode & GET_8BIT_CONSTANT);
			break;

		// Skips the next instruction if v[X] is not equal to v[Y]
		case 0x9000:
			if(v[opcode & GET_X >> 8] != v[(opcode & GET_Y) >> 4]){
				pc += 4;
			}
			else pc += 2;
            printf("SNE V%d,V%d\n", (opcode & GET_X) >> 8, (opcode & GET_Y) >> 4);
			break;

		case 0x8000:
			switch(opcode & GET_4BIT_CONSTANT){

				// Sets v[X] to v[Y]
				case 0x0000:
					v[(opcode & GET_X) >> 8] = v[(opcode & GET_Y) >> 4];
					pc += 2;
                    printf("LD V%d,V%d\n", (opcode & GET_X) >> 8, (opcode & GET_Y) >> 4);
					break;

				// Sets v[X] to v[X] OR v[Y]
				case 0x0001:
					v[(opcode & GET_X) >> 8] |= v[(opcode & GET_Y) >> 4];
					pc += 2;
                    printf("OR V%d,V%d\n", (opcode & GET_X) >> 8, (opcode & GET_Y) >> 4);
					break;

				// Sets v[X] to v[X] AND v[Y]
				case 0x0002:
					v[(opcode & GET_X) >> 8] &= v[(opcode & GET_Y) >> 4];
					pc += 2;
                    printf("AND V%d,V%d\n", (opcode & GET_X) >> 8, (opcode & GET_Y) >> 4);
					break;

				// Sets v[X] to v[X] XOR v[Y]
				case 0x0003:
					v[(opcode & GET_X) >> 8] ^= v[(opcode & GET_Y) >> 4];
					pc += 2;
                    printf("XOR V%d,V%d\n", (opcode & GET_X) >> 8, (opcode & GET_Y) >> 4);
					break;

				// Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there isn't.
				case 0x0004:
					if(v[(opcode & GET_Y) >> 4] > (0xFF - v[(opcode & GET_X) >> 8]))
						v[0xF] = 1;
					else
						v[0xF] = 0;
					v[(opcode & GET_X) >> 8] += v[(opcode & GET_Y) >> 4];
					pc += 2;
                    printf("ADD V%d,V%d\n", (opcode & GET_X) >> 8, (opcode & GET_Y) >> 4);
					break;

				// VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
				case 0x0005:
					if(v[(opcode & GET_X) >> 8] > v[(opcode & GET_Y) >> 4])
						v[0xF] = 1;
					else
						v[0xF] = 0;
					v[(opcode & GET_X) >> 8] -= v[(opcode & GET_Y) >> 4];
					pc += 2;
                    printf("SUB V%d,V%d\n", (opcode & GET_X) >> 8, (opcode & GET_Y) >> 4);
					break;

				// Stores the least significant bit of VX in VF and then shifts VX to the right by 1.
				case 0x0006:
					if((v[(opcode & GET_X) >> 8] & 0x01) == 0x01)
                        v[0xF] = 1;
                    else
                        v[0xF] = 0;
                    pc += 2;
                    v[(opcode & GET_X) >> 8] = v[(opcode & GET_X) >> 8]/2;
                    printf("SHR V%d,V%d\n", (opcode & GET_X) >> 8, (opcode & GET_Y) >> 4);
					break;

				// Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
				case 0x0007:
					if(v[(opcode & GET_Y) >> 4] > v[(opcode & GET_X) >> 8])
						v[0xF] = 1;
					else
						v[0xF] = 0;
					v[(opcode & GET_X) >> 8] = v[(opcode & GET_Y) >> 4] - v[(opcode & GET_X) >> 8];
					pc += 2;
                    printf("SUBN V%d,V%d\n", (opcode & GET_X) >> 8, (opcode & GET_Y) >> 4);
					break;

				// Stores the most significant bit of VX in VF and then shifts VX to the left by 1.
				case 0x000E:
                    if((v[(opcode & GET_X) >> 8] & 0x80) == 0x80)
                        v[0xF] = 1;
                    else
                        v[0xF] = 0;
					pc += 2;
                    v[(opcode & GET_X) >> 8] = v[(opcode & GET_X) >> 8]*2;
                    printf("SHL V%d,V%d\n", (opcode & GET_X) >> 8, (opcode & GET_Y) >> 4);
					break;
			}
			break;

		// Sets the I address to NNN
		case 0xA000:
			I = opcode & GET_ADDRESS;
			pc += 2;
            printf("LD I, %d\n", opcode & GET_ADDRESS);
			break;

		// Jumps to the address V0 + NNN
		case 0xB000:
			pc = (opcode & GET_ADDRESS) + v[0];
            printf("JP V0,%d\n", opcode & GET_ADDRESS);
			break;

		// Sets VX to the result of a bitwise and operation on a random number (Typically: 0 to 255) and NN.
		case 0xC000:
			v[opcode & GET_X >> 8] = (rand() % 255) & (opcode & GET_8BIT_CONSTANT);
			pc += 2;
            printf("RND V%d,%d\n", (opcode & GET_X) >> 8, opcode & GET_8BIT_CONSTANT);
			break;

		// Draws a sprite at (vx,vy) with size height of N pixels
		case 0xD000:
            xaux = v[(opcode & GET_X) >> 8];
            yaux = v[(opcode & GET_Y) >> 4];
            height = opcode & GET_4BIT_CONSTANT;

            v[0xF] = 0;         // Resets the VF

            for(int y = 0; y < height; y++){              // Passes through all lines of the sprite
                pixel = memory[I + y];      // Fetches the pixel value on the memory
                for(int x = 0; x < 8; x++){             // Passes through all 8 columns of the sprite, in each line
                    if((pixel & (0x80 >> x)) != 0){
                        if(display[(xaux + x + ((yaux + y)*64))] == 1)
                            v[0xF] = 1;                 // Registers the collision
                        display[(xaux + x + ((yaux + y)*64))] ^= 1;
                    }
                }
            }

            drawFlag = true;
			pc += 2;
            printf("DRW V%d,V%d,%d\n", (opcode & GET_X) >> 8, (opcode & GET_Y) >> 4, opcode & GET_4BIT_CONSTANT);
			break;

		case 0xE000:
			switch(opcode & GET_4BIT_CONSTANT){

				// Skips the next instruction if the key stored in VX is pressed.
				case 0x000E:
                    printf("SKP V%d\n", (opcode & GET_X) >> 8);
					if(keyboard[v[(opcode & GET_X) >> 8]] != 0)
						pc += 4;
					else
						pc += 2;
					break;

				// Skips the next instruction if the key stored in VX isn't pressed.
				case 0x0001:
                    printf("SKNP V%d\n", (opcode & GET_X) >> 8);
					if(keyboard[v[(opcode & GET_X) >> 8]] == 0)
						pc += 4;
					else
						pc += 2;
					break;
			}

			break;

		case 0xF000:
            switch(opcode & GET_8BIT_CONSTANT){
				// Sets VX to the value of the delay timer.
				case 0x0007:
					v[(opcode & GET_X) >> 8] = delayTimer;
					pc += 2;
                    printf("LD V%d, DT\n", (opcode & GET_X) >> 8);
					break;

                // A key press is awaited, and then stored in VX
				case 0x000A:
                    printf("KEYWAIT V%d, K\n", (opcode & GET_X) >> 8);
					pressedKey = false;
                    while(!pressedKey){
    					for(int i = 0; i < 16; i++){
    						if(keyboard[i] != 0){
    							v[(opcode & GET_X) >> 8] = i;
    							pressedKey = true;
                                break;
    						}
                        }
                    }
						pc += 2;
                        break;

				// Sets the delay timer to the value of v[X]
				case 0x0015:
                    printf("LD DT, V%d\n", (opcode & GET_X) >> 8);
					delayTimer = v[(opcode & GET_X) >> 8];
					pc += 2;
					break;

				// Sets the sound timer to the value of v[X]
				case 0x0018:
                    printf("LD ST, V%d\n", (opcode & GET_X) >> 8);
					soundTimer = v[(opcode & GET_X) >> 8];
					pc += 2;
					break;

				// Adds v[X] to I
				case 0x001E:
                    printf("ADD I, V%d\n", (opcode & GET_X) >> 8);
                    if((I + v[(opcode & GET_X) >> 8]) > 0xFFFF)
                        v[0xF] = 1;
                    else
                        v[0xF] = 0;

                    I += v[(opcode & GET_X) >> 8];
					pc += 2;
					break;

				// Sets I to the location of the sprite for the character in VX.
				case 0x0029:
                    printf("LD F, V%d\n", (opcode & GET_X) >> 8);
					I = v[(opcode & GET_X) >> 8] * 0x5;
					pc += 2;
					break;

				// Stores the Binary-coded decimal representation of VX at the addresses I, I plus 1, and I plus 2
				// Implementation by TJA
				case 0x0033:
                    printf("LD B, V%d\n", (opcode & GET_X) >> 8);
					memory[I]     = v[(opcode & 0x0F00) >> 8] / 100;
					memory[I + 1] = (v[(opcode & 0x0F00) >> 8] / 10) % 10;
					memory[I + 2] = (v[(opcode & 0x0F00) >> 8] % 100) % 10;
					pc += 2;
					break;

				//Stores V0 to VX (including VX) in memory starting at address I.
				case 0x0055:
                    printf("LD [I], V%d\n", (opcode & GET_X) >> 8);
					for(int i = 0; i <= ((opcode & GET_X) >> 8); i++){
						memory[I+i] = v[i];
					}
                    I += ((opcode & GET_X) >> 8) + 1;
					pc += 2;
					break;

				// Fills V0 to VX (including VX) with values from memory starting at address I.
				case 0x0065:
                    printf("I = %d\n", I);
                    printf("LD V%d, [I]\n", (opcode & GET_X) >> 8);
					for(int i = 0; i <= ((opcode & GET_X) >> 8) ; i++){
                        v[i] = memory[I+i];
					}
                    I += ((opcode & GET_X) >> 8) + 1;
					pc += 2;
					break;
			}
        break;

        default:
            printf("ERROR! Opcode not known!\n");
            break;
	}

	// Decrement the timers if they are greater than zero
	if(delayTimer > 0)
		delayTimer--;

	if(soundTimer > 0)
		soundTimer--;
}

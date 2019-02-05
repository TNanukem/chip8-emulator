# chip8-emulator
In my journey to learn more about emulation, this project showed itself as the "Hello World" of the topic. This is an CHIP-8 emulator built in C++.

## About the Chip8
Chip8 was basically a virtual machine developed in the 70s that allowed games to be written in its language and then easily interpreted. Several classical games can be played on a Chip8 machine, like Pong and Space Invaders.

## Technical Specifications
### Memory and registers
Chip8 had 4kB memory, however, the interpreter occuppied the first 512 bytes, so all programs written for the system started on the 512th memory position.

It also features 16 1-Byte registers, named V0 to VF and a 2-Byte special register named I, which is an address register.

### Input
The Chip8 features a hex keyboard as shown below.

|1|2|3|C|   
|4|5|6|D|   
|7|8|9|E|  
|A|0|B|F|  

## The Emulator
This emulator is a work in progress. Below you can see the next steps into the development.

### Next Steps
1. Debug and complete the screen graphics.
2. Save/load state system.
3. Create an UI for ROM selection and other options.
4. Sound system.
### Completed
1. Basic hardware structure.
2. Emulation lopp.
3. Opcodes.
4. Pause Game System.
5. Input handling.

## Resources
These are the resources i've been using to build this project:
  - http://www.multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/
  - https://en.wikipedia.org/wiki/CHIP-8

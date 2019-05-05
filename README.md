# CHIP8 Emulator

![alt text](https://github.com/TNanukem/chip8-emulator/blob/master/images/space.png)

In my journey to learn more about emulation, this project showed itself as the "Hello World" of the topic. This is an CHIP-8 emulator built in C++.

## About the Chip8
Chip8 was basically a virtual machine developed in the 70s that allowed games to be written in its language and then easily interpreted. Several classical games can be played on a Chip8 machine, like Pong and Space Invaders.

## Technical Specifications
### Memory and registers
Chip8 had 4kB memory, however, the interpreter occuppied the first 512 bytes, so all programs written for the system started on the 512th memory position.

It also features 16 1-Byte registers, named V0 to VF and a 2-Byte special register named I, which is an address register.

### Input
The Chip8 features a hex keyboard as shown below.

|   	|   	|  |  |
|---	|---	|---	|	---	|
|   1	|  2 	| 3 | C |
|   4	|  5 	| 6 | D |
|   7 |  8 	| 9 | E |
|   A	|  0 	| B | F |

## The Emulator
This emulator is a work in progress. Below you can see the next steps into the development.

By 05.05.2019 the first release was made to allow me to focus on other projects. In the future new updates will be made to this project.

### Commands
The following keys have some behavior in the emulator that you can use while playing.

|   Key	| Behavior | 
|---	|---	|	
|   Space	|  Pause Emulator 	|
|   F1	|  Save State 	|   	|
|   F2	|   Change State Slot	|
|   F3	|   Load State	|  

### Running a game
```
./chip8 GAMENAME
```
Examples:
```
./chip8 INVADERS
./chip8 PONG
```

### Next Steps
1. Correct sound system.
2. Create an UI for ROM selection and other options.
3. Change the architecture to be completely object oriented. 
### Completed
1. Basic hardware structure.
2. Emulation loop.
3. Opcodes.
4. Pause Game System.
5. Input handling.
6. Debug and complete the screen graphics.
7. Save/load state system.
8. Create .gitignore and Makefile.
9. Correct ROM file name handling for input and save/load state.
10. FPS Counter.

## About the project
This was my first approach in emulator development and I'm really proud of how it turned out. However, there are sensbile modifcations that can be made in order to improve the project as a whole.

The use of SDL may not have been the best choice, since it doesn't allow, natively, for the development of the UI, so, in order to make it will be necessary to use another library and try to integrate both of them. In my next emulations projects I probably would look foward to using another tool.

Also, the timing of this emulator is poorly implemented. The sound has bugs because the timers doesn't reduce at the expected frequency of 60 Hz, also, the emulator can't maintain a stable frame rate. For my next projects, with more complex systems, this will be a major worry.

Finally, some aspects about the architecture used still worry me. I didn't really used a well object-oriented structure because of my C development backgound. This is another major focus that should be addressed on future projects.

## Gallery
![alt text](https://github.com/TNanukem/chip8-emulator/blob/master/images/IBM.png)
![alt text](https://github.com/TNanukem/chip8-emulator/blob/master/images/pong.png)

## Resources
These are the resources i've been using to build this project:
  - http://www.multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/
  - https://en.wikipedia.org/wiki/CHIP-8
  
## Special Thanks
I wanna thank my good friend Lucas Akira (https://github.com/lucas-akira) for helping me debug the graphics of the emulator.

# Etch-a-Sketch
Etch a Sketch program is a graphical programming project produced in C language, utilizing SDL
libraries to emulate the Etch a Sketch toy by imitating its drawing operations.

To play, we press A and D to move vertically, left and right arrows to move horizontally. Moreover, the program adds new features to the game, for example, adjusting line color, linewidth, drawing speed and choosing background. It also allows user to save and load files for further editing.

The program has simple, straightforward and easy-to-use user interface with a tool bar, a column of trays, and a main drawing board. However, user must use terminal to run the program and type input such as file name. Saving operation can only store files in the same directory as the executable program.



# BUILD INSTRUCTIONS:

C programming project

To compile the program
>> make

Run the program
>> ./EtchaSketch

Minimum requirements: SDL2, SDL2_image, SDL2_ttf libraries installed and a C compiler
Compiled with clang, switch to other compilers (gcc) by opening Makefile -> edit CC

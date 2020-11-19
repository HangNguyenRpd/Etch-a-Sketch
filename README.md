# Etch-a-Sketch
Etch a Sketch program is a graphical programming project produced in C language, utilizing SDL
libraries to emulate the Etch a Sketch toy by imitating its drawing operations.

To play, we press A and D to move vertically, left and right arrows to move horizontally. Moreover, the program adds new features to the game, for example, adjusting line color, linewidth, drawing speed and choosing background. It also allows user to save and load files for further editing.

The program has simple, straightforward and easy-to-use user interface with a tool bar, a column of trays, and a main drawing board. However, user must use terminal to run the program and type input such as file name. Saving operation can only store files in the same directory as the executable program.

A.	User Interface

![alt text](https://github.com/HangNguyenRpd/Etch-a-Sketch/blob/master/Images/Etch-a-sketch-GUI.png)

A navigatable user interface created in Adobe Illustrator with the main drawing board, a tool bar (from left to right: New, Open, Save, Refresh buttons), and the tray column on the left.
The tool bar handles board (New and Refresh buttons), and the saving and loading features (Open and Save buttons).

![alt text](https://github.com/HangNguyenRpd/Etch-a-Sketch/blob/master/Images/Etch-a-sketch_Menu.png)

The trays manipulate drawing process that allow user to pick color, choose line-width, change background and adjust speed.

B. How to play
Please see User Manual: https://github.com/HangNguyenRpd/Etch-a-Sketch/blob/master/User_Manual.pdf

C. Algorithms flow
Please see Report for more details: https://github.com/HangNguyenRpd/Etch-a-Sketch/blob/master/ETCH%20A%20SKETCH%20PROGRAM%20REPORT.pdf

![alt text](https://github.com/HangNguyenRpd/Etch-a-Sketch/blob/master/Images/Etch-a-sketch_Flowchart.png)


# BUILD INSTRUCTIONS:

C programming project

To compile the program
>> make

Run the program
>> ./EtchaSketch

Minimum requirements: SDL2, SDL2_image, SDL2_ttf libraries installed and a C compiler
Compiled with clang, switch to other compilers (gcc) by opening Makefile -> edit CC

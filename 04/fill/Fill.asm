// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Fill.asm

// Runs an infinite loop that listens to the keyboard input.
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel;
// the screen should remain fully black as long as the key is pressed. 
// When no key is pressed, the program clears the screen, i.e. writes
// "white" in every pixel;
// the screen should remain fully clear as long as no key is pressed.

// Put your code here.
@16383
D=A
@start
M=D
@24575
D=A
@end
M=D

(BEGIN)
@24576
D=M
@NOT
D; JEQ
@end
D=M
@start
D=D-M
@IS
D; JNE
@BEGIN
0; JMP

(IS)
@start
M=M+1
@start
A=M
M=-1
@BEGIN
0; JMP

(NOT)
@start
A=M
M=0
@start
M=M-1
D=M
@16383
D=D-A
@BEGIN
D; JGE
@16383
D=A
@start
M=D
@BEGIN
0; JMP


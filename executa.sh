#!/bin/bash
reset
#g++ runMaze.cpp -o mazeExec -lGLU -lGL -lglut
#g++ -lGLU -lGL -lglut runMaze.cpp -o mazeExec
g++ -framework OpenGL -framework GLUT runMaze.cpp -o mazeExec

./mazeExec $1

rm mazeExec


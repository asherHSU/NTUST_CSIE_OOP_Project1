## Demo video is in /Group4_P1 

## 1. Description
This project is a pipe game.
It will generate strating point and end point at upper left corner and lower right corner.
Rotate the pipes and try to let water flow form strating point to end point.

## 2. Installation
Operating System: Windows 10
Visual Studio: Visual Studio 2019

Build this project by cmake
paste the {absolute path of project}\src in "Where is the source code"
example: C:\school\SchoolProgram\C++\Project1\Plumber\src

paste the {absolute path of project}\project in "Where to build the binaries"
example: C:\school\SchoolProgram\C++\Project1\Plumber\project

## 3. How to Play
select pipe 			W, A, S, D
Clockwise rotation by		E
Anticlockwise rotation by	Q
Save			1
load				2
Exit				ESC

## 4. Interface
There're two boards in the console:
- upper board is game board
- lower board is answer board

## 5. Meaning of color:
- Yello: strating point and end point
- Blue: water
- Purple: example answer
- Red Background: selected pipe

## 6. Meaning of Charator:
- A: water in answer board
- O: water in game board
- #: pipe wall
- P: pipe hole

## 7. Pipe type
T pipe	L pipe	+ pipe	line pipe
#P#		#P#		#P#		#P#
PPP		#PP		PPP		#P#
###		###		#P#		#P#

## 8. Game content
Every board is generated randomly So the difficulty of each level is randomized.
This design may allow you to pass easily.
Howevever, enjoy this game.

## 9. Credits
Game developed by 黃庭緯, 許良宏, 蕭瑋智
Sound effects obtained from Youtube
Background music composed by Nintendo Co.

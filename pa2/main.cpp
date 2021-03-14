
// File:        main.cpp
// Author:      cheeren
// Date:        2021-02-20
// Description: Partial test of PA2 functionality
//


#include "treasureMap.h"
#include "decoder.h"
#include "cs221util/RGBAPixel.h"
#include "cs221util/PNG.h"

using namespace cs221util;
using namespace std;

int main()
{

	PNG maze;
	maze.readFromFile("images/maze.png");
	pair<int,int> start(1,1);

    PNG base;
    base.readFromFile("images/sunshine.png");

    treasureMap M(base, maze, start);

    PNG treasure = M.renderMap();
	treasure.writeToFile("images/renderMapOutput.png");
    PNG treasuremaze = M.renderMaze();
	treasuremaze.writeToFile("images/renderMazeOutput.png");

    // decoder dec(treasure,start);
    // PNG soln = dec.renderSolution();
    // PNG solnmaze = dec.renderMaze();

    // soln.writeToFile("images/zdessoln.png");
    // solnmaze.writeToFile("images/zdessolnmaze.png");

  return 0;
}

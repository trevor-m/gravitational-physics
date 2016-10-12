#pragma

#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include "Timer.h"
#include "Object.h"
#include <math.h>
#include <stdexcept>
#include <string>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <sstream>
using std::vector;

#define BASE_FPS 60 //all speeds, constants, etc are based on this

#define STATE_GAME 0
#define STATE_PAUSED 1

#define GRID_WIDTH 64

#define CAP_LINES false
#define MAX_LINES 20000

struct Line
{
	double x1, x2, y1, y2;
};

class Game
{
private:
	//engine stuff
	SDL_Window* window;
	SDL_Renderer* renderer;
	int SCREEN_WIDTH = 1220; 
	int SCREEN_HEIGHT = 800;
	int CAMERA_SPEED = 5;
	bool FULLSCREEN = false;
	bool VSYNC = true;
	//textures
	void drawObject(Object* o);

	//game data
	bool follow;
	bool grid;
	bool trace;
	double dt;
	double fps;
	double cameraX;
	double cameraY;
	vector<Object*> objects;
	vector<Line> lines;

	//engine stuff
	bool running;
	int state;
	void init();
	void deinit();
	void update();
	void updatePaused();
	void draw();
public:
	Game();
	~Game();
	void Start(); //start the game
};


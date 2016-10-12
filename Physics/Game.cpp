#include "Game.h"


Game::Game()
{
	running = false;

	if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
	{
		throw std::runtime_error(SDL_GetError());
	}

	//create window
	window = SDL_CreateWindow("phi6", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, (FULLSCREEN ? SDL_WINDOW_FULLSCREEN : 0) | SDL_WINDOW_SHOWN);
	if (window == nullptr)
	{
		throw std::runtime_error(SDL_GetError());
	}

	//create renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | ((VSYNC) ? SDL_RENDERER_PRESENTVSYNC : 0));
	if (renderer == nullptr)
	{
		throw std::runtime_error(SDL_GetError());
	}

	srand(time(0));

	follow = false;
	//SDL_SetRelativeMouseMode(SDL_TRUE);
}

void Game::init()
{
	cameraX = cameraY = 0;
	int start = -5000;
	int length = 10000;

	/*for (int i = 0; i < 50; i++)
	{
		objects.push_back(new Object(start + rand() % length, start + rand() % length, rand() % 30 + 5, (rand() % 100) / 100.0 * 35 + 5));
	}
	for (int i = 0; i < 5; i++)
	{
		objects.push_back(new Object(start + rand() % length, start + rand() % length, rand() % 50 + 50, (rand() % 100) / 100.0 * 35 + 5));
	}*/
	double m1 = 500000;
	double m2 = 500000;

	double r1 = 300;
	double r2 = m1 * r1 / m2;

//	objects.push_back(new Object(r1 + r2, 600, m1));
	//objects.push_back(new Object(0, 600, m2));

	

	// ( G*M/(r1 +r2)^2 * r1)^(1/2)
	//objects[0]->vy = -sqrt(Object::GRAV_CONST *  objects[1]->GetMass() / pow(r1 + r2, 2) * r1);
	//objects[1]->vy = sqrt(Object::GRAV_CONST * objects[0]->GetMass() / pow(r1 + r2, 2) * r2);
	
	double c = 10;//4494639550224887556.2218890554723 * Object::GRAV_CONST;
	double rBlackhole = 50;
	double massBlackhole = c* c * rBlackhole / (2 * Object::GRAV_CONST);
	double density = massBlackhole / (4 / 3 * M_PI * pow(rBlackhole, 3));
	objects.push_back(new Object(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, rBlackhole, density));

	objects.push_back(new Object(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 100, 500));
	objects.push_back(new Object(SCREEN_WIDTH / 2 + 400, SCREEN_HEIGHT / 2, 30, 80));
	objects.push_back(new Object(SCREEN_WIDTH / 2 + 900, SCREEN_HEIGHT / 2, 40, 300));
	objects.push_back(new Object(SCREEN_WIDTH / 2 + 840, SCREEN_HEIGHT / 2, 8, 9));
	objects.push_back(new Object(SCREEN_WIDTH / 2 + 670, SCREEN_HEIGHT / 2, 7, 6));
	objects.push_back(new Object(SCREEN_WIDTH / 2 + 810, SCREEN_HEIGHT / 2, 3, 7));
	objects.push_back(new Object(SCREEN_WIDTH / 2 + 160, SCREEN_HEIGHT / 2, 25, 150));
	objects.push_back(new Object(SCREEN_WIDTH / 2 + 730, SCREEN_HEIGHT / 2, 12, 13));
	objects.push_back(new Object(SCREEN_WIDTH / 2 + 2500, SCREEN_HEIGHT / 2, 80, 100));
	objects.push_back(new Object(SCREEN_WIDTH / 2 + 2100, SCREEN_HEIGHT / 2, 30, 60));
	objects.push_back(new Object(SCREEN_WIDTH / 2 + 2150, SCREEN_HEIGHT / 2, 10, 5));
	//objects[0]->vy = -0.01;
	objects[1]->Orbit(objects[0], -1);
	objects[2]->Orbit(objects[0], 1);
	//objects[3]->vy = objects[2]->vy + -sqrt((Object::GRAV_CONST * objects[2]->GetMass()) / sqrt(objects[2]->DistanceSquared(objects[3])));
	objects[3]->Orbit(objects[2], 1);
	objects[5]->Orbit(objects[2], 1);
	objects[4]->Orbit(objects[0], -1);
	objects[6]->Orbit(objects[0], -1);
	objects[7]->Orbit(objects[2], 1);
	objects[8]->Orbit(objects[0], 1);
	objects[9]->Orbit(objects[8], 1);
	objects[10]->Orbit(objects[9], 1);
}

void Game::deinit()
{
}

Game::~Game()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	renderer = NULL;
	window = NULL;
	SDL_Quit();
}


void Game::drawObject(Object* o)
{
	Uint32 color = 0xFF000000;
	if (o->density * 20 <= 0xFFFFFFFF)
		color = 0xFFFFFFFF - o->density * 20;
	/*Uint8 r, g, b, a;
	SDL_GetRGBA(color, SDL_AllocFormat(SDL_GetWindowPixelFormat(window)), &r, &g, &b, &a);
	if (r == b == g == 0)
		r = 255;
	SDL_MapRGBA(SDL_AllocFormat(SDL_GetWindowPixelFormat(window)), r, g, b, 0xFF)*/

	filledCircleColor(renderer, o->x-cameraX, o->y-cameraY, o->r, color);
}

void Game::draw()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	
	if (grid)
	{
 		double xOffset = -(int)cameraX % GRID_WIDTH;
		double yOffset = -(int)cameraY % GRID_WIDTH;
		for (int x = 0; x < SCREEN_WIDTH/(double)GRID_WIDTH + 1; x++)
		{
			lineColor(renderer, xOffset + x*GRID_WIDTH, 0, xOffset + x*GRID_WIDTH, SCREEN_HEIGHT, 0xAAAAAAAA);
		}
		for (int y = 0; y < SCREEN_HEIGHT / (double)GRID_WIDTH + 1; y++)
		{
			lineColor(renderer, 0, yOffset + y*GRID_WIDTH, SCREEN_WIDTH, yOffset + y*GRID_WIDTH, 0xAAAAAAAA);
		}
	}
	//lines
	if (trace)
	{
		for (int i = 0; i < lines.size(); i++)
		{
			lineColor(renderer, lines[i].x1 - cameraX, lines[i].y1 - cameraY, lines[i].x2 - cameraX, lines[i].y2 - cameraY, 0xFF0000FF);
		}
	}
	//objects
	for (int i = 0; i < objects.size(); i++)
	{
		drawObject((Object*)objects[i]);
	}

	

	//pause menu
	if (state == STATE_PAUSED)
	{

	}

	SDL_RenderPresent(renderer);
}

void Game::update()
{
	//objects
	for (int i = 0; i < objects.size(); i++)
	{
		double x1 = objects[i]->x;
		double y1 = objects[i]->y;
		objects[i]->Update(objects, dt);

		Line l;
		l.x1 = x1;
		l.x2 = objects[i]->x;
		l.y1 = y1;
		l.y2 = objects[i]->y;
		lines.push_back(l);
		if (CAP_LINES && lines.size() > MAX_LINES)
			lines.erase(lines.begin());
	}

	if (follow)
	{
		//follow heaviest object
		int x = 0;
		for (int i = 0; i < objects.size(); i++)
		{
			if (objects[i]->GetMass() > objects[x]->GetMass())
				x = i;
		}

		cameraX = objects[x]->x - SCREEN_WIDTH / 2;
		cameraY = objects[x]->y - SCREEN_HEIGHT / 2;
	}
}


void Game::Start()
{

	init();
	running = true;
	state = STATE_GAME;
	SDL_Event e;
	int oldTime = SDL_GetTicks();
	int newTime;
	int numFrames = 0;
	Timer second;
	second.Start(1000);
	fps = 60;
	//buffer for title
	char output[20];

	while (running)
	{
		if (second.Done())
		{
			second.Start(1000);
			fps = numFrames;
			numFrames = 0;
			sprintf_s(output, 20, "shooter fps: %d", (int)fps);
			SDL_SetWindowTitle(window, output);
		}
		newTime = SDL_GetTicks();
		dt = (newTime - oldTime) * (1.0 / 1000.0 * BASE_FPS);

		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
				running = false;
			if (e.type == SDL_KEYDOWN)
			{
				//quit
				if (e.key.keysym.sym == SDLK_ESCAPE)
				{
					running = false;
					break;
				}

				if (e.key.keysym.sym == SDLK_SPACE)
				{
					follow = !follow;
				}
				if (e.key.keysym.sym == SDLK_g)
				{
					grid = !grid;
				}
				if (e.key.keysym.sym == SDLK_t)
				{
					trace = !trace;
				}
			}
		}
		const Uint8* keystate = SDL_GetKeyboardState(NULL);
		//player movement
		if (state == STATE_GAME)
		{
			if (keystate[SDL_SCANCODE_D])
			{
				cameraX += CAMERA_SPEED * dt;
			}
			if (keystate[SDL_SCANCODE_A])
			{
				cameraX -= CAMERA_SPEED * dt;
			}
			if (keystate[SDL_SCANCODE_W])
			{
				cameraY -= CAMERA_SPEED * dt;
			}
			if (keystate[SDL_SCANCODE_S])
			{
				cameraY += CAMERA_SPEED * dt;
			}
			if (keystate[SDL_SCANCODE_UP])
			{
				Object::GRAV_CONST += 0.00001/60.0 * dt;
			}
			if (keystate[SDL_SCANCODE_DOWN])
			{
				Object::GRAV_CONST -= 0.00001/60.0 * dt;
			}
		}
	
		//update & redraw
		if (state == STATE_GAME)
			update();
		draw();

		oldTime = newTime;
		numFrames++;
	}

	deinit();
}


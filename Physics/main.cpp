#include "Game.h"


int main(int argc, char** argv)
{
	try
	{
		Game* g = new Game();
		g->Start();
		delete g;
	}
	catch (std::exception& e)
	{
		SDL_ShowSimpleMessageBox(0, "Error!", e.what(), 0);
	}

	return 0;
}
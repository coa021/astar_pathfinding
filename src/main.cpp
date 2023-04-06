//#include <iostream>
//#include <SDL.h>
//#undef main
//
//int main(int argc, char* args[]) {
//	//The window we'll be rendering to
//	SDL_Window* window = NULL;
//
//	//The surface contained by the window
//	SDL_Surface* screenSurface = NULL;
//
//	//Initialize SDL
//	if (SDL_Init(SDL_INIT_VIDEO) < 0)
//	{
//		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
//	}
//	else
//	{
//		//Create window
//		window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
//		if (window == NULL)
//		{
//			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
//		}
//		else
//		{
//			//Get window surface
//			screenSurface = SDL_GetWindowSurface(window);
//
//			//Fill the surface white
//			SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));
//
//			//Update the surface
//			SDL_UpdateWindowSurface(window);
//
//			//Hack to get window to stay up
//			SDL_Event e; bool quit = false; while (quit == false) { while (SDL_PollEvent(&e)) { if (e.type == SDL_QUIT) quit = true; } }
//		}
//	}
//	//Destroy window
//	SDL_DestroyWindow(window);
//
//	//Quit SDL subsystems
//	SDL_Quit();
//
//	return 0;
//}

#include "AStar.h"

int main()
{
	srand(time(0));

	AStar astar;
	astar.initialize();
	astar.solve_astar();
	astar.connect_path();
	astar.print_graph();


	return 0;

}
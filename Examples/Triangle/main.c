#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#include <vulkan/vulkan.h>
#include <OpenVK/OpenVK.h>
#include <Wave/WaveMath.h>

#include "Helper.h"
#include "RendererHelper.h"
#include "Renderer.h"

int32_t main(int32_t argc, char** argv)
{
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS | SDL_INIT_TIMER);
	Window = SDL_CreateWindow("Example", 0, 30, WindowWidth, WindowHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_VULKAN);
	SDL_SetWindowMinimumSize(Window, 800, 540);

	RendererCreate();

	bool Run = true;
	while (Run)
	{
		while (SDL_PollEvent(&Event))
		{
			if (Event.type == SDL_QUIT)
				Run = false;
		}

		RendererRender();
	}

	RendererDestroy();
	SDL_DestroyWindow(Window);
	SDL_Quit();

	return 0;
}
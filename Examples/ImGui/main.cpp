#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#include <vulkan/vulkan.h>
#include <OpenVK/OpenVK.h>
#include <Wave/WaveMath.h>

#include <ImGui/imconfig.h>
#include <ImGui/imgui_tables.cpp>
#include <ImGui/imgui_internal.h>
#include <ImGui/imgui.cpp>
#include <ImGui/imgui_draw.cpp>
#include <ImGui/imgui_widgets.cpp>
#include <ImGui/imgui_demo.cpp>
#include <ImGui/imgui_impl_sdl.cpp>
#include <ImGui/imgui_impl_vulkan_but_better.h>

#include "Helper.h"
#include "Camera.h"
#include "RendererHelper.h"
#include "Renderer.h"

int32_t main(int32_t argc, char** argv)
{
	system("GLSLCompiler.bat");

	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS | SDL_INIT_TIMER);
	Window = SDL_CreateWindow("Example", 0, 30, WindowWidth, WindowHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_VULKAN | SDL_WINDOW_MAXIMIZED);
	SDL_GetWindowSize(Window, (int*)&WindowWidth, (int*)&WindowHeight);
	SDL_SetWindowMinimumSize(Window, 800, 540);

	RendererCreate();

	bool Run = true;
	while (Run)
	{
		while (SDL_PollEvent(&Event))
		{
			if (Event.type == SDL_QUIT)
				Run = false;

			if (Event.wheel.type == SDL_MOUSEWHEEL)
			{
				MouseWheel += Event.wheel.y;
			}

			ImGui_ImplSDL2_ProcessEvent(&Event);
		}

		RendererRender();
		MeasureFPS();
	}

	RendererDestroy();
	SDL_DestroyWindow(Window);
	SDL_Quit();

	return 0;
}
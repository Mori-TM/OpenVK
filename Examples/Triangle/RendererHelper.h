uint32_t SwapChain;
uint32_t GraphicsPipeline;
uint32_t RenderPass;
uint32_t Framebuffer;
uint32_t VertexBuffer;

typedef struct
{
	vec3 Pos;
	vec3 Color;
} Vertex;

Vertex Triangle[] =
{
	{ { -1.0, -1.0, 0.0}, { 1.0, 0.5, 0.2 } },
	{ {  1.0, -1.0, 0.0}, { 0.5, 1.0, 0.2 } },
	{ {  0.0,  1.0, 0.0}, { 0.5, 0.2, 1.0 } }
};

const char** GetExtensions(uint32_t* ExtensionCount)
{
	SDL_Vulkan_GetInstanceExtensions(Window, ExtensionCount, NULL);
	const char** Extensions = (const char**)malloc(*ExtensionCount * sizeof(char*));
	SDL_Vulkan_GetInstanceExtensions(Window, ExtensionCount, Extensions);
	return Extensions;
}

VkSurfaceKHR GetSurface(VkInstance* Instance)
{
	VkSurfaceKHR Surface;
	SDL_Vulkan_CreateSurface(Window, *Instance, &Surface);
	return Surface;
}

void GetWindowSize(uint32_t* Width, uint32_t* Height)
{
	SDL_GetWindowSize(Window, (int*)Width, (int*)Height);
}
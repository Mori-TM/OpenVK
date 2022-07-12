uint32_t SwapChain;
uint32_t GraphicsPipeline;
uint32_t DepthAttachment;
uint32_t RenderPass;
uint32_t Framebuffer;
uint32_t VertexBuffer;
uint32_t IndexBuffer;

typedef struct
{
	vec3 Pos;
	vec3 Color;
} Vertex;

Vertex CubeVertices[] =
{
	{ { -0.5, -0.5,  0.5 }, { 1.0f, 0.0f, 0.0f } },
	{ {  0.5, -0.5,  0.5 }, { 1.0f, 0.0f, 1.0f } },
	{ {  0.5,  0.5,  0.5 }, { 0.0f, 0.0f, 1.0f } },
	{ { -0.5,  0.5,  0.5 }, { 0.0f, 1.0f, 0.0f } },

	{ { -0.5, -0.5, -0.5 }, { 1.0f, 0.0f, 0.0f } },
	{ {  0.5, -0.5, -0.5 }, { 1.0f, 0.0f, 1.0f } },
	{ {  0.5,  0.5, -0.5 }, { 0.0f, 0.0f, 1.0f } },
	{ { -0.5,  0.5, -0.5 }, { 0.0f, 1.0f, 0.0f } },
};
 
uint32_t CubeIndices[] =
{
	// front
	0, 1, 2,
	2, 3, 0,
	// right
	1, 5, 6,
	6, 2, 1,
	// back
	7, 6, 5,
	5, 4, 7,
	// left
	4, 0, 3,
	3, 7, 4,
	// bottom
	4, 5, 1,
	1, 0, 4,
	// top
	3, 2, 6,
	6, 7, 3
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
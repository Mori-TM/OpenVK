static void (*GetWindowSizeFunc)(uint32_t* Width, uint32_t* Height) = NULL;
uint32_t OpenVkRendererFlags;

//typedef enum { OpenVkFalse = 0, OpenVkTrue = 1, OpenVkBoolMaxBits = 0xffffffffui32 } OpenVkBool;
typedef uint32_t OpenVkBool;
#define OpenVkTrue 1
#define OpenVkFalse 0

#define OPENVK_ERROR 0xffffffffui32

typedef enum
{
	OPENVK_VULKAN = 0x1,
	OPENVK_DIRECTX_12 = 0x2,
	OPENVK_RAYTRACING = 0x4,
	OPENVK_VALIDATION_LAYER = 0x8,
} OpenVkRendererFlagOptions;

typedef enum
{
	OPENVK_PRIMITIVE_TOPOLOGY_POINT = 0x0,
	OPENVK_PRIMITIVE_TOPOLOGY_LINE = 0x1,
	OPENVK_PRIMITIVE_TOPOLOGY_TRIANGLE = 0x2,
} OpenVkPrimitiveTopology;

typedef enum
{
	OPENVK_POLYGON_MODE_FILL = 0x0,
	OPENVK_POLYGON_MODE_LINE = 0x1,
	OPENVK_POLYGON_MODE_POINT = 0x2,
} OpenVkPolygonMode;

typedef enum
{
	OPENVK_CULL_MODE_NONE = 0x0,
	OPENVK_CULL_MODE_FRONT = 0x1,
	OPENVK_CULL_MODE_BACK = 0x2,
	OPENVK_CULL_MODE_FRONT_AND_BACK = 0x3,
} OpenVkCullMode;

typedef enum
{
	OPENVK_FRONT_FACE_COUNTER_CLOCK_WISE = 0x0,
	OPENVK_FRONT_FACE_CLOCK_WISE = 0x1,
} OpenVkFrontFace;

typedef enum
{
	OPENVK_SHADER_TYPE_VERTEX = 0x0,
	OPENVK_SHADER_TYPE_FRAGMENT = 0x1,
	OPENVK_SHADER_TYPE_RAYGEN = 0x2,
	OPENVK_SHADER_TYPE_MISS = 0x3,
	OPENVK_SHADER_TYPE_CLOSEST_HIT = 0x4,
} OpenVkShaderType;

typedef enum
{
	OPENVK_DESCRIPTOR_TYPE_UNIFORM_BUFFER = 0x0,
	OPENVK_DESCRIPTOR_TYPE_DYNAMIC_UNIFORM_BUFFER = 0x1,
	OPENVK_DESCRIPTOR_TYPE_IMAGE_SAMPLER = 0x2,
	OPENVK_DESCRIPTOR_TYPE_STORAGE_IMAGE = 0x3,
	OPENVK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE = 0x4,
} OpenVkDescriptorType;

typedef enum
{
	OPENVK_IMAGE_LAYOUT_COLOR_OUTPUT = 0x0,
	OPENVK_IMAGE_LAYOUT_DEPTH_OUTPUT = 0x1,
	OPENVK_IMAGE_LAYOUT_GENERAL_OUTPUT = 0x2,
} OpenVkImageLayout;

typedef enum
{
	OPENVK_FILTER_NEAREST = 0x0,
	OPENVK_FILTER_LINEAR = 0x1,
} OpenVkFilter;

typedef enum
{
	OPENVK_ADDRESS_MODE_REPEAT = 0x0,
	OPENVK_ADDRESS_MODE_MIRRORED_REPEAT = 0x1,
	OPENVK_ADDRESS_MODE_CLAMP_TO_EDGE = 0x2,
	OPENVK_ADDRESS_MODE_CLAMP_TO_BORDER = 0x3,
	OPENVK_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE = 0x4,
} OpenVkAddressMode;

typedef enum
{
	OPENVK_IMAGE_TYPE_TEXTURE = 0x0,
	OPENVK_IMAGE_TYPE_ATTACHMENT = 0x1,
	OPENVK_IMAGE_TYPE_STORAGE = 0x2,
} OpenVkImageTypes;

typedef enum
{
	OPENVK_PIPELINE_TYPE_GRAPHICS = 0x0,
	OPENVK_PIPELINE_TYPE_RAYTRACING = 0x1,
} OpenVkPipelineTypes;

typedef struct
{
	OpenVkBool	Dynamic;
	uint32_t	DescriptorPool;
} OpenVkDescriptorPool;

typedef struct
{
	uint32_t		PushConstantCount;
	uint32_t*		PushConstantShaderTypes;
	uint32_t*		PushConstantOffsets;
	uint32_t*		PushConstantSizes;
	uint32_t		DescriptorSetLayoutCount;
	uint32_t*		DescriptorSetLayouts;
} OpenVkPipelineLayoutCreateInfo;

typedef struct
{
	const char*		VertexPath;
	const char*		FragmentPath;
	size_t			BindingStride;
	uint32_t		ShaderAttributeFormatCount;
	uint32_t*		ShaderAttributeFormats;
	uint32_t*		ShaderAttributeOffsets;
	uint32_t		PrimitiveTopology;
	uint32_t		x;
	uint32_t		y;
	uint32_t		Width;
	uint32_t		Height;
	OpenVkBool		DepthClamp;
	uint32_t		PolygonMode;
	float			LineWidth;
	uint32_t		CullMode;
	uint32_t		FrontFace;
	uint32_t		MsaaSamples;
	OpenVkBool		AlphaBlending;
	uint32_t		ColorBlendAttachments;
	OpenVkBool		DepthStencil;
	uint32_t		RenderPass;
} OpenVkGraphicsPipelineCreateInfo;

typedef struct
{
	uint32_t	AttachmentCount;
	uint32_t*	Attachments;
	uint32_t	RenderPass;
	uint32_t	Width;
	uint32_t	Height;
} OpenVkFramebufferCreateInfo;

typedef struct
{
	uint32_t 				DescriptorSetLayout;
	OpenVkDescriptorPool 	DescriptorPool;
	uint32_t				DescriptorWriteCount;
	uint32_t*				DescriptorCounts;
	uint32_t*				DescriptorTypes;
	uint32_t*				UniformBuffers;
	size_t*					UniformBufferSizes;
	uint32_t*				Sampler;
	uint32_t*				ImageTypes;
	uint32_t*				Images;
	uint32_t*				ImageLayouts;
	uint32_t*				TopLevelAS;
	uint32_t*				Bindings;
} OpenVkDescriptorSetCreateInfo;

typedef struct
{
	float		ClearColor[4];
	uint32_t	ClearColors;
	OpenVkBool	ClearDepth;
	uint32_t	RenderPass;
	uint32_t	Framebuffer;
	uint32_t	x;
	uint32_t	y;
	uint32_t	Width;
	uint32_t	Height;
} OpenVkBeginRenderPassInfo;

void OpenVkRuntimeInfo(const char* Msg, const char* Val)
{
	printf("\x1B[36m[Renderer Info]\033[0m\t%s%s\n", Msg, Val);
}

OpenVkBool OpenVkRuntimeError(const char* Msg)
{
	printf("\x1B[31m[Renderer Error]\033[0m\t%s\n", Msg);

	return OPENVK_ERROR;
}

void* OpenVkMalloc(size_t Size)
{
	void* Mem = malloc(Size);
	if (Mem == NULL)
		OpenVkRuntimeError("Failed to allcoate Memory!");

	return Mem;
}

void* OpenVkRealloc(void* Data, size_t Size)
{
	void* Mem = realloc(Data, Size);
	if (Mem == NULL)
		OpenVkRuntimeError("Failed to reallcoate Memory!");

	return Mem;
}

void OpenVkFree(void* Data)
{
	if (Data != NULL)
		free(Data);	
	else
		OpenVkRuntimeError("No Memory to free!");
}

char* OpenVkReadFileData(const char* Path, size_t* Size)
{
	char* Buffer = NULL;
	size_t Length = 0;

	FILE* File = fopen(Path, "rb");
	if (!File)
		return NULL;

	fseek(File, 0, SEEK_END);
	Length = ftell(File);
	fseek(File, 0, SEEK_SET);
	Buffer = (char*)OpenVkMalloc(Length + 1);
	fread(Buffer, Length, 1, File);
	Buffer[Length] = '\0';

	fclose(File);

	*Size = Length;
	return Buffer;
}

uint32_t OpenVkAlignedSize(uint32_t Value, uint32_t Alignment)
{
	return (Value + Alignment - 1) & ~(Alignment - 1);
}

void* OpenVkAlignedMalloc(size_t Size, size_t Alignment)
{
	void* Data = NULL;
#if defined(_MSC_VER) || defined(__MINGW32__)
	Data = _aligned_malloc(Size, Alignment);
#else
	int Res = posix_memalign(&Data, Alignment, Size);
	if (Res != 0)
		Data = nullptr;
#endif
	return Data;
}

void OpenVkAlignedFree(void* Data)
{
#if	defined(_MSC_VER) || defined(__MINGW32__)
	_aligned_free(Data);
#else
	free(Data);
#endif
}

/*
Future useful code:
uint32_t VkCreateTranformBuffer(VkTransformMatrixKHR Matrix, uint32_t* AllignedMatrixSize, VkTransformMatrixKHR** AllignedMatrix)
{



	uint64_t AtomSize = VkRenderer.PhysicalDeviceProperties.limits.nonCoherentAtomSize;
	uint32_t AllignedSize = OpenVkAlignedSize(sizeof(VkTransformMatrixKHR), AtomSize);
	VkTransformMatrixKHR* TransformMatrix = (VkTransformMatrixKHR*)OpenVkAlignedMalloc(sizeof(VkTransformMatrixKHR), AllignedSize);

//	uint32_t SizeA = OpenVkAlignedSize(sizeof(VkTransformMatrixKHR), Size);

	memcpy(TransformMatrix, &Matrix, sizeof(VkTransformMatrixKHR));

	VkDynamicBufferInfo TranformBuffer;

	for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	//	if (VkCreateBuffer(AllignedSize, VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_STORAGE_BIT_KHR | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, &TranformBuffer.Buffers[i], &TranformBuffer.BufferMemories[i]) == OPENVK_ERROR)
	//		return OpenVkRuntimeError("Failed to create transform buffer");
		if (VkCreateVkBufferExt(VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			sizeof(VkTransformMatrixKHR), &TransformMatrix, &TranformBuffer.Buffers[i], &TranformBuffer.BufferMemories[i]) == OPENVK_ERROR)
			return OpenVkRuntimeError("Failed to create transform buffer");
	//	if (VkCreateVkBufferExt(VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
	//		VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
	//		AllignedSize, TransformMatrix, &TranformBuffer.Buffers[i], &TranformBuffer.BufferMemories[i]) == OPENVK_ERROR)
	//		return OpenVkRuntimeError("Failed to create transform buffer");

	if (AllignedMatrixSize != NULL)
		*AllignedMatrixSize = AllignedSize;

	if (AllignedMatrix != NULL)
		*AllignedMatrix = TransformMatrix;
	else
	{
		OpenVkAlignedFree(TransformMatrix);
	}

	return CMA_Push(&VkRenderer.DynamicBuffers, &TranformBuffer);
}

void RendererUpdate()
{

	UpdateFpsCamera(&CameraPos, &CameraDir, &CameraUp);

	UniformBufferObject UBO;
	UBO.viewInverse = LookAtMat4(CameraPos, Add3P(&CameraPos, &CameraDir), CameraUp);
	UBO.projInverse = PerspectiveMat4((float)WindowWidth / WindowHeight, ToRadians(70.0), 0.01, 1000.0);
	UBO.viewInverse = InverseMat4(UBO.viewInverse);
	UBO.projInverse = InverseMat4(UBO.projInverse);
	OpenVkUpdateBuffer(sizeof(UniformBufferObject), &UBO, UniformBuffer);

//	VkTransformMatrixKHR Matrix =
//	{
//		1.0, 0.0, 0.0, 2.0 * sin(SDL_GetTicks() * 0.001),
//		0.0, 2.0, 0.0, 0.0,
//		0.0, 0.0, 1.0, 0.0
//	};
	
	VkDynamicBufferInfo* BufferPTR = (VkDynamicBufferInfo*)CMA_GetAt(&VkRenderer.DynamicBuffers, TransformBuffer);

//	uint64_t Size = VkRenderer.PhysicalDeviceProperties.limits.nonCoherentAtomSize;
//	uint32_t SizeA = OpenVkAlignedSize(sizeof(VkTransformMatrixKHR), Size);
//	printf("Size: %d %d\n", AllignedMatrixSize, sizeof(mat4));

	mat4 Matrix =
	{
		1.0, 0.0, 0.0, 2.0 * sin(SDL_GetTicks() * 0.001),
		0.0, 2.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0
	//	1, 0, 0, 0,
	//	0, 1, 0, 0,
	//	0, 0, 1, 0,
	//	0, 0, 0, 0
	};

	void* Data;
	vkMapMemory(VkRenderer.Device, BufferPTR->BufferMemories[VkRenderer.CurrentFrame], 0, AllignedMatrixSize, 0, &Data);
	memcpy(Data, &Matrix, AllignedMatrixSize);


	VkMappedMemoryRange MemoryRange;
	MemoryRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
	MemoryRange.pNext = NULL;
	MemoryRange.memory = BufferPTR->BufferMemories[VkRenderer.CurrentFrame];
	MemoryRange.offset = 0;
	MemoryRange.size = AllignedMatrixSize;
	vkFlushMappedMemoryRanges(VkRenderer.Device, 1, &MemoryRange);

	vkUnmapMemory(VkRenderer.Device, BufferPTR->BufferMemories[VkRenderer.CurrentFrame]);
	
	//	OpenVkUpdateBuffer(sizeof(VkTransformMatrixKHR), &Matrix, TransformBuffer);
}
*/
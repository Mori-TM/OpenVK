void CreateRenderPass()
{
	RenderPass = OpenVkCreateRenderPass(1, false, false, 1, false);
}

uint32_t DescriptorSetLayout;
OpenVkDescriptorPool DescriptorPool;
uint32_t UniformBuffer;
uint32_t DescriptorSet;

typedef struct
{
	vec2 Resolution;
	uint32_t Ticks;
} UniformBufferObject;

void CreateDescriptors()
{
	{
		uint32_t DescriptorTypes[] = { OPENVK_DESCRIPTOR_TYPE_UNIFORM_BUFFER };
		uint32_t ShaderTypes[] = { OPENVK_SHADER_TYPE_FRAGMENT };
		uint32_t DescriptorCounts[] = { 1 };

		DescriptorSetLayout = OpenVkCreateDescriptorSetLayout(0, 1, DescriptorCounts, DescriptorTypes, ShaderTypes);
	}
	
	{
		UniformBuffer = OpenVkCreateUniformBuffer(sizeof(UniformBufferObject));
	}
	
	{
		uint32_t DescriptorTypes[] = { OPENVK_DESCRIPTOR_TYPE_UNIFORM_BUFFER };
		uint32_t DescriptorCounts[] = { 1 };
		DescriptorPool = OpenVkCreateDescriptorPool(1, DescriptorTypes, DescriptorCounts, false);
	}
	
	{
		uint32_t DescriptorCounts[] = { 1 };
		uint32_t DescriptorTypes[] = { OPENVK_DESCRIPTOR_TYPE_UNIFORM_BUFFER };
		uint32_t UniformBuffers[] = { UniformBuffer };
		size_t UniformSizes[] = { sizeof(UniformBufferObject) };
		uint32_t Bindings[] = { 0 };

		OpenVkDescriptorSetCreateInfo DescriptorSetCreateInfo;
		DescriptorSetCreateInfo.DescriptorSetLayout = DescriptorSetLayout;
		DescriptorSetCreateInfo.DescriptorPool = DescriptorPool;
		DescriptorSetCreateInfo.DescriptorWriteCount = 1;
		DescriptorSetCreateInfo.DescriptorCounts = DescriptorCounts;
		DescriptorSetCreateInfo.DescriptorTypes = DescriptorTypes;
		DescriptorSetCreateInfo.UniformBuffers = UniformBuffers;
		DescriptorSetCreateInfo.UniformBufferSizes = UniformSizes;
		DescriptorSetCreateInfo.Bindings = Bindings;

		DescriptorSet = OpenVkCreateDescriptorSet(&DescriptorSetCreateInfo);
	}
	
}

void CreateBuffers()
{
	VertexBuffer = OpenVkCreateVertexBuffer(ARRAY_SIZE(Vertices) * sizeof(Vertex), Vertices);
	IndexBuffer = OpenVkCreateIndexBuffer(ARRAY_SIZE(Indices) * sizeof(uint32_t), Indices);
}

void CreateGraphicsPipeline()
{
	uint32_t ShaderAttributeFormats[] = { 3 };
	uint32_t ShaderAttributeOffsets[] = { 0 };

	OpenVkGraphicsPipelineCreateInfo GraphicsPipelineCreateInfo;
	GraphicsPipelineCreateInfo.VertexPath = "Shader/Vertex.spv";
	GraphicsPipelineCreateInfo.FragmentPath = "Shader/Fragment.spv";
	GraphicsPipelineCreateInfo.BindingStride = sizeof(Vertex);
	GraphicsPipelineCreateInfo.ShaderAttributeFormatCount = 1;
	GraphicsPipelineCreateInfo.ShaderAttributeFormats = ShaderAttributeFormats;
	GraphicsPipelineCreateInfo.ShaderAttributeOffsets = ShaderAttributeOffsets;
	GraphicsPipelineCreateInfo.PrimitiveTopology = OPENVK_PRIMITIVE_TOPOLOGY_TRIANGLE;
	GraphicsPipelineCreateInfo.x = 0;
	GraphicsPipelineCreateInfo.y = 0;
	GraphicsPipelineCreateInfo.Width = WindowWidth;
	GraphicsPipelineCreateInfo.Height = WindowHeight;
	GraphicsPipelineCreateInfo.PolygonMode = OPENVK_POLYGON_MODE_FILL;
	GraphicsPipelineCreateInfo.LineWidth = 0.0;
	GraphicsPipelineCreateInfo.CullMode = OPENVK_CULL_MODE_NONE;
	GraphicsPipelineCreateInfo.FrontFace = OPENVK_FRONT_FACE_COUNTER_CLOCK_WISE;
	GraphicsPipelineCreateInfo.MsaaSamples = 1;
	GraphicsPipelineCreateInfo.AlphaBlending = false;
	GraphicsPipelineCreateInfo.ColorBlendAttachments = 1;
	GraphicsPipelineCreateInfo.PushConstantCount = 0;
	GraphicsPipelineCreateInfo.PushConstantShaderTypes = NULL;
	GraphicsPipelineCreateInfo.PushConstantOffsets = NULL;
	GraphicsPipelineCreateInfo.PushConstantSizes = NULL;
	GraphicsPipelineCreateInfo.DescriptorSetLayoutCount = 1;
	GraphicsPipelineCreateInfo.DescriptorSetLayouts = &DescriptorSetLayout;
	GraphicsPipelineCreateInfo.DepthStencil = false;
	GraphicsPipelineCreateInfo.RenderPass = RenderPass;

	GraphicsPipeline = OpenVkCreateGraphicsPipeline(&GraphicsPipelineCreateInfo);
}

void CreateFramebuffers()
{
	uint32_t Attachments[] = { SwapChain };

	OpenVkFramebufferCreateInfo FramebufferCreateInfo;
	FramebufferCreateInfo.AttachmentCount = ARRAY_SIZE(Attachments);
	FramebufferCreateInfo.Attachments = Attachments;
	FramebufferCreateInfo.RenderPass = RenderPass;
	FramebufferCreateInfo.Width = WindowWidth;
	FramebufferCreateInfo.Height = WindowHeight;
	Framebuffer = OpenVkCreateFramebuffer(&FramebufferCreateInfo);
}

void Draw()
{
	OpenVkBeginRenderPassInfo BeginInfo;
	BeginInfo.ClearColor[0] = 0.2;
	BeginInfo.ClearColor[1] = 0.2;
	BeginInfo.ClearColor[2] = 0.2;
	BeginInfo.ClearColor[3] = 1.0;
	BeginInfo.ClearColors = 1;
	BeginInfo.ClearDepth = false;
	BeginInfo.RenderPass = RenderPass;
	BeginInfo.Framebuffer = Framebuffer;
	BeginInfo.x = 0;
	BeginInfo.y = 0;
	BeginInfo.Width = WindowWidth;
	BeginInfo.Height = WindowHeight;
	OpenVkBeginRenderPass(&BeginInfo);
	{
		OpenVkSetScissor(0, 0, WindowWidth, WindowHeight);
		OpenVkSetViewport(0, 0, WindowWidth, WindowHeight);

		OpenVkBindGraphicsPipeline(GraphicsPipeline);

		OpenVkBindDescriptorSet(GraphicsPipeline, 0, DescriptorSet);
		OpenVkBindIndexBuffer(VertexBuffer, IndexBuffer);
		OpenVkDrawIndices(6);
	}
	OpenVkEndRenderPass();
}

void RendererCreate()
{
	SwapChain = OpenVkCreateRenderer(true, GetExtensions, GetSurface, GetWindowSize);
	CreateBuffers();
	CreateRenderPass();
	CreateDescriptors();
	CreateFramebuffers();
	CreateGraphicsPipeline();
}

void RendererDestroy()
{
	OpenVkDestroyRenderer();
}

void RendererDraw()
{
	OpenVkBegineFrame();
	{
		Draw();
	}
	OpenVkEndFrame();
}

void RendererResize()
{
	OpenVkRecreateSwapChain(&WindowWidth, &WindowHeight);
	CreateRenderPass();
	CreateDescriptors();
	CreateFramebuffers();
}

void RendererUpdate()
{
	UniformBufferObject UBO;
	UBO.Resolution = Vec2(WindowWidth, WindowHeight);
	UBO.Ticks = SDL_GetTicks();
	OpenVkUpdateUniformBuffer(sizeof(UniformBufferObject), &UBO, UniformBuffer);
}

void RendererRender()
{
	OpenVkDrawFrame(RendererDraw, RendererResize, RendererUpdate);
}
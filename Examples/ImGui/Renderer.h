void CreateRenderPass()
{
	RenderPass = OpenVkCreateRenderPass(1, false, false, 1, false);
}

void CreateImGuiDescriptorPool()
{
	uint32_t DescriptorTypes[] = { OPENVK_DESCRIPTOR_TYPE_IMAGE_SAMPLER };
	uint32_t DescriptorCounts[] = { 1 };
	ImGuiDescriptorPool = OpenVkCreateDescriptorPool(1, DescriptorTypes, DescriptorCounts, true);
}

void CreateUniformBuffer()
{
	uint32_t DescriptorTypes[] = { OPENVK_DESCRIPTOR_TYPE_UNIFORM_BUFFER };
	uint32_t ShaderTypes[] = { OPENVK_SHADER_TYPE_FRAGMENT };
	uint32_t DescriptorCounts[] = { 1 };
	DescriptorSetLayout = OpenVkCreateDescriptorSetLayout(0, 1, DescriptorCounts, DescriptorTypes, ShaderTypes);

	UniformBuffer = OpenVkCreateUniformBuffer(sizeof(UniformBufferObject));
}

void CreateDescriptors()
{
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

void InitImGui()
{
	ImGui::CreateContext();
	ImGuiIO* IO = &ImGui::GetIO();
	IO->WantCaptureMouse;
	IO->WantCaptureKeyboard;
	IO->ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	IO->Fonts->AddFontFromFileTTF("Fonts/Roboto-Medium.ttf", 28.0);

	ImGui::StyleColorsDark();
	ImGui_ImplSDL2_InitForVulkan(Window);

	ImGui_ImplVulkan_InitInfo InitInfo;
	InitInfo.DescriptorPool = VkRenderer.DynamicDescriptorPools.DescriptorPools[ImGuiDescriptorPool.DescriptorPool];
	InitInfo.Device = VkRenderer.Device;
	InitInfo.RenderPass = VkRenderer.RenderPasses[RenderPass];
	InitInfo.PhysicalDevice = VkRenderer.PhysicalDevice;
	InitInfo.ImageCount = MAX_FRAMES_IN_FLIGHT;

	ImGui_ImplVulkan_Init(&InitInfo);

	VkCommandBuffer ImCommandBuffer = VkBeginSingleTimeCommands();
	ImGui_ImplVulkan_CreateFontsTexture(ImCommandBuffer);
	VkEndSingleTimeCommandBuffer(ImCommandBuffer);

	vkDeviceWaitIdle(VkRenderer.Device);
	ImGui_ImplVulkan_DestroyFontUploadObjects();
}

void RendererCreate()
{
	SwapChain = OpenVkCreateRenderer(true, GetExtensions, GetSurface, GetWindowSize);
	CreateBuffers();
	CreateRenderPass();
	CreateImGuiDescriptorPool();
	CreateUniformBuffer();
	CreateDescriptors();
	CreateFramebuffers();
	CreateGraphicsPipeline();

	InitImGui();
	InitFpsCamera();
}

void RendererDestroy()
{
	vkDeviceWaitIdle(VkRenderer.Device);
	ImGui_ImplVulkan_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	OpenVkDestroyRenderer();
}

vec3 CameraPos = { 36.0, 19.0, -36.0 };
vec3 CameraFront = { 0.5, 0.0, 0.5 };
vec3 CameraUp = { 0.0, -1.0, 0.0 };
vec4 Background = { 0.5, 0.8, 0.9, 1.0 };

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

		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Transform");
		ImGui::DragFloat3("Position", (float*)&CameraPos, 0.1, -1000.0, 1000.0);
		ImGui::DragFloat3("Direction", (float*)&CameraFront, 0.1, -1000.0, 1000.0);
		ImGui::ColorPicker4("Background", (float*)&Background);
		Normalize3P(&CameraFront);
		ImGui::End();

		ImGui::ShowDemoWindow();
		ImGui::Render();
		ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), VkRenderer.CommandBuffers[VkRenderer.ImageIndex], 0, NULL);
	}
	OpenVkEndRenderPass();
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
	UpdateFpsCamera(&CameraPos, &CameraFront, &CameraUp);

	UniformBufferObject UBO;
	UBO.CameraPos = Vec4(CameraPos.x, CameraPos.y, CameraPos.z, 1.0);
	UBO.CameraLookAt = Add4(Vec4(CameraFront.x, CameraFront.y, CameraFront.z, 1.0), UBO.CameraPos);
	UBO.Background = Background;
	UBO.Resolution = Vec2(WindowWidth, WindowHeight);
	UBO.Ticks = SDL_GetTicks();
	OpenVkUpdateUniformBuffer(sizeof(UniformBufferObject), &UBO, UniformBuffer);
}

void RendererRender()
{
	OpenVkDrawFrame(RendererDraw, RendererResize, RendererUpdate);
}
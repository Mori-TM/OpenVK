void CreateRenderPass()
{
	DepthAttachment = OpenVkCreateDepthImageAttachment(WindowWidth, WindowHeight, 1, false);
	RenderPass = OpenVkCreateRenderPass(1, true, false, 1, false);
}

void CreateBuffers()
{
	VertexBuffer = OpenVkCreateVertexBuffer(ARRAY_SIZE(CubeVertices) * sizeof(Vertex), CubeVertices);
	IndexBuffer = OpenVkCreateIndexBuffer(ARRAY_SIZE(CubeIndices) * sizeof(uint32_t), CubeIndices);
}

void CreateGraphicsPipeline()
{
	uint32_t ShaderAttributeFormats[] = { 3, 3 };
	uint32_t ShaderAttributeOffsets[] = { 0, 12 };

	uint32_t PushConstantShaderTypes[] = { OPENVK_SHADER_TYPE_VERTEX };
	uint32_t PushConstantOffsets[] = { 0 };
	uint32_t PushConstantSizes[] = { sizeof(mat4) };

	OpenVkGraphicsPipelineCreateInfo GraphicsPipelineCreateInfo;
	GraphicsPipelineCreateInfo.VertexPath = "Shader/Vertex.spv";
	GraphicsPipelineCreateInfo.FragmentPath = "Shader/Fragment.spv";
	GraphicsPipelineCreateInfo.BindingStride = sizeof(Vertex);
	GraphicsPipelineCreateInfo.ShaderAttributeFormatCount = 2;
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
	GraphicsPipelineCreateInfo.PushConstantCount = 1;
	GraphicsPipelineCreateInfo.PushConstantShaderTypes = &PushConstantShaderTypes;
	GraphicsPipelineCreateInfo.PushConstantOffsets = &PushConstantOffsets;
	GraphicsPipelineCreateInfo.PushConstantSizes = &PushConstantSizes;
	GraphicsPipelineCreateInfo.DescriptorSetLayoutCount = 0;
	GraphicsPipelineCreateInfo.DescriptorSetLayouts = NULL;
	GraphicsPipelineCreateInfo.DepthStencil = true;
	GraphicsPipelineCreateInfo.RenderPass = RenderPass;

	GraphicsPipeline = OpenVkCreateGraphicsPipeline(&GraphicsPipelineCreateInfo);
}

void CreateFramebuffers()
{
	uint32_t Attachments[] = { SwapChain, DepthAttachment };

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
	BeginInfo.ClearDepth = true;
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

		mat4 PVM;

		mat4 P = PerspectiveMat4((float)WindowWidth / (float)WindowHeight, ToRadians(70.0), 0.01, 1000.0);
		mat4 V = LookAtMat4(Vec3(0, 1, 3), Vec3(0, 0, 0), Vec3(0, 1, 0));
		mat4 M = TranslateMat4(LoadMat4Identity(), Vec3(0, 0, 0));

		PVM = MultiplyMat4P(&P, &V);
		PVM = MultiplyMat4P(&PVM, &M);

		OpenVkPushConstant(GraphicsPipeline, OPENVK_SHADER_TYPE_VERTEX, 0, sizeof(mat4), &PVM);

		OpenVkBindIndexBuffer(VertexBuffer, IndexBuffer);
		OpenVkDrawIndices(ARRAY_SIZE(CubeIndices));
	}
	OpenVkEndRenderPass();
}

void RendererCreate()
{
	SwapChain = OpenVkCreateRenderer(true, GetExtensions, GetSurface, GetWindowSize);
	CreateBuffers();
	CreateRenderPass();
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
	CreateFramebuffers();
}

void RendererUpdate()
{

}

void RendererRender()
{
	OpenVkDrawFrame(RendererDraw, RendererResize, RendererUpdate);
}
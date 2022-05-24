void CreateRenderPass()
{
	RenderPass = OpenVkCreateRenderPass(1, false, false, 1, false);
}

void CreateBuffers()
{
	VertexBuffer = OpenVkCreateVertexBuffer(ARRAY_SIZE(Triangle) * sizeof(Vertex), Triangle);
}

void CreateGraphicsPipeline()
{
	uint32_t ShaderAttributeFormats[] = { 3, 3 };
	uint32_t ShaderAttributeOffsets[] = { 0, 12 };

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
	GraphicsPipelineCreateInfo.PushConstantCount = 0;
	GraphicsPipelineCreateInfo.PushConstantShaderTypes = NULL;
	GraphicsPipelineCreateInfo.PushConstantOffsets = NULL;
	GraphicsPipelineCreateInfo.PushConstantSizes = NULL;
	GraphicsPipelineCreateInfo.DescriptorSetLayoutCount = 0;
	GraphicsPipelineCreateInfo.DescriptorSetLayouts = NULL;
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

		OpenVkBindVertexBuffer(VertexBuffer);
		OpenVkDrawVertices(ARRAY_SIZE(Triangle));
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
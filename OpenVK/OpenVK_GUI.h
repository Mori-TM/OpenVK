#define OPENVK_GUI_MAX_CHAR_COUNT 256
#define OPENVK_GUI_BUFFER_ALLOCATE_BLOCK 65536

uint32_t(*OpenVkGetMousePos)(int32_t* x, int32_t* y) = NULL;

typedef struct
{
	float Pos[2];
	float UV[2];
	float Color[4];
} OpenVkGUIVertex;

typedef struct
{
	uint32_t Size;
	uint32_t AllocatedSize;
	void* Data;
	uint32_t Buffer;
} OpenVkGUIBuffer;

typedef struct
{
	int32_t x;
	int32_t y;
	uint32_t State;
} OpenVkGUIMouse;

typedef struct
{
	uint32_t RenderPass;
	uint32_t Pipeline;
	uint32_t PipelineLayout;
	uint32_t DescriptorSetLayout;
	uint32_t DescriptorPool;
	uint32_t Texture;
	uint32_t TextureSampler;
	uint32_t DescriptorSet;
	uint32_t Width; 
	uint32_t Height;

	uint32_t Size;
	uint32_t AtlasWidth;
	uint32_t AtlasHeight;
	uint32_t OverSample[2];
	uint32_t CharCount;
	stbtt_packedchar Chars[OPENVK_GUI_MAX_CHAR_COUNT];

	uint32_t LastIndex;
	OpenVkGUIBuffer VertexBuffer;
	OpenVkGUIBuffer IndexBuffer;

	float Rounding;
	uint32_t Border;

	OpenVkGUIMouse LastMouse;
	OpenVkGUIMouse LastPressedMouse;
	OpenVkGUIMouse CurMouse;
} OpenVkGUIInfo;

OpenVkGUIInfo OpenVkGUI;

void OpenVkGUIResizeBuffer(OpenVkGUIBuffer* Buffer, OpenVkBool VertexBuffer)
{
	if (Buffer->Size >= Buffer->AllocatedSize)
	{
		while (Buffer->Size >= Buffer->AllocatedSize)
			Buffer->AllocatedSize += OPENVK_GUI_BUFFER_ALLOCATE_BLOCK;

		if (Buffer->Buffer != OPENVK_ERROR)
			OpenVkDestroyDynamicBuffer(Buffer->Buffer);

		if (VertexBuffer)
		{
			Buffer->Buffer = OpenVkCreateDynamicVertexBuffer(Buffer->AllocatedSize * sizeof(OpenVkGUIVertex));
			Buffer->Data = realloc(Buffer->Data, Buffer->AllocatedSize * sizeof(OpenVkGUIVertex));
			OpenVkRuntimeInfo("Vertex Resize", "");
		}
			
		else
		{
			Buffer->Buffer = OpenVkCreateDynamicIndexBuffer(Buffer->AllocatedSize * sizeof(uint32_t));
			Buffer->Data = realloc(Buffer->Data, Buffer->AllocatedSize * sizeof(uint32_t));
			OpenVkRuntimeInfo("Index Resize", "");
		}
			
	}
		


}

typedef struct
{
	OpenVkGUIVertex Vertices[4];
	float Offset[2];
} OpenVkGUIGlyph;

OpenVkGUIGlyph OpenVkGUIMakeGlyph(uint32_t Character, float OffsetX, float OffsetY)
{
	stbtt_aligned_quad Quad;

	stbtt_GetPackedQuad(OpenVkGUI.Chars, OpenVkGUI.AtlasWidth, OpenVkGUI.AtlasHeight, Character, &OffsetX, &OffsetY, &Quad, 1);

	OpenVkGUIGlyph Info;
	Info.Offset[0] = OffsetX;
	Info.Offset[1] = OffsetY;

	Info.Vertices[0].Pos[0] = Quad.x0;
	Info.Vertices[0].Pos[1] = Quad.y1;
	Info.Vertices[0].UV[0] = Quad.s0;
	Info.Vertices[0].UV[1] = Quad.t1;
	Info.Vertices[0].Color[0] = 1.0;
	Info.Vertices[0].Color[1] = 1.0;
	Info.Vertices[0].Color[2] = 1.0;
	Info.Vertices[0].Color[3] = 1.0;
	
	Info.Vertices[1].Pos[0] = Quad.x0;
	Info.Vertices[1].Pos[1] = Quad.y0;
	Info.Vertices[1].UV[0] = Quad.s0;
	Info.Vertices[1].UV[1] = Quad.t0;
	Info.Vertices[1].Color[0] = 1.0;
	Info.Vertices[1].Color[1] = 1.0;
	Info.Vertices[1].Color[2] = 1.0;
	Info.Vertices[1].Color[3] = 1.0;

	Info.Vertices[2].Pos[0] = Quad.x1;
	Info.Vertices[2].Pos[1] = Quad.y0;
	Info.Vertices[2].UV[0] = Quad.s1;
	Info.Vertices[2].UV[1] = Quad.t0;
	Info.Vertices[2].Color[0] = 1.0;
	Info.Vertices[2].Color[1] = 1.0;
	Info.Vertices[2].Color[2] = 1.0;
	Info.Vertices[2].Color[3] = 1.0;
	
	Info.Vertices[3].Pos[0] = Quad.x1;
	Info.Vertices[3].Pos[1] = Quad.y1;
	Info.Vertices[3].UV[0] = Quad.s1;
	Info.Vertices[3].UV[1] = Quad.t1;
	Info.Vertices[3].Color[0] = 1.0;
	Info.Vertices[3].Color[1] = 1.0;
	Info.Vertices[3].Color[2] = 1.0;
	Info.Vertices[3].Color[3] = 1.0;

	return Info;
}

OpenVkGUIVertex Verts[] =
{
	{ {0.5f,  0.5f}, {1.0, 1.0}, {1.0, 0.0, 0.0, 1.0} },
	{ {0.5f, -0.5f}, {1.0, 0.0}, {0.0, 1.0, 0.0, 1.0} },
	{ {-0.5f, -0.5f}, {0.0, 0.0}, {0.0, 0.0, 1.0, 1.0} },
	{ {-0.5f,  0.5f}, {0.0, 1.0}, {1.0, 0.0, 1.0, 1.0} },
};

uint32_t Indi[] =
{
	0, 1, 2,
	0, 2, 3
};

const uint32_t OpenVkGUIFragmentShader[] =
{	
	// 1011.6.0
0x07230203,0x00010000,0x0008000a,0x0000002e,0x00000000,0x00020011,0x00000001,0x0006000b,
0x00000001,0x4c534c47,0x6474732e,0x3035342e,0x00000000,0x0003000e,0x00000000,0x00000001,
0x0007000f,0x00000004,0x00000004,0x6e69616d,0x00000000,0x00000009,0x0000000d,0x00030010,
0x00000004,0x00000007,0x00030003,0x00000002,0x000001c2,0x00040005,0x00000004,0x6e69616d,
0x00000000,0x00050005,0x00000009,0x4374754f,0x726f6c6f,0x00000000,0x00030005,0x0000000b,
0x00000000,0x00040006,0x0000000b,0x00000000,0x00005655,0x00050006,0x0000000b,0x00000001,
0x6f6c6f43,0x00000072,0x00030005,0x0000000d,0x00006e49,0x00040005,0x00000023,0x706d6153,
0x0072656c,0x00040047,0x00000009,0x0000001e,0x00000000,0x00040047,0x0000000d,0x0000001e,
0x00000000,0x00040047,0x00000023,0x00000022,0x00000000,0x00040047,0x00000023,0x00000021,
0x00000000,0x00020013,0x00000002,0x00030021,0x00000003,0x00000002,0x00030016,0x00000006,
0x00000020,0x00040017,0x00000007,0x00000006,0x00000004,0x00040020,0x00000008,0x00000003,
0x00000007,0x0004003b,0x00000008,0x00000009,0x00000003,0x00040017,0x0000000a,0x00000006,
0x00000002,0x0004001e,0x0000000b,0x0000000a,0x00000007,0x00040020,0x0000000c,0x00000001,
0x0000000b,0x0004003b,0x0000000c,0x0000000d,0x00000001,0x00040015,0x0000000e,0x00000020,
0x00000001,0x0004002b,0x0000000e,0x0000000f,0x00000001,0x00040020,0x00000010,0x00000001,
0x00000007,0x0004002b,0x0000000e,0x00000013,0x00000000,0x00040015,0x00000014,0x00000020,
0x00000000,0x0004002b,0x00000014,0x00000015,0x00000000,0x00040020,0x00000016,0x00000001,
0x00000006,0x0004002b,0x00000006,0x00000019,0x447a0000,0x00020014,0x0000001a,0x00040020,
0x0000001c,0x00000007,0x00000006,0x00090019,0x00000020,0x00000006,0x00000001,0x00000000,
0x00000000,0x00000000,0x00000001,0x00000000,0x0003001b,0x00000021,0x00000020,0x00040020,
0x00000022,0x00000000,0x00000021,0x0004003b,0x00000022,0x00000023,0x00000000,0x00040020,
0x00000025,0x00000001,0x0000000a,0x0004002b,0x00000006,0x0000002b,0x3f800000,0x00050036,
0x00000002,0x00000004,0x00000000,0x00000003,0x000200f8,0x00000005,0x0004003b,0x0000001c,
0x0000001d,0x00000007,0x00050041,0x00000010,0x00000011,0x0000000d,0x0000000f,0x0004003d,
0x00000007,0x00000012,0x00000011,0x00060041,0x00000016,0x00000017,0x0000000d,0x00000013,
0x00000015,0x0004003d,0x00000006,0x00000018,0x00000017,0x000500b8,0x0000001a,0x0000001b,
0x00000018,0x00000019,0x000300f7,0x0000001f,0x00000000,0x000400fa,0x0000001b,0x0000001e,
0x0000002a,0x000200f8,0x0000001e,0x0004003d,0x00000021,0x00000024,0x00000023,0x00050041,
0x00000025,0x00000026,0x0000000d,0x00000013,0x0004003d,0x0000000a,0x00000027,0x00000026,
0x00050057,0x00000007,0x00000028,0x00000024,0x00000027,0x00050051,0x00000006,0x00000029,
0x00000028,0x00000000,0x0003003e,0x0000001d,0x00000029,0x000200f9,0x0000001f,0x000200f8,
0x0000002a,0x0003003e,0x0000001d,0x0000002b,0x000200f9,0x0000001f,0x000200f8,0x0000001f,
0x0004003d,0x00000006,0x0000002c,0x0000001d,0x0005008e,0x00000007,0x0000002d,0x00000012,
0x0000002c,0x0003003e,0x00000009,0x0000002d,0x000100fd,0x00010038

};

const uint32_t OpenVkGUIVertexShader[] =
{
	// 1011.6.0
	0x07230203,0x00010000,0x0008000a,0x00000024,0x00000000,0x00020011,0x00000001,0x0006000b,
	0x00000001,0x4c534c47,0x6474732e,0x3035342e,0x00000000,0x0003000e,0x00000000,0x00000001,
	0x000a000f,0x00000000,0x00000004,0x6e69616d,0x00000000,0x0000000a,0x0000000f,0x0000001a,
	0x0000001b,0x00000021,0x00030003,0x00000002,0x000001c2,0x00040005,0x00000004,0x6e69616d,
	0x00000000,0x00060005,0x00000008,0x505f6c67,0x65567265,0x78657472,0x00000000,0x00060006,
	0x00000008,0x00000000,0x505f6c67,0x7469736f,0x006e6f69,0x00030005,0x0000000a,0x00000000,
	0x00040005,0x0000000f,0x6f506e49,0x00000073,0x00030005,0x00000018,0x00000000,0x00040006,
	0x00000018,0x00000000,0x00005655,0x00050006,0x00000018,0x00000001,0x6f6c6f43,0x00000072,
	0x00030005,0x0000001a,0x0074754f,0x00040005,0x0000001b,0x56556e49,0x00000000,0x00040005,
	0x00000021,0x6f436e49,0x00726f6c,0x00050048,0x00000008,0x00000000,0x0000000b,0x00000000,
	0x00030047,0x00000008,0x00000002,0x00040047,0x0000000f,0x0000001e,0x00000000,0x00040047,
	0x0000001a,0x0000001e,0x00000000,0x00040047,0x0000001b,0x0000001e,0x00000001,0x00040047,
	0x00000021,0x0000001e,0x00000002,0x00020013,0x00000002,0x00030021,0x00000003,0x00000002,
	0x00030016,0x00000006,0x00000020,0x00040017,0x00000007,0x00000006,0x00000004,0x0003001e,
	0x00000008,0x00000007,0x00040020,0x00000009,0x00000003,0x00000008,0x0004003b,0x00000009,
	0x0000000a,0x00000003,0x00040015,0x0000000b,0x00000020,0x00000001,0x0004002b,0x0000000b,
	0x0000000c,0x00000000,0x00040017,0x0000000d,0x00000006,0x00000002,0x00040020,0x0000000e,
	0x00000001,0x0000000d,0x0004003b,0x0000000e,0x0000000f,0x00000001,0x0004002b,0x00000006,
	0x00000011,0x00000000,0x0004002b,0x00000006,0x00000012,0x3f800000,0x00040020,0x00000016,
	0x00000003,0x00000007,0x0004001e,0x00000018,0x0000000d,0x00000007,0x00040020,0x00000019,
	0x00000003,0x00000018,0x0004003b,0x00000019,0x0000001a,0x00000003,0x0004003b,0x0000000e,
	0x0000001b,0x00000001,0x00040020,0x0000001d,0x00000003,0x0000000d,0x0004002b,0x0000000b,
	0x0000001f,0x00000001,0x00040020,0x00000020,0x00000001,0x00000007,0x0004003b,0x00000020,
	0x00000021,0x00000001,0x00050036,0x00000002,0x00000004,0x00000000,0x00000003,0x000200f8,
	0x00000005,0x0004003d,0x0000000d,0x00000010,0x0000000f,0x00050051,0x00000006,0x00000013,
	0x00000010,0x00000000,0x00050051,0x00000006,0x00000014,0x00000010,0x00000001,0x00070050,
	0x00000007,0x00000015,0x00000013,0x00000014,0x00000011,0x00000012,0x00050041,0x00000016,
	0x00000017,0x0000000a,0x0000000c,0x0003003e,0x00000017,0x00000015,0x0004003d,0x0000000d,
	0x0000001c,0x0000001b,0x00050041,0x0000001d,0x0000001e,0x0000001a,0x0000000c,0x0003003e,
	0x0000001e,0x0000001c,0x0004003d,0x00000007,0x00000022,0x00000021,0x00050041,0x00000016,
	0x00000023,0x0000001a,0x0000001f,0x0003003e,0x00000023,0x00000022,0x000100fd,0x00010038
};

OpenVkFile FragmentShader;

void OpenVkGUIInit(uint32_t Width, uint32_t Height, uint32_t RenderPass, uint32_t MsaaSamples, const char* Font, uint32_t (*GetMousePos)(int32_t* x, int32_t* y))
{
	OpenVkGetMousePos = GetMousePos;
	FragmentShader = OpenVkReadFile("Data/Shader/SceneFragment.frag");

	OpenVkGUI.RenderPass = RenderPass;
	OpenVkGUI.Width = Width;
	OpenVkGUI.Height = Height;

	OpenVkGUI.Size = 80;
	OpenVkGUI.AtlasWidth = 1024;
	OpenVkGUI.AtlasHeight = 1024;
	OpenVkGUI.OverSample[0] = 2;
	OpenVkGUI.OverSample[1] = 2;
	OpenVkGUI.CharCount = 256;

	OpenVkGUI.Rounding = 50.0;
	OpenVkGUI.Border = 20.0;

	uint32_t DescriptorTypes[] = { OPENVK_DESCRIPTOR_TYPE_IMAGE_SAMPLER };
	uint32_t ShaderTypes[] = { OPENVK_SHADER_TYPE_FRAGMENT };
	uint32_t DescriptorCounts[] = { 1 };
	uint32_t Bindings[] = { 0 };

	OpenVkGUI.DescriptorSetLayout = OpenVkCreateDescriptorSetLayout(1, Bindings, DescriptorCounts, DescriptorTypes, ShaderTypes);
	OpenVkGUI.DescriptorPool = OpenVkCreateDescriptorPool(OPENVK_DESCRIPTOR_POOL_DEFAULT, 1, DescriptorTypes, DescriptorCounts);


		OpenVkFile File = OpenVkReadFile(Font);
	//	stbtt_fontinfo info;
	//	if (!stbtt_InitFont(&info, (unsigned char*)File.Data, 0))
	//	{
	//		printf("failed\n");
	//	}

		/* create a bitmap for the phrase */
		unsigned char* Bitmap = (unsigned char*)calloc(OpenVkGUI.AtlasWidth * OpenVkGUI.AtlasHeight, sizeof(unsigned char));

	//	font_.charInfo = std::make_unique<stbtt_packedchar[]>(font_.charCount);

		stbtt_pack_context context;
		if (!stbtt_PackBegin(&context, Bitmap, OpenVkGUI.AtlasWidth, OpenVkGUI.AtlasHeight, 0, 1, NULL))
			printf("Failed to initialize font");

		stbtt_PackSetOversampling(&context, OpenVkGUI.OverSample[0], OpenVkGUI.OverSample[1]);
		if (!stbtt_PackFontRange(&context, (unsigned char*)File.Data, 0, OpenVkGUI.Size, 0, OPENVK_GUI_MAX_CHAR_COUNT, OpenVkGUI.Chars))
			printf("Failed to pack font");

		stbtt_PackEnd(&context);

	OpenVkGUI.Texture = OpenVkCreateTextureImage(Bitmap, OpenVkGUI.AtlasWidth, OpenVkGUI.AtlasHeight, OPENVK_FORMAT_R);
	OpenVkGUI.TextureSampler = OpenVkCreateImageSampler(OPENVK_FILTER_LINEAR, OPENVK_ADDRESS_MODE_REPEAT);

	uint32_t ImageTypes[] = { OPENVK_IMAGE_TYPE_TEXTURE };
	uint32_t ImageLayouts[] = { OPENVK_IMAGE_LAYOUT_COLOR_OUTPUT };

	OpenVkDescriptorSetCreateInfo DescriptorSetCreateInfo;
	DescriptorSetCreateInfo.DescriptorSetLayout = OpenVkGUI.DescriptorSetLayout;
	DescriptorSetCreateInfo.DescriptorPool = OpenVkGUI.DescriptorPool;
	DescriptorSetCreateInfo.DescriptorWriteCount = 1;
	DescriptorSetCreateInfo.DescriptorCounts = DescriptorCounts;
	DescriptorSetCreateInfo.DescriptorTypes = DescriptorTypes;
	DescriptorSetCreateInfo.Sampler = &OpenVkGUI.TextureSampler;
	DescriptorSetCreateInfo.ImageTypes = ImageTypes;
	DescriptorSetCreateInfo.ImageLayouts = ImageLayouts;
	DescriptorSetCreateInfo.Bindings = Bindings;
	DescriptorSetCreateInfo.Images = &OpenVkGUI.Texture;
	DescriptorSetCreateInfo.DescriptorSet = NULL;
	OpenVkGUI.DescriptorSet = OpenVkCreateDescriptorSet(&DescriptorSetCreateInfo);

	OpenVkPipelineLayoutCreateInfo Layout;
	Layout.PushConstantCount = 0;
	Layout.PushConstantShaderTypes = NULL;
	Layout.PushConstantOffsets = NULL;
	Layout.PushConstantSizes = NULL;
	Layout.DescriptorSetLayoutCount = 1;
	Layout.DescriptorSetLayouts = &OpenVkGUI.DescriptorSetLayout;
	OpenVkGUI.PipelineLayout = OpenVkCreatePipelineLayout(&Layout);

	uint32_t ShaderAttributeFormats[] = { OPENVK_FORMAT_RG32F, OPENVK_FORMAT_RG32F, OPENVK_FORMAT_RGBA32F };
	uint32_t ShaderAttributeOffsets[] = { 0, 8, 16 };

	OpenVkGraphicsPipelineCreateInfo GraphicsPipelineCreateInfo;
//	GraphicsPipelineCreateInfo.VertexShader.Size = sizeof(OpenVkGUIVertexShader);
//	GraphicsPipelineCreateInfo.VertexShader.Data = (char*)OpenVkGUIVertexShader;
//	GraphicsPipelineCreateInfo.VertexShader.Freeable = OpenVkFalse;
//	GraphicsPipelineCreateInfo.FragmentShader.Size = sizeof(OpenVkGUIFragmentShader);
//	GraphicsPipelineCreateInfo.FragmentShader.Data = (char*)OpenVkGUIFragmentShader;
//	GraphicsPipelineCreateInfo.FragmentShader.Freeable = OpenVkFalse;
	GraphicsPipelineCreateInfo.VertexShader = OpenVkReadFile("Data/Shader/GUIVertex.spv");
	GraphicsPipelineCreateInfo.FragmentShader = OpenVkReadFile("Data/Shader/GUIFragment.spv");
	GraphicsPipelineCreateInfo.BindingStride = sizeof(OpenVkGUIVertex);
	GraphicsPipelineCreateInfo.ShaderAttributeFormatCount = 3;
	GraphicsPipelineCreateInfo.ShaderAttributeFormats = ShaderAttributeFormats;
	GraphicsPipelineCreateInfo.ShaderAttributeOffsets = ShaderAttributeOffsets;
	GraphicsPipelineCreateInfo.PrimitiveTopology = OPENVK_PRIMITIVE_TOPOLOGY_TRIANGLE;
	GraphicsPipelineCreateInfo.x = 0;
	GraphicsPipelineCreateInfo.y = 0;
	GraphicsPipelineCreateInfo.Width = Width;
	GraphicsPipelineCreateInfo.Height = Height;
	GraphicsPipelineCreateInfo.DepthClamp = false;
	GraphicsPipelineCreateInfo.PolygonMode = OPENVK_POLYGON_MODE_FILL;
	GraphicsPipelineCreateInfo.LineWidth = 1.0;
	GraphicsPipelineCreateInfo.CullMode = OPENVK_CULL_MODE_NONE;
	GraphicsPipelineCreateInfo.FrontFace = OPENVK_FRONT_FACE_COUNTER_CLOCK_WISE;
	GraphicsPipelineCreateInfo.MsaaSamples = MsaaSamples;
	GraphicsPipelineCreateInfo.AlphaBlending = true;
	GraphicsPipelineCreateInfo.ColorBlendAttachments = 1;
	GraphicsPipelineCreateInfo.PipelineLayout = OpenVkGUI.PipelineLayout;
	GraphicsPipelineCreateInfo.DepthStencil = false;
	GraphicsPipelineCreateInfo.RenderPass = RenderPass;
	OpenVkGUI.Pipeline = OpenVkCreateGraphicsPipeline(&GraphicsPipelineCreateInfo);

	memset(&OpenVkGUI.VertexBuffer, 0, sizeof(OpenVkGUIBuffer));
	memset(&OpenVkGUI.IndexBuffer, 0, sizeof(OpenVkGUIBuffer));
	OpenVkGUI.VertexBuffer.Buffer = OPENVK_ERROR;
	OpenVkGUI.IndexBuffer.Buffer = OPENVK_ERROR;

//	OpenVkGUI.VertexBuffer = OpenVkCreateDynamicVertexBuffer(4 * sizeof(OpenVkGUIVertex));
//	OpenVkGUI.IndexBuffer = OpenVkCreateDynamicIndexBuffer(6 * sizeof(uint32_t));
}

void OpenVkGUIDestroy()
{

}

void OpenVkGUIBeginRender(uint32_t Width, uint32_t Height)
{
	/*
	OpenVkGUIGlyph Glyph;
	uint32_t Index = (float)SDL_GetTicks() * 0.01;
	Index %= 256;
	printf("%d\n", Index);
	Glyph = OpenVkGUIMakeGlyph((uint32_t)254, 0, 0);

	mat4 Proj = OrthoMat4(0.0, Width, Height, 0.0, -1.0, 1.0);
	for (uint32_t i = 0; i < 4; i++)
	{
		vec4 Pos = 
		{
			Glyph.Vertices[i].Pos[0] * 2,
			Glyph.Vertices[i].Pos[1] * 2 + 80,
			0.0, 
			1.0
		};

		vec4 P = MultiplyVec4Mat4P(&Pos, &Proj);
	//	PrintVec4(&P);
		Glyph.Vertices[i].Pos[0] = P.x;
		Glyph.Vertices[i].Pos[1] = P.y;
	}

	OpenVkUpdateDynamicBuffer(4 * sizeof(OpenVkGUIVertex), Glyph.Vertices, OpenVkGUI.VertexBuffer);
	OpenVkUpdateDynamicBuffer(6 * sizeof(uint32_t), Indi, OpenVkGUI.IndexBuffer);
	*/

	OpenVkGUI.LastMouse = OpenVkGUI.CurMouse;
	
	OpenVkGUI.CurMouse.State = OpenVkGetMousePos(&OpenVkGUI.CurMouse.x, &OpenVkGUI.CurMouse.y);
	if (OpenVkGUI.CurMouse.State == 1 && OpenVkGUI.LastMouse.State != 1)
	{
		OpenVkGUI.LastPressedMouse = OpenVkGUI.CurMouse;
	}

	OpenVkGUIResizeBuffer(&OpenVkGUI.VertexBuffer, OpenVkTrue);
	OpenVkGUIResizeBuffer(&OpenVkGUI.IndexBuffer, OpenVkFalse);
	OpenVkGUI.VertexBuffer.Size = 0;
	OpenVkGUI.IndexBuffer.Size = 0;
	OpenVkGUI.LastIndex = 0;

	OpenVkGUI.Width = Width;
	OpenVkGUI.Height = Height;

	OpenVkGUI.Rounding = 120.0 * sinf(SDL_GetTicks() * 0.0025) + 120.0;

	OpenVkSetScissor(0, 0, Width, Height);
	OpenVkSetViewport(0, 0, Width, Height);

	OpenVkBindPipeline(OpenVkGUI.Pipeline, OPENVK_PIPELINE_TYPE_GRAPHICS);
}

void OpenVkGUITransformVertices(float Left, float Right, float Bottom, float Top, float NearZ, float FarZ)
{
	float RLS = Right - Left;
	float TBS = Top - Bottom;

	float M[4] =
	{
		2 / RLS,
		-(Right + Left) / RLS,
		2 / TBS,
		-(Top + Bottom) / TBS
	};

	for (uint32_t j = 0; j < OpenVkGUI.VertexBuffer.Size; j++)
	{
		
		OpenVkGUIVertex* V = (OpenVkGUIVertex*)OpenVkGUI.VertexBuffer.Data;
		V[j].Pos[1] = OpenVkGUI.Height - V[j].Pos[1];
		float Pos[2] =
		{
			V[j].Pos[0],
			V[j].Pos[1]
		};

		float x = Pos[0] * M[0] + M[1];
		float y = Pos[1] * M[2] + M[3];

		V[j].Pos[0] = x;
		V[j].Pos[1] = y;
	}
}

void OpenVkGUIEndRender()
{
	if (OpenVkGUI.IndexBuffer.Size > 0 && OpenVkGUI.VertexBuffer.Size < OpenVkGUI.VertexBuffer.AllocatedSize && OpenVkGUI.IndexBuffer.Size < OpenVkGUI.IndexBuffer.AllocatedSize)
	{
		uint32_t VertexSize = OpenVkGUI.VertexBuffer.Size;		
		uint32_t IndexSize = OpenVkGUI.IndexBuffer.Size;

		OpenVkGUITransformVertices(0.0, OpenVkGUI.Width, OpenVkGUI.Height, 0.0, -1.0, 1.0);

		OpenVkUpdateDynamicBuffer(VertexSize * sizeof(OpenVkGUIVertex), OpenVkGUI.VertexBuffer.Data, OpenVkGUI.VertexBuffer.Buffer);
		OpenVkUpdateDynamicBuffer(IndexSize * sizeof(uint32_t), OpenVkGUI.IndexBuffer.Data, OpenVkGUI.IndexBuffer.Buffer);
	
		OpenVkBindDescriptorSet(OpenVkGUI.PipelineLayout, 0, OpenVkGUI.DescriptorSet, OPENVK_PIPELINE_TYPE_GRAPHICS);
		OpenVkBindDynamicIndexBuffer(OpenVkGUI.VertexBuffer.Buffer, OpenVkGUI.IndexBuffer.Buffer);
		OpenVkDrawIndices(IndexSize);
	}
}

//float OpenVkGUIClamp(float Value, float Min, float Max)
//{
//	if (Value >= Max)
//
//}

void OpenVkGUIAddVertices(uint32_t Count, OpenVkGUIVertex* Vertices)
{
	OpenVkGUIVertex* Vertex = (OpenVkGUIVertex*)OpenVkGUI.VertexBuffer.Data;

	if (OpenVkGUI.VertexBuffer.Size + Count <= OpenVkGUI.VertexBuffer.AllocatedSize)
	{
		Vertex += OpenVkGUI.VertexBuffer.Size;
		for (uint32_t i = 0; i < Count; i++)
		{			
			
			Vertex[i] = Vertices[i];
		//	Vertex[i].Color[0] = sinf(OpenVkGUI.VertexBuffer.Size + i * SDL_GetTicks() * 0.003) + 0.5;
		//	Vertex[i].Color[1] = cosf(OpenVkGUI.VertexBuffer.Size + i * SDL_GetTicks() * 0.001) + 0.5;
		//	Vertex[i].Color[2] = sinf(OpenVkGUI.VertexBuffer.Size + i * SDL_GetTicks() * 0.002) + 0.5;

		//	Normalize3P((vec3*)&Vertex[i].Color);
		}
			
	}

	OpenVkGUI.VertexBuffer.Size += Count;
}

void OpenVkGUIAddIndices(uint32_t Count, uint32_t* Indices)
{
	uint32_t* Index = (uint32_t*)OpenVkGUI.IndexBuffer.Data;

	if (OpenVkGUI.IndexBuffer.Size + Count <= OpenVkGUI.IndexBuffer.AllocatedSize)
	{
		Index += OpenVkGUI.IndexBuffer.Size;
		for (uint32_t i = 0; i < Count; i++)
			Index[i] = Indices[i];
	}

	OpenVkGUI.IndexBuffer.Size += Count;
}

void OpenVkGUITextPos(const char* Title, uint32_t x, uint32_t y, uint32_t MaxWidth)
{
//	y = OpenVkGUI.Height - y;
	y += OpenVkGUI.Size / 1.75;
//	MaxWidth += x;

	float OffsetX = 0;
	float OffsetY = 0;

	uint32_t Length = strlen(Title);
	uint32_t Index = (float)SDL_GetTicks() * 0.02;
	Index %= Length;

	for (uint32_t i = 0; i < Length; i++)
	{
		while (Title[i] == '\r' || Title[i] == '\n')
		{
			OffsetX = 0;
			OffsetY += OpenVkGUI.Size;

			i++;
			if (Title[i] == '\r' && Title[i + 1] == '\n') i++;
			if (Title[i] == '\n' && Title[i - 1] == '\r') i++;
		}
		while (Title[i] == '\t')
		{
			OffsetX += OpenVkGUI.Size * 2;
			i++;
		}

		if (Title[i] != ' ' && MaxWidth > 0 && OffsetX + OpenVkGUI.Size / 1.75 >= MaxWidth)
		{
			OffsetX = 0;
			OffsetY += OpenVkGUI.Size;
		}			

		OpenVkGUIGlyph Glyph = OpenVkGUIMakeGlyph(Title[i], OffsetX, OffsetY);
		OffsetX = Glyph.Offset[0];
		OffsetY = Glyph.Offset[1];

	//	mat4 Proj = OrthoMat4(0.0, OpenVkGUI.Width, OpenVkGUI.Height, 0.0, -1.0, 1.0);
	//	for (uint32_t j = 0; j < 4; j++)
	//	{
	//		vec4 Pos =
	//		{
	//			Glyph.Vertices[j].Pos[0] + x,
	//			Glyph.Vertices[j].Pos[1] + y,
	//			0.0,
	//			1.0
	//		};
	//
	//		vec4 P = MultiplyVec4Mat4P(&Pos, &Proj);
	//
	//		Glyph.Vertices[j].Pos[0] = P.x;
	//		Glyph.Vertices[j].Pos[1] = P.y;
	//	}
		for (uint32_t j = 0; j < 4; j++)
		{
			Glyph.Vertices[j].Pos[0] += x;
			Glyph.Vertices[j].Pos[1] += y;

			Glyph.Vertices[j].Color[0] = sinf(OpenVkGUI.VertexBuffer.Size + (j) * SDL_GetTicks() * 0.003) + 0.5;
			Glyph.Vertices[j].Color[1] = cosf(OpenVkGUI.VertexBuffer.Size + (j) * SDL_GetTicks() * 0.001) + 0.5;
			Glyph.Vertices[j].Color[2] = sinf(OpenVkGUI.VertexBuffer.Size + (j) * SDL_GetTicks() * 0.002) + 0.5;
		}

		OpenVkGUIAddVertices(4, Glyph.Vertices);

		uint32_t Indices[6];
		Indices[0] = OpenVkGUI.LastIndex;
		Indices[1] = OpenVkGUI.LastIndex + 1;
		Indices[2] = OpenVkGUI.LastIndex + 2;
		Indices[3] = OpenVkGUI.LastIndex;
		Indices[4] = OpenVkGUI.LastIndex + 2;
		Indices[5] = OpenVkGUI.LastIndex + 3;
		OpenVkGUIAddIndices(6, Indices);

		OpenVkGUI.LastIndex += 4;
	}
}

void OpenVkGUIQuadPos(uint32_t x, uint32_t y, uint32_t w, uint32_t h)
{
//	y = OpenVkGUI.Height - y - h;
	OpenVkGUIVertex Vertices[4] =
	{
		//{ {x + w,  y + h},	{1.0, 0.05}, {1.0, 0.0, 0.0, 1.0} },
		//{ {x + w, y},		{1.0, 0.0}, {0.0, 1.0, 0.0, 1.0} },
		//{ {x, y},			{0.0, 0.0}, {0.0, 0.0, 1.0, 1.0} },
		//{ {x,  y + h},		{0.0, 0.05}, {1.0, 0.0, 1.0, 1.0} },

		{ {x + w,  y + h},	{1001.0, 1001.0}, {w, h, OpenVkGUI.Rounding, 0.2} },
		{ {x + w, y},		{1001.0, 999.0},  {w, h, OpenVkGUI.Rounding, 0.2} },
		{ {x, y},			{999.0, 999.0},   {w, h, OpenVkGUI.Rounding, 0.2} },
		{ {x,  y + h},		{999.0, 1001.0},  {w, h, OpenVkGUI.Rounding, 0.2} },
	};
	OpenVkGUIAddVertices(4, Vertices);

	uint32_t Indices[6];
	Indices[0] = OpenVkGUI.LastIndex;
	Indices[1] = OpenVkGUI.LastIndex + 1;
	Indices[2] = OpenVkGUI.LastIndex + 2;
	Indices[3] = OpenVkGUI.LastIndex;
	Indices[4] = OpenVkGUI.LastIndex + 2;
	Indices[5] = OpenVkGUI.LastIndex + 3;
	OpenVkGUIAddIndices(6, Indices);

	OpenVkGUI.LastIndex += 4;
}

OpenVkBool OpenVkGUICollision(uint32_t x, uint32_t y, uint32_t w, uint32_t h)
{
//	int32_t MouseX;
//	int32_t MouseY;
//	uint32_t MouseState = OpenVkGetMousePos(&MouseX, &MouseY);

	if (OpenVkGUI.CurMouse.State == 1 &&
		OpenVkGUI.CurMouse.x < x + w &&
		OpenVkGUI.CurMouse.x > x &&
		OpenVkGUI.CurMouse.y < y + h &&
		OpenVkGUI.CurMouse.y > y)
	{
	//	*x += MouseX;
	//	*y += MouseY;
		return OpenVkTrue;
	}
		

	return OpenVkFalse;
}

void OpenVkGUIBegin(const char* Title)
{
	static uint32_t x = 60;
	static uint32_t y = 40;

	static uint32_t tx = 60;
	static uint32_t ty = 40;
	uint32_t w = 860;
	uint32_t h = 260;

	static OpenVkBool Collision = OpenVkFalse;
	OpenVkBool TmpCollision = OpenVkGUICollision(x, y, w, h);

//	if (TmpCollision && !Collision)
//	{
//		tx = x;
//		ty = y;
//	}
	if (!TmpCollision && Collision)
	{
		tx = x;
		ty = y;
	}

	x = tx;
	y = ty;

	
	if (TmpCollision && !Collision)
		Collision = OpenVkTrue;
	if (!TmpCollision && OpenVkGUI.CurMouse.State != 1)
		Collision = OpenVkFalse;



	if (Collision)
	{
		x += OpenVkGUI.CurMouse.x - OpenVkGUI.LastPressedMouse.x;
		y += OpenVkGUI.CurMouse.y - OpenVkGUI.LastPressedMouse.y;
	//	tx = x;
	//	ty = y;
	}
	else
	{
		
		
		//	x = OpenVkGUI.LastPressedMouse.x;
		//	y = OpenVkGUI.LastPressedMouse.y;
	}

//	if (Collision && OpenVkGUI.CurMouse.x != OpenVkGUI.LastMouse.x && OpenVkGUI.CurMouse.y != OpenVkGUI.LastMouse.y)
	

	/*
	static int32_t MouseX = 60;
	static int32_t MouseY = 40;
	

	OpenVkBool Collision = OpenVkGUICollision(x, y, w, h);

	if (Collision)
	{
		int32_t TmpMouseX;
		int32_t TmpMouseY;
		uint32_t MouseState = OpenVkGetMousePos(&TmpMouseX, &TmpMouseY);
		x = TmpMouseX - MouseX;
		y = TmpMouseY - MouseY;
	}
	else
	{
	//	MouseX = x;
	//	MouseY = y;
	}
	*/
	OpenVkGUIQuadPos(x, y, w, h);
	OpenVkGUITextPos(Title, OpenVkGUI.Border + x, OpenVkGUI.Border + y, w - OpenVkGUI.Border);
}

void OpenVkGUIEnd()
{

}

OpenVkBool OpenVkGUIButton(const char* Name)
{

}
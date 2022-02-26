#ifdef OPENVK_LIB_EXPORTS
#define OPENVK_LIB __declspec(dllexport)
#else
#define OPENVK_LIB __declspec(dllimport)
#endif

#include "OpenVK_Helper.h"
#include "OpenVK_Vulkan.h"

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
} OpenVkShaderType;

typedef enum
{
	OPENVK_DESCRIPTOR_TYPE_UNIFORM_BUFFER = 0x0,
	OPENVK_DESCRIPTOR_TYPE_DYNAMIC_UNIFORM_BUFFER = 0x1,
	OPENVK_DESCRIPTOR_TYPE_IMAGE_SAMPLER = 0x2,
} OpenVkDescriptorType;

typedef enum
{
	OPENVK_IMAGE_LAYOUT_COLOR_OUTPUT = 0x0,
	OPENVK_IMAGE_LAYOUT_DEPTH_OUTPUT = 0x1,
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
} OpenVkImageTypes;

typedef struct
{
	OpenVkBool	Dynamic;
	uint32_t	DescriptorPool;
} OpenVkDescriptorPool;

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
	uint32_t		PolygonMode;
	float			LineWidth;
	uint32_t		CullMode;
	uint32_t		FrontFace;
	uint32_t		MsaaSamples;
	OpenVkBool		AlphaBlending;
	uint32_t		PushConstantCount;
	uint32_t*		PushConstantShaderTypes;
	uint32_t*		PushConstantOffsets;
	uint32_t*		PushConstantSizes;
	uint32_t		DescriptorSetLayoutCount;
	uint32_t*		DescriptorSetLayouts;
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
	uint32_t 				DescriptorCount;
	uint32_t*				DescriptorTypes;
	uint32_t*				UniformBuffers;
	size_t*					UniformBufferSizes;
	uint32_t*				Sampler;
	uint32_t*				ImageTypes;
	uint32_t*				Images;
	uint32_t*				ImageLayouts;
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

#ifdef __cplusplus
extern "C" {
#endif
	OPENVK_LIB uint32_t OpenVkCreateRenderer(OpenVkBool ValidationLayers, uint32_t ExtensionCount, const char** Extensions, VkSurfaceKHR(*GetSurface)(VkInstance* Instance), void (*GetWindowSize)(uint32_t* Width, uint32_t* Height));

	OPENVK_LIB OpenVkVulkanRendererInfo* OpenVkGetRendererInfoVulkan();

	OPENVK_LIB void OpenVkDestroyRenderer();

	OPENVK_LIB void OpenVkRecreateSwapChain(uint32_t* Width, uint32_t* Height);

	OPENVK_LIB uint32_t OpenVkCreateRenderPass(uint32_t ColorAttachmentCount, OpenVkBool DepthAttachment, OpenVkBool MsaaAttachment, uint32_t MsaaSamples, OpenVkBool Sampled);

	OPENVK_LIB uint32_t OpenVkCreateGraphicsPipeline(OpenVkGraphicsPipelineCreateInfo* Info);

	OPENVK_LIB uint32_t OpenVkCreateFramebuffer(OpenVkFramebufferCreateInfo* Info);

	OPENVK_LIB uint32_t OpenVkCreateDescriptorSetLayout(uint32_t Binding, uint32_t BindingCount, uint32_t* DescriptorTypes, uint32_t* ShaderTypes);

	OPENVK_LIB OpenVkDescriptorPool OpenVkCreateDescriptorPool(uint32_t PoolSizeCount, uint32_t* DescriptorTypes, uint32_t* DescriptorCounts, OpenVkBool Dynamic);

	OPENVK_LIB uint32_t OpenVkCreateDescriptorSet(OpenVkDescriptorSetCreateInfo* Info);

	OPENVK_LIB OpenVkBool OpenVkDrawFrame(void (*RenderFunc)(void), void (*ResizeFunc)(void), void (*UpdateFunc)(void));

	OPENVK_LIB OpenVkBool OpenVkBegineFrame();

	OPENVK_LIB OpenVkBool OpenVkEndFrame();

	OPENVK_LIB void OpenVkBeginRenderPass(OpenVkBeginRenderPassInfo* Info);

	OPENVK_LIB void OpenVkEndRenderPass();

	OPENVK_LIB uint32_t OpenVkCreateTextureImage(const char* Path, OpenVkBool FlipVertical);

	OPENVK_LIB uint32_t OpenVkCreateImageSampler(OpenVkFilter Filter, OpenVkAddressMode AddressMode);

	OPENVK_LIB uint32_t OpenVkCreateColorImageAttachment(uint32_t Width, uint32_t Height, uint32_t MsaaSamples, OpenVkBool Sampled);

	OPENVK_LIB uint32_t OpenVkCreateDepthImageAttachment(uint32_t Width, uint32_t Height, uint32_t MsaaSamples, OpenVkBool Sampled);

	OPENVK_LIB uint32_t OpenVkCreateUniformBuffer(size_t Size);

	OPENVK_LIB void OpenVkUpdateUniformBuffer(size_t Size, const void* UBO, uint32_t UniformBuffer);

	OPENVK_LIB uint32_t OpenVkCreateDynamicUniformBuffer(size_t Size);

	OPENVK_LIB void OpenVkUpdateDynamicUniformBuffer(size_t Size, const void* UBO, uint32_t UniformBuffer);

	OPENVK_LIB uint32_t OpenVkCreateVertexBuffer(size_t Size, const void* Vertices);

	OPENVK_LIB uint32_t OpenVkCreateIndexBuffer(size_t Size, const void* Indices);

	OPENVK_LIB void OpenVkBindGraphicsPipeline(uint32_t Pipeline);

	OPENVK_LIB void OpenVkSetViewport(float x, float y, float Width, float Height);

	OPENVK_LIB void OpenVkSetScissor(int32_t x, int32_t y, uint32_t Width, uint32_t Height);

	OPENVK_LIB void OpenVkBindVertexBuffer(uint32_t VertexBuffer);

	OPENVK_LIB void OpenVkBindIndexBuffer(uint32_t VertexBuffer, uint32_t IndexBuffer);

	OPENVK_LIB void OpenVkDrawVertices(uint32_t VertexCount);

	OPENVK_LIB void OpenVkDrawIndices(uint32_t IndexCount);

	OPENVK_LIB void OpenVkBindDescriptorSet(uint32_t Pipeline, uint32_t Set, uint32_t DescriptorSet);

	OPENVK_LIB void OpenVkPushConstant(uint32_t Pipeline, OpenVkShaderType ShaderType, uint32_t Offset, size_t Size, const void* Data);
#ifdef __cplusplus
}
#endif
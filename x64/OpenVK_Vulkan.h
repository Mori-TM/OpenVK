#define MAX_FRAMES_IN_FLIGHT 3

typedef struct
{
	VkImage* Images;
	VkDeviceMemory* ImageMemoryies;
	VkImageView* ImageViews;
} OpenVkVulkanImageInfo;

typedef struct
{
	VkFramebuffer* Framebuffers;
} OpenVkVulkanFramebufferInfo;

typedef struct
{
	uint32_t DescriptorPoolCount;
	VkDescriptorPool* DescriptorPools;
} OpenVkVulkanDescriptorPoolInfo;

typedef struct
{
	VkInstance Instance;
	VkPhysicalDevice PhysicalDevice;
	VkDevice Device;

	VkQueue GraphicsQueue;
	VkQueue PresentQueue;

	VkSurfaceKHR Surface;

	uint32_t SwapChainImageCount;
	VkSwapchainKHR SwapChain;
	VkFormat SwapChainImageFormat;
	VkExtent2D SwapChainExtent;

	uint32_t ImageCount;
	OpenVkVulkanImageInfo* Images;

	uint32_t RenderPassCount;
	VkRenderPass* RenderPasses;

	uint32_t PipelineCount;
	VkPipelineLayout* PipelineLayouts;
	VkPipeline* Pipelines;

	uint32_t FramebufferCount;
	OpenVkVulkanFramebufferInfo* Framebuffers;

	VkCommandPool CommandPool;
	VkCommandBuffer* CommandBuffers;

	uint32_t CurrentFrame;
	uint32_t ImageIndex;
	VkSemaphore ImageAvailableSemaphores[MAX_FRAMES_IN_FLIGHT];
	VkSemaphore RenderFinishedSemaphores[MAX_FRAMES_IN_FLIGHT];
	VkFence InFlightFences[MAX_FRAMES_IN_FLIGHT];
	VkFence* InFlightImages;

	uint32_t DescriptorSetLayoutCount;
	VkDescriptorSetLayout* DescriptorSetLayouts;

	uint32_t DescriptorSetCount;
	VkDescriptorSet* DescriptorSets;

	uint32_t BufferCount;
	VkBuffer* Buffers;
	VkDeviceMemory* BufferMemories;

	uint32_t MipLevels;

	uint32_t TextureImageCount;
	VkImage* TextureImages;
	VkImageView* TextureImageViews;
	VkDeviceMemory* TextureImageMemories;

	uint32_t SamplerCount;
	VkSampler* Sampler;

	VkSampleCountFlagBits MsaaSamples;

	OpenVkVulkanDescriptorPoolInfo DynamicDescriptorPools;
	OpenVkVulkanDescriptorPoolInfo StaticDescriptorPools;

	VkPhysicalDeviceProperties PhysicalDeviceProperties;
	VkPhysicalDeviceFeatures PhysicalDeviceFeatures;
} OpenVkVulkanRendererInfo;
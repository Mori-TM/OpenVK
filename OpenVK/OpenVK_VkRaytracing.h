struct
{
	PFN_vkGetBufferDeviceAddressKHR vkGetBufferDeviceAddress;
	PFN_vkCreateAccelerationStructureKHR vkCreateAccelerationStructure;
	PFN_vkDestroyAccelerationStructureKHR vkDestroyAccelerationStructure;
	PFN_vkGetAccelerationStructureBuildSizesKHR vkGetAccelerationStructureBuildSizes;
	PFN_vkGetAccelerationStructureDeviceAddressKHR vkGetAccelerationStructureDeviceAddress;
	PFN_vkCmdBuildAccelerationStructuresKHR vkCmdBuildAccelerationStructures;
	PFN_vkBuildAccelerationStructuresKHR vkBuildAccelerationStructures;
	PFN_vkCmdTraceRaysKHR vkCmdTraceRays;
	PFN_vkGetRayTracingShaderGroupHandlesKHR vkGetRayTracingShaderGroupHandles;
	PFN_vkCreateRayTracingPipelinesKHR vkCreateRayTracingPipelines;
} KHR;

typedef struct
{
	uint64_t DeviceAddress;
	VkBuffer Handle;
	VkDeviceMemory Memory;
} VkRaytracingScratchBuffer;

typedef struct
{
	VkAccelerationStructureKHR Handle;
	uint64_t DeviceAddress;
	VkDeviceMemory Memory;
	VkBuffer Buffer;
} VkAccelerationStructure;

typedef struct
{
	uint32_t VertexCount;
	uint32_t IndexCount;
	VkAccelerationStructureGeometryKHR AccelerationStructureGeometry;
} VkRaytracingGeometryInfo;

typedef struct
{
	VkPhysicalDeviceRayTracingPipelinePropertiesKHR  RayTracingPipelineProperties;
	VkPhysicalDeviceAccelerationStructureFeaturesKHR AccelerationStructureFeatures;

	VkPhysicalDeviceBufferDeviceAddressFeatures EnabledBufferDeviceAddresFeatures;
	VkPhysicalDeviceRayTracingPipelineFeaturesKHR EnabledRayTracingPipelineFeatures;
	VkPhysicalDeviceAccelerationStructureFeaturesKHR EnabledAccelerationStructureFeatures;

	VkPhysicalDeviceFeatures2 PhysicalDeviceFeatures2;

	CMA_MemoryZone Geometry;

	CMA_MemoryZone BottomLevelAS;
	CMA_MemoryZone TopLevelAS;

	uint32_t ShaderGroupCount;
	VkRayTracingShaderGroupCreateInfoKHR* ShaderGroups;

	uint32_t ShaderBindingCount;
	uint32_t* ShaderBindings;
} VkRaytracerInfo;

VkRaytracerInfo VkRaytracer;

/*
* ....
* 
if (vkCreateDevice(VkRenderer.PhysicalDevice, &CreateInfo, NULL, &VkRenderer.Device) != VK_SUCCESS)
	return OpenVkRuntimeError("Failed to Create Device");
*/
void VkGetRaytracingFeatures(VkDeviceCreateInfo* DeviceCreateInfo)
{
	memset(&VkRaytracer, 0, sizeof(VkRaytracerInfo));
	VkRaytracer.Geometry = CMA_Create(sizeof(VkRaytracingGeometryInfo));
	VkRaytracer.BottomLevelAS = CMA_Create(sizeof(VkAccelerationStructure));
	VkRaytracer.TopLevelAS = CMA_Create(sizeof(VkAccelerationStructure));

	VkRaytracer.EnabledBufferDeviceAddresFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BUFFER_DEVICE_ADDRESS_FEATURES;
	VkRaytracer.EnabledBufferDeviceAddresFeatures.bufferDeviceAddress = VK_TRUE;

	VkRaytracer.EnabledRayTracingPipelineFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_FEATURES_KHR;
	VkRaytracer.EnabledRayTracingPipelineFeatures.rayTracingPipeline = VK_TRUE;
	VkRaytracer.EnabledRayTracingPipelineFeatures.pNext = &VkRaytracer.EnabledBufferDeviceAddresFeatures;

	VkRaytracer.EnabledAccelerationStructureFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_FEATURES_KHR;
	VkRaytracer.EnabledAccelerationStructureFeatures.accelerationStructure = VK_TRUE;
	VkRaytracer.EnabledAccelerationStructureFeatures.pNext = &VkRaytracer.EnabledRayTracingPipelineFeatures;

	VkRenderer.DeviceExtensions[VkRenderer.DeviceExtensionCount++] = VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME;
	VkRenderer.DeviceExtensions[VkRenderer.DeviceExtensionCount++] = VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME;
	VkRenderer.DeviceExtensions[VkRenderer.DeviceExtensionCount++] = VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME;
	VkRenderer.DeviceExtensions[VkRenderer.DeviceExtensionCount++] = VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME;
	VkRenderer.DeviceExtensions[VkRenderer.DeviceExtensionCount++] = VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME;
	VkRenderer.DeviceExtensions[VkRenderer.DeviceExtensionCount++] = VK_KHR_SPIRV_1_4_EXTENSION_NAME;
	VkRenderer.DeviceExtensions[VkRenderer.DeviceExtensionCount++] = VK_KHR_SHADER_FLOAT_CONTROLS_EXTENSION_NAME;

	for (uint32_t i = 0; i < VkRenderer.DeviceExtensionCount; i++)
		printf("Ext: %s\n", VkRenderer.DeviceExtensions[i]);

	DeviceCreateInfo->enabledExtensionCount = VkRenderer.DeviceExtensionCount;
	DeviceCreateInfo->ppEnabledExtensionNames = VkRenderer.DeviceExtensions;

	VkRaytracer.PhysicalDeviceFeatures2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
	VkRaytracer.PhysicalDeviceFeatures2.pNext = &VkRaytracer.EnabledAccelerationStructureFeatures;
	VkRaytracer.PhysicalDeviceFeatures2.features = *DeviceCreateInfo->pEnabledFeatures;

	DeviceCreateInfo->pEnabledFeatures = NULL;
	DeviceCreateInfo->pNext = &VkRaytracer.PhysicalDeviceFeatures2;
}

/*
* After OpenVk create renderer
*/
void VkGetRaytracingInfos()
{
	VkRaytracer.RayTracingPipelineProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_PROPERTIES_KHR;
	VkPhysicalDeviceProperties2 DeviceProperties2;
	DeviceProperties2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
	DeviceProperties2.pNext = &VkRaytracer.RayTracingPipelineProperties;
	vkGetPhysicalDeviceProperties2(VkRenderer.PhysicalDevice, &DeviceProperties2);

	VkRaytracer.AccelerationStructureFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_FEATURES_KHR;
	VkPhysicalDeviceFeatures2 DeviceFeatures2;
	DeviceFeatures2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
	DeviceFeatures2.pNext = &VkRaytracer.AccelerationStructureFeatures;
	vkGetPhysicalDeviceFeatures2(VkRenderer.PhysicalDevice, &DeviceFeatures2);

	KHR.vkGetBufferDeviceAddress = (PFN_vkGetBufferDeviceAddressKHR)(vkGetDeviceProcAddr(VkRenderer.Device, "vkGetBufferDeviceAddressKHR"));
	KHR.vkCmdBuildAccelerationStructures = (PFN_vkCmdBuildAccelerationStructuresKHR)(vkGetDeviceProcAddr(VkRenderer.Device, "vkCmdBuildAccelerationStructuresKHR"));
	KHR.vkBuildAccelerationStructures = (PFN_vkBuildAccelerationStructuresKHR)(vkGetDeviceProcAddr(VkRenderer.Device, "vkBuildAccelerationStructuresKHR"));
	KHR.vkCreateAccelerationStructure = (PFN_vkCreateAccelerationStructureKHR)(vkGetDeviceProcAddr(VkRenderer.Device, "vkCreateAccelerationStructureKHR"));
	KHR.vkDestroyAccelerationStructure = (PFN_vkDestroyAccelerationStructureKHR)(vkGetDeviceProcAddr(VkRenderer.Device, "vkDestroyAccelerationStructureKHR"));
	KHR.vkGetAccelerationStructureBuildSizes = (PFN_vkGetAccelerationStructureBuildSizesKHR)(vkGetDeviceProcAddr(VkRenderer.Device, "vkGetAccelerationStructureBuildSizesKHR"));
	KHR.vkGetAccelerationStructureDeviceAddress = (PFN_vkGetAccelerationStructureDeviceAddressKHR)(vkGetDeviceProcAddr(VkRenderer.Device, "vkGetAccelerationStructureDeviceAddressKHR"));
	KHR.vkCmdTraceRays = (PFN_vkCmdTraceRaysKHR)(vkGetDeviceProcAddr(VkRenderer.Device, "vkCmdTraceRaysKHR"));
	KHR.vkGetRayTracingShaderGroupHandles = (PFN_vkGetRayTracingShaderGroupHandlesKHR)(vkGetDeviceProcAddr(VkRenderer.Device, "vkGetRayTracingShaderGroupHandlesKHR"));
	KHR.vkCreateRayTracingPipelines = (PFN_vkCreateRayTracingPipelinesKHR)(vkGetDeviceProcAddr(VkRenderer.Device, "vkCreateRayTracingPipelinesKHR"));

}

/*
* Raytracing Pipline
* Shader Binding Table
* Top Level Acceleration Structure
* Bottom Level Acceleration Structure
* get Buffer Device Address
* Acceleration Structure Buffer
* create/delete ScratchBuffer
*/

VkRaytracingScratchBuffer VkCreateScratchBuffer(VkDeviceSize Size)
{
	VkRaytracingScratchBuffer ScratchBuffer;

	VkBufferCreateInfo BufferCreateInfo;
	BufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	BufferCreateInfo.pNext = NULL;
	BufferCreateInfo.flags = 0;
	BufferCreateInfo.size = Size;
	BufferCreateInfo.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT;
	BufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;//?
	BufferCreateInfo.queueFamilyIndexCount = 0;
	BufferCreateInfo.pQueueFamilyIndices = 0;
	if (vkCreateBuffer(VkRenderer.Device, &BufferCreateInfo, NULL, &ScratchBuffer.Handle) != VK_SUCCESS)
	{
		ScratchBuffer.DeviceAddress = OpenVkRuntimeError("Failed to create scratch buffer");
		return ScratchBuffer;
	}

	VkMemoryRequirements MemoryRequirements;
	vkGetBufferMemoryRequirements(VkRenderer.Device, ScratchBuffer.Handle, &MemoryRequirements);
	
	VkMemoryAllocateFlagsInfo MemoryAllocateFlagsInfo;
	MemoryAllocateFlagsInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_FLAGS_INFO;
	MemoryAllocateFlagsInfo.pNext = NULL;
	MemoryAllocateFlagsInfo.flags = VK_MEMORY_ALLOCATE_DEVICE_ADDRESS_BIT_KHR;
	MemoryAllocateFlagsInfo.deviceMask = 0;

	VkMemoryAllocateInfo MemoryAllocateInfo;
	MemoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	MemoryAllocateInfo.pNext = &MemoryAllocateFlagsInfo;
	MemoryAllocateInfo.allocationSize = MemoryRequirements.size;
	MemoryAllocateInfo.memoryTypeIndex = VkFindMemoryType(MemoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	if (vkAllocateMemory(VkRenderer.Device, &MemoryAllocateInfo, NULL, &ScratchBuffer.Memory) != VK_SUCCESS)
	{
		ScratchBuffer.DeviceAddress = OpenVkRuntimeError("Failed to allocate scratch buffer");
		return ScratchBuffer;
	}

	if (vkBindBufferMemory(VkRenderer.Device, ScratchBuffer.Handle, ScratchBuffer.Memory, 0) != VK_SUCCESS)
	{
		ScratchBuffer.DeviceAddress = OpenVkRuntimeError("Failed to bind scratch buffer");
		return ScratchBuffer;
	}

	VkBufferDeviceAddressInfoKHR BufferDeviceAddressInfo;
	BufferDeviceAddressInfo.sType = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO;
	BufferDeviceAddressInfo.pNext = NULL;
	BufferDeviceAddressInfo.buffer = ScratchBuffer.Handle;
	ScratchBuffer.DeviceAddress = KHR.vkGetBufferDeviceAddress(VkRenderer.Device, &BufferDeviceAddressInfo);

	return ScratchBuffer;
}

void VkDestroyScratchBuffer(VkRaytracingScratchBuffer* ScratchBuffer)
{
	if (ScratchBuffer->Memory != VK_NULL_HANDLE) {
		vkFreeMemory(VkRenderer.Device, ScratchBuffer->Memory, NULL);
	}
	if (ScratchBuffer->Handle != VK_NULL_HANDLE) {
		vkDestroyBuffer(VkRenderer.Device, ScratchBuffer->Handle, NULL);
	}
}

OpenVkBool VkCreateAccelerationStructureBuffer(VkAccelerationStructure* AccelerationStructure, VkAccelerationStructureBuildSizesInfoKHR BuildSizeInfo)
{
	VkBufferCreateInfo BufferCreateInfo;
	BufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	BufferCreateInfo.pNext = NULL;
	BufferCreateInfo.flags = 0;
	BufferCreateInfo.size = BuildSizeInfo.accelerationStructureSize;
	BufferCreateInfo.usage = VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_STORAGE_BIT_KHR | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT;
	BufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;//?
	BufferCreateInfo.queueFamilyIndexCount = 0;
	BufferCreateInfo.pQueueFamilyIndices = 0;
	if (vkCreateBuffer(VkRenderer.Device, &BufferCreateInfo, NULL, &AccelerationStructure->Buffer) != VK_SUCCESS)
		return OpenVkRuntimeError("Failed to create acceleration structure buffer");
	
	VkMemoryRequirements MemoryRequirements;
	vkGetBufferMemoryRequirements(VkRenderer.Device, AccelerationStructure->Buffer, &MemoryRequirements);

	VkMemoryAllocateFlagsInfo MemoryAllocateFlagsInfo;
	MemoryAllocateFlagsInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_FLAGS_INFO;
	MemoryAllocateFlagsInfo.pNext = NULL;
	MemoryAllocateFlagsInfo.flags = VK_MEMORY_ALLOCATE_DEVICE_ADDRESS_BIT_KHR;
	MemoryAllocateFlagsInfo.deviceMask = 0;

	VkMemoryAllocateInfo MemoryAllocateInfo;
	MemoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	MemoryAllocateInfo.pNext = &MemoryAllocateFlagsInfo;
	MemoryAllocateInfo.allocationSize = MemoryRequirements.size;
	MemoryAllocateInfo.memoryTypeIndex = VkFindMemoryType(MemoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	if (vkAllocateMemory(VkRenderer.Device, &MemoryAllocateInfo, NULL, &AccelerationStructure->Memory) != VK_SUCCESS)
		return OpenVkRuntimeError("Failed to allocate acceleration structure buffer");

	if (vkBindBufferMemory(VkRenderer.Device, AccelerationStructure->Buffer, AccelerationStructure->Memory, 0) != VK_SUCCESS)
		return OpenVkRuntimeError("Failed to bind acceleration structure buffer");

	return OpenVkTrue;
}

uint64_t VkGetBufferDeviceAddress(VkBuffer Buffer)
{
	VkBufferDeviceAddressInfoKHR BufferDeviceAI;
	BufferDeviceAI.sType = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO;
	BufferDeviceAI.pNext = NULL;
	BufferDeviceAI.buffer = Buffer;
	return KHR.vkGetBufferDeviceAddress(VkRenderer.Device, &BufferDeviceAI);
}

uint32_t VkCreateRaytracingGeometry(size_t VertexCount, void* Vertices, size_t IndexCount, void* Indices, size_t VertexSize)
{
	uint32_t VertexBuffer = VkCreateBufferExt(VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		VertexCount * VertexSize, Vertices, NULL);

	VkDeviceOrHostAddressConstKHR VertexBufferDeviceAddress;
	VkDeviceOrHostAddressConstKHR IndexBufferDeviceAddress;
	
	VkBufferInfo* Vertex = (VkBufferInfo*)CMA_GetAt(&VkRenderer.Buffers, VertexBuffer);
	VertexBufferDeviceAddress.deviceAddress = VkGetBufferDeviceAddress(Vertex->Buffer);

	if (IndexCount != 0)
	{
		uint32_t IndexBuffer = VkCreateBufferExt(VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			IndexCount * sizeof(uint32_t), Indices, NULL);

		VkBufferInfo* Index = (VkBufferInfo*)CMA_GetAt(&VkRenderer.Buffers, IndexBuffer);
		IndexBufferDeviceAddress.deviceAddress = VkGetBufferDeviceAddress(Index->Buffer);
	}
	else
	{
		//works maybe
		IndexBufferDeviceAddress.deviceAddress = 0;
		IndexBufferDeviceAddress.hostAddress = NULL;
		IndexCount = VertexCount;
	}

	VkAccelerationStructureGeometryKHR AccelerationStructureGeometry;
	memset(&AccelerationStructureGeometry, 0, sizeof(VkAccelerationStructureGeometryKHR));
	AccelerationStructureGeometry.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_KHR;
	AccelerationStructureGeometry.pNext = NULL;
	AccelerationStructureGeometry.flags = VK_GEOMETRY_OPAQUE_BIT_KHR;
	AccelerationStructureGeometry.geometryType = VK_GEOMETRY_TYPE_TRIANGLES_KHR;
	AccelerationStructureGeometry.geometry.triangles.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_TRIANGLES_DATA_KHR;
	AccelerationStructureGeometry.geometry.triangles.vertexFormat = VK_FORMAT_R32G32B32_SFLOAT; //dynamic input
	AccelerationStructureGeometry.geometry.triangles.vertexData = VertexBufferDeviceAddress;
	AccelerationStructureGeometry.geometry.triangles.maxVertex = VertexCount;
	AccelerationStructureGeometry.geometry.triangles.vertexStride = VertexSize;
	AccelerationStructureGeometry.geometry.triangles.indexType = VK_INDEX_TYPE_UINT32;
	AccelerationStructureGeometry.geometry.triangles.indexData = IndexBufferDeviceAddress;
	AccelerationStructureGeometry.geometry.triangles.transformData.deviceAddress = 0;
	AccelerationStructureGeometry.geometry.triangles.transformData.hostAddress = NULL;

	VkRaytracingGeometryInfo GeometryInfo;
	GeometryInfo.AccelerationStructureGeometry = AccelerationStructureGeometry;
	GeometryInfo.VertexCount = VertexCount;
	GeometryInfo.IndexCount = IndexCount;

	return CMA_Push(&VkRaytracer.Geometry, &GeometryInfo);
}

uint32_t VkCreateBottomLevelAccelerationStructure()
{
	uint32_t GeometryCount = 0;
	uint32_t IndexCount = 0;
	VkAccelerationStructureGeometryKHR* Geometry = (VkAccelerationStructureGeometryKHR*)OpenVkMalloc(VkRaytracer.Geometry.Size * sizeof(VkAccelerationStructureGeometryKHR));

	for (uint32_t i = 0; i < VkRaytracer.Geometry.Size; i++)
	{
		VkRaytracingGeometryInfo* Geo = (VkRaytracingGeometryInfo*)CMA_GetAt(&VkRaytracer.Geometry, i);
		if (Geo != NULL)
		{
			
			printf("Geo: %d, Max vertex: %d, vertex stride: %d, index: %x\n", GeometryCount, Geo->AccelerationStructureGeometry.geometry.triangles.maxVertex, Geo->AccelerationStructureGeometry.geometry.triangles.vertexStride, Geo->AccelerationStructureGeometry.geometry.triangles.indexData);
			Geometry[GeometryCount++] = Geo->AccelerationStructureGeometry;			
			IndexCount += Geo->IndexCount;
		}		
	}

	// Get size info
	
	VkAccelerationStructureBuildGeometryInfoKHR AccelerationStructureBuildGeometryInfo;
	AccelerationStructureBuildGeometryInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR;
	AccelerationStructureBuildGeometryInfo.pNext = NULL;
	AccelerationStructureBuildGeometryInfo.type = VK_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL_KHR;
	AccelerationStructureBuildGeometryInfo.flags = VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_KHR;
	AccelerationStructureBuildGeometryInfo.mode = VK_BUILD_ACCELERATION_STRUCTURE_MODE_BUILD_KHR;
	AccelerationStructureBuildGeometryInfo.srcAccelerationStructure = 0;
	AccelerationStructureBuildGeometryInfo.dstAccelerationStructure = 0;
	AccelerationStructureBuildGeometryInfo.geometryCount = GeometryCount;
	AccelerationStructureBuildGeometryInfo.pGeometries = Geometry;
	AccelerationStructureBuildGeometryInfo.ppGeometries = NULL;
	AccelerationStructureBuildGeometryInfo.scratchData.deviceAddress = 0;
	AccelerationStructureBuildGeometryInfo.scratchData.hostAddress = NULL;

	const uint32_t NumTriangles = IndexCount / 3;
	VkAccelerationStructureBuildSizesInfoKHR AccelerationStructureBuildSizesInfo;
	AccelerationStructureBuildSizesInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_SIZES_INFO_KHR;
	AccelerationStructureBuildSizesInfo.pNext = NULL;
	AccelerationStructureBuildSizesInfo.accelerationStructureSize = 0;
	AccelerationStructureBuildSizesInfo.updateScratchSize = 0;
	AccelerationStructureBuildSizesInfo.buildScratchSize = 0;
	
	/*
	// Get size info
	VkAccelerationStructureBuildGeometryInfoKHR AccelerationStructureBuildGeometryInfo{};
	AccelerationStructureBuildGeometryInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR;
	AccelerationStructureBuildGeometryInfo.type = VK_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL_KHR;
	AccelerationStructureBuildGeometryInfo.flags = VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_KHR;
	AccelerationStructureBuildGeometryInfo.geometryCount = GeometryCount;
	AccelerationStructureBuildGeometryInfo.pGeometries = Geometry;

	const uint32_t NumTriangles = 1;
	VkAccelerationStructureBuildSizesInfoKHR AccelerationStructureBuildSizesInfo{};
	AccelerationStructureBuildSizesInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_SIZES_INFO_KHR;
	*/
	VkAccelerationStructure BottomLevelAS;

	KHR.vkGetAccelerationStructureBuildSizes(VkRenderer.Device, VK_ACCELERATION_STRUCTURE_BUILD_TYPE_DEVICE_KHR, &AccelerationStructureBuildGeometryInfo, &NumTriangles, &AccelerationStructureBuildSizesInfo);
	VkCreateAccelerationStructureBuffer(&BottomLevelAS, AccelerationStructureBuildSizesInfo);

	VkAccelerationStructureCreateInfoKHR AccelerationStructureCreateInfo;
	AccelerationStructureCreateInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_CREATE_INFO_KHR;
	AccelerationStructureCreateInfo.pNext = NULL;
	AccelerationStructureCreateInfo.createFlags = 0;
	AccelerationStructureCreateInfo.buffer = BottomLevelAS.Buffer;
	AccelerationStructureCreateInfo.offset = 0;
	AccelerationStructureCreateInfo.size = AccelerationStructureBuildSizesInfo.accelerationStructureSize;
	AccelerationStructureCreateInfo.type = VK_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL_KHR;
	AccelerationStructureCreateInfo.deviceAddress = 0;
	KHR.vkCreateAccelerationStructure(VkRenderer.Device, &AccelerationStructureCreateInfo, NULL, &BottomLevelAS.Handle);

	// Create a small scratch buffer used during build of the bottom level acceleration structure
	VkRaytracingScratchBuffer ScratchBuffer = VkCreateScratchBuffer(AccelerationStructureBuildSizesInfo.buildScratchSize);

	VkAccelerationStructureBuildGeometryInfoKHR AccelerationBuildGeometryInfo;
	memset(&AccelerationBuildGeometryInfo, 0, sizeof(VkAccelerationStructureBuildGeometryInfoKHR));
	AccelerationBuildGeometryInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR;
//	AccelerationBuildGeometryInfo.pNext = NULL;
	AccelerationBuildGeometryInfo.type = VK_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL_KHR;
	AccelerationBuildGeometryInfo.flags = VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_KHR;
	AccelerationBuildGeometryInfo.mode = VK_BUILD_ACCELERATION_STRUCTURE_MODE_BUILD_KHR;
//	AccelerationBuildGeometryInfo.srcAccelerationStructure = 0;
	AccelerationBuildGeometryInfo.dstAccelerationStructure = BottomLevelAS.Handle;
	AccelerationBuildGeometryInfo.geometryCount = GeometryCount;
	AccelerationBuildGeometryInfo.pGeometries = Geometry;
//	AccelerationBuildGeometryInfo.ppGeometries = NULL;
	AccelerationBuildGeometryInfo.scratchData.deviceAddress = ScratchBuffer.DeviceAddress;
//	AccelerationBuildGeometryInfo.scratchData.hostAddress = NULL;

	VkAccelerationStructureBuildRangeInfoKHR AccelerationStructureBuildRangeInfo;
	AccelerationStructureBuildRangeInfo.primitiveCount = NumTriangles;
	AccelerationStructureBuildRangeInfo.primitiveOffset = 0;
	AccelerationStructureBuildRangeInfo.firstVertex = 0;
	AccelerationStructureBuildRangeInfo.transformOffset = 0;
	VkAccelerationStructureBuildRangeInfoKHR* AccelerationStructureBuildRangeInfoPP = &AccelerationStructureBuildRangeInfo;

	// Build the acceleration structure on the device via a one-time command buffer submission
	// Some implementations may support acceleration structure building on the host (VkPhysicalDeviceAccelerationStructureFeaturesKHR->accelerationStructureHostCommands), but we prefer device builds
	VkCommandBuffer CommandBuffer = VkBeginSingleTimeCommands();
	KHR.vkCmdBuildAccelerationStructures(CommandBuffer, 1, &AccelerationBuildGeometryInfo, &AccelerationStructureBuildRangeInfoPP);
	VkEndSingleTimeCommandBuffer(CommandBuffer);

	VkAccelerationStructureDeviceAddressInfoKHR AccelerationDeviceAddressInfo;
	AccelerationDeviceAddressInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_DEVICE_ADDRESS_INFO_KHR;
	AccelerationDeviceAddressInfo.pNext = NULL;
	AccelerationDeviceAddressInfo.accelerationStructure = BottomLevelAS.Handle;
	BottomLevelAS.DeviceAddress = KHR.vkGetAccelerationStructureDeviceAddress(VkRenderer.Device, &AccelerationDeviceAddressInfo);

	VkDestroyScratchBuffer(&ScratchBuffer);
	OpenVkFree(Geometry);

	return CMA_Push(&VkRaytracer.BottomLevelAS, &BottomLevelAS);
}

OpenVkBool VkCreateTopLevelAccelerationStructure(uint32_t BottomLevelAccelerationStructure)
{
	uint32_t GeometryCount = 0;
	uint32_t IndexCount = 0;
	for (uint32_t i = 0; i < VkRaytracer.Geometry.Size; i++)
	{
		VkRaytracingGeometryInfo* Geo = (VkRaytracingGeometryInfo*)CMA_GetAt(&VkRaytracer.Geometry, i);
		if (Geo != NULL)
		{
			GeometryCount++;
			IndexCount += Geo->IndexCount;
		}
	}

	VkAccelerationStructure* BottomLevelAS = (VkAccelerationStructure*)CMA_GetAt(&VkRaytracer.BottomLevelAS, BottomLevelAccelerationStructure);
	if (BottomLevelAS == NULL)
		return OpenVkRuntimeError("Failed to find bottom level as");

	VkTransformMatrixKHR transformMatrix = {
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f };

	VkAccelerationStructureInstanceKHR instance{};
	instance.transform = transformMatrix;
	instance.instanceCustomIndex = 0;
	instance.mask = 0xFF;
	instance.instanceShaderBindingTableRecordOffset = 0;
	instance.flags = VK_GEOMETRY_INSTANCE_TRIANGLE_FACING_CULL_DISABLE_BIT_KHR;
	instance.accelerationStructureReference = BottomLevelAS->DeviceAddress;

//	VkCreateBufferExt(VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
//		VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
//		Size, Vertices);

	uint32_t InstancesPTR = VkCreateBufferExt(VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		sizeof(VkAccelerationStructureInstanceKHR), &instance, NULL);

	VkBufferInfo* instancesBuffer = (VkBufferInfo*)CMA_GetAt(&VkRenderer.Buffers, InstancesPTR);

	// Buffer for instance data
//	vks::Buffer instancesBuffer;
//	VK_CHECK_RESULT(vulkanDevice->createBuffer(
//		VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR,
//		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
//		&instancesBuffer,
//		sizeof(VkAccelerationStructureInstanceKHR),
//		&instance));

	VkDeviceOrHostAddressConstKHR instanceDataDeviceAddress{};
	instanceDataDeviceAddress.deviceAddress = VkGetBufferDeviceAddress(instancesBuffer->Buffer);

	VkAccelerationStructureGeometryKHR accelerationStructureGeometry{};
	accelerationStructureGeometry.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_KHR;
	accelerationStructureGeometry.geometryType = VK_GEOMETRY_TYPE_INSTANCES_KHR;
	accelerationStructureGeometry.flags = VK_GEOMETRY_OPAQUE_BIT_KHR;
	accelerationStructureGeometry.geometry.instances.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_INSTANCES_DATA_KHR;
	accelerationStructureGeometry.geometry.instances.arrayOfPointers = VK_FALSE;
	accelerationStructureGeometry.geometry.instances.data = instanceDataDeviceAddress;

	// Get size info
	/*
	The pSrcAccelerationStructure, dstAccelerationStructure, and mode members of pBuildInfo are ignored. Any VkDeviceOrHostAddressKHR members of pBuildInfo are ignored by this command, except that the hostAddress member of VkAccelerationStructureGeometryTrianglesDataKHR::transformData will be examined to check if it is NULL.*
	*/
	VkAccelerationStructureBuildGeometryInfoKHR accelerationStructureBuildGeometryInfo{};
	accelerationStructureBuildGeometryInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR;
	accelerationStructureBuildGeometryInfo.type = VK_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL_KHR;
	accelerationStructureBuildGeometryInfo.flags = VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_KHR;
	accelerationStructureBuildGeometryInfo.geometryCount = GeometryCount;
	accelerationStructureBuildGeometryInfo.pGeometries = &accelerationStructureGeometry;

	VkAccelerationStructure TopLevelAS;

	const uint32_t PrimitiveCount = GeometryCount;
	VkAccelerationStructureBuildSizesInfoKHR accelerationStructureBuildSizesInfo{};
	accelerationStructureBuildSizesInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_SIZES_INFO_KHR;
	KHR.vkGetAccelerationStructureBuildSizes(
		VkRenderer.Device,
		VK_ACCELERATION_STRUCTURE_BUILD_TYPE_DEVICE_KHR,
		&accelerationStructureBuildGeometryInfo,
		&PrimitiveCount,
		&accelerationStructureBuildSizesInfo);

	VkCreateAccelerationStructureBuffer(&TopLevelAS, accelerationStructureBuildSizesInfo);

	VkAccelerationStructureCreateInfoKHR accelerationStructureCreateInfo{};
	accelerationStructureCreateInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_CREATE_INFO_KHR;
	accelerationStructureCreateInfo.buffer = TopLevelAS.Buffer;
	accelerationStructureCreateInfo.size = accelerationStructureBuildSizesInfo.accelerationStructureSize;
	accelerationStructureCreateInfo.type = VK_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL_KHR;
	KHR.vkCreateAccelerationStructure(VkRenderer.Device, &accelerationStructureCreateInfo, nullptr, &TopLevelAS.Handle);

	// Create a small scratch buffer used during build of the top level acceleration structure
	VkRaytracingScratchBuffer scratchBuffer = VkCreateScratchBuffer(accelerationStructureBuildSizesInfo.buildScratchSize);

	VkAccelerationStructureBuildGeometryInfoKHR accelerationBuildGeometryInfo{};
	accelerationBuildGeometryInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR;
	accelerationBuildGeometryInfo.type = VK_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL_KHR;
	accelerationBuildGeometryInfo.flags = VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_KHR;
	accelerationBuildGeometryInfo.mode = VK_BUILD_ACCELERATION_STRUCTURE_MODE_BUILD_KHR;
	accelerationBuildGeometryInfo.dstAccelerationStructure = TopLevelAS.Handle;
	accelerationBuildGeometryInfo.geometryCount = GeometryCount;
	accelerationBuildGeometryInfo.pGeometries = &accelerationStructureGeometry;
	accelerationBuildGeometryInfo.scratchData.deviceAddress = scratchBuffer.DeviceAddress;

	VkAccelerationStructureBuildRangeInfoKHR accelerationStructureBuildRangeInfo{};
	accelerationStructureBuildRangeInfo.primitiveCount = GeometryCount;
	accelerationStructureBuildRangeInfo.primitiveOffset = 0;
	accelerationStructureBuildRangeInfo.firstVertex = 0;
	accelerationStructureBuildRangeInfo.transformOffset = 0;
	VkAccelerationStructureBuildRangeInfoKHR* accelerationBuildStructureRangeInfos[] = {&accelerationStructureBuildRangeInfo};

	// Build the acceleration structure on the device via a one-time command buffer submission
	// Some implementations may support acceleration structure building on the host (VkPhysicalDeviceAccelerationStructureFeaturesKHR->accelerationStructureHostCommands), but we prefer device builds
	VkCommandBuffer commandBuffer = VkBeginSingleTimeCommands();
	KHR.vkCmdBuildAccelerationStructures(
		commandBuffer,
		1,
		&accelerationBuildGeometryInfo,
		accelerationBuildStructureRangeInfos);
	VkEndSingleTimeCommandBuffer(commandBuffer);

	VkAccelerationStructureDeviceAddressInfoKHR accelerationDeviceAddressInfo{};
	accelerationDeviceAddressInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_DEVICE_ADDRESS_INFO_KHR;
	accelerationDeviceAddressInfo.accelerationStructure = TopLevelAS.Handle;
	TopLevelAS.DeviceAddress = KHR.vkGetAccelerationStructureDeviceAddress(VkRenderer.Device, &accelerationDeviceAddressInfo);
	
	VkDestroyScratchBuffer(&scratchBuffer);
//	instancesBuffer.destroy();
	VkDestroyBuffer(InstancesPTR);

	return CMA_Push(&VkRaytracer.TopLevelAS, &TopLevelAS);
}

uint32_t VkCreateRaytracingPipeline(uint32_t PipelineLayout, uint32_t ShaderCount, uint32_t* ShaderTypes, const char** Shader)
{
	VkRenderer.Pipelines = (VkPipeline*)OpenVkRealloc(VkRenderer.Pipelines, (VkRenderer.PipelineCount + 1) * sizeof(VkPipeline));
	VkPipelineShaderStageCreateInfo* ShaderStages = (VkPipelineShaderStageCreateInfo*)OpenVkMalloc(ShaderCount * sizeof(VkPipelineShaderStageCreateInfo));
	VkRaytracer.ShaderGroups = (VkRayTracingShaderGroupCreateInfoKHR*)OpenVkRealloc(VkRaytracer.ShaderGroups, (VkRaytracer.ShaderGroupCount + ShaderCount) * sizeof(VkRayTracingShaderGroupCreateInfoKHR));

	for (uint32_t i = 0; i < ShaderCount; i++)
	{
		VkShaderModule ShaderModule;
		if (VkCreateShaderModule(Shader[i], &ShaderModule) == OPENVK_ERROR)
			return OpenVkRuntimeError("Failed to create raytracing shader module");

		ShaderStages[i].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		ShaderStages[i].pNext = NULL;
		ShaderStages[i].flags = 0;
		if (ShaderTypes[i] == OPENVK_SHADER_TYPE_RAYGEN) ShaderStages[i].stage = VK_SHADER_STAGE_RAYGEN_BIT_KHR;
		if (ShaderTypes[i] == OPENVK_SHADER_TYPE_MISS) ShaderStages[i].stage = VK_SHADER_STAGE_MISS_BIT_KHR;
		if (ShaderTypes[i] == OPENVK_SHADER_TYPE_CLOSEST_HIT) ShaderStages[i].stage = VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR;
		ShaderStages[i].module = ShaderModule;
		ShaderStages[i].pName = "main";
		ShaderStages[i].pSpecializationInfo = NULL;

		VkRayTracingShaderGroupCreateInfoKHR* ShaderGroup = &VkRaytracer.ShaderGroups[VkRaytracer.ShaderGroupCount];
		ShaderGroup->sType = VK_STRUCTURE_TYPE_RAY_TRACING_SHADER_GROUP_CREATE_INFO_KHR;
		ShaderGroup->pNext = NULL;
		if (ShaderTypes[i] == OPENVK_SHADER_TYPE_RAYGEN || ShaderTypes[i] == OPENVK_SHADER_TYPE_MISS)
		{
			ShaderGroup->type = VK_RAY_TRACING_SHADER_GROUP_TYPE_GENERAL_KHR;
			ShaderGroup->generalShader = i;
			ShaderGroup->closestHitShader = VK_SHADER_UNUSED_KHR;
			ShaderGroup->anyHitShader = VK_SHADER_UNUSED_KHR;
			ShaderGroup->intersectionShader = VK_SHADER_UNUSED_KHR;
			ShaderGroup->pShaderGroupCaptureReplayHandle = NULL;
		}
			
		if (ShaderTypes[i] == OPENVK_SHADER_TYPE_CLOSEST_HIT)
		{
			ShaderGroup->type = VK_RAY_TRACING_SHADER_GROUP_TYPE_TRIANGLES_HIT_GROUP_KHR;
			ShaderGroup->generalShader = VK_SHADER_UNUSED_KHR;
			ShaderGroup->closestHitShader = i;
			ShaderGroup->anyHitShader = VK_SHADER_UNUSED_KHR;
			ShaderGroup->intersectionShader = VK_SHADER_UNUSED_KHR;
			ShaderGroup->pShaderGroupCaptureReplayHandle = NULL;
		}	
		
		VkRaytracer.ShaderGroupCount++;
	}

//	VkDynamicState DynamicStates[2] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
//
//	VkPipelineDynamicStateCreateInfo DynamicStateInfo;
//	DynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
//	DynamicStateInfo.pNext = NULL;
//	DynamicStateInfo.flags = 0;
//	DynamicStateInfo.dynamicStateCount = 2;
//	DynamicStateInfo.pDynamicStates = DynamicStates;

	VkRayTracingPipelineCreateInfoKHR PipelineCreateInfo;
	PipelineCreateInfo.sType = VK_STRUCTURE_TYPE_RAY_TRACING_PIPELINE_CREATE_INFO_KHR;
	PipelineCreateInfo.pNext = NULL;
	PipelineCreateInfo.flags = 0;
	PipelineCreateInfo.stageCount = ShaderCount;
	PipelineCreateInfo.pStages = ShaderStages;
	PipelineCreateInfo.groupCount = VkRaytracer.ShaderGroupCount;
	PipelineCreateInfo.pGroups = VkRaytracer.ShaderGroups;
	PipelineCreateInfo.maxPipelineRayRecursionDepth = 1;
	PipelineCreateInfo.pLibraryInfo = NULL;
	PipelineCreateInfo.pLibraryInterface = NULL;
	PipelineCreateInfo.pDynamicState = NULL;
	PipelineCreateInfo.layout = VkRenderer.PipelineLayouts[PipelineLayout];
	PipelineCreateInfo.basePipelineHandle = 0;
	PipelineCreateInfo.basePipelineIndex = 0;

	if (KHR.vkCreateRayTracingPipelines(VkRenderer.Device, VK_NULL_HANDLE, VK_NULL_HANDLE, 1, &PipelineCreateInfo, NULL, &VkRenderer.Pipelines[VkRenderer.PipelineCount]) != VK_SUCCESS)
		return OpenVkRuntimeError("Failed to create ray tracing pipeline");

	for (uint32_t i = 0; i < ShaderCount; i++)
		vkDestroyShaderModule(VkRenderer.Device, ShaderStages[i].module, NULL);

	OpenVkFree(ShaderStages);

	return VkRenderer.PipelineCount++;
}

uint32_t* VkCreateShaderBindingTable(uint32_t Pipeline, uint32_t ShaderCount)
{
	VkRaytracer.ShaderBindings = (uint32_t*)OpenVkRealloc(VkRaytracer.ShaderBindings, (VkRaytracer.ShaderBindingCount + ShaderCount) * sizeof(uint32_t));

	const uint32_t HandleSize = VkRaytracer.RayTracingPipelineProperties.shaderGroupHandleSize;
	const uint32_t HandleSizeAligned = OpenVkAlignedSize(VkRaytracer.RayTracingPipelineProperties.shaderGroupHandleSize, VkRaytracer.RayTracingPipelineProperties.shaderGroupHandleAlignment);
	const uint32_t GroupCount = VkRaytracer.ShaderGroupCount;
	const uint32_t SbtSize = GroupCount * HandleSizeAligned;

	uint8_t* ShaderHandleStorage = (uint8_t*)OpenVkMalloc(SbtSize);
	if (KHR.vkGetRayTracingShaderGroupHandles(VkRenderer.Device, VkRenderer.Pipelines[Pipeline], 0, GroupCount, SbtSize, ShaderHandleStorage) != VK_SUCCESS)
	{
		OpenVkRuntimeError("Failed to get ray tracing shader group handles");
		return NULL;
	}
		

	for (uint32_t i = 0; i < ShaderCount; i++)
	{
		VkRaytracer.ShaderBindings[VkRaytracer.ShaderBindingCount] = VkCreateBufferExt(VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_SHADER_BINDING_TABLE_BIT_KHR | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			HandleSize, ShaderHandleStorage + (HandleSizeAligned * i), NULL);

		VkRaytracer.ShaderBindingCount++;
	}

	return VkRaytracer.ShaderBindings + (VkRaytracer.ShaderBindingCount - ShaderCount);
}


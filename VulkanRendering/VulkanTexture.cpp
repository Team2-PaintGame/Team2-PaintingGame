/******************************************************************************
This file is part of the Newcastle Vulkan Tutorial Series

Author:Rich Davison
Contact:richgdavison@gmail.com
License: MIT (see LICENSE file at the top of the source tree)
*//////////////////////////////////////////////////////////////////////////////
#include "Precompiled.h"
#include "VulkanTexture.h"
#include "VulkanRenderer.h"
#include "TextureLoader.h"
#include "VulkanUtils.h"

using namespace NCL;
using namespace Rendering;

VulkanRenderer* VulkanTexture::vkRenderer = nullptr;

VulkanTexture::VulkanTexture() {
	width		= 0;
	height		= 0;
	mipCount	= 0;
	layerCount	= 0;
	format		= vk::Format::eUndefined;
}

VulkanTexture::VulkanTexture(const std::string& name) : VulkanTexture() {
	char* texData = nullptr;
	int width = 0;
	int height = 0;
	int channels = 0;
	int flags = 0;
	TextureLoader::LoadTexture(name, texData, width, height, channels, flags);

	if (width == 0 || height == 0) {
		std::cout << __FUNCTION__ << " can't load texture " << name << "\n";
	}
	GenerateTextureFromDataInternal(width, height, channels, false, { texData }, name);
	delete texData;
}

VulkanTexture::~VulkanTexture() {
	vk::Device sourceDevice = vkRenderer->GetDevice();
	sourceDevice.freeMemory(deviceMem);
}

int VulkanTexture::CalculateMipCount(uint32_t width, uint32_t height) {
	if (height == 0 || width == 0) {
		return 0;
	}
	return (int)floor(log2(float(std::min(width, height)))) + 1;
}

void VulkanTexture::GenerateTextureFromDataInternal(uint32_t width, uint32_t height, uint32_t channelCount, bool isCube, std::vector<char*>dataSrcs, const std::string& debugName) {
	 vk::Format				format = vk::Format::eR8G8B8A8Unorm;
	 vk::ImageAspectFlags	aspect = vk::ImageAspectFlagBits::eColor;
	 vk::ImageLayout		layout = vk::ImageLayout::eShaderReadOnlyOptimal;
	 vk::ImageUsageFlags	usage  = vk::ImageUsageFlagBits::eSampled | vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eTransferSrc;

	 int mipCount = CalculateMipCount(width, height);
	 GenerateTextureInternal(width, height, mipCount, isCube, debugName, format, aspect, usage, layout, vk::PipelineStageFlagBits::eFragmentShader);

	 //tex is currently empty, need to fill it with our data from stbimage!
	 int faceSize = width * height * channelCount;
	 int allocationSize = faceSize * (int)dataSrcs.size();

	 vk::Device device = vkRenderer->GetDevice();

	 vk::Buffer	stagingBuffer = device.createBuffer(
		 vk::BufferCreateInfo({}, allocationSize, vk::BufferUsageFlagBits::eTransferSrc)
	 );
	 vk::MemoryRequirements	stagingReqs = {};
	 vk::MemoryAllocateInfo	stagingInfo = {};
	 vk::DeviceMemory		stagingMemory;

	 device.getBufferMemoryRequirements(stagingBuffer, &stagingReqs);

	 stagingInfo.setAllocationSize(stagingReqs.size);
	 vkRenderer->MemoryTypeFromPhysicalDeviceProps(vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, stagingReqs.memoryTypeBits, stagingInfo.memoryTypeIndex);

	 vk::CommandBuffer cmdBuffer = vkRenderer->BeginCmdBuffer();

	 stagingMemory = device.allocateMemory(stagingInfo);
	 device.bindBufferMemory(stagingBuffer, stagingMemory, 0);

	 //our buffer now has memory! Copy some texture date to it...
	 char* gpuPtr = (char*)device.mapMemory(stagingMemory, 0, allocationSize);
	 for (int i = 0; i < dataSrcs.size(); ++i) {
		 memcpy(gpuPtr, dataSrcs[i], faceSize);
		 gpuPtr += faceSize;

		 vkRenderer->ImageTransitionBarrier(cmdBuffer, this, vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal, aspectType, vk::PipelineStageFlagBits::eHost, vk::PipelineStageFlagBits::eTransfer, 0, i);
	 }
	 device.unmapMemory(stagingMemory);

	 vk::BufferImageCopy copyInfo;
	 copyInfo.imageSubresource.setAspectMask(vk::ImageAspectFlagBits::eColor).setMipLevel(0).setLayerCount((uint32_t)dataSrcs.size());
	 copyInfo.imageExtent = vk::Extent3D(width, height, 1);
	 copyInfo.bufferOffset = 0;

	 //Copy from staging buffer to image memory...
	 cmdBuffer.copyBufferToImage(stagingBuffer, *image, vk::ImageLayout::eTransferDstOptimal, copyInfo);

	 if (mipCount > 1) {
		 GenerateMipMaps(cmdBuffer, vk::ImageLayout::eShaderReadOnlyOptimal, vk::PipelineStageFlagBits::eFragmentShader);
	 }
	 else { 
		 vkRenderer->ImageTransitionBarrier(cmdBuffer, *image, vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eShaderReadOnlyOptimal, aspectType, vk::PipelineStageFlagBits::eTransfer, vk::PipelineStageFlagBits::eFragmentShader);
		 defaultView = GenerateDefaultView(aspectType);
	 }

	 vkRenderer->SubmitCmdBufferWait(cmdBuffer);

	 device.destroyBuffer(stagingBuffer); //These can't be destroyed until the cmd buffer has definately completed...
	 device.freeMemory(stagingMemory);
}
 //Compatability func
 TextureBase* VulkanTexture::TextureFromFilenameLoader(const std::string& name) {
	 UniqueVulkanTexture tex = TextureFromFile(name);
	 return tex.release();
 }

 UniqueVulkanTexture VulkanTexture::TextureFromFile(const std::string& name) {
	char* texData	= nullptr;
	int width		= 0;
	int height		= 0;
	int channels	= 0;
	int flags		= 0;
	TextureLoader::LoadTexture(name, texData, width, height, channels, flags);

	if (width == 0 || height == 0) {
		std::cout << __FUNCTION__ << " can't load texture " << name << "\n";
		return nullptr;
	}
	VulkanTexture* texture = new VulkanTexture();
	texture->GenerateTextureFromDataInternal(width, height, channels, false, { texData }, name);
	delete texData;
	return UniqueVulkanTexture(texture);
};

UniqueVulkanTexture VulkanTexture::VulkanCubemapFromFiles(
	const std::string& negativeXFile, const std::string& positiveXFile,
	const std::string& negativeYFile, const std::string& positiveYFile,
	const std::string& negativeZFile, const std::string& positiveZFile,
	const std::string& debugName) {

	std::vector<const string*> allFiles = { &negativeXFile, &positiveXFile, &negativeYFile, &positiveYFile, &negativeZFile, &positiveZFile };

	std::vector<char*> texData(6, nullptr);
	int width[6]	 = { 0 };
	int height[6]	 = { 0 };
	int channels[6]  = { 0 };
	int flags[6]	 = { 0 };

	bool loadedData = true;
	for (int i = 0; i < 6; ++i) {
		TextureLoader::LoadTexture(*(allFiles[i]), texData[i], width[i], height[i], channels[i], flags[i]);
		if (i > 0 && (width[i] != width[0] || height[0] != height[0])) {
			std::cout << __FUNCTION__ << " cubemap input textures don't match in size?\n";
			loadedData = false;
			break;
		}
		if (width[i] == 0 || height[i] == 0) {
			std::cout << __FUNCTION__ << " can't load cubemap " << *allFiles[i] << "\n";
			loadedData = false;
			break;
		}
	}
	if (!loadedData) {
		for (int i = 0; i < 6; ++i) {
			delete texData[i];
		}
		return nullptr;
	}
	VulkanTexture* cubeTex = new VulkanTexture();
	cubeTex->GenerateTextureFromDataInternal(width[0], height[0], channels[0], true, texData, debugName);

	//delete the old texData;
	for (int i = 0; i < 6; ++i) {
		delete texData[i];
	}

	return UniqueVulkanTexture(cubeTex);
}

void	VulkanTexture::InitTextureDeviceMemory(VulkanTexture& img) {
	vk::MemoryRequirements memReqs = vkRenderer->GetDevice().getImageMemoryRequirements(*img.image);

	img.allocInfo = vk::MemoryAllocateInfo(memReqs.size);

	bool found = vkRenderer->MemoryTypeFromPhysicalDeviceProps({}, memReqs.memoryTypeBits, img.allocInfo.memoryTypeIndex);

	img.deviceMem = vkRenderer->GetDevice().allocateMemory(img.allocInfo);

	vkRenderer->GetDevice().bindImageMemory(*img.image, img.deviceMem, 0);
}

void VulkanTexture::GenerateTextureInternal(uint32_t texWidth, uint32_t texHeight, uint32_t texMipCount, bool texIsCubemap, const std::string& debugName, vk::Format texFormat, vk::ImageAspectFlags texAspect, vk::ImageUsageFlags texUsage, vk::ImageLayout outLayout, vk::PipelineStageFlags pipeType) {
	width		= texWidth;
	height		= texHeight;
	mipCount	= texMipCount;
	format		= texFormat;
	aspectType = texAspect;
	layerCount = 1;

	createInfo = vk::ImageCreateInfo()
		.setImageType(vk::ImageType::e2D)
		.setExtent(vk::Extent3D(texWidth, texHeight, 1))
		.setFormat(texFormat)
		.setUsage(texUsage)
		.setMipLevels(mipCount)
		.setArrayLayers(1)
		.setImageType(vk::ImageType::e2D);

	if (texIsCubemap) {
		createInfo.setArrayLayers(6).setFlags(vk::ImageCreateFlagBits::eCubeCompatible);
		layerCount = 6;
	}

	image = vkRenderer->GetDevice().createImageUnique(createInfo);

	InitTextureDeviceMemory(*this);

	defaultView = GenerateDefaultView(aspectType);

	Vulkan::SetDebugName(vkRenderer->GetDevice(), vk::ObjectType::eImage, Vulkan::GetVulkanHandle(*image), debugName);
	Vulkan::SetDebugName(vkRenderer->GetDevice(),vk::ObjectType::eImageView, Vulkan::GetVulkanHandle(*defaultView), debugName);

	vk::CommandBuffer tempBuffer = vkRenderer->BeginCmdBuffer();
	vkRenderer->ImageTransitionBarrier(tempBuffer, this, vk::ImageLayout::eUndefined, outLayout, aspectType, vk::PipelineStageFlagBits::eTopOfPipe, pipeType);
	vkRenderer->SubmitCmdBufferWait(tempBuffer);
}

UniqueVulkanTexture VulkanTexture::CreateDepthTexture(uint32_t width, uint32_t height, const std::string& debugName, bool hasStencil, bool useMips) {
	vk::Format			 format		= hasStencil ? vk::Format::eD24UnormS8Uint : vk::Format::eD32Sfloat;
	vk::ImageAspectFlags aspect		= hasStencil ? vk::ImageAspectFlagBits::eDepth | vk::ImageAspectFlagBits::eStencil : vk::ImageAspectFlagBits::eDepth;
	vk::ImageLayout		 layout		= hasStencil ? vk::ImageLayout::eDepthStencilAttachmentOptimal : vk::ImageLayout::eDepthAttachmentOptimal;	
	vk::ImageUsageFlags  usage		= vk::ImageUsageFlagBits::eDepthStencilAttachment | vk::ImageUsageFlagBits::eSampled;

	VulkanTexture* t = new VulkanTexture();
	t->GenerateTextureInternal(width, height, 1, false, debugName, format, aspect, usage, layout, vk::PipelineStageFlagBits::eEarlyFragmentTests);
	return UniqueVulkanTexture(t);
}

UniqueVulkanTexture VulkanTexture::CreateColourTexture(uint32_t width, uint32_t height, const std::string& debugName, bool isFloat, bool useMips) {
	vk::Format			 format		= isFloat ? vk::Format::eR32G32B32A32Sfloat : vk::Format::eB8G8R8A8Unorm;
	vk::ImageAspectFlags aspect		= vk::ImageAspectFlagBits::eColor;
	vk::ImageLayout		 layout		= vk::ImageLayout::eColorAttachmentOptimal;	
	vk::ImageUsageFlags  usage		= vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eSampled;
	VulkanTexture* t = new VulkanTexture();
	t->GenerateTextureInternal(width, height, 1, false, debugName, format, aspect, usage, layout, vk::PipelineStageFlagBits::eColorAttachmentOutput);
	return UniqueVulkanTexture(t);
}

vk::UniqueImageView  VulkanTexture::GenerateDefaultView(vk::ImageAspectFlags type) {
	vk::ImageViewCreateInfo createInfo =  vk::ImageViewCreateInfo()
		.setViewType(layerCount == 6 ? vk::ImageViewType::eCube : vk::ImageViewType::e2D)
		.setFormat(format)
		.setSubresourceRange(vk::ImageSubresourceRange(type, 0, mipCount, 0, layerCount))
		.setImage(*image);
	return vkRenderer->GetDevice().createImageViewUnique(createInfo);
}

void VulkanTexture::GenerateMipMaps(vk::CommandBuffer  buffer, vk::ImageLayout endLayout, vk::PipelineStageFlags endFlags) {
	bool localCmdBuffer = false;
	if (!buffer) {
		buffer = vkRenderer->BeginCmdBuffer();
		localCmdBuffer = true;
	}

	for (int layer = 0; layer < layerCount; ++layer) {	
		vkRenderer->ImageTransitionBarrier(buffer, this, vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferSrcOptimal, aspectType, vk::PipelineStageFlagBits::eAllCommands, vk::PipelineStageFlagBits::eTransfer, 0, layer);
		for (int mip = 1; mip < mipCount; ++mip) {

			vk::ImageBlit blitData;
			blitData.srcSubresource.setAspectMask(vk::ImageAspectFlagBits::eColor)
			.setMipLevel(mip - 1)
			.setBaseArrayLayer(layer)
			.setLayerCount(1);
			blitData.srcOffsets[0] = vk::Offset3D(0, 0, 0);
			blitData.srcOffsets[1].x = std::max(width >> (mip - 1), 1);
			blitData.srcOffsets[1].y = std::max(height >> (mip - 1), 1);
			blitData.srcOffsets[1].z = 1;

			blitData.dstSubresource.setAspectMask(vk::ImageAspectFlagBits::eColor)
			.setMipLevel(mip)
			.setLayerCount(1)
			.setBaseArrayLayer(layer);
			blitData.dstOffsets[0] = vk::Offset3D(0, 0, 0);
			blitData.dstOffsets[1].x = std::max(width >> mip, 1);
			blitData.dstOffsets[1].y = std::max(height >> mip, 1);
			blitData.dstOffsets[1].z = 1;

			vkRenderer->ImageTransitionBarrier(buffer, this, vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal, aspectType, vk::PipelineStageFlagBits::eHost, vk::PipelineStageFlagBits::eTransfer, mip, layer);
			buffer.blitImage(*image, vk::ImageLayout::eTransferSrcOptimal, *image, vk::ImageLayout::eTransferDstOptimal, blitData, vk::Filter::eLinear);
			vkRenderer->ImageTransitionBarrier(buffer, *image, vk::ImageLayout::eTransferSrcOptimal, endLayout, aspectType, vk::PipelineStageFlagBits::eTransfer, endFlags, mip - 1, layer);

			if (mip < this->mipCount - 1) {
				vkRenderer->ImageTransitionBarrier(buffer, *image, vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eTransferSrcOptimal, aspectType, vk::PipelineStageFlagBits::eTransfer, vk::PipelineStageFlagBits::eTransfer, mip, layer);
			}
			else {
				vkRenderer->ImageTransitionBarrier(buffer, *image, vk::ImageLayout::eTransferDstOptimal, endLayout, aspectType, vk::PipelineStageFlagBits::eTransfer, endFlags, mip, layer);
			}
		}
	}
	if (localCmdBuffer) {
		vkRenderer->SubmitCmdBufferWait(buffer);
	}

	defaultView = GenerateDefaultView(aspectType);
}
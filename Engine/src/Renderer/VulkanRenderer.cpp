//#include "VulkanRenderer.h"
//#include "Core/Log.h"
//#include "Core/Window.h"
//#include "Shader.h"
//
//#include <GLFW/glfw3.h>
//
//namespace Wizard {
//    
//    VulkanRenderer::VulkanRenderer()
//    {
//        Init();
//    }
//
//    VulkanRenderer::~VulkanRenderer()
//    {
//        Shutdown();
//    }
//
//    void VulkanRenderer::Init()
//    {
//        CreateInstance();
//        CreateSurface();
//        PickPhysicalDevice();
//        CreateLogicalDevice();
//        CreateSwapChain();
//        CreateImageViews();
//
//        CreateRenderPass();
//        CreatePipeline();
//        CreateFramebuffers();
//        CreateCommandPool();
//    }
//
//    void VulkanRenderer::Shutdown()
//    {
//        vkDestroyCommandPool(m_Device, m_CommandPool, nullptr);
//        for (int i = 0; i < m_Framebuffers.size(); ++i) {
//            vkDestroyFramebuffer(m_Device, m_Framebuffers[i], nullptr);
//        }
//        vkDestroyPipeline(m_Device, m_GraphicsPipeline, nullptr);
//        vkDestroyRenderPass(m_Device, m_RenderPass, nullptr);
//        vkDestroyPipelineLayout(m_Device, m_PipelineLayout, nullptr);
//        for (int i = 0; i < m_SwapChainImageViews.size(); ++i) {
//            vkDestroyImageView(m_Device, m_SwapChainImageViews[i], nullptr);
//        }
//        vkDestroySwapchainKHR(m_Device, m_SwapChain, nullptr);
//        vkDestroyDevice(m_Device, nullptr);
//        vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);
//        vkDestroyInstance(m_Instance, nullptr);
//    }
//
//    void VulkanRenderer::DrawFrame()
//    {
//
//    }
//
//    void VulkanRenderer::CreateSurface()
//    {
//        if (glfwCreateWindowSurface(m_Instance, Window::Get()->GetWindow(), nullptr, &m_Surface) != VK_SUCCESS) {
//            WZ_ENGINE_ERROR("Failed To Create Window Surface");
//            return;
//        }
//    }
//
//    void VulkanRenderer::CreateInstance()
//    {
//#ifdef WZ_DEBUG
//        if (!CheckValidationSupport()) {
//            WZ_ENGINE_ERROR("Vulkan Validation Requested But Not Found");
//        }
//#endif
//
//        VkApplicationInfo appInfo {};
//        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
//        appInfo.pApplicationName = "Wizard";
//        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
//        appInfo.pEngineName = "Winzard Engine";
//        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
//        appInfo.apiVersion = VK_API_VERSION_1_3;
//
//        VkInstanceCreateInfo instanceCreateInfo {};
//        instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
//        instanceCreateInfo.pApplicationInfo = &appInfo;
//
//#ifdef WZ_APPLE
//        instanceCreateInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
//#endif
//
//        std::vector<const char*> extensions = GetRequireInstanceExtensions();
//
//        instanceCreateInfo.enabledExtensionCount = extensions.size();
//        instanceCreateInfo.ppEnabledExtensionNames = extensions.data();
//
//#ifdef WZ_DEBUG
//        instanceCreateInfo.enabledLayerCount = m_ValidationLayers.size();
//        instanceCreateInfo.ppEnabledLayerNames = m_ValidationLayers.data();
//#else
//        instanceCreateInfo.enabledLayerCount = 0;
//#endif
//        VkResult result = vkCreateInstance(&instanceCreateInfo, nullptr, &m_Instance);
//        if (result != VK_SUCCESS) {
//            WZ_ENGINE_ERROR("Failed When Createing Vulkan Instance Error Code: {}", result);
//            return;
//        }
//    }
//
//    std::vector<const char*> VulkanRenderer::GetRequireInstanceExtensions()
//    {
//        uint32_t extensionCount;
//        const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&extensionCount);
//
//        std::vector<const char*> extensions(glfwExtensions, glfwExtensions + extensionCount);
//
//#ifdef WZ_APPLE
//        extensions.emplace_back("VK_KHR_portability_enumeration");
//#endif
//
//        return extensions;
//    }
//
//    bool VulkanRenderer::CheckValidationSupport()
//    {
//#ifdef WZ_DEBUG
//        uint32_t layerCount;
//        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
//        std::vector<VkLayerProperties> availabalLayers(layerCount);
//        vkEnumerateInstanceLayerProperties(&layerCount, availabalLayers.data());
//        
//        for (const char* name : m_ValidationLayers) {
//            bool layerFound = false;
//            for (const auto& layerProperties : availabalLayers) {
//                if (strcmp(name, layerProperties.layerName) == 0) {
//                    layerFound = true;
//                    break;
//                }
//            }
//
//            if (!layerFound)
//                return false;
//        }
//#endif
//        return true;
//    }
//
//    void VulkanRenderer::PickPhysicalDevice()
//    {
//        uint32_t deviceCount;
//        vkEnumeratePhysicalDevices(m_Instance, &deviceCount, nullptr);
//        if (deviceCount == 0) {
//            WZ_ENGINE_ERROR("Can't Find GPUs Support");
//            return;
//        }
//        std::vector<VkPhysicalDevice> devices(deviceCount);
//        vkEnumeratePhysicalDevices(m_Instance, &deviceCount, devices.data());
//
//        for (int i = 0; i < devices.size(); ++i) {
//            if (IsDeviceSuitable(devices[i])) {
//                m_PhysicalDevice = devices[i];
//                break;
//            }
//        }
//
//        if (m_PhysicalDevice == VK_NULL_HANDLE) {
//            WZ_ENGINE_ERROR("Can't Find Suitable PhysicalDevice");
//            return;
//        }
//    }
//
//    bool VulkanRenderer::IsDeviceSuitable(VkPhysicalDevice device)
//    {
//        FindQueueFamilies(device);
//        bool deviceExtensionSupport = CheckDeviceExtensionSupport(device);
//
//        bool swapChainAdequate = false;
//        if (deviceExtensionSupport) {
//            QuerySwapChainSupport(device);
//            swapChainAdequate = !m_SwapChainDetails.formats.empty() && !m_SwapChainDetails.presentModes.empty();
//        }
//        return m_QueueFamilies.IsComplete() && deviceExtensionSupport && swapChainAdequate;
//    }
//
//    void VulkanRenderer::FindQueueFamilies(VkPhysicalDevice device) 
//    {
//        uint32_t queueFamilyCount = 0;
//        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
//        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
//        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());
//
//        for (int i = 0; i < queueFamilies.size(); ++i) {
//            if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
//                m_QueueFamilies.graphicsFamily = i;
//            }
//
//            VkBool32 presentSupport = false;
//            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_Surface, &presentSupport);
//            if (presentSupport) {
//                m_QueueFamilies.presentFamily = i;
//            }
//
//            if (m_QueueFamilies.IsComplete()) {
//                break;
//            }
//        }
//    }
//
//    bool VulkanRenderer::CheckDeviceExtensionSupport(VkPhysicalDevice device)
//    {
//        uint32_t extensionCount;
//        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
//
//        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
//        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());
//        
//        // TODO: could be a better code to optmize
//        std::set<std::string> requiredExtensions(m_DeviceExtensions.begin(), m_DeviceExtensions.end());
//        for (const auto& extension : availableExtensions) {
//            requiredExtensions.erase(extension.extensionName);
//        }
//
//        return requiredExtensions.empty();
//    }
//
//    void VulkanRenderer::CreateLogicalDevice()
//    {
//        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos { };
//        std::set<uint32_t> uniqueQueueFamilies = {
//            m_QueueFamilies.graphicsFamily.value(),
//            m_QueueFamilies.presentFamily.value()
//        };
//
//        float queuePriority = 1.0f;
//        for (uint32_t queueFamily : uniqueQueueFamilies) {
//            VkDeviceQueueCreateInfo queueCreateInfo {};
//            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
//            queueCreateInfo.queueFamilyIndex = queueFamily;
//            queueCreateInfo.queueCount = 1;
//            queueCreateInfo.pQueuePriorities = &queuePriority;
//            queueCreateInfos.push_back(queueCreateInfo);
//        }
//
//        VkPhysicalDeviceFeatures deviceFeatures { };
//
//        VkDeviceCreateInfo createInfo { };
//        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
//        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
//        createInfo.pQueueCreateInfos = queueCreateInfos.data();
//        createInfo.pEnabledFeatures = &deviceFeatures;
//        createInfo.enabledExtensionCount = m_DeviceExtensions.size();
//        createInfo.ppEnabledExtensionNames = m_DeviceExtensions.data();
//
//        VkResult result = vkCreateDevice(m_PhysicalDevice, &createInfo, nullptr, &m_Device);
//        if (result != VK_SUCCESS) {
//            WZ_ENGINE_ERROR("Failed To Create Logical Device Error Code: {}", result);
//            return;
//        }
//
//        vkGetDeviceQueue(m_Device, m_QueueFamilies.graphicsFamily.value(), 0, &m_GraphicsQueue);
//        vkGetDeviceQueue(m_Device, m_QueueFamilies.presentFamily.value(), 0, &m_PresentQueue);
//    }
//
//    void VulkanRenderer::CreateSwapChain()
//    {
//        QuerySwapChainSupport(m_PhysicalDevice);
//
//        VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat();
//        VkPresentModeKHR presentMode = ChooseSwapPresentMode();
//        m_Extent = ChooseSwapExtent();
//        m_SwapChainFormat = surfaceFormat.format;
//
//        uint32_t imageCount = m_SwapChainDetails.capabilities.minImageCount + 1;
//        if (m_SwapChainDetails.capabilities.maxImageCount > 0 && imageCount > m_SwapChainDetails.capabilities.maxImageCount) {
//            imageCount = m_SwapChainDetails.capabilities.maxImageCount;
//        }
//
//        VkSwapchainCreateInfoKHR createInfo{};
//        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
//        createInfo.surface = m_Surface;
//        createInfo.minImageCount = imageCount;
//        createInfo.imageFormat = m_SwapChainFormat;
//        createInfo.imageColorSpace = surfaceFormat.colorSpace;
//        createInfo.imageExtent = m_Extent;
//        createInfo.imageArrayLayers = 1;
//        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
//
//        uint32_t indices[] = {
//            m_QueueFamilies.graphicsFamily.value(),
//            m_QueueFamilies.presentFamily.value()
//        };
//
//        if (m_QueueFamilies.graphicsFamily != m_QueueFamilies.presentFamily) {
//            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
//            createInfo.queueFamilyIndexCount = 2;
//            createInfo.pQueueFamilyIndices = indices;
//        }
//        else {
//            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
//            createInfo.queueFamilyIndexCount = 0;
//            createInfo.pQueueFamilyIndices = nullptr;
//        }
//        
//        createInfo.preTransform = m_SwapChainDetails.capabilities.currentTransform;
//        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
//        createInfo.presentMode = presentMode;
//        createInfo.clipped = VK_TRUE;
//        createInfo.oldSwapchain = VK_NULL_HANDLE;
//
//        VkResult result = vkCreateSwapchainKHR(m_Device, &createInfo, nullptr, &m_SwapChain);
//        if (result != VK_SUCCESS) {
//            WZ_ENGINE_ERROR("Failed To Create Vulkan SwapChain");
//            return;
//        }
//
//        vkGetSwapchainImagesKHR(m_Device, m_SwapChain, &imageCount, nullptr);
//        m_SwapChainImages.resize(imageCount);
//        vkGetSwapchainImagesKHR(m_Device, m_SwapChain, &imageCount, m_SwapChainImages.data());
//    }
//
//    void VulkanRenderer::CreateImageViews()
//    {
//        m_SwapChainImageViews.resize(m_SwapChainImages.size());
//        for (int i = 0; i < m_SwapChainImages.size(); ++i) {
//            VkImageViewCreateInfo createInfo{};
//            createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
//            createInfo.image = m_SwapChainImages[i];
//            createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
//            createInfo.format = m_SwapChainFormat;
//            createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
//            createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
//            createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
//            createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
//            createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
//            createInfo.subresourceRange.baseMipLevel = 0;
//            createInfo.subresourceRange.levelCount = 1;
//            createInfo.subresourceRange.baseArrayLayer = 0;
//            createInfo.subresourceRange.layerCount = 1;
//            if (vkCreateImageView(m_Device, &createInfo, nullptr, &m_SwapChainImageViews[i]) != VK_SUCCESS) {
//                WZ_ENGINE_ERROR("Failed To Create Image Views");
//                return;
//            }
//        }
//    }
//
//    void VulkanRenderer::QuerySwapChainSupport(VkPhysicalDevice device)
//    {
//        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, m_Surface, &m_SwapChainDetails.capabilities);
//        uint32_t formatCount;
//        vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_Surface, &formatCount, nullptr);
//
//        if (formatCount != 0) {
//            m_SwapChainDetails.formats.resize(formatCount);
//            vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_Surface, &formatCount, m_SwapChainDetails.formats.data());
//        }
//
//        uint32_t presentModeCount;
//        vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_Surface, &presentModeCount, nullptr);
//
//        if (presentModeCount != 0) {
//            m_SwapChainDetails.presentModes.resize(presentModeCount);
//            vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_Surface, &presentModeCount, m_SwapChainDetails.presentModes.data());
//        }
//    }
//
//    VkSurfaceFormatKHR VulkanRenderer::ChooseSwapSurfaceFormat()
//    {
//        for (const auto& availableFormat : m_SwapChainDetails.formats) {
//            if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
//                return availableFormat;
//            }
//        }
//
//        return m_SwapChainDetails.formats[0];
//    }
//
//    VkPresentModeKHR VulkanRenderer::ChooseSwapPresentMode()
//    {
//        for (const auto& availablePresentMode : m_SwapChainDetails.presentModes) {
//            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
//                return availablePresentMode;
//            }
//        }
//
//        return VK_PRESENT_MODE_FIFO_KHR;
//    }
//
//    VkExtent2D VulkanRenderer::ChooseSwapExtent()
//    {
//        if (m_SwapChainDetails.capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
//            return m_SwapChainDetails.capabilities.currentExtent;
//        }
//        else {
//            int width;
//            int height;
//            glfwGetFramebufferSize(Window::Get()->GetWindow(), &width, &height);
//            
//            VkExtent2D actualExtent = {
//                (uint32_t)width,
//                (uint32_t)height
//            };
//
//            actualExtent.width = std::clamp(actualExtent.width, 
//                m_SwapChainDetails.capabilities.minImageExtent.width,
//                m_SwapChainDetails.capabilities.maxImageExtent.width);
//
//            actualExtent.height = std::clamp(actualExtent.height,
//                m_SwapChainDetails.capabilities.minImageExtent.height,
//                m_SwapChainDetails.capabilities.maxImageExtent.height);
//
//            return actualExtent;
//        }
//    }
//
//    void VulkanRenderer::CreateRenderPass()
//    {
//        VkAttachmentDescription colorAttachment{};
//        colorAttachment.format = m_SwapChainFormat;
//        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
//        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
//        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
//        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
//        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
//        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
//        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
//
//        VkAttachmentReference colorAttachmentRef{};
//        colorAttachmentRef.attachment = 0;
//        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
//
//        VkSubpassDescription subpass{};
//        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
//        subpass.colorAttachmentCount = 1;
//        subpass.pColorAttachments = &colorAttachmentRef;
//
//        VkRenderPassCreateInfo renderPassInfo{};
//        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
//        renderPassInfo.attachmentCount = 1;
//        renderPassInfo.pAttachments = &colorAttachment;
//        renderPassInfo.subpassCount = 1;
//        renderPassInfo.pSubpasses = &subpass;
//
//        VkResult result = vkCreateRenderPass(m_Device, &renderPassInfo, nullptr, &m_RenderPass);
//        if (result != VK_SUCCESS) {
//            WZ_ENGINE_ERROR("Failed When Creating RenderPass");
//            return;
//        }
//    }
//
//    void VulkanRenderer::CreatePipeline()
//    {
//        std::vector<char> vertShaderCode = Shader::ReadByteCode("C:/Dev/Wizard/Engine/datas/shaders/vert.spv");
//        std::vector<char> fragShaderCode = Shader::ReadByteCode("C:/Dev/Wizard/Engine/datas/shaders/frag.spv");
//
//        VkShaderModule vertShaderModule = CreateShaderModule(vertShaderCode);
//        VkShaderModule fragShaderModule = CreateShaderModule(fragShaderCode);
//
//        VkPipelineShaderStageCreateInfo vertShaderStageCreateInfo{};
//        vertShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
//        vertShaderStageCreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
//        vertShaderStageCreateInfo.module = vertShaderModule;
//        vertShaderStageCreateInfo.pName = "main";
//
//        VkPipelineShaderStageCreateInfo fragShaderStageCreateInfo{};
//        fragShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
//        fragShaderStageCreateInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
//        fragShaderStageCreateInfo.module = fragShaderModule;
//        fragShaderStageCreateInfo.pName = "main";
//
//        VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageCreateInfo, fragShaderStageCreateInfo };
//
//        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
//        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
//        vertexInputInfo.vertexBindingDescriptionCount = 0;
//        vertexInputInfo.pVertexBindingDescriptions = nullptr;
//        vertexInputInfo.vertexAttributeDescriptionCount = 0;
//        vertexInputInfo.pVertexAttributeDescriptions = nullptr;
//
//        std::vector<VkDynamicState> dynamicStates = { 
//            VK_DYNAMIC_STATE_VIEWPORT,
//            VK_DYNAMIC_STATE_SCISSOR
//        };
//
//        VkPipelineDynamicStateCreateInfo  dynamicState{};
//        dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
//        dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
//        dynamicState.pDynamicStates = dynamicStates.data();
//
//        VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
//        inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
//        inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
//        inputAssembly.primitiveRestartEnable = VK_FALSE;
//
//        VkViewport viewport{};
//        viewport.x = 0.f;
//        viewport.y = 0.f;
//        viewport.width = (float)m_Extent.width;
//        viewport.height = (float)m_Extent.height;
//        viewport.minDepth = 1.f;
//        viewport.maxDepth = 1.f;
//
//        VkRect2D scissor{};
//        scissor.offset = { 0 , 0 };
//        scissor.extent = m_Extent;
//
//        VkPipelineViewportStateCreateInfo viewportState{};
//        viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
//        viewportState.viewportCount = 1;
//        viewportState.scissorCount = 1;
//
//        VkPipelineRasterizationStateCreateInfo rasterizer{};
//        rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
//        rasterizer.depthClampEnable = VK_FALSE;
//        rasterizer.rasterizerDiscardEnable = VK_FALSE;
//        rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
//        rasterizer.lineWidth = 1.0f;
//        rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
//        rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
//        rasterizer.depthBiasEnable = VK_FALSE;
//        rasterizer.depthBiasConstantFactor = 0.0f;
//        rasterizer.depthBiasClamp = 0.0f; 
//        rasterizer.depthBiasSlopeFactor = 0.0f; 
//
//        VkPipelineMultisampleStateCreateInfo multisampling{};
//        multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
//        multisampling.sampleShadingEnable = VK_FALSE;
//        multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
//        multisampling.minSampleShading = 1.0f;
//        multisampling.pSampleMask = nullptr;
//        multisampling.alphaToCoverageEnable = VK_FALSE;
//        multisampling.alphaToOneEnable = VK_FALSE;
//
//        VkPipelineColorBlendAttachmentState colorBlendAttachment{};
//        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
//        colorBlendAttachment.blendEnable = VK_FALSE;
//        colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
//        colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
//        colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
//        colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
//        colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
//        colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
//
//        VkPipelineColorBlendStateCreateInfo colorBlending{};
//        colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
//        colorBlending.logicOpEnable = VK_FALSE;
//        colorBlending.logicOp = VK_LOGIC_OP_COPY;
//        colorBlending.attachmentCount = 1;
//        colorBlending.pAttachments = &colorBlendAttachment;
//        colorBlending.blendConstants[0] = 0.0f;
//        colorBlending.blendConstants[1] = 0.0f;
//        colorBlending.blendConstants[2] = 0.0f;
//        colorBlending.blendConstants[3] = 0.0f;
//
//        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
//        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
//        pipelineLayoutInfo.setLayoutCount = 0;
//        pipelineLayoutInfo.pSetLayouts = nullptr;
//        pipelineLayoutInfo.pushConstantRangeCount = 0;
//        pipelineLayoutInfo.pPushConstantRanges = nullptr;
//
//        
//        VkResult result = vkCreatePipelineLayout(m_Device, &pipelineLayoutInfo, nullptr, &m_PipelineLayout);
//        if (result != VK_SUCCESS) {
//            WZ_ENGINE_ERROR("Faild When Creating PipelineLayout");
//            return;
//        }
//
//        VkGraphicsPipelineCreateInfo pipelineInfo{};
//        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
//        pipelineInfo.stageCount = 2;
//        pipelineInfo.pStages = shaderStages;
//        pipelineInfo.pVertexInputState = &vertexInputInfo;
//        pipelineInfo.pInputAssemblyState = &inputAssembly;
//        pipelineInfo.pViewportState = &viewportState;
//        pipelineInfo.pRasterizationState = &rasterizer;
//        pipelineInfo.pMultisampleState = &multisampling;
//        pipelineInfo.pDepthStencilState = nullptr;
//        pipelineInfo.pColorBlendState = &colorBlending;
//        pipelineInfo.pDynamicState = &dynamicState;
//        pipelineInfo.layout = m_PipelineLayout;
//        pipelineInfo.renderPass = m_RenderPass;
//        pipelineInfo.subpass = 0;
//
//        result = vkCreateGraphicsPipelines(m_Device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_GraphicsPipeline);
//        if (result != VK_SUCCESS) {
//            WZ_ENGINE_ERROR("Failed When Creating Render Pipeline {}", result);
//        }
//
//        vkDestroyShaderModule(m_Device, vertShaderModule, nullptr);
//        vkDestroyShaderModule(m_Device, fragShaderModule, nullptr);
//    }
//
//    void VulkanRenderer::CreateFramebuffers()
//    {
//        m_Framebuffers.resize(m_SwapChainImageViews.size());
//        for (int i = 0; i < m_Framebuffers.size(); ++i) {
//            VkImageView attachments[] = {
//                m_SwapChainImageViews[i]
//            };
//
//            VkFramebufferCreateInfo framebufferInfo{};
//            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
//            framebufferInfo.renderPass = m_RenderPass;
//            framebufferInfo.attachmentCount = 1;
//            framebufferInfo.pAttachments = attachments;
//            framebufferInfo.width = m_Extent.width;
//            framebufferInfo.height = m_Extent.height;
//            framebufferInfo.layers = 1;
//
//            VkResult result = vkCreateFramebuffer(m_Device, &framebufferInfo, nullptr, &m_Framebuffers[i]);
//            if (result != VK_SUCCESS) {
//                WZ_ENGINE_ERROR("Failed When Createing VkFramebuffer");
//                return;
//            }
//        }
//    }
//
//    void VulkanRenderer::CreateCommandPool()
//    {
//        VkCommandPoolCreateInfo poolInfo{};
//        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
//        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
//        poolInfo.queueFamilyIndex = m_QueueFamilies.graphicsFamily.value();
//        VkResult result = vkCreateCommandPool(m_Device, &poolInfo, nullptr, &m_CommandPool);
//        if (result != VK_SUCCESS) {
//            WZ_ENGINE_ERROR("Failed When Creating Command Pool");
//        }
//    }
//
//    void VulkanRenderer::CreateCommandBuffer()
//    {
//        VkCommandBufferAllocateInfo info{};
//        info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
//        info.commandPool = m_CommandPool;
//        info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
//        info.commandBufferCount = 1;
//        VkResult result = vkAllocateCommandBuffers(m_Device, &info, &m_CommandBuffer);
//        if (result != VK_SUCCESS) {
//            WZ_ENGINE_ERROR("Failed When Creating Command Buffer");
//        }
//    }
//
//    void VulkanRenderer::RecordCommandBuffer(VkCommandBuffer buffer, uint32_t imageIndex)
//    {
//        VkCommandBufferBeginInfo begininfo{};
//        begininfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
//        begininfo.flags = 0;
//        begininfo.pInheritanceInfo = 0;
//        if (vkBeginCommandBuffer(buffer, &begininfo) != VK_SUCCESS) {
//            WZ_ENGINE_ERROR("Error When Begin CommandBuffer");
//        }
//
//        VkRenderPassBeginInfo renderpassInfo{};
//        renderpassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
//        renderpassInfo.renderPass = m_RenderPass;
//        renderpassInfo.framebuffer = m_Framebuffers[imageIndex];
//        renderpassInfo.renderArea.offset = { 0,0 };
//        renderpassInfo.renderArea.extent = m_Extent;
//
//        VkClearValue  clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
//        renderpassInfo.clearValueCount = 1;
//        renderpassInfo.pClearValues = &clearColor;
//
//        vkCmdBeginRenderPass(buffer, &renderpassInfo, VK_SUBPASS_CONTENTS_INLINE);
//
//        vkCmdBindPipeline(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_GraphicsPipeline);
//
//        VkViewport viewport{};
//        viewport.x = 0.0f;
//        viewport.y = 0.0f;
//        viewport.width = static_cast<float>(m_Extent.width);
//        viewport.height = static_cast<float>(m_Extent.height);
//        viewport.minDepth = 0.0f;
//        viewport.maxDepth = 1.0f;
//        vkCmdSetViewport(buffer, 0, 1, &viewport);
//
//        VkRect2D scissor{};
//        scissor.offset = { 0, 0 };
//        scissor.extent = m_Extent;
//        vkCmdSetScissor(buffer, 0, 1, &scissor);
//        vkCmdDraw(buffer, 3, 1, 0, 0);
//
//        vkCmdEndRenderPass(buffer);
//
//        if (vkEndCommandBuffer(buffer) != VK_SUCCESS) {
//            WZ_ENGINE_ERROR("Failed To Record Command Buffer!");
//        }
//    }
//
//    VkShaderModule VulkanRenderer::CreateShaderModule(std::vector<char>& bytecode)
//    {
//        VkShaderModuleCreateInfo createInfo{};
//        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
//        createInfo.codeSize = bytecode.size();
//        createInfo.pCode = (const uint32_t*)bytecode.data();
//
//        VkShaderModule shaderModule;
//        VkResult result = vkCreateShaderModule(m_Device, &createInfo, nullptr, &shaderModule);
//        if (result != VK_SUCCESS) {
//            WZ_ENGINE_ERROR("Failed When Creating Shader Module");
//            return nullptr;
//        }
//
//        return shaderModule;
//    }
//}
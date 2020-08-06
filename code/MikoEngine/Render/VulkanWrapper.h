#pragma once

/*=============================================================================
Документация
===============================================================================
TODO: возможно вынести в отдельный заголовок VulkanWrapper_doc.h

	---------------------------------------------------------------------------
	Глобальные переменные
	---------------------------------------------------------------------------
		enableValidationLayers - включается слоя проверок
			TODO: возможно удалить. включать в создании рендера

	---------------------------------------------------------------------------
	Классы
	---------------------------------------------------------------------------
	Extensions - класс позволяющий включать и проверять расширения в вулкане
		Используется в Instance
		Методы:
		TODO:
			убрать в cpp, пользователю недоступен

	ValidationLayers - класс позволяющий включать и проверять слои в вулкане
		Используется в Instance
		Методы:
		TODO:
			убрать в cpp, пользователю недоступен

	PhysicalDevice

	Instance - класс обертка над VkInstance
		Включает расширения и слои. 
		Создает VkInstance
		Создает Surface
		Возвращает список физических устройств и может выбрать оптимальный
		TODO:
			убрать метод CreateDebugMessenger (создавать при инициализации)
			включить возможность установки расширений и слоев

	VkObject / VkInstanceObject / VkDeviceObject - базовый класс от которого наследуются все ресурсы в данном пространстве имен
		VkInstanceObject хранит указатель на Instance 
		VkDeviceObject хранит указатель на Virtual Device

	DebugMessenger - класс включающий сообщения от слоя проверок.
		Используется в Instance

	Surface


*/

#if SE_VULKAN

namespace vkWrapper
{
#if SE_DEBUG
	const bool enableValidationLayers = true; // удалить когда исправлю Extensions::Default()
#else
	const bool enableValidationLayers = false;
#endif

	static const int MAX_FRAMES_IN_FLIGHT = 3; // TODO:

	struct QueueFamilyIndices
	{
		bool IsComplete()
		{
			return graphicsFamily.has_value() && presentFamily.has_value() && transferFamily.has_value();
		}

		std::set<uint32_t> GetQueueFamilySet()
		{
			std::set<uint32_t> res = { graphicsFamily.value(), presentFamily.value(), transferFamily.value() };
			return res;
		}

		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;
		std::optional<uint32_t> transferFamily;
	};

	struct QueueInfos // TODO: переименовать в QueueFamilyIndices???
	{
		// Most ideal queue = 3
		// Second most ideal queue = 2
		// Queue for minimum functionality = 1
		// Not found = 0

		int32_t                 graphics_queue_index = -1;
		int32_t                 graphics_queue_quality = 0;
		int32_t                 compute_queue_index = -1;
		int32_t                 compute_queue_quality = 0;
		int32_t                 transfer_queue_index = -1;
		int32_t                 transfer_queue_quality = 0;
		int32_t                 presentation_queue_index = -1;
		int32_t                 queue_count = 0;
		VkDeviceQueueCreateInfo infos[32];

		bool asynchronous_compute()
		{
			return compute_queue_index != graphics_queue_index;
		}

		bool transfer()
		{
			return transfer_queue_index != compute_queue_index && transfer_queue_index != graphics_queue_index;
		}
	};

	struct SwapChainSupportDetails
	{
		VkSurfaceCapabilitiesKHR        capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR>   presentModes;
	};

	class Surface;
	class DebugMessenger;
	class LogicalDevice;

	class Extensions
	{
	public:
		void Enable(const std::string& extname);
		bool IsEnabled(const std::string& extname) const;
		void Disable(const std::string& extname);

		bool CheckSupport();
		bool CheckDeviceSupport(VkPhysicalDevice& device);

		void operator+=(const Extensions& that);

		uint32_t Count() const;
		const char* const* Data() const;

		static Extensions Default();

	private:
		void updateBuf();

		std::vector<const char*> m_extBuf; // TODO: убрать, можно все таки по строке
		std::vector<std::string> m_exts;
	};

	class ValidationLayers
	{
	public:
		void Enable(const std::string& layername);
		bool IsEnabled(const std::string& layername) const;
		void Disable(const std::string& layername);

		bool CheckSupport();

		uint32_t Count() const;
		const char* const* Data() const;

		static ValidationLayers Default();

	private:
		void updateBuf();

		std::vector<const char*> m_layersBuf; // TODO: убрать, можно все таки по строке
		std::vector<std::string> m_layers;
	};

	// TODO: почистить класс от лишнего
	class PhysicalDevice
	{
		friend class SwapChain;
		friend class LogicalDevice;
	public:
		PhysicalDevice() = default;
		PhysicalDevice(VkPhysicalDevice &physical);

		bool IsDeviceSuitable(VkSurfaceKHR& surface, VkPhysicalDeviceType type, bool require_ray_tracing);

		SwapChainSupportDetails QuerySwapChainSupport(VkSurfaceKHR& surface); // TODO: private???
		bool FindQueueFamilies(VkSurfaceKHR& surface, QueueInfos& infos);// TODO: private???

		VkPhysicalDevice Get();

		VkPhysicalDeviceProperties& GetProperties() { return m_device_properties; }
		QueueInfos& queue_infos() { return m_selected_queues; }
		SwapChainSupportDetails& GetSwapChainDetails() { return m_swapChainSupportDetails; }
		Extensions& DeviceExtensions(){ return m_deviceExtensions; }
		VkPhysicalDeviceRayTracingPropertiesNV& ray_tracing_properties(){ return m_ray_tracing_properties; }

	private:
		static Extensions defaultExtensions();

		VkPhysicalDevice m_physical = VK_NULL_HANDLE; // implicitly destroyed with instance
		VkPhysicalDeviceProperties m_device_properties;
		VkPhysicalDeviceRayTracingPropertiesNV m_ray_tracing_properties;
		Extensions m_deviceExtensions;		
		SwapChainSupportDetails m_swapChainSupportDetails; 
		QueueInfos m_selected_queues;

	public:

		// ======>>>> OLD

		VkSampleCountFlagBits GetMaxUsableSampleCount();

		VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

		VkFormat FindDepthFormat();

		bool HasStencilComponent(VkFormat format);

		uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);	
	private:		
		VkSampleCountFlagBits m_msaaSamples = VK_SAMPLE_COUNT_1_BIT;
		QueueFamilyIndices m_indices;
		
	};

	class Instance
	{
	public:
		void Init();
		void Close();

		Surface* CreateSurface(GLFWwindow* window);

		DebugMessenger* CreateDebugMessenger();

		void GetPhysicalDevices(std::vector<PhysicalDevice>& physicals);

		PhysicalDevice PickPhysicalDevice(Surface* surface, bool require_ray_tracing);

		VkInstance& Get();

	private:
		VkInstance m_instance;
		ValidationLayers m_validationLayers;
		Extensions m_extensions;
	};

	template<typename T>
	class VkObject
	{
	public:
		VkObject(T* parent) : m_parent(parent) {}
		//VkObject(std::shared_ptr<T> parent) : m_parent(parent) {}
		virtual ~VkObject() = default;// TODO: возможно удалить
		virtual void Init() = 0;// TODO: возможно удалить
		virtual void Close() = 0;// TODO: возможно удалить

	protected:
		T* m_parent;
		//std::weak_ptr<T> m_parent; // T наследовать от enable_shared_from_this
	};

	using VkInstanceObject = VkObject<Instance>;
	using VkDeviceObject = VkObject<LogicalDevice>;
		
	class DebugMessenger : VkInstanceObject
	{
	public:
		DebugMessenger(Instance *vkinstance);
		~DebugMessenger();

		void Init() override;
		void Close() override;

		static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			void* pUserData);

		static void PopulateCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& info);

	private:
		VkDebugUtilsMessengerEXT m_messenger;
	};

	class Surface : VkInstanceObject
	{
	public:
		Surface(Instance *vkinstance, GLFWwindow* window);
		~Surface();

		void Init() override;
		void Close() override;

		VkSurfaceKHR& Get();

		void GetSize(uint32_t& width, uint32_t& height);

	private:
		GLFWwindow* m_window;
		VkSurfaceKHR m_surface;
	};	

	class SwapChain : VkDeviceObject
	{
	public:
		SwapChain(LogicalDevice* device, Surface* surface);
		~SwapChain();

		void Init() override;
		void Close() override;

		uint32_t AcquireNextImage(VkSemaphore semaphore);

		VkSwapchainKHR& Get()
		{
			return m_swapChain;
		}

		VkImage GetImage(uint32_t index)
		{
			return m_images[index];
		}
		VkFormat GetFormat()
		{
			return m_imageFormat;
		}
		uint32_t Count()
		{
			return m_images.size();
		}

		VkExtent2D GetExtent()
		{
			return m_extent;
		}

	private:
		VkSwapchainKHR m_swapChain;
		Surface* m_surface;

		std::vector<VkImage> m_images;

		VkFormat m_imageFormat;
		VkExtent2D m_extent;

		float aspectRatio;
	};

	class Queue : VkDeviceObject
	{
	public:
		Queue(LogicalDevice* device, uint32_t familyIndex);
		~Queue();

		void Init() override;
		void Close() override;

		void Wait();
		void Present(VkSemaphore semaphore, SwapChain* swapchain, uint32_t imageIndex);

		VkQueue& Get()
		{
			return m_queue;
		}
		uint32_t GetFamilyIndex()
		{
			return m_familyIndex;
		}

	private:
		VkQueue m_queue;
		uint32_t m_familyIndex;
	};

	class LogicalDevice
	{
	public:
		LogicalDevice(PhysicalDevice* phyDevice);
		~LogicalDevice();

		void Init(bool require_ray_tracing);
		void Close();

		VkDevice& Get();
		PhysicalDevice* GetPhysical();
		Queue* GetGraphicQueue()
		{
			return m_graphicsQueue;
		}
		Queue* GetPresentQueue()
		{
			return m_presentQueue;
		}
		Queue* GetTransferQueue()
		{
			return m_transferQueue;
		}

		bool MsaaEnabled()
		{
			return m_msaaEnabled;
		}

	//private:
		VkDevice m_device;
		VkPhysicalDeviceDescriptorIndexingFeaturesEXT m_indexing_features;
		// associated queue to this device.
		VkQueue                                       m_vk_graphics_queue = nullptr;  // TODO: заменить на Queue
		VkQueue                                       m_vk_compute_queue = nullptr;  // TODO: заменить на Queue
		VkQueue                                       m_vk_transfer_queue = nullptr;  // TODO: заменить на Queue
		VkQueue                                       m_vk_presentation_queue = nullptr;  // TODO: заменить на Queue
		Queue* m_graphicsQueue;
		Queue* m_presentQueue;
		Queue* m_transferQueue;

		// physical device from which this object has been create.
		PhysicalDevice* m_physicalDevice;

		// options
		bool m_msaaEnabled;

		ValidationLayers m_validationLayers;
		Extensions m_extensions;
	};

	class Synchronization : VkDeviceObject
	{
	public:
		Synchronization(LogicalDevice* device, uint32_t framesInFlight = MAX_FRAMES_IN_FLIGHT);
		~Synchronization();

		void Init() override;
		void Close() override;

		void WaitForFence();
		void ResetFence();
		void NextFrame();

		VkSemaphore& GetImgAvailableSemaphore();
		VkSemaphore& GetRenderFinishedSemaphore();
		VkFence& GetFence();

	private:
		std::vector<VkSemaphore> m_imgAvailableSemaphores;
		std::vector<VkSemaphore> m_renderFinishedSemaphores;
		std::vector<VkFence> m_inFlightFences;

		uint32_t m_framesInFlight;
		uint32_t m_currentFrame;
	};




} // namespace vkWrapper
#endif // SE_VULKAN
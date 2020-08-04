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

		std::vector<const char*> m_extBuf;
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

		std::vector<const char*> m_layersBuf;
		std::vector<std::string> m_layers;
	};

	class PhysicalDevice
	{
	public:
		PhysicalDevice() = default;
		PhysicalDevice(VkPhysicalDevice &physical);

		void FindSupportDetails(VkSurfaceKHR &surface);

		VkSampleCountFlagBits GetMaxUsableSampleCount();

		VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

		VkFormat FindDepthFormat();

		bool HasStencilComponent(VkFormat format);

		QueueFamilyIndices FindQueueFamilies(VkSurfaceKHR &surface);

		SwapChainSupportDetails QuerySwapChainSupport(VkSurfaceKHR &surface);

		bool IsDeviceSuitable(VkSurfaceKHR& surface);

		bool CheckDeviceExtensionSupport();

		int RateDeviceSuitability();

		uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

		static Extensions DefaultExtensions();

		VkPhysicalDevice Get();

	private:
		VkPhysicalDevice m_physical = VK_NULL_HANDLE; // implicitly destroyed with instance
		VkSampleCountFlagBits m_msaaSamples = VK_SAMPLE_COUNT_1_BIT;
		QueueFamilyIndices m_indices;
		SwapChainSupportDetails m_swapChainSupport;
	};

	class Instance
	{
	public:
		void Init();
		void Close();

		Surface* CreateSurface(GLFWwindow* window);

		DebugMessenger* CreateDebugMessenger();

		void GetPhysicalDevices(std::vector<PhysicalDevice>& physicals);

		PhysicalDevice PickPhysicalDevice(Surface* surface);

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
		
	class DebugMessenger : public VkInstanceObject
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

	class Surface : public VkInstanceObject
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

} // namespace vkWrapper
#endif // SE_VULKAN
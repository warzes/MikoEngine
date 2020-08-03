#pragma once

#if SE_VULKAN

namespace vkWrapper
{
#if SE_DEBUG
	const bool enableValidationLayers = true; // удалить когда исправлю Extensions::Default()
#else
	const bool enableValidationLayers = false;
#endif

	class Surface;
	class DebugMessenger;
	class PhysicalDevice;

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

	template <typename Self>
	class VkInstanceObject
	{
	public:
		using Ptr = std::shared_ptr<Self>;

		VkInstanceObject(std::shared_ptr<Instance> parent) : m_parent(parent) {}

		virtual void Init() = 0; // TODO: возможно удалить
		virtual void Close() = 0;// TODO: возможно удалить

	protected:
		std::weak_ptr<Instance> m_parent; // TODO: я не делаю проверку на реальное существование Instance
	};

	//template <typename Self>
	//class VkDeviceObject
	//{
	//public:
	//	using Ptr = std::shared_ptr<Self>;

	//	VkDeviceObject(std::shared_ptr<LogicalDevice> parent) : m_parent(parent) {}

	//	virtual void Init() = 0;// TODO: возможно удалить
	//	virtual void Close() = 0;// TODO: возможно удалить

	//protected:
	//	std::weak_ptr<LogicalDevice> m_parent;
	//};
	
	class DebugMessenger : public VkInstanceObject<DebugMessenger>
	{
	public:
		DebugMessenger(std::shared_ptr<Instance> vkinstance);
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

	class Surface : public VkInstanceObject<Surface>
	{
	public:
		Surface(std::shared_ptr<Instance> vkinstance, GLFWwindow* window);

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
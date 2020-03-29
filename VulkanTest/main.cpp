#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <functional>
#include <cstdlib>
#include <vector>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

class HelloTriangleApplication
{
public:
	void run()
	{
		initWindow();
		initVulkan();
		mainLoop();
		cleanup();
	}

private:
	GLFWwindow* window;
	VkInstance instance;

	void initVulkan()
	{
		createInstance();
	}

	void mainLoop()
	{
		while (!glfwWindowShouldClose(window))
		{
			glfwPollEvents();
		}
	}

	void checkExtensions(const char**& requiredExtensions, uint32_t& requiredExtensionCount)
	{
		//check for extensions that are supported

		//request number of extensions
		uint32_t extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

		//allocate vector memory with extensionCount
		std::vector<VkExtensionProperties> extensions(extensionCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

		//test supported and required extensions by name
		int cont = 0;
		std::cout << "available extensions:" << std::endl;
		for (size_t i = 0; i < requiredExtensionCount; i++)
		{
			for (const auto& extension : extensions)
			{
				//a way to compare char* to char[], they are essentially the same
				if (strcmp(requiredExtensions[i], extension.extensionName) == 0)
				{
					std::cout << "\t" << extension.extensionName << std::endl;
					cont++;
				}
			}
		}
		std::cout << "required extensions: " << requiredExtensionCount << std::endl;
		std::cout << "supported required extensions: " << cont << std::endl;

		if (cont - requiredExtensionCount != 0) 
		{
			throw std::runtime_error("not supported extensions found");
		}
	}

	void createInstance()
	{
		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Hello Triangle";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "No Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		//pNext is not set for it to be nullptr

		VkInstanceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;

		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		createInfo.enabledExtensionCount = glfwExtensionCount;
		createInfo.ppEnabledExtensionNames = glfwExtensions;

		checkExtensions(glfwExtensions, glfwExtensionCount);

		//create vulkan instance
		createInfo.enabledLayerCount = 0;

		if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create instance!");
		}

	}

	void cleanup()
	{
		//clean up all other objects

		//destroy vkInstance
		vkDestroyInstance(instance, nullptr);

		//destroy window element
		glfwDestroyWindow(this->window);

		//terminate all remaining cursors and windows
		glfwTerminate();
	}
	void initWindow()
	{
		//initialize window manager
		glfwInit();

		//do not use any graphics API
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		//make window unresizable
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		//create window, 4th parameter specifies the monitor to open the window in, and the last is relevant to 
		//openGL only
		this->window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Vulkan Test", nullptr, nullptr);
	}
};

int main()
{
	HelloTriangleApplication app;

	try
	{
		app.run();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

#include <iostream>

#include <study/draw_create_window.h>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void processInput(GLFWwindow *window)
{
	int key = glfwGetKey(window, GLFW_KEY_ESCAPE);
	if (key == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (key != 0)
	{
		std::cout << "processInput : " << key << std::endl;
	}
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
	std::cout << "width : " << width << "  height : " << height << std::endl;
}

int main()
{
	DrawCreateWindow window(SCR_WIDTH, SCR_HEIGHT);
	window.setFramebufferSizeCallback(framebuffer_size_callback);
	window.setProcessInputCallback(processInput);
	window.Init();
	return 0;
}

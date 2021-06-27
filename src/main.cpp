#include <iostream>

#include <study/draw_create_window.h>
#include <study/draw_first_triangle.h>
#include <study/draw_rectangle_with_vao.h>
#include <study/draw_color_triangle.h>
#include <study/draw_texture.h>
#include <study/draw_mix_texture.h>
#include <study/draw_rotate_mix_texture.h>
#include <study/coordinate_systems/draw_perspective_texture.h>
#include <study/coordinate_systems/draw_3d_box.h>

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
	// DrawCreateWindow window;
	// DrawFirstTriangle window;
	// DrawRectangleWithVAO window;
	// DrawColorTriangle window;
	// DrawTexture window;
	// DrawMixTexture window;
	// DrawRotateMixTexture window;
	// DrawPerspectiveTexture window;
	Draw3DBox window;
	window.setFramebufferSizeCallback(framebuffer_size_callback);
	window.setProcessInputCallback(processInput);
	int ret = window.Init(SCR_WIDTH, SCR_HEIGHT);
	std::cout << "init ret: " << ret << std::endl;
	ret = window.InitOpenGL();
	std::cout << "initOpenGL ret: " << ret << std::endl;
	window.Draw();
	return 0;
}

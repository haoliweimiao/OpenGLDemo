#include <iostream>

#include <study/config.h>
#include <study/draw_create_window.h>
#include <study/draw_first_triangle.h>
#include <study/draw_rectangle_with_vao.h>
#include <study/draw_color_triangle.h>
#include <study/draw_texture.h>
#include <study/draw_mix_texture.h>
#include <study/draw_rotate_mix_texture.h>
#include <study/coordinate_systems/draw_perspective_texture.h>
#include <study/coordinate_systems/draw_3d_box.h>
#include <study/coordinate_systems/draw_more_3d_box.h>
#include <study/camera/draw_camera_1.h>
#include <study/camera/draw_camera_2.h>
#include <study/camera/draw_camera_3.h>
#include <study/color/draw_color.h>
#include <study/basic_lighting/draw_basic_lighting.h>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

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

	// glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	// glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	// glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
	// std::cout << cameraDirection.x << " " << cameraDirection.y << " " << cameraDirection.z << std::endl;
	// glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	// glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
	// std::cout << cameraRight.x << " " << cameraRight.y << " " << cameraRight.z << std::endl;
	// glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);
	// std::cout << cameraUp.x << " " << cameraUp.y << " " << cameraUp.z << std::endl;
	// 0 0 1
	// 1 0 0
	// 0 1 0
	// glm::vec3 test = glm::cross(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	// std::cout << test.x << " " << test.y << " " << test.z << std::endl;

	// DrawCreateWindow window;
	// DrawFirstTriangle window;
	// DrawRectangleWithVAO window;
	// DrawColorTriangle window;
	// DrawTexture window;
	// DrawMixTexture window;
	// DrawRotateMixTexture window;
	// DrawPerspectiveTexture window;
	// Draw3DBox window;
	// DrawMore3DBox window;
	// DrawCamera1 window;
	// DrawCamera2 window;
	// DrawCamera3 window;
	// DrawColor window;
	DrawBasicLighting window;
	window.setFramebufferSizeCallback(framebuffer_size_callback);
	window.setProcessInputCallback(processInput);
	int ret = window.Init(SCR_WIDTH, SCR_HEIGHT);
	std::cout << "init ret: " << ret << std::endl;
	ret = window.InitOpenGL();
	std::cout << "initOpenGL ret: " << ret << std::endl;
	window.Draw();
	return 0;
}

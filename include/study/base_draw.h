//
// @description: Base class about draw OpenGL
// @author: von.wu
// @date: 2021-06-26 16:06:57
//

#ifndef BASE_DRAW_H
#define BASE_DRAW_H
#ifdef __cplusplus
extern "C"
{
#endif
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <iostream>

    typedef void (*ProcessInputCallback)(GLFWwindow *);

    class BaseDraw
    {
    private:
        int mScreenWidth;
        int mScreenHeight;
        GLFWwindow *mWindow;
        GLFWframebuffersizefun mBufferCallback = NULL;
        ProcessInputCallback mInputCallback = NULL;

    public:
        BaseDraw(int width, int height);
        int Init();
        GLFWwindow *getWindow();
        int Draw();
        int Destory();
        void setFramebufferSizeCallback(GLFWframebuffersizefun callback);
        void setProcessInputCallback(ProcessInputCallback callback);
        ~BaseDraw();
    };

    BaseDraw::BaseDraw(int width, int height)
    {
        mScreenWidth = width;
        mScreenHeight = height;
    }

    BaseDraw::~BaseDraw()
    {
    }

    int BaseDraw::Init()
    {
        // init OpenGL and somethind in here
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        mWindow = glfwCreateWindow(mScreenWidth, mScreenHeight, "LearnOpenGL", NULL, NULL);
        if (mWindow == NULL)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return -1;
        }
        glfwMakeContextCurrent(mWindow);
        if (mBufferCallback != NULL)
        {
            glfwSetFramebufferSizeCallback(mWindow, mBufferCallback);
        }

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
            return -1;
        }

        while (!glfwWindowShouldClose(mWindow))
        {
            Draw();
            if (mInputCallback != NULL)
            {
                (*mInputCallback)(mWindow);
            }
            glfwSwapBuffers(mWindow);
            glfwPollEvents();
        }

        glfwTerminate();
        return 0;
    }

    GLFWwindow *BaseDraw::getWindow()
    {
        return mWindow;
    }

    void BaseDraw::setFramebufferSizeCallback(GLFWframebuffersizefun callback)
    {
        this->mBufferCallback = callback;
    }

    void BaseDraw::setProcessInputCallback(ProcessInputCallback callback)
    {
        this->mInputCallback = callback;
    }

    int BaseDraw::Draw()
    {
        // draw in here
        return 0;
    }

    int BaseDraw::Destory()
    {
        // destory
        return 0;
    }

#ifdef __cplusplus
}
#endif
#endif // BASE_DRAW_H
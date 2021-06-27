//
// @description: Base class about draw OpenGL
// @author: von.wu
// @date: 2021-06-26 16:06:57
//

#ifndef BASE_DRAW_H
#define BASE_DRAW_H

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include <string.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct OpenGLContext OpenGLContext;

    struct OpenGLContext
    {
        // Put your user data here...
        void *userData;

        // Create window width and height (px)
        int mScreenWidth;
        int mScreenHeight;
    };

    typedef void (*ProcessInputCallback)(GLFWwindow *);

    typedef void (*DrawCall)(OpenGLContext);

    // Destroy and recycler resource
    typedef void (*Destory)(OpenGLContext);

    class BaseDraw
    {
    private:
        GLFWwindow *mWindow;
        GLFWframebuffersizefun mBufferCallback = NULL;
        ProcessInputCallback mInputCallback = NULL;
        DrawCall mDrawCall = NULL;
        Destory mDestroyMethod = NULL;

    public:
        BaseDraw();
        OpenGLContext mContext;
        int Init(int width, int height);
        GLFWwindow *getWindow();
        int Draw();
        // 要实现绘制图像，以下方法必须实现
        int InitOpenGL();
        void setDrawMethod(DrawCall method);
        // 监听回调，需要则设置
        void setFramebufferSizeCallback(GLFWframebuffersizefun callback);
        void setProcessInputCallback(ProcessInputCallback callback);
        // 销毁 OpenGL 程序会调用该方法，在此方法释放资源
        void setDestroyMethod(Destory method);
        ~BaseDraw();
    };

    BaseDraw::BaseDraw()
    {
        // Initialize the context
        memset(&mContext, 0, sizeof(OpenGLContext));
    }

    BaseDraw::~BaseDraw()
    {
    }

    int BaseDraw::InitOpenGL()
    {
        return 0;
    }

    int BaseDraw::Init(int width, int height)
    {
        if (width <= 0 || height <= 0)
        {
            return -1;
        }
        mContext.mScreenWidth = width;
        mContext.mScreenHeight = height;

        // init OpenGL and somethind in here
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        mWindow = glfwCreateWindow(mContext.mScreenWidth, mContext.mScreenHeight, "LearnOpenGL", NULL, NULL);
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

    void BaseDraw::setDrawMethod(DrawCall method)
    {
        this->mDrawCall = method;
    }

    void BaseDraw::setDestroyMethod(Destory method)
    {
        this->mDestroyMethod = method;
    }

    int BaseDraw::Draw()
    {
        // draw in here
        while (!glfwWindowShouldClose(mWindow))
        {
            if (mInputCallback != NULL)
            {
                (*mInputCallback)(mWindow);
            }

            // call draw
            if (mDrawCall != NULL)
            {
                (*mDrawCall)(mContext);
            }

            // DrawMethod();

            glfwSwapBuffers(mWindow);
            glfwPollEvents();
        }

        if (mDestroyMethod != NULL)
        {
            (*mDestroyMethod)(mContext);
        }

        glfwTerminate();
        return 0;
    }

#ifdef __cplusplus
}
#endif
#endif // BASE_DRAW_H
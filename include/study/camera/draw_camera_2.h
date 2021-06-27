//
// @description: Draw more 3D box and move camera view by keyboard WASD
// @author: von.wu
// @date: 2021-06-27 22:00:56
//

#ifndef DRAW_CAMERA_2_H
#define DRAW_CAMERA_2_H
// include something
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <utils/texture_utils.h>
#include <utils/file_utils.h>
#include "../base_draw.h"
#include "../shader.h"
#include "../config.h"

#ifdef __cplusplus
extern "C"
{
#endif
    // your code
    typedef struct
    {
        GLuint VBO, VAO;
        GLuint texture1, texture2;
        Shader *ourShader;
    } UserDataDrawCamera2;

    void processInputDrawCamera2(GLFWwindow *window)
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        float cameraSpeed = 2.5 * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            cameraPos += cameraSpeed * cameraFront;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            cameraPos -= cameraSpeed * cameraFront;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }

    void DrawCamera2Method(OpenGLContext context)
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        UserDataDrawCamera2 *userData = (UserDataDrawCamera2 *)context.userData;
        Shader *ourShader = userData->ourShader;
        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        // 开启了深度缓冲，所以需要清除深度信息
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, userData->texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, userData->texture2);

        // activate shader
        ourShader->use();

        // camera/view transformation
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        ourShader->setMat4("view", view);

        // render boxes
        glBindVertexArray(userData->VAO);
        for (unsigned int i = 0; i < 10; i++)
        {
            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            ourShader->setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glBindVertexArray(0);
    }

    void DrawCamera2DestoryMethod(OpenGLContext context)
    {
        UserDataDrawCamera2 *userData = (UserDataDrawCamera2 *)context.userData;

        // optional: de-allocate all resources once they've outlived their purpose:
        // ------------------------------------------------------------------------
        glDeleteVertexArrays(1, &userData->VAO);
        glDeleteBuffers(1, &userData->VBO);
        glDeleteProgram(userData->ourShader->ID);
        delete userData->ourShader;
    }

    class DrawCamera2 : public BaseDraw
    {
    private:
        /* data */
    public:
        DrawCamera2(/* args */);
        int InitOpenGL();
        void setProcessInputCallback(ProcessInputCallback callback);
        ~DrawCamera2();
    };

    DrawCamera2::DrawCamera2(/* args */)
    {
    }

    DrawCamera2::~DrawCamera2()
    {
    }

    void DrawCamera2::setProcessInputCallback(ProcessInputCallback callback)
    {
        // override, do nothing
    }

    int DrawCamera2::InitOpenGL()
    {
        // 绘制3D图形需要开启深度测试，否则有伪像出现
        glEnable(GL_DEPTH_TEST);
        // build and compile our shader zprogram
        // ------------------------------------
        mContext.userData = malloc(sizeof(UserDataDrawCamera2));

        UserDataDrawCamera2 *userData = (UserDataDrawCamera2 *)mContext.userData;
        userData->ourShader = new Shader("glsl/7.2.camera.vs", "glsl/7.2.camera.fs");

        // set up vertex data (and buffer(s)) and configure vertex attributes
        // ------------------------------------------------------------------
        float vertices[] = {
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

            -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f};

        glGenVertexArrays(1, &userData->VAO);
        glGenBuffers(1, &userData->VBO);

        glBindVertexArray(userData->VAO);

        glBindBuffer(GL_ARRAY_BUFFER, userData->VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);
        // texture coord attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        userData->texture1 = createTexture("png/container.jpg");
        userData->texture2 = createTexture("png/awesomeface.png");

        userData->ourShader->use();
        userData->ourShader->setInt("texture1", 0);
        userData->ourShader->setInt("texture2", 1);

        // pass projection matrix to shader (as projection matrix rarely changes there's no need to do this per frame)
        // -----------------------------------------------------------------------------------------------------------
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)mContext.mScreenWidth / (float)mContext.mScreenHeight, 0.1f, 100.0f);
        userData->ourShader->setMat4("projection", projection);

        BaseDraw::setDestroyMethod(DrawCamera2DestoryMethod);
        BaseDraw::setDrawMethod(DrawCamera2Method);
        BaseDraw::setProcessInputCallback(processInputDrawCamera2);

        return 0;
    }
#ifdef __cplusplus
}
#endif
#endif // DRAW_CAMERA_2_H
//
// @description: Draw more 3D box and move camera view by mouse move
// @author: von.wu
// @date: 2021-06-27 22:00:56
//

#ifndef DRAW_CAMERA_3_H
#define DRAW_CAMERA_3_H
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
    } UserDataDrawCamera3;

    void processInputDrawCamera3(GLFWwindow *window)
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

    // glfw: whenever the mouse scroll wheel scrolls, this callback is called
    // ----------------------------------------------------------------------
    void DrawCamera3ScrollCallback(GLFWwindow *window, double xoffset, double yoffset)
    {
        fov -= (float)yoffset;
        if (fov < 1.0f)
            fov = 1.0f;
        if (fov > 45.0f)
            fov = 45.0f;
    }

    // glfw: whenever the mouse moves, this callback is called
    // -------------------------------------------------------
    void DrawCamera3MouseMove(GLFWwindow *window, double xpos, double ypos)
    {
        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
        lastX = xpos;
        lastY = ypos;

        float sensitivity = 0.1f; // change this value to your liking
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw += xoffset;
        pitch += yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraFront = glm::normalize(front);
    }

    void DrawCamera3Method(OpenGLContext context)
    {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        UserDataDrawCamera3 *userData = (UserDataDrawCamera3 *)context.userData;
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

        // pass projection matrix to shader (as projection matrix rarely changes there's no need to do this per frame)
        // -----------------------------------------------------------------------------------------------------------
        glm::mat4 projection = glm::perspective(glm::radians(fov), (float)context.mScreenWidth / (float)context.mScreenHeight, 0.1f, 100.0f);
        ourShader->setMat4("projection", projection);

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

    void DrawCamera3DestoryMethod(OpenGLContext context)
    {
        UserDataDrawCamera3 *userData = (UserDataDrawCamera3 *)context.userData;

        // optional: de-allocate all resources once they've outlived their purpose:
        // ------------------------------------------------------------------------
        glDeleteVertexArrays(1, &userData->VAO);
        glDeleteBuffers(1, &userData->VBO);
        glDeleteProgram(userData->ourShader->ID);
        delete userData->ourShader;
    }

    class DrawCamera3 : public BaseDraw
    {
    private:
        /* data */
    public:
        DrawCamera3(/* args */);
        int InitOpenGL();
        void setProcessInputCallback(ProcessInputCallback callback);
        ~DrawCamera3();
    };

    DrawCamera3::DrawCamera3(/* args */)
    {
    }

    DrawCamera3::~DrawCamera3()
    {
    }

    void DrawCamera3::setProcessInputCallback(ProcessInputCallback callback)
    {
        // override, do nothing
    }

    int DrawCamera3::InitOpenGL()
    {
        // 绘制3D图形需要开启深度测试，否则有伪像出现
        glEnable(GL_DEPTH_TEST);

        // build and compile our shader zprogram
        // ------------------------------------
        mContext.userData = malloc(sizeof(UserDataDrawCamera3));

        // tell GLFW to capture our mouse
        glfwSetInputMode(getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        UserDataDrawCamera3 *userData = (UserDataDrawCamera3 *)mContext.userData;
        userData->ourShader = new Shader("glsl/7.3.camera.vs", "glsl/7.3.camera.fs");

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

        BaseDraw::setDestroyMethod(DrawCamera3DestoryMethod);
        BaseDraw::setDrawMethod(DrawCamera3Method);
        BaseDraw::setProcessInputCallback(processInputDrawCamera3);
        BaseDraw::setCursorposfun(DrawCamera3MouseMove);
        BaseDraw::setScrollfun(DrawCamera3ScrollCallback);
        return 0;
    }
#ifdef __cplusplus
}
#endif
#endif // DRAW_CAMERA_3_H
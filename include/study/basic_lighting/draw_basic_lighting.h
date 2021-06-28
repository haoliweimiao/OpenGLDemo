//
// @description: Draw basic lighting demo
// @author: von.wu
// @date: 2021-06-28 21:23:57
//

#ifndef DRAW_BASIC_LIGHTING
#define DRAW_BASIC_LIGHTING
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
    // your code
    typedef struct
    {
        GLuint VBO, cubeVAO, lightCubeVAO;
        GLuint texture1, texture2;
        Shader *lightingShader;
        Shader *lightCubeShader;
    } UserDataDrawBasicLighting;

    void processInputDrawBasicLighting(GLFWwindow *window)
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.ProcessKeyboard(FORWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.ProcessKeyboard(BACKWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.ProcessKeyboard(LEFT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.ProcessKeyboard(RIGHT, deltaTime);
    }

    // glfw: whenever the mouse scroll wheel scrolls, this callback is called
    // ----------------------------------------------------------------------
    void DrawBasicLightingScrollCallback(GLFWwindow *window, double xoffset, double yoffset)
    {
        camera.ProcessMouseScroll(yoffset);
    }

    // glfw: whenever the mouse moves, this callback is called
    // -------------------------------------------------------
    void DrawBasicLightingMouseMove(GLFWwindow *window, double xpos, double ypos)
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

        camera.ProcessMouseMovement(xoffset, yoffset);
    }

    void DrawBasicLightingMethod(OpenGLContext context)
    {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        UserDataDrawBasicLighting *userData = (UserDataDrawBasicLighting *)context.userData;
        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Shader lightingShader = *userData->lightingShader;
        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();
        lightingShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        lightingShader.setVec3("lightPos", lightPos);

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        lightingShader.setMat4("model", model);

        // render the cube
        glBindVertexArray(userData->cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        Shader lightCubeShader = *userData->lightCubeShader;
        // also draw the lamp object
        lightCubeShader.use();
        lightCubeShader.setMat4("projection", projection);
        lightCubeShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
        lightCubeShader.setMat4("model", model);

        glBindVertexArray(userData->lightCubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindVertexArray(0);
    }

    void DrawBasicLightingDestoryMethod(OpenGLContext context)
    {
        UserDataDrawBasicLighting *userData = (UserDataDrawBasicLighting *)context.userData;

        // optional: de-allocate all resources once they've outlived their purpose:
        // ------------------------------------------------------------------------
        glDeleteVertexArrays(1, &userData->cubeVAO);
        glDeleteBuffers(1, &userData->VBO);
        glDeleteProgram(userData->lightCubeShader->ID);
        delete userData->lightCubeShader;
        glDeleteProgram(userData->lightingShader->ID);
        delete userData->lightingShader;
    }

    class DrawBasicLighting : public BaseDraw
    {
    private:
        /* data */
    public:
        DrawBasicLighting(/* args */);
        int InitOpenGL();
        ~DrawBasicLighting();
    };

    DrawBasicLighting::DrawBasicLighting(/* args */)
    {
    }

    DrawBasicLighting::~DrawBasicLighting()
    {
    }

    int DrawBasicLighting::InitOpenGL()
    {
        // 绘制3D图形需要开启深度测试，否则有伪像出现
        glEnable(GL_DEPTH_TEST);

        // build and compile our shader zprogram
        // ------------------------------------
        mContext.userData = malloc(sizeof(UserDataDrawBasicLighting));

        // tell GLFW to capture our mouse
        glfwSetInputMode(getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        UserDataDrawBasicLighting *userData = (UserDataDrawBasicLighting *)mContext.userData;
        userData->lightingShader = new Shader("glsl/basic_lighting/2.1.basic_lighting.vs", "glsl/basic_lighting/2.1.basic_lighting.fs");
        userData->lightCubeShader = new Shader("glsl/basic_lighting/2.1.light_cube.vs", "glsl/basic_lighting/2.1.light_cube.fs");

        float vertices[] = {
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
            0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
            0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,

            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

            -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,

            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,

            -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,

            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f};

        glGenVertexArrays(1, &userData->cubeVAO);
        glGenBuffers(1, &userData->VBO);

        glBindVertexArray(userData->cubeVAO);

        glBindBuffer(GL_ARRAY_BUFFER, userData->VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);
        // normal attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glGenVertexArrays(1, &userData->lightCubeVAO);
        glBindVertexArray(userData->lightCubeVAO);

        // we only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need (it's already bound, but we do it again for educational purposes)
        glBindBuffer(GL_ARRAY_BUFFER, userData->VBO);
        // note that we update the lamp's position attribute's stride to reflect the updated buffer data
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        BaseDraw::setDestroyMethod(DrawBasicLightingDestoryMethod);
        BaseDraw::setDrawMethod(DrawBasicLightingMethod);
        BaseDraw::setProcessInputCallback(processInputDrawBasicLighting);
        BaseDraw::setCursorposfun(DrawBasicLightingMouseMove);
        BaseDraw::setScrollfun(DrawBasicLightingScrollCallback);
        return 0;
    }

#ifdef __cplusplus
}
#endif
#endif // DRAW_BASIC_LIGHTING
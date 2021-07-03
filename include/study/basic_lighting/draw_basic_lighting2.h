//
// @description: Draw basic lighting demo 2
// @author: von.wu
// @date: 2021-07-03 18:35:05
//

#ifndef DRAW_BASIC_LIGHTING2
#define DRAW_BASIC_LIGHTING2
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
    } UserDataDrawBasicLighting2;

    void processInputDrawBasicLighting2(GLFWwindow *window)
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
    void DrawBasicLighting2ScrollCallback(GLFWwindow *window, double xoffset, double yoffset)
    {
        camera.ProcessMouseScroll(yoffset);
    }

    // glfw: whenever the mouse moves, this callback is called
    // -------------------------------------------------------
    void DrawBasicLighting2MouseMove(GLFWwindow *window, double xpos, double ypos)
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

    void DrawBasicLighting2Method(OpenGLContext context)
    {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        UserDataDrawBasicLighting2 *userData = (UserDataDrawBasicLighting2 *)context.userData;
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
        lightingShader.setVec3("viewPos", camera.Position);

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

    void DrawBasicLighting2DestoryMethod(OpenGLContext context)
    {
        UserDataDrawBasicLighting2 *userData = (UserDataDrawBasicLighting2 *)context.userData;

        // optional: de-allocate all resources once they've outlived their purpose:
        // ------------------------------------------------------------------------
        glDeleteVertexArrays(1, &userData->cubeVAO);
        glDeleteBuffers(1, &userData->VBO);
        glDeleteProgram(userData->lightCubeShader->ID);
        delete userData->lightCubeShader;
        glDeleteProgram(userData->lightingShader->ID);
        delete userData->lightingShader;
    }

    class DrawBasicLighting2 : public BaseDraw
    {
    private:
        /* data */
    public:
        DrawBasicLighting2(/* args */);
        int InitOpenGL();
        ~DrawBasicLighting2();
    };

    DrawBasicLighting2::DrawBasicLighting2(/* args */)
    {
    }

    DrawBasicLighting2::~DrawBasicLighting2()
    {
    }

    int DrawBasicLighting2::InitOpenGL()
    {
        // 绘制3D图形需要开启深度测试，否则有伪像出现
        glEnable(GL_DEPTH_TEST);

        // build and compile our shader zprogram
        // ------------------------------------
        mContext.userData = malloc(sizeof(UserDataDrawBasicLighting2));

        // tell GLFW to capture our mouse
        glfwSetInputMode(getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        UserDataDrawBasicLighting2 *userData = (UserDataDrawBasicLighting2 *)mContext.userData;
        userData->lightingShader = new Shader("glsl/basic_lighting/2.2.basic_lighting.vs", "glsl/basic_lighting/2.2.basic_lighting.fs");
        userData->lightCubeShader = new Shader("glsl/basic_lighting/2.2.light_cube.vs", "glsl/basic_lighting/2.2.light_cube.fs");

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

        BaseDraw::setDestroyMethod(DrawBasicLighting2DestoryMethod);
        BaseDraw::setDrawMethod(DrawBasicLighting2Method);
        BaseDraw::setProcessInputCallback(processInputDrawBasicLighting2);
        BaseDraw::setCursorposfun(DrawBasicLighting2MouseMove);
        BaseDraw::setScrollfun(DrawBasicLighting2ScrollCallback);
        return 0;
    }

#ifdef __cplusplus
}
#endif
#endif // DRAW_BASIC_LIGHTING2
//
// @description: draw more 3d box in window
// @author: von.wu
// @date: 2021-06-27 20:09:19
//

#ifndef DRAW_MORE_3D_BOX_H
#define DRAW_MORE_3D_BOX_H
// include something
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <utils/texture_utils.h>
#include <utils/file_utils.h>
#include "../base_draw.h"
#include "../shader.h"
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
    } UserDataDrawMore3DBox;

    // world space positions of our cubes
    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)};

    void DrawMore3DBoxMethod(OpenGLContext context)
    {
        UserDataDrawMore3DBox *userData = (UserDataDrawMore3DBox *)context.userData;
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

        // GLuint programId = userData->ourShader->ID;
        // create transformations
        glm::mat4 view = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(45.0f), (float)context.mScreenWidth / (float)context.mScreenHeight, 0.1f, 100.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

        ourShader->setMat4("view", view);
        ourShader->setMat4("projection", projection);

        // render box
        glBindVertexArray(userData->VAO);
        double nowTime = glfwGetTime();
        for (unsigned int i = 0; i < 10; i++)
        {
            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * (i + 1) * nowTime;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            ourShader->setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        glBindVertexArray(0);
    }

    void DrawMore3DBoxDestoryMethod(OpenGLContext context)
    {
        UserDataDrawMore3DBox *userData = (UserDataDrawMore3DBox *)context.userData;

        // optional: de-allocate all resources once they've outlived their purpose:
        // ------------------------------------------------------------------------
        glDeleteVertexArrays(1, &userData->VAO);
        glDeleteBuffers(1, &userData->VBO);
        glDeleteProgram(userData->ourShader->ID);
        delete userData->ourShader;
    }

    class DrawMore3DBox : public BaseDraw
    {
    private:
        /* data */
    public:
        DrawMore3DBox(/* args */);
        int InitOpenGL();
        ~DrawMore3DBox();
    };

    DrawMore3DBox::DrawMore3DBox(/* args */)
    {
    }

    DrawMore3DBox::~DrawMore3DBox()
    {
    }

    int DrawMore3DBox::InitOpenGL()
    {
        // 绘制3D图形需要开启深度测试，否则有伪像出现
        glEnable(GL_DEPTH_TEST);
        // build and compile our shader zprogram
        // ------------------------------------
        mContext.userData = malloc(sizeof(UserDataDrawMore3DBox));

        UserDataDrawMore3DBox *userData = (UserDataDrawMore3DBox *)mContext.userData;
        userData->ourShader = new Shader("glsl/6.3.coordinate_systems.vs", "glsl/6.3.coordinate_systems.fs");

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
        // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
        // -------------------------------------------------------------------------------------------
        userData->ourShader->use(); // don't forget to activate/use the shader before setting uniforms!
        // either set it manually like so:
        glUniform1i(glGetUniformLocation(userData->ourShader->ID, "texture1"), 0);
        // or set it via the texture class
        userData->ourShader->setInt("texture2", 1);

        BaseDraw::setDestroyMethod(DrawMore3DBoxDestoryMethod);
        BaseDraw::setDrawMethod(DrawMore3DBoxMethod);

        return 0;
    }
#ifdef __cplusplus
}
#endif
#endif // DRAW_MORE_3D_BOX_H
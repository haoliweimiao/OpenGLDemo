//
// @description: Draw Colorful Triangle Demo
// @author: von.wu
// @date: 2021-06-26 22:36:28
//

#ifndef DRAW_COLOR_TRIANGLE_H
#define DRAW_COLOR_TRIANGLE_H

#include "base_draw.h"
#include "shader.h"

#ifdef __cplusplus
extern "C"
{
#endif
    // your code

    typedef struct
    {
        // Handle to a program object
        GLuint shaderProgram;
        GLuint VBO, VAO;
        Shader *ourShader;
    } UserDataDrawColorTriangle;

    void DrawColorTriangleMethod(OpenGLContext context)
    {
        UserDataDrawColorTriangle *userData = (UserDataDrawColorTriangle *)context.userData;
        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // render the triangle
        userData->ourShader->use();
        glBindVertexArray(userData->VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
    }

    void DrawColorTriangleDestoryMethod(OpenGLContext context)
    {
        UserDataDrawColorTriangle *userData = (UserDataDrawColorTriangle *)context.userData;

        // optional: de-allocate all resources once they've outlived their purpose:
        // ------------------------------------------------------------------------
        glDeleteVertexArrays(1, &userData->VAO);
        glDeleteBuffers(1, &userData->VBO);
        glDeleteProgram(userData->shaderProgram);
        delete userData->ourShader;
    }

    class DrawColorTriangle : public BaseDraw
    {
    private:
        /* data */

    public:
        DrawColorTriangle(/* args */);
        int InitOpenGL();
        ~DrawColorTriangle();
    };

    DrawColorTriangle::DrawColorTriangle(/* args */)
    {
    }

    DrawColorTriangle::~DrawColorTriangle()
    {
    }

    int DrawColorTriangle::InitOpenGL()
    {
        mContext.userData = malloc(sizeof(UserDataDrawColorTriangle));

        UserDataDrawColorTriangle *userData = (UserDataDrawColorTriangle *)mContext.userData;
        userData->ourShader = new Shader("../glsl/3.3.shader.vs", "../glsl/3.3.shader.fs"); // you can name your shader files however you like

        int programId = (int)userData->ourShader->ID;
        if (programId == -1)
        {
            // F5 run in this project, glsl file need use this path
            userData->ourShader = new Shader("./glsl/3.3.shader.vs", "./glsl/3.3.shader.fs");
        }
        userData->shaderProgram = userData->ourShader->ID;
        // set up vertex data (and buffer(s)) and configure vertex attributes
        // ------------------------------------------------------------------
        float vertices[] = {
            // positions         // colors
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom right
            -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
            0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f    // top
        };

        glGenVertexArrays(1, &userData->VAO);
        glGenBuffers(1, &userData->VBO);
        // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
        glBindVertexArray(userData->VAO);

        glBindBuffer(GL_ARRAY_BUFFER, userData->VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);
        // color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);

        BaseDraw::setDrawMethod(DrawColorTriangleMethod);
        BaseDraw::setDestroyMethod(DrawColorTriangleDestoryMethod);
        return 0;
    }

#ifdef __cplusplus
}
#endif
#endif // DRAW_COLOR_TRIANGLE_H
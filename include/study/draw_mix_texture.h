//
// @description: Draw mix texture demo
// @author: von.wu
// @date: 2021-06-27 16:04:35
//

#ifndef DRAW_MIX_TEXTURE_DEMO
#define DRAW_MIX_TEXTURE_DEMO

#include "utils/texture_utils.h"
#include "utils/file_utils.h"
#include "base_draw.h"
#include "shader.h"

#ifdef __cplusplus
extern "C"
{
#endif
    // your code

    typedef struct
    {
        GLuint VBO, VAO, EBO;
        GLuint texture1, texture2;
        Shader *ourShader;
    } UserDataDrawMixTexture;

    void DrawMixTextureMethod(OpenGLContext context)
    {
        UserDataDrawMixTexture *userData = (UserDataDrawMixTexture *)context.userData;
        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, userData->texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, userData->texture2);

        // render container
        userData->ourShader->use();
        glBindVertexArray(userData->VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    void DrawMixTextureDestoryMethod(OpenGLContext context)
    {
        UserDataDrawMixTexture *userData = (UserDataDrawMixTexture *)context.userData;

        // optional: de-allocate all resources once they've outlived their purpose:
        // ------------------------------------------------------------------------
        glDeleteVertexArrays(1, &userData->VAO);
        glDeleteBuffers(1, &userData->VBO);
        glDeleteBuffers(1, &userData->EBO);
        glDeleteProgram(userData->ourShader->ID);
        delete userData->ourShader;
    }

    class DrawMixTexture : public BaseDraw
    {
    private:
        /* data */
    public:
        DrawMixTexture(/* args */);
        int InitOpenGL();
        ~DrawMixTexture();
    };

    DrawMixTexture::DrawMixTexture(/* args */)
    {
    }

    DrawMixTexture::~DrawMixTexture()
    {
    }

    int DrawMixTexture::InitOpenGL()
    {
        // build and compile our shader zprogram
        // ------------------------------------
        mContext.userData = malloc(sizeof(UserDataDrawMixTexture));

        UserDataDrawMixTexture *userData = (UserDataDrawMixTexture *)mContext.userData;
        userData->ourShader = new Shader("glsl/4.2.texture.vs", "glsl/4.2.texture.fs");

        // set up vertex data (and buffer(s)) and configure vertex attributes
        // ------------------------------------------------------------------
        float vertices[] = {
            // positions          // colors           // texture coords
            0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // top right
            0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
            -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f   // top left
        };
        unsigned int indices[] = {
            0, 1, 3, // first triangle
            1, 2, 3  // second triangle
        };

        glGenVertexArrays(1, &userData->VAO);
        glGenBuffers(1, &userData->VBO);
        glGenBuffers(1, &userData->EBO);

        glBindVertexArray(userData->VAO);

        glBindBuffer(GL_ARRAY_BUFFER, userData->VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, userData->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);
        // color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        // texture coord attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        userData->texture1 = createTexture("png/container.jpg");
        userData->texture2 = createTexture("png/awesomeface.png");
        // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
        // -------------------------------------------------------------------------------------------
        userData->ourShader->use(); // don't forget to activate/use the shader before setting uniforms!
        // either set it manually like so:
        glUniform1i(glGetUniformLocation(userData->ourShader->ID, "texture1"), 0);
        // or set it via the texture class
        userData->ourShader->setInt("texture2", 1);

        BaseDraw::setDestroyMethod(DrawMixTextureDestoryMethod);
        BaseDraw::setDrawMethod(DrawMixTextureMethod);

        return 0;
    }

#ifdef __cplusplus
}
#endif
#endif // DRAW_MIX_TEXTURE_DEMO
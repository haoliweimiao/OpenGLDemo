//
// @description: Draw Texture
// @author: von.wu
// @date: 2021-06-27 10:09:08
//

#ifndef DRAW_TEXTURE_H
#define DRAW_TEXTURE_H

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "base_draw.h"
#include "shader.h"
#include <utils/file_utils.h>

#ifdef __cplusplus
extern "C"
{
#endif
    // your code

    typedef struct
    {
        GLuint VBO, VAO, EBO;
        GLuint texture;
        Shader *ourShader;
    } UserDataDrawTexture;

    void DrawTextureMethod(OpenGLContext context)
    {
        UserDataDrawTexture *userData = (UserDataDrawTexture *)context.userData;
        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // bind Texture
        glBindTexture(GL_TEXTURE_2D, userData->texture);

        // render container
        userData->ourShader->use();
        glBindVertexArray(userData->VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    void DrawTextureDestoryMethod(OpenGLContext context)
    {
        UserDataDrawTexture *userData = (UserDataDrawTexture *)context.userData;

        // optional: de-allocate all resources once they've outlived their purpose:
        // ------------------------------------------------------------------------
        glDeleteVertexArrays(1, &userData->VAO);
        glDeleteBuffers(1, &userData->VBO);
        glDeleteBuffers(1, &userData->EBO);
        glDeleteProgram(userData->ourShader->ID);
        delete userData->ourShader;
    }

    class DrawTexture : public BaseDraw
    {
    private:
        /* data */
    public:
        DrawTexture(/* args */);
        int InitOpenGL();
        ~DrawTexture();
    };

    DrawTexture::DrawTexture(/* args */)
    {
    }

    DrawTexture::~DrawTexture()
    {
    }

    int DrawTexture::InitOpenGL()
    {
        mContext.userData = malloc(sizeof(UserDataDrawTexture));

        UserDataDrawTexture *userData = (UserDataDrawTexture *)mContext.userData;
        userData->ourShader = new Shader("../glsl/4.1.texture.vs", "../glsl/4.1.texture.fs");

        int programId = (int)userData->ourShader->ID;
        if (programId == -1)
        {
            // F5 run in this project, glsl file need use this path
            userData->ourShader = new Shader("./glsl/4.1.texture.vs", "./glsl/4.1.texture.fs");
        }

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

        // load and create a texture
        // -------------------------
        glGenTextures(1, &userData->texture);
        glBindTexture(GL_TEXTURE_2D, userData->texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // load image, create texture and generate mipmaps
        int width, height, nrChannels;
        // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.

        char imagePath[256];
        getFilePath("png/container.jpg", imagePath);
        if (imagePath == NULL)
        {
            std::cout << "load image file failed !" << std::endl;
            return -1;
        }

        unsigned char *data = stbi_load(imagePath, &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);

        BaseDraw::setDestroyMethod(DrawTextureDestoryMethod);
        BaseDraw::setDrawMethod(DrawTextureMethod);
        return 0;
    }

#ifdef __cplusplus
}
#endif
#endif // DRAW_TEXTURE_H
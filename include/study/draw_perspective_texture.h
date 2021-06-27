//
// @description: Draw a Perspective texture
// @author: von.wu
// @date: 2021-06-27 17:38:52
//

#ifndef DRAW_PERSPECTIVE_TEXTURE
#define DRAW_PERSPECTIVE_TEXTURE
// include something
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "utils/texture_utils.h"
#include "utils/file_utils.h"
#include "base_draw.h"
#include "shader.h"

#ifdef __cplusplus
extern "C"
{
#endif
    // your code
    // your code

    typedef struct
    {
        GLuint VBO, VAO, EBO;
        GLuint texture1, texture2;
        Shader *ourShader;
    } UserDataDrawPerspectiveTexture;

    void DrawPerspectiveTextureMethod(OpenGLContext context)
    {
        UserDataDrawPerspectiveTexture *userData = (UserDataDrawPerspectiveTexture *)context.userData;
        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, userData->texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, userData->texture2);

        // activate shader
        userData->ourShader->use();

        // create transformations
        glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        projection = glm::perspective(glm::radians(45.0f), (float)context.mScreenWidth / (float)context.mScreenHeight, 0.1f, 100.0f);
        // retrieve the matrix uniform locations
        unsigned int modelLoc = glGetUniformLocation(userData->ourShader->ID, "model");
        unsigned int viewLoc = glGetUniformLocation(userData->ourShader->ID, "view");
        // pass them to the shaders (3 different ways)
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
        // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        userData->ourShader->setMat4("projection", projection);

        // render container
        glBindVertexArray(userData->VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    void DrawPerspectiveTextureDestoryMethod(OpenGLContext context)
    {
        UserDataDrawPerspectiveTexture *userData = (UserDataDrawPerspectiveTexture *)context.userData;

        // optional: de-allocate all resources once they've outlived their purpose:
        // ------------------------------------------------------------------------
        glDeleteVertexArrays(1, &userData->VAO);
        glDeleteBuffers(1, &userData->VBO);
        glDeleteBuffers(1, &userData->EBO);
        glDeleteProgram(userData->ourShader->ID);
        delete userData->ourShader;
    }

    class DrawPerspectiveTexture : public BaseDraw
    {
    private:
        /* data */
    public:
        DrawPerspectiveTexture(/* args */);
        int InitOpenGL();
        ~DrawPerspectiveTexture();
    };

    DrawPerspectiveTexture::DrawPerspectiveTexture(/* args */)
    {
    }

    DrawPerspectiveTexture::~DrawPerspectiveTexture()
    {
    }

    int DrawPerspectiveTexture::InitOpenGL()
    {
        // build and compile our shader zprogram
        // ------------------------------------
        mContext.userData = malloc(sizeof(UserDataDrawPerspectiveTexture));

        UserDataDrawPerspectiveTexture *userData = (UserDataDrawPerspectiveTexture *)mContext.userData;
        userData->ourShader = new Shader("glsl/6.1.coordinate_systems.vs", "glsl/6.1.coordinate_systems.fs");

        // set up vertex data (and buffer(s)) and configure vertex attributes
        // ------------------------------------------------------------------
        float vertices[] = {
            // positions          // texture coords
            0.5f, 0.5f, 0.0f, 1.0f, 1.0f,   // top right
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  // bottom right
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left
            -0.5f, 0.5f, 0.0f, 0.0f, 1.0f   // top left
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

        BaseDraw::setDestroyMethod(DrawPerspectiveTextureDestoryMethod);
        BaseDraw::setDrawMethod(DrawPerspectiveTextureMethod);

        return 0;
    }

#ifdef __cplusplus
}
#endif
#endif // DRAW_PERSPECTIVE_TEXTURE
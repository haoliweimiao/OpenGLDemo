//
// @description: Draw a rectangle with VAO VBO EBO
// @author: von.wu
// @date: 2021-06-26 21:10:20
//

#ifndef DRAW_RECTANGLE_WITH_VAO
#define DRAW_RECTANGLE_WITH_VAO
#ifdef __cplusplus
extern "C"
{
#endif
#include "base_draw.h"
    typedef struct
    {
        // Handle to a program object
        GLuint shaderProgram;
        GLuint VBO, VAO, EBO;
    } UserDataDrawRectangleWithVAO;

    void DrawRectangleWithVAOMethod(OpenGLContext context)
    {
        UserDataDrawRectangleWithVAO *userData = (UserDataDrawRectangleWithVAO *)context.userData;

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        glUseProgram(userData->shaderProgram);
        glBindVertexArray(userData->VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // glDrawElements(GL_LINES, 6, GL_UNSIGNED_INT, 0);
        // glDrawElements(GL_LINE_LOOP, 6, GL_UNSIGNED_INT, 0);
    }

    void DrawRectangleWithVAODestroyMethod(OpenGLContext context)
    {
        UserDataDrawRectangleWithVAO *userData = (UserDataDrawRectangleWithVAO *)context.userData;

        // optional: de-allocate all resources once they've outlived their purpose:
        // ------------------------------------------------------------------------
        glDeleteVertexArrays(1, &userData->VAO);
        glDeleteBuffers(1, &userData->VBO);
        glDeleteBuffers(1, &userData->EBO);
        glDeleteProgram(userData->shaderProgram);
    }

    // your code
    class DrawRectangleWithVAO : public BaseDraw
    {
    private:
        /* data */
        UserDataDrawRectangleWithVAO mUserData;

        const char *vertexShaderSource = "#version 330 core\n"
                                         "layout (location = 0) in vec3 aPos;\n"
                                         "void main()\n"
                                         "{\n"
                                         "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                         "}\0";
        const char *fragmentShaderSource = "#version 330 core\n"
                                           "out vec4 FragColor;\n"
                                           "void main()\n"
                                           "{\n"
                                           "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                           "}\n\0";

    public:
        DrawRectangleWithVAO(/* args */);
        int InitOpenGL();
        ~DrawRectangleWithVAO();
    };

    DrawRectangleWithVAO::DrawRectangleWithVAO(/* args */)
    {
    }

    DrawRectangleWithVAO::~DrawRectangleWithVAO()
    {
    }

    int DrawRectangleWithVAO::InitOpenGL()
    {
        mContext.userData = malloc(sizeof(UserDataDrawRectangleWithVAO));
        UserDataDrawRectangleWithVAO *userData = (UserDataDrawRectangleWithVAO *)mContext.userData;

        // build and compile our shader program
        // ------------------------------------
        // vertex shader
        unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);
        // check for shader compile errors
        int success;
        char infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                      << infoLog << std::endl;
        }
        // fragment shader
        unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);
        // check for shader compile errors
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                      << infoLog << std::endl;
        }
        // link shaders
        userData->shaderProgram = glCreateProgram();
        GLuint shaderProgram = userData->shaderProgram;
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
        // check for linking errors
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                      << infoLog << std::endl;
        }
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        // set up vertex data (and buffer(s)) and configure vertex attributes
        // ------------------------------------------------------------------
        float vertices[] = {
            0.5f, 0.5f, 0.0f,   // top right
            0.5f, -0.5f, 0.0f,  // bottom right
            -0.5f, -0.5f, 0.0f, // bottom left
            -0.5f, 0.5f, 0.0f   // top left
        };
        unsigned int indices[] = {
            // note that we start from 0!
            0, 1, 3, // first Triangle
            1, 2, 3  // second Triangle
        };

        glGenVertexArrays(1, &userData->VAO);
        glGenBuffers(1, &userData->VBO);
        glGenBuffers(1, &userData->EBO);

        // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
        glBindVertexArray(userData->VAO);

        glBindBuffer(GL_ARRAY_BUFFER, userData->VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, userData->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
        // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
        glBindVertexArray(0);

        BaseDraw::setDrawMethod(DrawRectangleWithVAOMethod);
        BaseDraw::setDestroyMethod(DrawRectangleWithVAODestroyMethod);
        return 0;
    }

#ifdef __cplusplus
}
#endif
#endif // DRAW_RECTANGLE_WITH_VAO
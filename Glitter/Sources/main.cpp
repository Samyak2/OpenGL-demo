// Local Headers
#include "glitter.hpp"

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard Headers
#include <cstdio>
#include <cstdlib>

#include <chrono>

int main(int argc, char * argv[]) {

    // Load GLFW and Create a Window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    auto mWindow = glfwCreateWindow(mWidth, mHeight, "Gaem", nullptr, nullptr);

    // Check for Valid Context
    if (mWindow == nullptr) {
        fprintf(stderr, "Failed to Create OpenGL Context");
        return EXIT_FAILURE;
    }

    // Create Context and Load OpenGL Functions
    glfwMakeContextCurrent(mWindow);
    gladLoadGL();
    fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));

    float vertices[] = {
        0.0f, 0.5f, 1.0f, 0.0f, 0.0f, // Vertex 1 at (0, 0.5) and red color
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // Vertex 2 at (0.5, -0.5) and green color
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f // Vertex 3 at (-0.5f, -0.5f) and blue color
    };

    // create and bind a Vertex Array Object (VAO)
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo; // Create a new Vertex Buffer Object (VBO)
    glGenBuffers(1, &vbo); // Generate the buffer, its ID will be stored in vbo

    glBindBuffer(GL_ARRAY_BUFFER, vbo); // make vbo the active array buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Copy vertex data to the active buffer

    // Shaders
    const char* vertexSource = R"glsl(
        #version 150 core

        in vec2 position;
        in vec3 color;

        out vec3 Color;

        void main()
        {
            Color = color;
            gl_Position = vec4(position, 0.0, 1.0);
        }
    )glsl";
    const char* fragmentSource = R"glsl(
        #version 150 core

        in vec3 Color;
        out vec4 outColor;

        void main()
        {
           outColor = vec4(Color, 1.0);
        }
    )glsl";

    // Load and compile shaders
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    // Get shader logs
    char buffer[512];
    glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
    fprintf(stderr, "Shader Log: %s\n", buffer);

    // create shader program by combining the shaders
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    // link shader program and use it
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    // Get the position attribute from shader program
    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    // and specify its properties (attrib_number, number of values, type of each, ..., stride, offset)
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);

    // Get the color attribute
    GLint colorAttrib = glGetAttribLocation(shaderProgram, "color");
    glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (2 * sizeof(float)));

    // Get the triangleColor Uniform
    //GLint triangleColor = glGetUniformLocation(shaderProgram, "triangleColor");
    //glUniform3f(triangleColor, 1.0f, 0.0f, 0.0f);

    // Enable vertex attribute array
    glEnableVertexAttribArray(posAttrib);
    glEnableVertexAttribArray(colorAttrib);

    GLenum errorcode = glGetError();
    if (errorcode == GL_NO_ERROR) {
        fprintf(stderr, "No errors\n");
    }
    else {
        fprintf(stderr, "Some error. Error code: %d\n", errorcode);
    }

    // start a timer for fade/breathe effect
    //auto t_start = std::chrono::high_resolution_clock::now();

    // the multiplier for breathe/fade effect, lower value makes it slower
    //float breathe_multiplier = 2.0f;

    // Rendering Loop
    while (glfwWindowShouldClose(mWindow) == false) {
        if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(mWindow, true);

        // Background Fill Color
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // get time since startup
        //auto t_now = std::chrono::high_resolution_clock::now();
        //float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();

        // change color wrt sine of time
        //glUniform3f(triangleColor, (sin(time * breathe_multiplier) + 1.0f) / 2.0f, 0.0f, 0.0f);

        // draw the arrays
        glDrawArrays(GL_TRIANGLES, 0, 5);

        // Flip Buffers and Draw
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
    }

    glDeleteProgram(shaderProgram);
    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);

    glDeleteBuffers(1, &vbo);

    glDeleteVertexArrays(1, &vao);

    glfwTerminate();

    return EXIT_SUCCESS;
}

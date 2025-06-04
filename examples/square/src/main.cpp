#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "window.hpp"
#include "shader.hpp"
#include "vbo.hpp"

int main()
{
    glrhi::window window(1280, 720, "Test");

    glrhi::shader shader("../examples/square/shaders/basic.vert", "../examples/square/shaders/basic.frag");
    shader.setVec3("color", glm::vec3(1.0f, 1.0f, 0.0f));

    float vertices[] = {
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };
    unsigned int VAO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glrhi::vbo VBO(vertices, sizeof(vertices));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!window.shouldClose())
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
 
        window.swapBuffers();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}
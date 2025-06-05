#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "glrhi/glrhi.hpp"

int main()
{
    glrhi::window window(1280, 720, "Texture example");

    glrhi::shader shader("../examples/texture/shaders/basic.vert", "../examples/texture/shaders/basic.frag");

    float vertices[] = {
         0.0f,  0.5f, 0.0f, 0.5f, 1.0f,
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f
    };
    unsigned int indices[] = {
        0, 1, 2
    };
    
    glrhi::vao VAO;

    glrhi::vbo VBO(vertices, sizeof(vertices));
    glrhi::ebo EBO(indices, sizeof(indices));

    VAO.addAttribute(3, GL_FLOAT, GL_FALSE, 0);
    VAO.addAttribute(2, GL_FLOAT, GL_FALSE, 3*sizeof(float));

    VAO.init(VBO, EBO, 5*sizeof(float));

    glrhi::texture txt;
    txt.loadFile("../examples/texture/red_brick.jpg", GL_RGB8);

    shader.use();
    txt.bind(0, shader, "txt");
    VAO.bind();

    while (!window.shouldClose())
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
 
        window.swapBuffers();
    }

    return 0;
}
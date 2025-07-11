#include "glrhi/glrhi.hpp"

int main()
{
    glrhi::window window(1280, 720, "Uniform buffer example");

    glrhi::shader shader("../examples/uniform_buffer/shaders/basic.vert", "../examples/uniform_buffer/shaders/basic.frag");
    shader.setVec3("color", glm::vec3(1.0f, 0.0f, 1.0f));

    float vertices[] = {
         0.0f,  0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f
    };
    unsigned int indices[] = {
        0, 1, 2
    };
    
    glrhi::vao VAO;

    glrhi::vbo VBO(vertices, sizeof(vertices));
    glrhi::ebo EBO(indices, sizeof(indices));

    VAO.addAttribute(3, GL_FLOAT, GL_FALSE, 0);
    VAO.init(VBO, EBO, 3*sizeof(float));

    unsigned int uboExampleBlock;
    glGenBuffers(1, &uboExampleBlock); // gen buffer
    glBindBuffer(GL_UNIFORM_BUFFER, uboExampleBlock); // bind
    glBufferData(GL_UNIFORM_BUFFER, 16, NULL, GL_STATIC_DRAW); // allocate memory
    glBindBuffer(GL_UNIFORM_BUFFER, 0); // unbind

    glBindBufferBase(GL_UNIFORM_BUFFER, 0, uboExampleBlock); // set binding point

    glBindBuffer(GL_UNIFORM_BUFFER, uboExampleBlock); // bind
    float color[4] = {1.0f, 0.0f, 1.0f, 1.0f}; // the color to send
    glBufferSubData(GL_UNIFORM_BUFFER, 0, 16, &color); // send the data
    glBindBuffer(GL_UNIFORM_BUFFER, 0); // unbind

    while (!window.shouldClose())
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();
        VAO.bind();
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
 
        window.swapBuffers();
    }

    return 0;
}
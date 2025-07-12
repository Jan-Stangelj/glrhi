#include "glrhi/glrhi.hpp"

int main()
{
    glrhi::window window(1280, 720, "SSBO example");

    glrhi::shader shader("../examples/ssbo/shaders/basic.vert", "../examples/ssbo/shaders/basic.frag");

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


    GLuint SSBO;
    glCreateBuffers(1, &SSBO);

    glNamedBufferStorage(SSBO, sizeof(float) * 4, nullptr, GL_DYNAMIC_STORAGE_BIT);

    float color[] = {1.0f, 0.5f, 1.0f, 1.0f};
    glNamedBufferSubData(SSBO, 0, sizeof(float) * 4, color);

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, SSBO);


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
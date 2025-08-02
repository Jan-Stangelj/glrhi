#include <glrhi/glrhi.hpp>
#include <glrhi/glrenderer.hpp>

#include <vector>

int main()
{
    glrhi::window window(1280, 720, "Mesh example");

    glrhi::shader shader("../examples/mesh/shaders/basic.vert", "../examples/mesh/shaders/basic.frag");
    shader.setVec3("color", glm::vec3(1.0f, 0.0f, 1.0f));

    std::vector<glrhi::vertex> vertices = {
         {glm::vec3(0.0f,  0.5f, 0.0f), glm::vec2(0.0f, 0.0f)},
         {glm::vec3(0.5f, -0.5f, 0.0f), glm::vec2(0.0f, 0.0f)},
         {glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec2(0.0f, 0.0f)}
    };
    std::vector<GLuint> indices = {
        0, 1, 2
    };
    
    glrhi::mesh triangle(vertices, indices);

    while (!window.shouldClose())
    {
        window.poolEvents();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();
        triangle.draw();
 
        window.swapBuffers();
    }

    return 0;
}
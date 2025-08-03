#include "glrhi/renderer/debugcam.hpp"
#include <glrhi/glrhi.hpp>
#include <glrhi/glrenderer.hpp>

#include <vector>

int main()
{
    glrhi::window window(1280, 720, "Camera example");

    glrhi::shader shader("../examples/camera/shaders/basic.vert", "../examples/camera/shaders/basic.frag");
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

    glrhi::camera cam;
    cam.position.z = 3.0f;

    cam.uploadData();
    cam.bind();

    glrhi::debugCamera dbgcam;

    while (!window.shouldClose())
    {
        window.poolEvents();

        dbgcam.apply(cam, window, 0.01f);
        cam.uploadData();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();
        triangle.draw();
 
        window.swapBuffers();
    }

    return 0;
}
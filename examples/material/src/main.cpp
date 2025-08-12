#include <glrhi/glrhi.hpp>
#include <glrhi/renderer.hpp>

#include <vector>
#include <chrono>

int main()
{
    glrhi::window window(1280, 720, "Material example");

    glrhi::shader shader("../examples/material/shaders/basic.vert", "../examples/material/shaders/basic.frag");

    std::vector<glrhi::vertex> vertices = {
         {glm::vec3(0.0f,  0.5f, 0.0f), glm::vec2(0.5f, 1.0f), glm::vec3(0.0f), glm::vec3(0.0f)},
         {glm::vec3(0.5f, -0.5f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.0f)},
         {glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.0f)}
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

    glrhi::material mat;

    mat.setAlbedoTexture("../examples/material/albedo.jpg");
    mat.setARMtexture("../examples/material/arm.jpg");
    mat.setNormalTexture("../examples/material/normal.png");

    mat.bind(shader);

    auto timer = std::chrono::high_resolution_clock::now();

    while (!window.shouldClose()) {

        auto currentTime = std::chrono::high_resolution_clock::now();

        std::chrono::duration<float> delta = currentTime - timer;
        float dTime = delta.count();
        timer = currentTime;

        window.poolEvents();

        dbgcam.apply(cam, window, dTime);
        cam.uploadData();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();
        triangle.draw();
 
        window.swapBuffers();
    }

    return 0;
}
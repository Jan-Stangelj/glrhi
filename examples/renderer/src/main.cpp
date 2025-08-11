#include "glrhi/core/ebo.hpp"
#include <glrhi/glrhi.hpp>
#include <glrhi/glrenderer.hpp>

#include <chrono>
#include <iostream>
#include <cmath>

int main()
{
    glrhi::window window(1280, 720, "Model example");

    glrhi::shader buffer("../examples/renderer/shaders/buffer.vert", "../examples/renderer/shaders/buffer.frag");

    glrhi::model sponza("../examples/renderer/sponza/Sponza.gltf");
    sponza.size = glm::vec3(0.1f);

    glrhi::gbuffer gbuffer(1280, 720);

    glrhi::camera cam;
    cam.position.z = 3.0f;

    cam.uploadData();
    cam.bind();

    glrhi::debugCamera dbgcam;

    auto timer = std::chrono::high_resolution_clock::now();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    while (!window.shouldClose()) {

        auto currentTime = std::chrono::high_resolution_clock::now();

        std::chrono::duration<float> delta = currentTime - timer;
        float dTime = delta.count();
        timer = currentTime;
        std::cout << floor(1.0f / dTime) << '\n';

        window.poolEvents();

        dbgcam.apply(cam, window, dTime);
        cam.uploadData();

        gbuffer.bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        sponza.draw(buffer);

        gbuffer.renderResoult();
 
        window.swapBuffers();
    }

    return 0;
}
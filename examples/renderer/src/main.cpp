#include <glrhi/glrhi.hpp>
#include <glrhi/glrenderer.hpp>

#include <glrhi/utils/timer.hpp>

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

    glrhi::timer timer;

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    while (!window.shouldClose()) {

        float dTime = timer.get();
        timer.reset();
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
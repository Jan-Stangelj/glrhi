#include "glrhi/core/compute.hpp"
#include "glrhi/renderer/model.hpp"
#include <glrhi/glrhi.hpp>
#include <glrhi/glrenderer.hpp>

#include <glrhi/utils/timer.hpp>

#include <iostream>
#include <cmath>

int main()
{
    glrhi::window window(1280, 720, "Model example");

    glrhi::shader buffer("../examples/renderer/shaders/buffer.vert", "../examples/renderer/shaders/buffer.frag");
    glrhi::compute postProcess("../examples/renderer/shaders/postprocess.comp");

    glrhi::model sponza("../examples/renderer/sponza/Sponza.gltf");
    sponza.size = glm::vec3(0.01f);

    glrhi::model helmet("../examples/renderer/helmet/DamagedHelmet.gltf");
    helmet.size = glm::vec3(0.5f);
    helmet.position = glm::vec3(0.0f, 1.5f, 0.0f);
    helmet.rotation = glm::vec3(90.0f, 0.0f, 0.0f);

    glrhi::gbuffer gbuffer(1280, 720);

    glrhi::camera cam;
    cam.position.y = 5.0f;

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
        helmet.draw(buffer);

        gbuffer.bindTextures(postProcess);
        postProcess.dispatch(1280, 720, 1);

        gbuffer.renderResoult();
 
        window.swapBuffers();
    }

    return 0;
}
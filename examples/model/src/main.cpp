#include "glm/fwd.hpp"
#include "glrhi/core/ebo.hpp"
#include "glrhi/renderer/model.hpp"
#include <glrhi/glrhi.hpp>
#include <glrhi/glrenderer.hpp>

#include <chrono>

int main()
{
    glrhi::window window(1280, 720, "Model example");

    glrhi::shader shader("../examples/model/shaders/basic.vert", "../examples/model/shaders/basic.frag");

    glrhi::model test("../examples/model/sponza/Sponza.gltf");
    test.size = glm::vec3(0.1f);

    glrhi::camera cam;
    cam.position.z = 3.0f;

    cam.uploadData();
    cam.bind();

    glrhi::debugCamera dbgcam;

    auto timer = std::chrono::high_resolution_clock::now();

    glEnable(GL_DEPTH_TEST);

    while (!window.shouldClose()) {

        auto currentTime = std::chrono::high_resolution_clock::now();

        std::chrono::duration<float> delta = currentTime - timer;
        float dTime = delta.count();
        timer = currentTime;

        window.poolEvents();

        dbgcam.apply(cam, window, dTime);
        cam.uploadData();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        test.draw(shader);
 
        window.swapBuffers();
    }

    return 0;
}
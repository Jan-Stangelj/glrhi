#include "glrhi/core/ebo.hpp"
#include "glrhi/core/fbo.hpp"
#include "glrhi/core/texture2D.hpp"
#include <glrhi/glrhi.hpp>
#include <glrhi/glrenderer.hpp>

#include <chrono>
#include <iostream>
#include <cmath>

int main()
{
    glrhi::window window(1280, 720, "Model example");

    glrhi::shader buffer("../examples/renderer/shaders/buffer.vert", "../examples/renderer/shaders/buffer.frag");
    glrhi::shader out("../examples/renderer/shaders/out.vert", "../examples/renderer/shaders/out.frag");

    glrhi::model sponza("../examples/renderer/sponza/Sponza.gltf");
    sponza.size = glm::vec3(0.1f);

    glrhi::texture2D albedo(1280, 720, GL_RGB8);
    glrhi::texture2D normalRoughness(1280, 720, GL_RGBA16F);
    glrhi::texture2D emissionMetallic(1280, 720, GL_RGBA8);
    glrhi::texture2D position(1280, 720, GL_RGB16F);

    glrhi::texture2D depth(1280, 720, GL_DEPTH_COMPONENT24);

    glrhi::fbo gBuffer;

    gBuffer.attachColorTexture(albedo);
    gBuffer.attachColorTexture(normalRoughness);
    gBuffer.attachColorTexture(emissionMetallic);
    gBuffer.attachColorTexture(position);
    gBuffer.attachDepthTexture(depth);

    gBuffer.init();

    float quad[] = {
        -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f
    };

    unsigned int quadIndices[] = {
        0, 1, 2,
        1, 2, 3
    };

    glrhi::vao quadVAO;

    glrhi::vbo quadVBO(quad, sizeof(quad));
    glrhi::ebo quadEBO(quadIndices, sizeof(quadIndices));

    quadVAO.addAttribute(3, GL_FLOAT, GL_FALSE, 0);
    quadVAO.addAttribute(2, GL_FLOAT, GL_FALSE, 3*sizeof(float));
    quadVAO.init(quadVBO, quadEBO, 5*sizeof(float));

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
        std::cout << floor(1.0f / dTime) << '\n';

        window.poolEvents();

        dbgcam.apply(cam, window, dTime);
        cam.uploadData();

        gBuffer.bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        sponza.draw(buffer);

        gBuffer.unbind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        albedo.bind(0);
        out.setInt("u_albedo", 0);

        normalRoughness.bind(1);
        out.setInt("u_normalRoughness", 1);

        emissionMetallic.bind(2);
        out.setInt("u_emissionMetallic", 2);

        position.bind(3);
        out.setInt("u_position", 3);

        out.use();
        quadVAO.bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
 
        window.swapBuffers();
    }

    return 0;
}
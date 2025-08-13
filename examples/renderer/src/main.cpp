#include "glm/trigonometric.hpp"
#include "glrhi/renderer/lighting.hpp"
#include <glrhi/renderer.hpp>

int main()
{
    glrhi::renderer renderer(1280, 720, "Renderer example");

    //renderer.pushPostProcessShader("../examples/renderer/shaders/postprocess.comp");

    renderer.addModel("sponza", "../examples/renderer/sponza/Sponza.gltf");
    renderer.getModel("sponza").size = glm::vec3(0.01f);

    renderer.addModel("helmet", "../examples/renderer/helmet/DamagedHelmet.gltf");
    renderer.getModel("helmet").size = glm::vec3(0.5f);
    renderer.getModel("helmet").position = glm::vec3(0.0f, 1.5f, 0.0f);
    renderer.getModel("helmet").rotation = glm::vec3(90.0f, 0.0f, 0.0f);

    renderer.addLight("light1");
    glrhi::light& l1 = renderer.getLight("light1");
    l1.type = glrhi::POINT;
    l1.direction = glm::vec4(1.0f);
    l1.position = glm::vec4(0.0f, 3.0f, 0.0f, 0.0f);
    l1.color = glm::vec4(1.0f);
    l1.strength = 100.0f;

    glrhi::debugCamera dbgcam;

    while (renderer.running()) {

        renderer.getInput();

        dbgcam.apply(renderer.getCamera(), renderer.getWindow(), renderer.deltaTime());

        renderer.gBufferPass();
        renderer.lightingPass();
        renderer.postProcessPass();
        renderer.renderResoult();
    }

    return 0;
}
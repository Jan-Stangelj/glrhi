#include "glrhi/renderer/lighting.hpp"
#include <glrhi/renderer.hpp>

int main()
{
    glrhi::renderer renderer(1280, 720, "Renderer example");

    renderer.pushPostProcessShader("../examples/renderer/shaders/postprocess.comp");

    glrhi::scene scene;

    unsigned int sponza = scene.addModel("../examples/renderer/sponza/Sponza.gltf");
    scene.getModel(sponza).size = glm::vec3(0.01f);

    unsigned int helmet = scene.addModel("../examples/renderer/helmet/DamagedHelmet.gltf");
    scene.getModel(helmet).size = glm::vec3(0.5f);
    scene.getModel(helmet).position = glm::vec3(0.0f, 1.5f, 0.0f);
    scene.getModel(helmet).rotation = glm::vec3(90.0f, 0.0f, 0.0f);

    unsigned int light = scene.addLight();
    glrhi::light& l1 = scene.getLight(light);
    l1.type = glrhi::POINT;
    l1.direction = glm::vec4(1.0f);
    l1.position = glm::vec4(0.0f, 3.0f, 0.0f, 0.0f);
    l1.color = glm::vec4(1.0f);
    l1.strength = 100.0f;

    glrhi::debugCamera dbgcam;

    while (renderer.running()) {

        renderer.getInput();

        dbgcam.apply(renderer.getCamera(), renderer.getWindow(), renderer.deltaTime());

        renderer.gBufferPass(scene);
        renderer.lightingPass(scene);
        renderer.postProcessPass();
        renderer.renderResoult();
    }

    return 0;
}
#include <glrhi/renderer.hpp>

int main()
{
    glrhi::renderer renderer(1280, 720, "Renderer example");
    glrhi::scene scene;

    unsigned int sponza = scene.addModel("../examples/renderer/sponza/Sponza.gltf");
    scene.getModel(sponza).size = glm::vec3(0.01f);

    unsigned int helmet = scene.addModel("../examples/renderer/helmet/DamagedHelmet.gltf");
    scene.getModel(helmet).size = glm::vec3(0.5f);
    scene.getModel(helmet).position = glm::vec3(0.0f, 1.5f, 0.0f);
    scene.getModel(helmet).rotation = glm::vec3(90.0f, 0.0f, 0.0f);

    scene.setSkybox("../examples/renderer/skybox.hdr");

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
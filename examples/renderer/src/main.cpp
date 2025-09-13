#include "glrhi/core/compute.hpp"
#include "glrhi/core/ebo.hpp"
#include "glrhi/core/shader.hpp"
#include "glrhi/core/texture2D.hpp"
#include "glrhi/renderer/camera.hpp"
#include "glrhi/renderer/lighting.hpp"
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

    unsigned int sun = scene.addLight();
    scene.getLight(sun).direction = glm::vec4(0.5f, 1.0f, 0.5f, 1.0f);
    scene.getLight(sun).color = glm::vec4(1.0f);
    scene.getLight(sun).strength = 5.0f;
    scene.getLight(sun).type = glrhi::DIRECTIONAL;

    scene.setSkybox("../examples/renderer/skybox.hdr");

    glrhi::debugCamera dbgcam;

    glrhi::camera voxelCam;
    voxelCam.position = glm::vec3(0.0f, 10.0f, 0.0f);
    voxelCam.direction = glm::vec3(-90.0f, 0.0f, 0.0f);
    voxelCam.type = true;
    voxelCam.near = -20.0f;
    voxelCam.far = 20.0f;
    voxelCam.width = 40;
    voxelCam.height = 40;
    voxelCam.uploadData();

    unsigned int voxelTex = 0;
    glCreateTextures(GL_TEXTURE_3D, 1, &voxelTex);


    glTextureParameteri(voxelTex, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(voxelTex, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Texture wrapping options
    glTextureParameteri(voxelTex, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTextureParameteri(voxelTex, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTextureParameteri(voxelTex, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
    float borderColor[] = {0.0f, 0.0f, 0.0f, 0.0f};
    glTextureParameterfv(voxelTex, GL_TEXTURE_BORDER_COLOR, borderColor);

    glTextureStorage3D(voxelTex, 1, GL_RGBA16F, 64, 64, 64);

    //glBindTextureUnit(voxelTex, 0);
    //glBindImageTexture(0, voxelTex, 0, GL_TRUE, 0, GL_READ_WRITE, GL_RGBA16F);

    glrhi::shader voxelization("../shaders/voxelization.vert", "../shaders/voxelization.frag", "../shaders/voxelization.geom");
    glrhi::compute clearVoxels("../shaders/voxelClear.comp");

    while (renderer.running()) {

        renderer.getInput();

        glViewport(0, 0, 64, 64);
        glBindImageTexture(0, voxelTex, 0, GL_TRUE, 0, GL_READ_WRITE, GL_RGBA16F);
        clearVoxels.dispatch(64, 64, 64);
        voxelization.use();
        voxelCam.bind();
        glDisable(GL_CULL_FACE);
        scene.drawModels(voxelization);
        glEnable(GL_CULL_FACE);

        glViewport(0, 0, 1280, 720);

        dbgcam.apply(renderer.getCamera(), renderer.getWindow(), renderer.deltaTime());
        
        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_TEXTURE_FETCH_BARRIER_BIT);
        glBindTextureUnit(8, voxelTex);
        renderer.gBufferPass(scene);
        renderer.lightingPass(scene);
        renderer.renderResoult();
    }

    return 0;
}
#include "glm/fwd.hpp"
#include "glrhi/core/shader.hpp"
#include "glrhi/core/texture2D.hpp"
#include "glrhi/core/ubo.hpp"
#include "glrhi/renderer/camera.hpp"
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

    scene.sunDir = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
    scene.sunColor = glm::vec4(1.0f);
    scene.sunStrenght = 10.0f;

    scene.setSkybox("../examples/renderer/skybox.hdr");

    glrhi::debugCamera dbgcam;

    // Voxel settings
    float size = 40.0f;
    float resolution = 256.0f;

    // Voxelization begin
    glrhi::camera voxelCam;
    voxelCam.position = glm::vec3(0.0f, size / 2, 0.0f);
    voxelCam.direction = glm::vec3(-90.0f, 0.0f, 0.0f);
    voxelCam.type = true;
    voxelCam.near = 0.1f;
    voxelCam.far = size;
    voxelCam.width = size;
    voxelCam.height = size;
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

    glTextureStorage3D(voxelTex, 1, GL_RGBA16F, resolution, resolution, resolution);

    float settings[] = {size, resolution};
    glrhi::ubo voxelSettings(sizeof(float)*2, settings);
    voxelSettings.addBindingPoint(2);

    glrhi::shader voxelization("../shaders/voxelization.vert", "../shaders/voxelization.frag", "../shaders/voxelization.geom");
    glrhi::shader drawVoxels("../shaders/drawVoxels.vert", "../shaders/drawVoxels.frag", "../shaders/drawVoxels.geom");

    renderer.getCamera().position = glm::vec3(0.0f);

    glViewport(0, 0, resolution, resolution);
    glBindImageTexture(0, voxelTex, 0, GL_TRUE, 0, GL_READ_WRITE, GL_RGBA16F);
    float color[] = {0.0f, 0.0f, 0.0f, 0.0f};
    glClearTexImage(voxelTex, 0, GL_RGBA, GL_FLOAT, color);
    voxelization.use();
    voxelCam.bind();
    glDisable(GL_CULL_FACE);
    scene.drawModels(voxelization);
    glEnable(GL_CULL_FACE);
    glViewport(0, 0, 1280, 720);
    // Voxelization end

    while (renderer.running()) {

        renderer.getInput();

        dbgcam.apply(renderer.getCamera(), renderer.getWindow(), renderer.deltaTime());
        
        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_TEXTURE_FETCH_BARRIER_BIT);
        glBindTextureUnit(8, voxelTex);
        //renderer.gBufferPass(scene);
        //renderer.lightingPass(scene);
        //renderer.renderResoult();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        renderer.getCamera().bind();
        renderer.getCamera().uploadData();

        glBindImageTexture(0, voxelTex, 0, GL_TRUE, 0, GL_READ_WRITE, GL_RGBA16F);
        drawVoxels.use();
        glDrawArrays(GL_POINTS, 0, resolution * resolution * resolution);

        renderer.getWindow().swapBuffers();
    }

    return 0;
}
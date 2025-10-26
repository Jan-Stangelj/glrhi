#include "glrhi/core/ebo.hpp"
#include <cmath>
#include <cstdint>
#include <glrhi/renderer.hpp>
#include <iostream>

int main()
{
    glrhi::renderer renderer(1280, 720, "Renderer example");
    glrhi::scene scene;

    unsigned int sponza = scene.addModel("../examples/renderer/sponza/Sponza.gltf");
    scene.getModel(sponza).size = glm::vec3(0.01f);

    scene.sunDir = glm::vec4(0.35f, 1.0f, 0.35f, 1.0f);
    scene.sunColor = glm::vec4(1.0f);
    scene.sunStrenght = 25.0f;
    scene.sunAperture = 3.0f;

    scene.setSkybox("../examples/renderer/skybox.hdr");

    glrhi::debugCamera dbgcam;
    dbgcam.speed = 3.0f;

    // Voxel settings
    float size = 35.0f;
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

    unsigned int voxelAlbedoTex = 0;
    glCreateTextures(GL_TEXTURE_3D, 1, &voxelAlbedoTex);

    glTextureParameteri(voxelAlbedoTex, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(voxelAlbedoTex, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Texture wrapping options
    glTextureParameteri(voxelAlbedoTex, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTextureParameteri(voxelAlbedoTex, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTextureParameteri(voxelAlbedoTex, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
    float borderColor[] = {0.0f, 0.0f, 0.0f, 0.0f};
    glTextureParameterfv(voxelAlbedoTex, GL_TEXTURE_BORDER_COLOR, borderColor);

    glTextureStorage3D(voxelAlbedoTex, log2(resolution), GL_RGBA8, resolution, resolution, resolution);

    unsigned int voxelLightingTex = 0;
    glCreateTextures(GL_TEXTURE_3D, 1, &voxelLightingTex);

    glTextureParameteri(voxelLightingTex, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(voxelLightingTex, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Texture wrapping options
    glTextureParameteri(voxelLightingTex, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTextureParameteri(voxelLightingTex, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTextureParameteri(voxelLightingTex, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
    glTextureParameterfv(voxelLightingTex, GL_TEXTURE_BORDER_COLOR, borderColor);

    glTextureStorage3D(voxelLightingTex, log2(resolution), GL_RGBA16F, resolution, resolution, resolution);

    glrhi::ssbo tempVoxels(2 * sizeof(uint32_t) * resolution * resolution * resolution);
    GLuint clearColor = 0;

    float settings[] = {size, resolution};
    glrhi::ubo voxelSettings(sizeof(float)*2, settings);
    voxelSettings.addBindingPoint(2);

    glrhi::shader voxelization("../shaders/voxelization.vert", "../shaders/voxelization.frag", "../shaders/voxelization.geom");
    glrhi::shader drawVoxels("../shaders/drawVoxels.vert", "../shaders/drawVoxels.frag", "../shaders/drawVoxels.geom");

    glrhi::compute convertVoxels("../shaders/convertVoxels.comp");
    glrhi::compute injectRadiance("../shaders/injectRadiance.comp");

    scene.updateLightBuffer(injectRadiance);

    glViewport(0, 0, resolution, resolution);

    glBindImageTexture(0, voxelAlbedoTex, 0, GL_TRUE, 0, GL_READ_WRITE, GL_RGBA8);

    float color[] = {0.0f, 0.0f, 0.0f, 0.0f};
    glClearTexImage(voxelAlbedoTex, 0, GL_RGBA, GL_BYTE, color);

    voxelization.use();
    voxelCam.bind();

    glDisable(GL_CULL_FACE);

    glClearNamedBufferData(tempVoxels.getID(), GL_R32UI, GL_RED_INTEGER, GL_UNSIGNED_INT, &clearColor);

    tempVoxels.addBindingPoint(1);
    scene.drawModels(voxelization);
    glMemoryBarrier(GL_ALL_BARRIER_BITS);

    convertVoxels.dispatch(resolution / 4, resolution / 2, resolution / 4);

    glGenerateTextureMipmap(voxelAlbedoTex);
    glMemoryBarrier(GL_ALL_BARRIER_BITS);

    glBindTextureUnit(8, voxelAlbedoTex);
    glBindImageTexture(0, voxelLightingTex, 0, GL_TRUE, 0, GL_READ_WRITE, GL_RGBA16F);
    glClearTexImage(voxelLightingTex, 0, GL_RGBA, GL_FLOAT, color);
    injectRadiance.dispatch(resolution / 4, resolution / 2, resolution / 4);
    glGenerateTextureMipmap(voxelLightingTex);

    glEnable(GL_CULL_FACE);
    glViewport(0, 0, 1280, 720);

    // Voxelization end

    while (renderer.running()) {

        renderer.getInput();

        dbgcam.apply(renderer.getCamera(), renderer.getWindow(), renderer.deltaTime());

        std::cout << renderer.deltaTime() << '\n';

        glBindTextureUnit(8, voxelLightingTex);
        renderer.gBufferPass(scene);
        renderer.lightingPass(scene);
        renderer.renderResoult();

        /*glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        renderer.getCamera().bind();
        renderer.getCamera().uploadData();

        glBindImageTexture(0, voxelLightingTex, 0, GL_TRUE, 0, GL_READ_WRITE, GL_RGBA16F);
        drawVoxels.use();
        glDrawArrays(GL_POINTS, 0, resolution * resolution * resolution);*/

        renderer.getWindow().swapBuffers();
    }

    return 0;
}
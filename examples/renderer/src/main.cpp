#include "glrhi/core/ebo.hpp"
#include <cmath>
#include <cstdint>
#include <glrhi/renderer.hpp>
#include <iostream>

int main()
{
    glrhi::renderer renderer(1920, 1080, "Renderer example");
    glrhi::scene scene;

    unsigned int sponza = scene.addModel("../examples/renderer/sponza/Sponza.gltf");
    scene.getModel(sponza).size = glm::vec3(0.01f);

    scene.sunDir = glm::vec4(0.25f, 1.0f, 0.25f, 1.0f);
    scene.sunColor = glm::vec4(1.0f, 0.9f, 0.9f, 1.0f);
    scene.sunStrenght = 50.0f;
    scene.sunAperture = 1.0f;

    scene.setSkybox("../examples/renderer/skybox.hdr");

    glrhi::debugCamera dbgcam;
    dbgcam.speed = 3.0f;

    // Voxel settings
    float size = 35.0f;
    float resolution = 512.0f;

    // Voxelization setup begin
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
    tempVoxels.addBindingPoint(1);

    float settings[] = {size, resolution};
    glrhi::ubo voxelSettings(sizeof(float)*2, settings);
    voxelSettings.addBindingPoint(2);

    glrhi::shader voxelization("../shaders/voxelization.vert", "../shaders/voxelization.frag", "../shaders/voxelization.geom");
    glrhi::shader drawVoxels("../shaders/drawVoxels.vert", "../shaders/drawVoxels.frag", "../shaders/drawVoxels.geom");

    glrhi::compute convertVoxels("../shaders/convertVoxels.comp");
    glrhi::compute injectRadiance("../shaders/injectRadiance.comp");

    // Voxelization setup end

    // Voxelization begin

    glViewport(0, 0, resolution, resolution);
    glDisable(GL_CULL_FACE);

    float color[] = {0.0f, 0.0f, 0.0f, 0.0f};
    GLuint clearColor = 0;
    glClearTexImage(voxelAlbedoTex, 0, GL_RGBA, GL_BYTE, color);
    glClearTexImage(voxelLightingTex, 0, GL_RGBA, GL_FLOAT, color);
    glClearNamedBufferData(tempVoxels.getID(), GL_R32UI, GL_RED_INTEGER, GL_UNSIGNED_INT, &clearColor);

    voxelization.use();
    voxelCam.bind();

    scene.drawModels(voxelization);

    glBindImageTexture(0, voxelAlbedoTex, 0, GL_TRUE, 0, GL_READ_WRITE, GL_RGBA8);
    convertVoxels.dispatch(resolution / 4, resolution / 2, resolution / 4);
    glGenerateTextureMipmap(voxelAlbedoTex);

    glBindTextureUnit(8, voxelAlbedoTex);
    glBindImageTexture(0, voxelLightingTex, 0, GL_TRUE, 0, GL_READ_WRITE, GL_RGBA16F);
    scene.updateLightBuffer(injectRadiance);
    injectRadiance.dispatch(resolution / 4, resolution / 2, resolution / 4);
    glGenerateTextureMipmap(voxelLightingTex);

    glEnable(GL_CULL_FACE);
    glViewport(0, 0, 1920, 1080);

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
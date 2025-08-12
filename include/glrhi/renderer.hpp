#pragma once

#include "glrhi.hpp"

#include "renderer/mesh.hpp"
#include "renderer/camera.hpp"
#include "renderer/debugcam.hpp"
#include "renderer/material.hpp"
#include "renderer/model.hpp"
#include "renderer/gbuffer.hpp"

#include "utils/timer.hpp"

#include <map>
#include <vector>
#include <string_view>
#include <filesystem>

namespace glrhi {
    class renderer {
    public:

        renderer(unsigned int width, unsigned int height, const char* title);
        ~renderer() = default;

        glrhi::camera& getCamera();

        void addModel(const char* name, const std::filesystem::path& model);
        void removeModel(const char* name);
        glrhi::model& getModel(const char* name);

        void pushPostProcessShader(const std::filesystem::path& computeShader);
        void popPostProcessShader();

        void getInput();
        void gBufferPass();
        void postProcessPass();
        void renderResoult();

        double deltaTime();

    private:

        glrhi::gbuffer m_gBuffer;
        glrhi::camera m_camera;

        glrhi::timer deltaTimer;

        std::map<std::string_view, glrhi::model> m_models;
        std::vector<glrhi::compute> m_postProcessShaders;
    };
}

#include <GLFW/glfw3.h>

#define SK_GANESH
#define SK_GL
#include <include/gpu/ganesh/SkSurfaceGanesh.h>
#include "include/gpu/GrBackendSurface.h"
#include "include/gpu/GrDirectContext.h"
#include "include/gpu/gl/GrGLInterface.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkColorSpace.h"
#include "include/core/SkSurface.h"
#include "include/core/SkTypeface.h"
#include "include/core/SkFont.h"
#include "include/core/SkTextBlob.h"
#include "include/core/SkBitmap.h"
#include "include/core/SkFontMetrics.h"
#include "include/encode/SkPngEncoder.h"

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>

#include <fmt/core.h>
#include <string>

#define SAMPLES 16

struct Skia
{
    std::unique_ptr<SkSurface> surface;
    std::unique_ptr<GrDirectContext> context;

    Skia() {}

    void init(int w, int h)
    {
        auto opts = GrContextOptions{};
        opts.fAllowMSAAOnNewIntel = true;

        auto interface = GrGLMakeNativeInterface();
        context = std::unique_ptr<GrDirectContext>(GrDirectContext::MakeGL(interface, opts).release());

        GrGLFramebufferInfo fbi{};
        fbi.fFBOID = 0;
        fbi.fFormat = GL_RGBA8;

        auto render_target = GrBackendRenderTarget{
            w, h,
            SAMPLES,
            0,
            fbi
        };

        surface = std::unique_ptr<SkSurface>(SkSurfaces::WrapBackendRenderTarget(
            context.get(),
            render_target,
            kBottomLeft_GrSurfaceOrigin,
            kRGBA_8888_SkColorType,
            nullptr,
            nullptr
        ).release());

        if (!surface)
            throw std::runtime_error{ "SkSurface is null" };
    }
};

struct GlfwApplication
{
    GLFWwindow* window;

    GlfwApplication(const GlfwApplication& copy) = delete;
    GlfwApplication& operator=(const GlfwApplication& copy) = delete;

    GlfwApplication() {}
    ~GlfwApplication()
    {
        glfwTerminate();
    }

    void init(int w, int h, const std::string& title)
    {
        if (!glfwInit())
            throw std::runtime_error{ "GLFW no init" };

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        glfwWindowHint(GLFW_SAMPLES, SAMPLES);
        glfwWindowHint(GLFW_STENCIL_BITS, 0);
        glfwWindowHint(GLFW_DEPTH_BITS, 0);

        window = glfwCreateWindow(w, h, title.c_str(), nullptr, nullptr);

        if (!window)
            throw std::runtime_error{ "GLFW no window" };

        glfwMakeContextCurrent(window);
    }

    bool is_running()
    {
        return !glfwWindowShouldClose(window);
    }

    void update()
    {
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
};

int main()
{
    GlfwApplication app{};
    app.init(800, 800, "Flywheel");

    Skia skia{};
    skia.init(800, 800);

    SkCanvas* canvas = skia.surface->getCanvas();

    SkFont textFont{};
    textFont.setTypeface(SkTypeface::MakeFromName("Iosevka Fixed Extended", SkFontStyle::Normal()));
    textFont.setSize(16);
    textFont.setEmbolden(false);
    
    SkPaint paint{};
    paint.setAntiAlias(true);
    paint.setColor(SkColors::kRed);

    while (app.is_running())
    {
        canvas->drawString("Hello World", 0, textFont.getSpacing(), textFont, paint);
        canvas->flush();

        app.update();
    }
}
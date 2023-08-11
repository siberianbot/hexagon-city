#include <exception>
#include <iostream>

#include <Penrose/Assets/AssetDictionary.hpp>
#include <Penrose/Assets/AssetManager.hpp>
#include <Penrose/Core/Engine.hpp>
#include <Penrose/ECS/ECSManager.hpp>
#include <Penrose/Rendering/RenderContext.hpp>

#include <Penrose/Builtin/Rendering/ForwardSceneDrawRenderOperator.hpp>

#include "src/BulidingComponent.hpp"

using namespace Penrose;

void printError(const std::exception &error, int level) { // NOLINT(misc-no-recursion)
    if (level > 1) {
        std::cerr << std::string(2 * level, ' ');
    }

    if (level != 0) {
        std::cerr << "-> ";
    }

    std::cerr << error.what() << std::endl;

    try {
        std::rethrow_if_nested(error);
    } catch (const std::exception &nested) {
        printError(nested, level + 1);
    }
}

int main() {
    std::set_terminate([]() {
        auto caught = std::current_exception();

        if (caught) {
            std::cerr << "Unhandled engine exception" << std::endl;

            try {
                std::rethrow_exception(caught);
            } catch (const std::exception &error) {
                printError(error, 0);
            }
        } else {
            std::cerr << "Abnormal termination" << std::endl;
        }

#if defined(SIGTRAP)
        std::raise(SIGTRAP);
#endif

        std::abort();
    });

    Engine engine;

    engine.resources().get<AssetDictionary>()->addDir("data");

    auto assetManager = engine.resources().get<AssetManager>();
    assetManager->queueMeshLoading("models/building.obj");
    assetManager->queueShaderLoading("shaders/default-forward-rendering.vert.spv");
    assetManager->queueShaderLoading("shaders/default-forward-rendering.frag.spv");

    auto graph = RenderGraph()
            .setTarget("swapchain", RenderTarget::makeSwapchain())
            .setTarget("depth", RenderTarget::makeImage(RenderTargetType::DepthStencil,
                                                        RenderFormat::D32Float,
                                                        std::nullopt))
            .setSubgraph("default", RenderSubgraph()
                    .addAttachment(RenderAttachment("swapchain")
                                           .setClearValue(RenderAttachmentClearValue({0, 0, 0, 1}))
                                           .setLoadOp(RenderAttachmentLoadOp::Clear)
                                           .setStoreOp(RenderAttachmentStoreOp::Store)
                                           .setInitialLayout(RenderAttachmentLayout::Undefined)
                                           .setFinalLayout(RenderAttachmentLayout::Present))
                    .addAttachment(RenderAttachment("depth")
                                           .setFormat(RenderFormat::D32Float)
                                           .setClearValue(RenderAttachmentClearValue().setDepth(1))
                                           .setLoadOp(RenderAttachmentLoadOp::Clear)
                                           .setStoreOp(RenderAttachmentStoreOp::Store)
                                           .setInitialLayout(RenderAttachmentLayout::Undefined)
                                           .setFinalLayout(RenderAttachmentLayout::DepthStencilAttachment))
                    .addPass(RenderPass()
                                     .addColorAttachmentIdx(0)
                                     .setDepthStencilAttachment(1)
                                     .setOperator(RenderPassOperator(
                                             std::string(ForwardSceneDrawRenderOperator::NAME))))
            );

    auto renderContext = engine.resources().get<RenderContext>();
    renderContext->setRenderGraph(graph);

    auto ecsManager = engine.resources().get<ECSManager>();
    ecsManager->registerComponent<BuildingComponent>();

    engine.run();

    return 0;
}

#include <exception>
#include <iostream>

#include <Penrose/Engine.hpp>
#include <Penrose/Assets/AssetDictionary.hpp>
#include <Penrose/Assets/AssetManager.hpp>
#include <Penrose/ECS/ECSManager.hpp>
#include <Penrose/Rendering/RenderGraphContext.hpp>

#include <Penrose/Builtin/Penrose/Rendering/ForwardSceneDrawRenderOperator.hpp>
#include <Penrose/Builtin/Debug/Rendering/DebugUIDrawRenderOperator.hpp>

#include "src/DebugCameraSystem.hpp"
#include "src/GameUISystem.hpp"
#include "src/GridBuildingComponent.hpp"
#include "src/GridBuildingsSystem.hpp"
#include "src/GridCellComponent.hpp"
#include "src/GridDrawableProvider.hpp"
#include "src/GridGenerationSystem.hpp"
#include "src/GridPositionComponent.hpp"
#include "src/HoveredComponent.hpp"
#include "src/PlayerStateContext.hpp"
#include "src/Raycaster.hpp"
#include "src/RayCollisionVolumeComponent.hpp"
#include "src/SelectedComponent.hpp"
#include "src/SelectionSystem.hpp"

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

    engine.resources().add<GridBuildingComponentFactory, ComponentFactory>();
    engine.resources().add<GridCellComponentFactory, ComponentFactory>();
    engine.resources().add<GridPositionComponentFactory, ComponentFactory>();
    engine.resources().add<RayCollisionVolumeComponentFactory, ComponentFactory>();
    engine.resources().add<HoveredComponentFactory, ComponentFactory>();
    engine.resources().add<SelectedComponentFactory, ComponentFactory>();

    engine.resources().add<DebugCameraSystem, System>();
    engine.resources().add<GameUISystem, System>();
    engine.resources().add<GridBuildingsSystem, System>();
    engine.resources().add<GridGenerationSystem, System>();
    engine.resources().add<SelectionSystem, System>();

    engine.resources().add<GridDrawableProvider, DrawableProvider>();

    engine.resources().add<PlayerStateContext>();
    engine.resources().add<Raycaster>();

    engine.resources().get<AssetDictionary>()->addDir("data");

    auto assetManager = engine.resources().get<AssetManager>();
    assetManager->enqueue("models/building.asset");
    assetManager->enqueue("textures/building.asset");
    assetManager->enqueue("shaders/default-forward-rendering.vert.asset");
    assetManager->enqueue("shaders/default-forward-rendering.frag.asset");

    auto graph = RenderGraphInfo()
            .setTarget("swapchain", RenderTargetInfo(RenderTargetSource::Swapchain))
            .setTarget("depth", RenderTargetInfo(RenderTargetSource::Image,
                                                 RenderTargetType::DepthStencil,
                                                 RenderFormat::D32Float,
                                                 std::nullopt))
            .setSubgraph("default", RenderSubgraphInfo()
                    .addAttachment(RenderAttachmentInfo("swapchain")
                                           .setClearValue(RenderAttachmentClearValueInfo({0, 0, 0, 1}))
                                           .setLoadOp(RenderAttachmentLoadOp::Clear)
                                           .setStoreOp(RenderAttachmentStoreOp::Store)
                                           .setInitialLayout(RenderAttachmentLayout::Undefined)
                                           .setFinalLayout(RenderAttachmentLayout::Present))
                    .addAttachment(RenderAttachmentInfo("depth")
                                           .setFormat(RenderFormat::D32Float)
                                           .setClearValue(RenderAttachmentClearValueInfo().setDepth(1))
                                           .setLoadOp(RenderAttachmentLoadOp::Clear)
                                           .setStoreOp(RenderAttachmentStoreOp::Store)
                                           .setInitialLayout(RenderAttachmentLayout::Undefined)
                                           .setFinalLayout(RenderAttachmentLayout::DepthStencilAttachment))
                    .addPass(RenderSubgraphPassInfo()
                                     .addColorAttachmentIdx(0)
                                     .setDepthStencilAttachment(1)
                                     .setOperatorInfo(RenderOperatorInfo(
                                             std::string(ForwardSceneDrawRenderOperator::NAME))))
            )
            .setSubgraph("ui", RenderSubgraphInfo()
                    .addAttachment(RenderAttachmentInfo("swapchain")
                                           .setLoadOp(RenderAttachmentLoadOp::Load)
                                           .setStoreOp(RenderAttachmentStoreOp::Store)
                                           .setInitialLayout(RenderAttachmentLayout::Present)
                                           .setFinalLayout(RenderAttachmentLayout::Present))
                    .addPass(RenderSubgraphPassInfo()
                                     .addColorAttachmentIdx(0)
                                     .setOperatorInfo(RenderOperatorInfo(
                                             std::string(DebugUIDrawRenderOperator::NAME))))
            );

    auto renderContext = engine.resources().get<RenderGraphContext>();
    renderContext->setRenderGraph(graph);

    engine.run();

    return 0;
}

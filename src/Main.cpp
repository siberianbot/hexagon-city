#include <exception>
#include <iostream>

#include <Penrose/Assets/AssetDictionary.hpp>
#include <Penrose/Assets/AssetManager.hpp>
#include <Penrose/Engine.hpp>
#include <Penrose/Rendering/RenderGraphContext.hpp>

#include <Penrose/Builtin/ImGui/Rendering/ImGuiRenderOperator.hpp>
#include <Penrose/Builtin/Penrose/Rendering/ForwardSceneDrawRenderOperator.hpp>

#include "src/Components/GridBuildingComponent.hpp"
#include "src/Components/GridCellComponent.hpp"
#include "src/Components/GridPositionComponent.hpp"
#include "src/Components/HoveredComponent.hpp"
#include "src/Components/RayCollisionVolumeComponent.hpp"
#include "src/Components/SelectedComponent.hpp"
#include "src/InGameEvents.hpp"
#include "src/PlayerStateContext.hpp"
#include "src/RandomGenerator.hpp"
#include "src/Raycaster.hpp"
#include "src/Rendering/GridDrawableProvider.hpp"
#include "src/Systems/CitySimulationSystem.hpp"
#include "src/Systems/DebugCameraSystem.hpp"
#include "src/Systems/GameUISystem.hpp"
#include "src/Systems/GridBuildingsSystem.hpp"
#include "src/Systems/GridGenerationSystem.hpp"
#include "src/Systems/SelectionSystem.hpp"

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

    engine.resources()
        .add<InGameEventQueue, ResourceGroup::Events>()
        .implements<Initializable>()
        .implements<Updatable>()
        .done();

    engine.resources()
        .add<GridBuildingComponentFactory, ResourceGroup::ECSComponent>()
        .implements<ComponentFactory>()
        .done();
    engine.resources().add<GridCellComponentFactory, ResourceGroup::ECSComponent>().implements<ComponentFactory>().done(
    );
    engine.resources()
        .add<GridPositionComponentFactory, ResourceGroup::ECSComponent>()
        .implements<ComponentFactory>()
        .done();
    engine.resources()
        .add<RayCollisionVolumeComponentFactory, ResourceGroup::ECSComponent>()
        .implements<ComponentFactory>()
        .done();
    engine.resources().add<HoveredComponentFactory, ResourceGroup::ECSComponent>().implements<ComponentFactory>().done(
    );
    engine.resources().add<SelectedComponentFactory, ResourceGroup::ECSComponent>().implements<ComponentFactory>().done(
    );

    engine.resources()
        .add<CitySimulationSystem, ResourceGroup::ECSSystem>()
        .implements<Initializable>()
        .implements<System>()
        .done();
    engine.resources()
        .add<DebugCameraSystem, ResourceGroup::ECSSystem>()
        .implements<Initializable>()
        .implements<System>()
        .done();
    engine.resources()
        .add<GameUISystem, ResourceGroup::ECSSystem>()
        .implements<Initializable>()
        .implements<Runnable>()
        .implements<System>()
        .done();
    engine.resources()
        .add<GridBuildingsSystem, ResourceGroup::ECSSystem>()
        .implements<Initializable>()
        .implements<System>()
        .done();
    engine.resources()
        .add<GridGenerationSystem, ResourceGroup::ECSSystem>()
        .implements<Initializable>()
        .implements<System>()
        .done();
    engine.resources().add<SelectionSystem, ResourceGroup::ECSSystem>().implements<System>().done();

    engine.resources().add<GridDrawableProvider, ResourceGroup::Rendering>().implements<DrawableProvider>().done();

    engine.resources().add<PlayerStateContext, ResourceGroup::Custom>().done();
    engine.resources().add<RandomGenerator, ResourceGroup::Custom>().done();
    engine.resources().add<Raycaster, ResourceGroup::Custom>().done();

    engine.resources().get<AssetDictionary>()->addDir("data");

    auto assetManager = engine.resources().get<AssetManager>();
    assetManager->enqueue("layouts/player.asset");
    assetManager->enqueue("layouts/simulation.asset");
    assetManager->enqueue("layouts/selection.asset");
    assetManager->enqueue("models/building.asset");
    assetManager->enqueue("textures/building.asset");
    assetManager->enqueue("shaders/default-forward-rendering.vert.asset");
    assetManager->enqueue("shaders/default-forward-rendering.frag.asset");

    auto uiParams = ParamsCollection();
    uiParams.setString(ImGuiRenderOperator::PARAM_UI_NAME, "GameUI");

    auto graph =
        RenderGraphInfo()
            .setTarget("swapchain", RenderTargetInfo(RenderTargetSource::Swapchain))
            .setTarget(
                "depth",
                RenderTargetInfo(
                    RenderTargetSource::Image,
                    RenderTargetType::DepthStencil,
                    RenderFormat::D32Float,
                    std::nullopt
                )
            )
            .setSubgraph(
                "default",
                RenderSubgraphInfo()
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
                    .addPass(
                        RenderSubgraphPassInfo().addColorAttachmentIdx(0).setDepthStencilAttachment(1).setOperatorInfo(
                            RenderOperatorInfo(std::string(ForwardSceneDrawRenderOperator::NAME))
                        )
                    )
            )
            .setSubgraph(
                "ui",
                RenderSubgraphInfo()
                    .addAttachment(RenderAttachmentInfo("swapchain")
                                       .setLoadOp(RenderAttachmentLoadOp::Load)
                                       .setStoreOp(RenderAttachmentStoreOp::Store)
                                       .setInitialLayout(RenderAttachmentLayout::Present)
                                       .setFinalLayout(RenderAttachmentLayout::Present))
                    .addPass(RenderSubgraphPassInfo().addColorAttachmentIdx(0).setOperatorInfo(
                        RenderOperatorInfo(ImGuiRenderOperator::name(), std::move(uiParams))
                    ))
            );

    auto renderContext = engine.resources().get<RenderGraphContext>();
    renderContext->setRenderGraph(graph);

    engine.run();

    return 0;
}

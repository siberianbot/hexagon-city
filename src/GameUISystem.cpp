#include "GameUISystem.hpp"

#include <Penrose/Events/CustomEvent.hpp>

#include "src/BuildingCreateRequestedEvent.hpp"
#include "src/BuildingDestroyRequestedEvent.hpp"
#include "src/BuildingUpgradeRequestedEvent.hpp"
#include "src/GridBuildingComponent.hpp"
#include "src/GridCellComponent.hpp"
#include "src/SelectionChangedEvent.hpp"

GameUISystem::GameUISystem(ResourceSet *resources)
        : _ecsManager(resources->getLazy<ECSManager>()),
          _eventQueue(resources->getLazy<EventQueue>()),
          _uiContext(resources->getLazy<UIContext>()) {
    //
}

void GameUISystem::init() {

    auto buildingTypeDropDown = std::shared_ptr<DropDown>(new DropDown( // NOLINT(modernize-make-shared)
            "Building Type",
            {
                    {static_cast<std::uint64_t>(GridBuildingType::Residential), "Residential"},
                    {static_cast<std::uint64_t>(GridBuildingType::Commercial),  "Commercial"},
                    {static_cast<std::uint64_t>(GridBuildingType::Industrial),  "Industrial"}
            }));

    this->_cellContainer = std::shared_ptr<Container>(new Container( // NOLINT(modernize-make-shared)
            {
                    buildingTypeDropDown,
                    std::make_shared<Button>("Create building", [this, buildingTypeDropDown]() {
                        if (!buildingTypeDropDown->getSelected().has_value()) {
                            // TODO notify "Choose building type"

                            return;
                        }

                        this->_eventQueue->pushEvent<EventType::CustomEvent>(
                                makeCustomEventArgs(new BuildingCreateRequestedEvent(
                                        *this->_selection,
                                        static_cast<GridBuildingType>(*buildingTypeDropDown->getSelected())))
                        );
                    })
            }));

    this->_buildingContainer = std::shared_ptr<Container>(new Container( // NOLINT(modernize-make-shared)
            {
                    std::make_shared<Button>("Upgrade building", [this]() {
                        this->_eventQueue->pushEvent<EventType::CustomEvent>(
                                makeCustomEventArgs(new BuildingUpgradeRequestedEvent(*this->_selection))
                        );
                    }),
                    std::make_shared<Button>("Destroy building", [this]() {
                        this->_eventQueue->pushEvent<EventType::CustomEvent>(
                                makeCustomEventArgs(new BuildingDestroyRequestedEvent(*this->_selection))
                        );
                        this->_selection = std::nullopt;
                    })
            }));

    this->_selectionWindow = std::shared_ptr<Window>(new Window( // NOLINT(modernize-make-shared)
            "Selection",
            {
                    this->_cellContainer,
                    this->_buildingContainer,
            }
    ));
    this->_selectionWindow->setVisible(false);

    this->_uiContext->setRoot("Selection", this->_selectionWindow);

    this->_eventHandlerIdx = this->_eventQueue->addHandler<EventType::CustomEvent, CustomEventArgs>(
            [this](const CustomEvent *event) {
                if (event->getArgs().type == SelectionChangedEvent::name()) {
                    auto selectionChanged = std::dynamic_pointer_cast<SelectionChangedEvent>(event->getArgs().data);

                    this->_selection = selectionChanged->getNewSelection();
                }
            });
}

void GameUISystem::destroy() {
    this->_eventQueue->removeHandler(this->_eventHandlerIdx);

    this->_uiContext->removeRoot("Selection");

    this->_selectionWindow = nullptr;
}

void GameUISystem::update(float) {

    if (!this->_selection.has_value()) {
        this->_selectionWindow->setVisible(false);
        return;
    }

    this->_cellContainer->setVisible(this->_ecsManager->hasComponent<GridCellComponent>(*this->_selection));
    this->_buildingContainer->setVisible(this->_ecsManager->hasComponent<GridBuildingComponent>(*this->_selection));
    this->_selectionWindow->setVisible(true);
}

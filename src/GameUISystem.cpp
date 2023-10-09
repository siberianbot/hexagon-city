#include "GameUISystem.hpp"

#include <Penrose/Events/CustomEvent.hpp>

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

    this->_emptyLotWindow = std::shared_ptr<Window>(new Window( // NOLINT(modernize-make-shared)
            "Empty Lot",
            {
                    std::make_shared<Button>("Create building", [this]() {
                        // TODO
                    })
            }
    ));
    this->_emptyLotWindow->setVisible(false);

    this->_buildingWindow = std::shared_ptr<Window>(new Window( // NOLINT(modernize-make-shared)
            "Building",
            {
                    std::make_shared<Button>("Upgrade building", [this]() {
                        // TODO
                    }),
                    std::make_shared<Button>("Destroy building", [this]() {
                        // TODO
                    })
            }
    ));
    this->_buildingWindow->setVisible(false);

    this->_uiContext->setRoot("EmptyLot", this->_emptyLotWindow);
    this->_uiContext->setRoot("Building", this->_buildingWindow);

    this->_eventHandlerIdx = this->_eventQueue->addHandler<EventType::CustomEvent, CustomEventArgs>(
            [this](const CustomEvent *event) {
                if (event->getArgs().type == SelectionChangedEvent::name()) {
                    auto selectionChanged = std::dynamic_pointer_cast<SelectionChangedEvent>(event->getArgs().data);

                    this->_selection = selectionChanged->getNewSelection();

                    if (!this->_selection.has_value()) {
                        this->_emptyLotWindow->setVisible(false);
                        this->_buildingWindow->setVisible(false);

                        return;
                    }

                    auto maybeCell = this->_ecsManager->tryGetComponent<GridCellComponent>(*this->_selection);

                    if (maybeCell.has_value()) {
                        if ((*maybeCell)->building().has_value()) {
                            this->_selection = (*maybeCell)->building();
                        } else {
                            this->_emptyLotWindow->setVisible(true);

                            return;
                        }
                    }

                    auto maybeBuilding = this->_ecsManager->tryGetComponent<GridBuildingComponent>(*this->_selection);

                    if (maybeBuilding.has_value()) {
                        this->_buildingWindow->setVisible(true);
                    }
                }
            });
}

void GameUISystem::destroy() {
    this->_eventQueue->removeHandler(this->_eventHandlerIdx);

    this->_uiContext->removeRoot("EmptyLot");
    this->_uiContext->removeRoot("Building");

    this->_emptyLotWindow = nullptr;
    this->_buildingWindow = nullptr;
}

void GameUISystem::update(float) {
    if (!this->_selection.has_value()) {
        return;
    }

    // TODO
}

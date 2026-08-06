#pragma once

#include "input/InputEvent.h"
#include "input/InputManager.h"

class Application {
public:
    explicit Application(InputManager& inputManager);

    void update();

    bool hasReceivedInput() const;
    const InputEvent& getLastInput() const;

private:
    void handleInput(const InputEvent& event);

    InputManager& inputManager;
    InputEvent lastInput{};
    bool receivedInput = false;
};
#pragma once

#include <queue>

#include "input/IInputDevice.h"

class FakeInputDevice final : public IInputDevice {
public:
    void addEvent(const InputEvent& event) {
        events.push(event);
    }

    bool poll(InputEvent& event) override {
        if (events.empty()) {
            return false;
        }

        event = events.front();
        events.pop();

        return true;
    }

private:
    std::queue<InputEvent> events;
};
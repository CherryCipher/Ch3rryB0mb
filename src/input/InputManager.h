#pragma once

#include <vector>

#include "IInputDevice.h"

//v1
//This inputmanager class is used to manage all input devices that are added to it.
//It allows for polling of input events from all input devices that have been added to it.
class InputManager {
public:
    void addDevice(IInputDevice& device);
    bool poll(InputEvent& event);

private:
    std::vector<IInputDevice*> devices;
};
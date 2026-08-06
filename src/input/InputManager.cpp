#include "InputManager.h"

//v1
//This inputmanager class is used to manage all input devices that are added to it.
//It allows for polling of input events from all input devices that have been added to it.

//addDevice adds an input device to the input manager.
//This allows the input manager to poll for events from that device.
void InputManager::addDevice(IInputDevice& device) {
    devices.push_back(&device);
}

//Poll polls all input devices that have been added to the input manager for input events.
//If an input event is found, it is returned in the event parameter and the function returns true.
//If no input events are found, the function returns false.
bool InputManager::poll(InputEvent& event) {
    for (IInputDevice* device : devices) {
        if (device->poll(event)) {
            return true;
        }
    }

    return false;
}
#pragma once

#include "InputEvent.h"

//V1
//An interface for input devices to implement.
//This allows the input system to poll for events from different types of input device.
//I made this to allow for different types of input devices to be used in the future,
//such as a touchscreen from the CYD but also buttons
class IInputDevice {
public:
    virtual ~IInputDevice() = default;

    //this virtual function is used to poll for input events from any input device.
    //The =0 makes this function pure virtual
    virtual bool poll(InputEvent& event) = 0;
};
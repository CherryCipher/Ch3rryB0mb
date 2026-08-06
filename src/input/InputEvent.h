#pragma once

#include "InputAction.h"

enum class InputEventType {
    Pressed,
    Released,
    LongPressed
};

struct InputEvent {
    InputAction action;
    InputEventType type;
};
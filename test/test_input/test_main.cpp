#include <unity.h>

#include "input/InputEvent.h"

void setUp() {
}

void tearDown() {
}

void test_input_event_stores_action_and_type() {
    InputEvent event{
        InputAction::Down,
        InputEventType::Pressed
    };

    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(InputAction::Down),
        static_cast<int>(event.action)
    );

    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(InputEventType::Pressed),
        static_cast<int>(event.type)
    );
}

int main() {
    UNITY_BEGIN();

    RUN_TEST(test_input_event_stores_action_and_type);

    return UNITY_END();
}
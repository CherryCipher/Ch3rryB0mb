#include <unity.h>

#include "input/InputEvent.h"
#include "FakeInputDevice.h"
#include "input/InputManager.h"
#include "app/Application.h"

void setUp() {
}

void tearDown() {
}

void test_application_has_no_input_before_update() {
    InputManager manager;
    FakeInputDevice device;

    manager.addDevice(device);

    Application application(manager);

    application.update();

    TEST_ASSERT_FALSE(application.hasReceivedInput());
}

void test_application_processes_input_during_update() {
    InputManager manager;
    FakeInputDevice device;

    manager.addDevice(device);

    Application application(manager);

    device.addEvent({
        InputAction::Select,
        InputEventType::Pressed
    });

    application.update();

    TEST_ASSERT_TRUE(application.hasReceivedInput());

    const InputEvent& receivedEvent = application.getLastInput();

    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(InputAction::Select),
        static_cast<int>(receivedEvent.action)
    );

    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(InputEventType::Pressed),
        static_cast<int>(receivedEvent.type)
    );
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

void test_fake_input_device_returns_added_event() {
    FakeInputDevice device;

    device.addEvent({
        InputAction::Down,
        InputEventType::Pressed
    });

    InputEvent receivedEvent;
    bool received = device.poll(receivedEvent);

    TEST_ASSERT_TRUE(received);

    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(InputAction::Down),
        static_cast<int>(receivedEvent.action)
    );

    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(InputEventType::Pressed),
        static_cast<int>(receivedEvent.type)
    );
}

void test_fake_input_device_returns_false_when_empty() {
    FakeInputDevice device;
    InputEvent receivedEvent;

    bool received = device.poll(receivedEvent);

    TEST_ASSERT_FALSE(received);
}

void test_input_manager_receives_event_from_second_device() {
    InputManager manager;

    FakeInputDevice firstDevice;
    FakeInputDevice secondDevice;

    manager.addDevice(firstDevice);
    manager.addDevice(secondDevice);

    secondDevice.addEvent({
        InputAction::Select,
        InputEventType::Pressed
    });

    InputEvent receivedEvent;
    bool received = manager.poll(receivedEvent);

    TEST_ASSERT_TRUE(received);

    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(InputAction::Select),
        static_cast<int>(receivedEvent.action)
    );

    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(InputEventType::Pressed),
        static_cast<int>(receivedEvent.type)
    );
}

int main() {
    UNITY_BEGIN();

    RUN_TEST(test_input_event_stores_action_and_type);
    RUN_TEST(test_fake_input_device_returns_added_event);
    RUN_TEST(test_fake_input_device_returns_false_when_empty);
    RUN_TEST(test_input_manager_receives_event_from_second_device);
    RUN_TEST(test_application_has_no_input_before_update);
    RUN_TEST(test_application_processes_input_during_update);
    return UNITY_END();
}
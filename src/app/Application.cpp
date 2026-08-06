#include "Application.h"
//v1
//Appliccation class implementation


//Input functions for the application class, which handles input events from the InputManager and stores the last input event received.
//The Application class is responsible for updating the application state and handling input events, allowing other parts of the application to query the last input event received.

//Struct to hold the last input event received by the application
Application::Application(InputManager& inputManager)
    : inputManager(inputManager) {
}

//Function to update the application state, polling for input events and handling them if received
void Application::update() {
    InputEvent event;

    if (inputManager.poll(event)) {
        handleInput(event);
    }
}

//Function to handle the input event, storing it as the last input and marking that input has been received
void Application::handleInput(const InputEvent& event) {
    lastInput = event;
    receivedInput = true;
}

//Function to check if the application has received any input events
bool Application::hasReceivedInput() const {
    return receivedInput;
}

//Constant function to retrieve the last input event received by the application
const InputEvent& Application::getLastInput() const {
    return lastInput;
}
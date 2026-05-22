#include <iostream>

#include "NetworkView.hpp"
#include "ActionInterpreter.hpp"
#include "IAction.hpp"

NetworkView::NetworkView(short port) : server_(port, message_queue_) {
    server_.start();
}

std::unique_ptr<IAction> NetworkView::waitForAction() {
    while (true) {
        // The View sleeps here until the background network thread pushes a message
        PlayerMessage msg = message_queue_.waitAndPop();
        
        auto action = ActionInterpreter::interpret(msg.command);
        if (action) {
            // TODO: We now have msg.playerId! We will need to pass this to the action/model.
            return action;
        }
        
        displayMessage("Command was not recognized. Try again :)");
    }
}

void NetworkView::waitForContinue() {
    //TODO: delete this function :(
    std::string dummy;
    std::getline(std::cin, dummy);
}

void NetworkView::displayMessage(const std::string& message) {
    // Display on the server console, then broadcast to all connected players!
    std::cout << "[SERVER BROADCAST] " << message << std::endl;
    server_.broadcast(message);
}
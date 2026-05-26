#include <iostream>

#include "NetworkView.hpp"
#include "ActionInterpreter.hpp"
#include "IAction.hpp"

NetworkView::NetworkView(asio::io_context& io_context, std::vector<asio::ip::tcp::socket> sockets) 
    : server_(io_context, std::move(sockets), message_queue_) {
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
        
        displayBroadcastMessage("Command was not recognized. Try again :)");
    }
}

void NetworkView::waitForContinue() {
    //TODO: delete this function :(
    std::string dummy;
    std::getline(std::cin, dummy);
}

void NetworkView::displayBroadcastMessage(const std::string& message) {
    // Display on the server console, then broadcast to all connected players!
    std::cout << "[SERVER BROADCAST] " << message << std::endl;
    server_.broadcast(message);
}

void NetworkView::displayMessageToPlayer(int playerId, const std::string& message) {
    //std::cout << "[SERVER -> Player " << playerId << "] " << message << std::endl;
    server_.sendToPlayer(playerId, message);
}
#include <iostream>

#include "NetworkView.hpp"
#include "ActionInterpreter.hpp"
#include "IAction.hpp"
#include "PlayerMessage.hpp"

NetworkView::NetworkView(asio::io_context& io_context, std::vector<asio::ip::tcp::socket> sockets) 
    : server_(io_context, std::move(sockets), commands_queue_) {
    server_.start();
}

std::unique_ptr<IAction> NetworkView::waitForAction() {
    while (true) {
        // The View sleeps here until the background network thread pushes a message
        PlayerMessage msg = commands_queue_.waitAndPop();
        
        auto action = ActionInterpreter::interpret(msg);
        if (action) {
            return action;
        }
        server_.sendToPlayer({msg.playerId, "Command was not recognized. Try again :)"});
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

void NetworkView::displayMessageToPlayer(const PlayerMessage& pMessage) {
    //std::cout << "[SERVER -> Player " << playerId << "] " << message << std::endl;
    server_.sendToPlayer(pMessage);
}
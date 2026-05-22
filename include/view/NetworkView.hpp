#pragma once

#include <string>
#include <memory>

#include "IGameView.hpp"
#include "TCPServer.hpp"
#include "ThreadSafeQueue.hpp"
#include "PlayerMessage.hpp"

class IAction;

class NetworkView : public IGameView {
public:
    NetworkView(short port);
    ~NetworkView() = default;

    std::unique_ptr<IAction> waitForAction() override;
    void waitForContinue() override;
    void displayMessage(const std::string& message) override;

private:
    ThreadSafeQueue<PlayerMessage> message_queue_;
    TCPServer server_;
};
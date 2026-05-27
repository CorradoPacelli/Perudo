#pragma once

#include <string>
#include <memory>
#include <vector>
#include <asio.hpp>

#include "IGameView.hpp"
#include "TCPServer.hpp"
#include "ThreadSafeQueue.hpp"

class IAction;

class NetworkView : public IGameView {
public:
    NetworkView(asio::io_context& io_context, std::vector<asio::ip::tcp::socket> sockets);
    ~NetworkView() = default;

    std::unique_ptr<IAction> waitForAction() override;
    void waitForContinue() override;
    void displayBroadcastMessage(const std::string& message) override;
    void displayMessageToPlayer(const PlayerMessage& pMessage) override;

private:
    // queue where commands for players are received
    ThreadSafeQueue<PlayerMessage> commands_queue_;
    TCPServer server_;
};
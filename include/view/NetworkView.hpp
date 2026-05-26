#pragma once

#include <string>
#include <memory>
#include <vector>
#include <asio.hpp>

#include "IGameView.hpp"
#include "TCPServer.hpp"
#include "ThreadSafeQueue.hpp"
#include "PlayerMessage.hpp"

class IAction;

class NetworkView : public IGameView {
public:
    NetworkView(asio::io_context& io_context, std::vector<asio::ip::tcp::socket> sockets);
    ~NetworkView() = default;

    std::unique_ptr<IAction> waitForAction() override;
    void waitForContinue() override;
    void displayBroadcastMessage(const std::string& message) override;
    void displayMessageToPlayer(int playerId, const std::string& message) override;

private:
    ThreadSafeQueue<PlayerMessage> message_queue_;
    TCPServer server_;
};
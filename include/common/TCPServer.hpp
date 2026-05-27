#pragma once

#include <asio.hpp>
#include <thread>
#include <string>
#include <memory>
#include <vector>

#include "PlayerMessage.hpp"
#include "ThreadSafeQueue.hpp"

class Session;

// The TCPServer class listens for incoming connections
// and manages the network thread.
class TCPServer {
public:
    TCPServer(asio::io_context& io_context, std::vector<asio::ip::tcp::socket> sockets, ThreadSafeQueue<PlayerMessage>& queue);
    ~TCPServer();

    void start();
    
    void stop();

    void broadcast(const std::string& message);

    void sendToPlayer(const PlayerMessage& pMessage);

    void addSession(std::shared_ptr<Session> session);
    void removeSession(std::shared_ptr<Session> session);

private:
    asio::io_context& io_context_;
    std::thread network_thread_;
    ThreadSafeQueue<PlayerMessage>& commands_queue_;
    std::vector<std::shared_ptr<Session>> sessions_;
    int next_player_id_{0};
};
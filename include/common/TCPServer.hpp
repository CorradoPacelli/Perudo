#pragma once

#include <asio.hpp>
#include <thread>
#include <string>
#include <set>
#include <memory>
#include "PlayerMessage.hpp"

#include "ThreadSafeQueue.hpp"

class Session;

// The TCPServer class listens for incoming connections
// and manages the network thread.
class TCPServer {
public:
    TCPServer(short port, ThreadSafeQueue<PlayerMessage>& queue);
    ~TCPServer();

    // Starts the Network Thread
    void start();
    
    // Stops the server cleanly
    void stop();

    // Broadcast a message to all connected clients
    void broadcast(const std::string& message);

    void addSession(std::shared_ptr<Session> session);
    void removeSession(std::shared_ptr<Session> session);

private:
    void doAccept();

    asio::io_context io_context_;
    asio::ip::tcp::acceptor acceptor_;
    std::thread network_thread_;
    ThreadSafeQueue<PlayerMessage>& queue_;
    std::set<std::shared_ptr<Session>> sessions_;
    int next_player_id_{0};
};
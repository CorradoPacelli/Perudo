#pragma once

#include <asio.hpp>
#include <thread>
#include <string>
#include <map>
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

    // Starts the Network Thread
    void start();
    
    // Stops the server cleanly
    void stop();

    // Broadcast a message to all connected clients
    void broadcast(const std::string& message);
    // TODO: change this int in something else, I don't like it
    void sendToPlayer(int playerId, const std::string& message);

    void addSession(std::shared_ptr<Session> session);
    void removeSession(std::shared_ptr<Session> session);

private:
    asio::io_context& io_context_;
    std::thread network_thread_;
    ThreadSafeQueue<PlayerMessage>& queue_;
    std::map<int, std::shared_ptr<Session>> sessions_;
    int next_player_id_{0};
};
#pragma once

#include <asio.hpp>
#include <memory>
#include <string>
#include <deque>
#include "PlayerMessage.hpp"
#include "ThreadSafeQueue.hpp"

class TCPServer;

// The Session class manages a single connected client (player).
// It uses enable_shared_from_this to keep the object alive as long as
// there are pending asynchronous operations.
class Session : public std::enable_shared_from_this<Session> {
public:
    Session(asio::ip::tcp::socket socket, int playerId, ThreadSafeQueue<PlayerMessage>& queue, TCPServer& server);
    
    // Starts the asynchronous read operation
    void start();

    // Safely queues a message to be sent to this player
    void send(const std::string& message);

private:
    void doRead();
    // TODO: for now doWrite can only handle broadcast messages
    void doWrite();

    asio::ip::tcp::socket socket_;
    int player_id_;
    ThreadSafeQueue<PlayerMessage>& queue_;
    TCPServer& server_;
    asio::streambuf buffer_;
    // TODO: in the future write_msgs_ will contains PLayersMessage so we can write to a single player
    std::deque<std::string> write_msgs_;
};
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

    int getPlayerId() const;

private:
    void doRead();
    // TODO: for now doWrite can only handle broadcast messages
    void doWrite();

    asio::ip::tcp::socket socket_;
    int player_id_;
    // commands is ment for input
    ThreadSafeQueue<PlayerMessage>& commands_;
    // messages is ment for output
    std::deque<std::string> messages_;
    TCPServer& server_;
    asio::streambuf buffer_;
};
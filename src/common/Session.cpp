#include "Session.hpp"
#include "TCPServer.hpp"
#include <iostream>
#include <istream>

Session::Session(asio::ip::tcp::socket socket, int playerId, ThreadSafeQueue<PlayerMessage>& queue, TCPServer& server)
    : socket_(std::move(socket)), player_id_(playerId), commands_(queue), server_(server) {}

void Session::start() {
    // Tell the server we have connected
    server_.addSession(shared_from_this());
    doRead();
}

void Session::send(const std::string& message) {
    // Post the write command to the io_context to guarantee thread safety
    auto self(shared_from_this());
    asio::post(socket_.get_executor(), [this, self, message]() {
        bool write_in_progress = !messages_.empty();
        messages_.push_back(message + "\n");
        if (!write_in_progress) {
            doWrite();
        }
    });
}

int Session::getPlayerId() const { 
    return player_id_; 
}

void Session::doRead() {
    auto self(shared_from_this());
    
    // Read asynchronously until a newline character (\n) is received,
    // which denotes the end of the message from the client.
    asio::async_read_until(socket_, buffer_, '\n',
        [this, self](std::error_code ec, std::size_t length) {
            if (!ec) {
                std::istream is(&buffer_);
                std::string message;
                std::getline(is, message);
                
                // If a client uses Windows (e.g., telnet), it will send \r\n.
                // Remove the carriage return (\r) if present.
                if (!message.empty() && message.back() == '\r') {
                    message.pop_back();
                }

                // The async thread pushes the message directly to the thread-safe queue.
                if (!message.empty()) {
                    commands_.push(PlayerMessage{player_id_, message});
                }

                // Listen again for the next message from this client.
                doRead();
            } else {
                // The client disconnected or an error occurred. Clean up memory!
                server_.removeSession(self);
            }
        });
}

void Session::doWrite() {
    auto self(shared_from_this());
    asio::async_write(socket_, asio::buffer(messages_.front()),
        [this, self](std::error_code ec, std::size_t /*length*/) {
            if (!ec) {
                messages_.pop_front();
                if (!messages_.empty()) {
                    doWrite();
                }
            } else {
                server_.removeSession(self);
            }
        });
}
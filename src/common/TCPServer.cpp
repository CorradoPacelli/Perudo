#include "TCPServer.hpp"
#include "Session.hpp"

TCPServer::TCPServer(short port, ThreadSafeQueue<PlayerMessage>& queue)
    : io_context_(),
      acceptor_(io_context_, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)),
      queue_(queue) {
    
    // Start accepting the first connection immediately
    doAccept();
}

TCPServer::~TCPServer() {
    stop();
}

void TCPServer::start() {
    // Launch the io_context in its dedicated thread (Network Thread)
    network_thread_ = std::thread([this]() {
        io_context_.run();
    });
}

void TCPServer::stop() {
    io_context_.stop();
    if (network_thread_.joinable()) {
        network_thread_.join();
    }
    sessions_.clear();
    next_player_id_ = 0;
}

void TCPServer::broadcast(const std::string& message) {
    // Use post to safely execute this on the background network thread
    asio::post(io_context_, [this, message]() {
        for (auto& session : sessions_) {
            session->send(message);
        }
    });
}

void TCPServer::addSession(std::shared_ptr<Session> session) {
    sessions_.insert(session);
}

void TCPServer::removeSession(std::shared_ptr<Session> session) {
    sessions_.erase(session);
}

void TCPServer::doAccept() {
    acceptor_.async_accept(
        [this](std::error_code ec, asio::ip::tcp::socket socket) {
            if (!ec) {
                int new_id = next_player_id_++;
                std::make_shared<Session>(std::move(socket), new_id, queue_, *this)->start();
            }
            
            // Accept the next client
            doAccept();
        });
}
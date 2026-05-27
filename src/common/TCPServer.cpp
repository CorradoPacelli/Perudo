#include "TCPServer.hpp"
#include "Session.hpp"
#include <algorithm>

TCPServer::TCPServer(asio::io_context& io_context, std::vector<asio::ip::tcp::socket> sockets, ThreadSafeQueue<PlayerMessage>& queue)
    : io_context_(io_context), commands_queue_(queue) 
{
    // We already accepted the clients in main.cpp, so we just wrap them in Sessions!
    for (auto& socket : sockets) {
        int playerId = next_player_id_++;
        auto session = std::make_shared<Session>(std::move(socket), playerId, commands_queue_, *this);
        
        // Start the async read loop for this connected client
        session->start(); 
    }
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

void TCPServer::sendToPlayer(const PlayerMessage& pMessage) {
    asio::post(io_context_, [this, pMessage]() {
        // Linear search: very fast for a small number of players!
        auto it = std::find_if(sessions_.begin(), sessions_.end(),
            [&](const std::shared_ptr<Session>& s) {
                return s->getPlayerId() == pMessage.playerId;
            });
        if (it != sessions_.end()) {
            (*it)->send(pMessage.message);
        }
    });
}

void TCPServer::addSession(std::shared_ptr<Session> session) {
    if (std::find(sessions_.begin(), sessions_.end(), session) == sessions_.end()) {
        sessions_.push_back(session);
    }
}

void TCPServer::removeSession(std::shared_ptr<Session> session) {
    sessions_.erase(std::remove(sessions_.begin(), sessions_.end(), session), sessions_.end());
}
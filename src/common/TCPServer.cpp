#include "TCPServer.hpp"
#include "Session.hpp"

TCPServer::TCPServer(asio::io_context& io_context, std::vector<asio::ip::tcp::socket> sockets, ThreadSafeQueue<PlayerMessage>& queue)
    : io_context_(io_context), queue_(queue) 
{
    // We already accepted the clients in main.cpp, so we just wrap them in Sessions!
    for (auto& socket : sockets) {
        int playerId = next_player_id_++;
        auto session = std::make_shared<Session>(std::move(socket), playerId, queue_, *this);
        sessions_[playerId] = session;
        
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
        for (auto& [id, session] : sessions_) {
            session->send(message);
        }
    });
}

void TCPServer::sendToPlayer(int playerId, const std::string& message) {
    asio::post(io_context_, [this, playerId, message]() {
        auto it = sessions_.find(playerId);
        if (it != sessions_.end()) {
            it->second->send(message);
        }
    });
}

void TCPServer::addSession(std::shared_ptr<Session> session) {
    sessions_[session->getPlayerId()] = session;
}

void TCPServer::removeSession(std::shared_ptr<Session> session) {
    sessions_.erase(session->getPlayerId());
}
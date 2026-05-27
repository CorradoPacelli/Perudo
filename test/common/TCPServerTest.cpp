#include <gtest/gtest.h>
#include <asio.hpp>
#include <memory>
#include <string>
#include <vector>
#include <thread>

#include "TCPServer.hpp"
#include "ThreadSafeQueue.hpp"
#include "PlayerMessage.hpp"

class TCPServerTest : public ::testing::Test {
protected:
    void TearDown() override {
        if (server) {
            server->stop();
        }
    }

    // Helper to start the server and connect dummy clients before the test begins
    void StartServerWithClients(int num_clients) {
        asio::ip::tcp::acceptor acceptor(server_io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), test_port));
        
        std::vector<asio::ip::tcp::socket> server_sockets;
        
        for(int i = 0; i < num_clients; ++i) {
            clients.emplace_back(std::make_unique<asio::ip::tcp::socket>(client_io_context));
            
            // Connect from a background thread to avoid any localhost handshake deadlocks
            std::thread client_thread([this, i]() {
                clients[i]->connect(asio::ip::tcp::endpoint(asio::ip::address::from_string("127.0.0.1"), test_port));
            });
            
            server_sockets.emplace_back(server_io_context);
            acceptor.accept(server_sockets.back());
            client_thread.join();
        }

        server = std::make_unique<TCPServer>(server_io_context, std::move(server_sockets), queue);
        server->start();
    }

    const short test_port = 54321;
    asio::io_context server_io_context;
    asio::io_context client_io_context;
    std::vector<std::unique_ptr<asio::ip::tcp::socket>> clients;
    ThreadSafeQueue<PlayerMessage> queue;
    std::unique_ptr<TCPServer> server;
};

TEST_F(TCPServerTest, ReceivesMessageFromClient) {
    StartServerWithClients(1);

    // Send a message ending with a newline
    std::string message = "Hello Server!\n";
    asio::write(*clients[0], asio::buffer(message));

    // Wait for the server to process it and pop it from the queue
    PlayerMessage received = queue.waitAndPop();
    EXPECT_EQ(received.playerId, 0); // The first connected client should be ID 0
    EXPECT_EQ(received.message, "Hello Server!");
}

TEST_F(TCPServerTest, HandlesWindowsCarriageReturn) {
    StartServerWithClients(1);

    // Send a message with Windows style line ending (\r\n)
    std::string message = "Windows Message\r\n";
    asio::write(*clients[0], asio::buffer(message));

    PlayerMessage received = queue.waitAndPop();
    EXPECT_EQ(received.message, "Windows Message"); // The \r should be stripped out!
}

TEST_F(TCPServerTest, BroadcastsMessageToClients) {
    StartServerWithClients(2);

    // Because Session setup in the new constructor is fully synchronous, we don't need
    // to send dummy ping messages anymore. The server already tracks the connected clients!
    server->broadcast("Hello from the Game Server!");

    // Read the response synchronously on client 1
    asio::streambuf buffer1;
    asio::read_until(*clients[0], buffer1, '\n');
    std::istream is1(&buffer1);
    std::string response1;
    std::getline(is1, response1);

    // Read the response synchronously on client 2
    asio::streambuf buffer2;
    asio::read_until(*clients[1], buffer2, '\n');
    std::istream is2(&buffer2);
    std::string response2;
    std::getline(is2, response2);

    EXPECT_EQ(response1, "Hello from the Game Server!");
    EXPECT_EQ(response2, "Hello from the Game Server!");
}

TEST_F(TCPServerTest, SendsMessageToSpecificPlayer) {
    StartServerWithClients(2);

    // Send a private message only to the second client (Player ID 1)
    server->sendToPlayer(PlayerMessage(1, "Secret message for player 1!"));

    // Read the response synchronously on client 2
    asio::streambuf buffer;
    asio::read_until(*clients[1], buffer, '\n');
    std::istream is(&buffer);
    std::string response;
    std::getline(is, response);

    EXPECT_EQ(response, "Secret message for player 1!");
}
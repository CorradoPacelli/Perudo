#include <gtest/gtest.h>
#include <asio.hpp>
#include <memory>
#include <string>

#include "TCPServer.hpp"
#include "ThreadSafeQueue.hpp"
#include "PlayerMessage.hpp"

class TCPServerTest : public ::testing::Test {
protected:
    void SetUp() override {
        // We start the server on an arbitrary testing port
        server = std::make_unique<TCPServer>(test_port, queue);
        server->start();
    }

    void TearDown() override {
        server->stop();
    }

    const short test_port = 54321;
    ThreadSafeQueue<PlayerMessage> queue;
    std::unique_ptr<TCPServer> server;
};

TEST_F(TCPServerTest, ReceivesMessageFromClient) {
    asio::io_context io_context;
    asio::ip::tcp::socket client_socket(io_context);
    
    // Connect the dummy client to our local server
    client_socket.connect(asio::ip::tcp::endpoint(asio::ip::address::from_string("127.0.0.1"), test_port));

    // Send a message ending with a newline
    std::string message = "Hello Server!\n";
    asio::write(client_socket, asio::buffer(message));

    // Wait for the server to process it and pop it from the queue
    PlayerMessage received = queue.waitAndPop();
    EXPECT_EQ(received.playerId, 0); // The first connected client should be ID 0
    EXPECT_EQ(received.command, "Hello Server!");
}

TEST_F(TCPServerTest, HandlesWindowsCarriageReturn) {
    asio::io_context io_context;
    asio::ip::tcp::socket client_socket(io_context);
    client_socket.connect(asio::ip::tcp::endpoint(asio::ip::address::from_string("127.0.0.1"), test_port));

    // Send a message with Windows style line ending (\r\n)
    std::string message = "Windows Message\r\n";
    asio::write(client_socket, asio::buffer(message));

    PlayerMessage received = queue.waitAndPop();
    EXPECT_EQ(received.command, "Windows Message"); // The \r should be stripped out!
}

TEST_F(TCPServerTest, BroadcastsMessageToClients) {
    asio::io_context io_context;
    asio::ip::tcp::socket client1(io_context);
    asio::ip::tcp::socket client2(io_context);

    // Connect both dummy clients to our local server
    client1.connect(asio::ip::tcp::endpoint(asio::ip::address::from_string("127.0.0.1"), test_port));
    client2.connect(asio::ip::tcp::endpoint(asio::ip::address::from_string("127.0.0.1"), test_port));

    // Because connection and session setup are asynchronous, we send a dummy message
    // from both clients and wait for them to appear in the queue. This mathematically
    // guarantees that the server has fully accepted them and added them to `sessions_`.
    asio::write(client1, asio::buffer("Ping 1\n"));
    asio::write(client2, asio::buffer("Ping 2\n"));

    queue.waitAndPop();
    queue.waitAndPop();

    // Now that both are securely connected, broadcast a message from the server
    server->broadcast("Hello from the Game Server!");

    // Read the response synchronously on client 1
    asio::streambuf buffer1;
    asio::read_until(client1, buffer1, '\n');
    std::istream is1(&buffer1);
    std::string response1;
    std::getline(is1, response1);

    // Read the response synchronously on client 2
    asio::streambuf buffer2;
    asio::read_until(client2, buffer2, '\n');
    std::istream is2(&buffer2);
    std::string response2;
    std::getline(is2, response2);

    EXPECT_EQ(response1, "Hello from the Game Server!");
    EXPECT_EQ(response2, "Hello from the Game Server!");
}
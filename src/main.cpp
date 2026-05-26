#include "GameModel.hpp"
#include "GameController.hpp"
#include "Player.hpp"
#include "NetworkView.hpp"
#include <asio.hpp>
#include <iostream>
#include <vector>

// Let's define the number of players required to start the game.
constexpr int REQUIRED_PLAYERS = 2;

int main() {
    try
    {
        asio::io_context io_context;
        asio::ip::tcp::acceptor acceptor(io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 8080));

        std::cout << "Perudo server is running on port 8080." << std::endl;
        std::cout << "Waiting for " << REQUIRED_PLAYERS << " players to connect..." << std::endl;

        GameModel model;
        std::vector<asio::ip::tcp::socket> client_sockets;
        client_sockets.reserve(REQUIRED_PLAYERS);

        for (int i = 0; i < REQUIRED_PLAYERS; ++i)
        {
            // This call blocks until a client connects.
            client_sockets.emplace_back(io_context);
            acceptor.accept(client_sockets.back());

            // Request the player's name
            std::string welcome_msg = "Please enter your name:\n";
            asio::write(client_sockets.back(), asio::buffer(welcome_msg));

            // Read the response synchronously
            asio::streambuf buffer;
            asio::read_until(client_sockets.back(), buffer, '\n');
            std::istream is(&buffer);
            std::string playerName;
            std::getline(is, playerName);
            if (!playerName.empty() && playerName.back() == '\r') {
                playerName.pop_back();
            }
            if (playerName.empty()) {
                playerName = "Player" + std::to_string(i + 1);
            }

            // Now that a client has connected, add a corresponding player to the model.
            model.addPlayer(Player(playerName));
            
            std::cout << "Player " << (i + 1) << " ('" << playerName << "') has connected." << std::endl;
        }

        std::cout << "\n" << model.getPlayers().size() << " players have connected. Starting the game!" << std::endl;

        NetworkView view(io_context, std::move(client_sockets));
        GameController controller(model, view);
        controller.run();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
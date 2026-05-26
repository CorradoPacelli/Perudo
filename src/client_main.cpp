#include <iostream>
#include <string>
#include <thread>
#include <asio.hpp>

int main(int argc, char* argv[]) {
    try {
        // Default to localhost and port 8080
        std::string host = "127.0.0.1";
        std::string port = "8080";

        // Allow the user to specify a host and port via command line
        if (argc == 3) {
            host = argv[1];
            port = argv[2];
        }

        // 1. Setup the ASIO context and connect to the server
        asio::io_context io_context;
        asio::ip::tcp::resolver resolver(io_context);
        auto endpoints = resolver.resolve(host, port);

        asio::ip::tcp::socket socket(io_context);
        asio::connect(socket, endpoints);

        std::cout << "========================================\n";
        std::cout << "  Connected to Perudo Server at " << host << ":" << port << "\n";
        std::cout << "  Type your commands and press Enter.\n";
        std::cout << "  Type 'quit' to exit.\n";
        std::cout << "========================================\n\n";

        // The server will first ask for our name. Let's read the prompt.
        asio::streambuf name_buffer;
        asio::read_until(socket, name_buffer, '\n');
        std::istream name_is(&name_buffer);
        std::string prompt;
        std::getline(name_is, prompt);
        std::cout << prompt << std::endl;

        // Send our name to the server
        std::string my_name;
        std::getline(std::cin, my_name);
        asio::write(socket, asio::buffer(my_name + "\n"));

        // 2. Start a background thread to continuously read messages from the server
        std::thread read_thread([&socket]() {
            try {
                asio::streambuf buffer;
                asio::error_code ec;
                while (true) {
                    // Read until the server sends a newline
                    asio::read_until(socket, buffer, '\n', ec);
                    if (ec) {
                        std::cout << "\n[Disconnected from server]" << std::endl;
                        break; // Connection closed or error occurred
                    }
                    
                    std::istream is(&buffer);
                    std::string message;
                    std::getline(is, message);
                    
                    // Clean up carriage returns if any
                    if (!message.empty() && message.back() == '\r') {
                        message.pop_back();
                    }
                    
                    std::cout << message << std::endl;
                }
            } catch (std::exception& e) {
                std::cerr << "\nRead thread exception: " << e.what() << std::endl;
            }
        });

        // 3. The Main Thread continuously reads user input from the console
        std::string input;
        while (std::getline(std::cin, input)) {
            if (input == "quit" || input == "exit") {
                break;
            }
            
            // Add the newline character because the server expects line-based messages
            input += "\n";
            asio::write(socket, asio::buffer(input));
        }

        // 4. Clean up gracefully if the user types 'quit'
        socket.close();
        if (read_thread.joinable()) {
            read_thread.join();
        }

    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
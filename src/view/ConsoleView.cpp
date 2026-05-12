#include "ConsoleView.hpp"
#include <iostream>

std::string ConsoleView::waitForInput() {
    std::string input;
    std::getline(std::cin, input);
    return input;
}

void ConsoleView::displayMessage(const std::string& message) {
    std::cout << message << std::endl;
}
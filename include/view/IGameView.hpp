#pragma once

#include <string>

class IGameView {
public:
    virtual ~IGameView() = default;

    /**
     * @brief Waits for input from the player/user.
     * @return A string containing the raw user input.
     */
    virtual std::string waitForInput() = 0;

    /**
     * @brief Displays a message to the player/user.
     */
    virtual void displayMessage(const std::string& message) = 0;
};
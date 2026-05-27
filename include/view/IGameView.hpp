#pragma once

#include <string>
#include <memory>
#include "IAction.hpp"

class PlayerMessage;

class IGameView {
public:
    virtual ~IGameView() = default;

    /**
     * @brief Waits for the user to perform an action.
     * @return A unique pointer to the generated action.
     */
    virtual std::unique_ptr<IAction> waitForAction() = 0;

    /**
     * @brief Waits for the user to just press Enter (no action parsed).
     * TODO: remove this function completely
     */
    virtual void waitForContinue() = 0;

    /**
     * @brief Displays a message to all players.
     */
    virtual void displayBroadcastMessage(const std::string& message) = 0;

    /**
     * @brief Displays a private message to a specific player.
     */
    virtual void displayMessageToPlayer(const PlayerMessage& pMessage) = 0;
};
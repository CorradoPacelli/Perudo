#pragma once

#include <string>
#include <memory>

#include "IGameView.hpp"

class IAction;

class ConsoleView : public IGameView {
public:
    virtual ~ConsoleView() = default;

    std::unique_ptr<IAction> waitForAction() override;
    void waitForContinue() override;
    void displayBroadcastMessage(const std::string& message) override;
    void displayMessageToPlayer(const PlayerMessage& pMessage) override;
};
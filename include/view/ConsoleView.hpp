#pragma once

#include "IGameView.hpp"
#include <string>
#include <memory>

class IAction;

class ConsoleView : public IGameView {
public:
    virtual ~ConsoleView() = default;

    std::unique_ptr<IAction> waitForAction() override;

    void displayMessage(const std::string& message) override;
};
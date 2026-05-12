#pragma once

#include "IGameView.hpp"
#include <string>

class ConsoleView : public IGameView {
public:
    virtual ~ConsoleView() = default;

    std::string waitForInput() override;

    void displayMessage(const std::string& message) override;
};
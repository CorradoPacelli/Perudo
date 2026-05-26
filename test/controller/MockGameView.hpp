#pragma once

#include <memory>
#include <vector>
#include <string>
#include <queue>
#include <functional>

#include "IGameView.hpp"
#include "IAction.hpp"

class MockGameView : public IGameView {
public:
    std::vector<std::string> messages;
    int actionCallCount = 0;
    std::queue<std::unique_ptr<IAction>> actionQueue;
    std::function<void()> onWaitForActionCallback;

    // Mocking the displayMessage to capture output instead of printing it to console
    virtual ~MockGameView() = default;
    void displayBroadcastMessage(const std::string& msg) override;
    void displayMessageToPlayer(int playerId, const std::string& message);
    void waitForContinue() override;
    std::unique_ptr<IAction> waitForAction() override;
};
#include "MockGameView.hpp"
#include "ExitAction.hpp"

void MockGameView::displayBroadcastMessage(const std::string& msg) {
    messages.push_back(msg);
}

void MockGameView::displayMessageToPlayer(int playerId, const std::string& message){
    displayBroadcastMessage(message);
}

std::unique_ptr<IAction> MockGameView::waitForAction() {
    actionCallCount++;

    if (onWaitForActionCallback) {
        onWaitForActionCallback();
    }

    if (!actionQueue.empty()) {
        auto action = std::move(actionQueue.front());
        actionQueue.pop();
        return action;
    }

    // By immediately returning ExitAction, we ensure the game model will eventually 
    // trigger stopGame(), fulfilling the GameController's loop termination condition.
    return std::make_unique<ExitAction>();
}

void MockGameView::waitForContinue() {
    return;
}
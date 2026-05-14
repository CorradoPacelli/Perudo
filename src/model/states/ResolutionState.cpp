#include <optional>

#include "ResolutionState.hpp"
#include "RollingState.hpp"
#include "EndGameState.hpp"
#include "GameModel.hpp"
#include "DudoAction.hpp"
#include "ExactlyAction.hpp"
#include "BidAction.hpp"
#include "Die.hpp"
#include "IGameView.hpp"

ResolutionState::ResolutionState(ActionType triggerAction) : triggerAction(triggerAction) {}

void ResolutionState::onEnter(GameModel& context) {
    std::optional<Bid> lastBid = context.getLastBid();
    if (!lastBid) throw; // TODO: the first action can not be Dudo! throw for now;

    int result = checkLastBid(context);

    switch (triggerAction)
    {
    case ActionType::DUDO :

        if (result >= 0){
            context.getCurrentPlayer().loseDie();
            resultMessage = context.getCurrentPlayer().getName() + " was wrong and loses a die!";
        } else {
            context.getPreviousAlivePlayer().loseDie();
            resultMessage = context.getPreviousAlivePlayer().getName() + " was caught lying and loses a die!";
        }
        break;

    case ActionType::EXACTLY :

        if (result == 0){
            resultMessage = context.getCurrentPlayer().getName() + " guessed exactly! Everyone else loses a die!";
            for (auto& p : context.getPlayers()) {
                if (p != context.getCurrentPlayer() ){
                    p.loseDie();
                }
            }
        } else {
            context.getCurrentPlayer().loseDice(2);
            resultMessage = context.getCurrentPlayer().getName() + " was wrong and loses 2 dice!";
        }
        break;
    
    default:
        break;
    }
}

bool ResolutionState::requiresAction() const {
    return false;
}

void ResolutionState::handleAction(GameModel& context, const IAction& action) {
    // This state does not require action
}

void ResolutionState::step(GameModel& context) {
    //Decide where to transit
    if (context.isOnlyOnePlayerAlive()){
        requestStateChange(context, std::make_unique<EndGameState>());
    } else {
        requestStateChange(context, std::make_unique<RollingState>());
    }
}

void ResolutionState::render(const GameModel& context, IGameView& view) const {
    std::string msg = "\n--- RESOLUTION PHASE---\n";
    
    for (const auto& player : context.getPlayers()) {
        if (!player.isAlive()) continue;
        msg += player.getName() + ": ";
        for (const auto& die : player.getHand()) {
            msg += std::to_string(die.getFace()) + " ";
        }
        msg += "\n";
    }
    
    msg += "\n" + resultMessage;
    msg += "\nPress Enter to continue..."; 
    
    view.displayMessage(msg);
}

int ResolutionState::checkLastBid(GameModel& context) {
    std::optional<Bid> toBeat = context.getPreviousAlivePlayer().getLastBid();
    if (!toBeat) throw; //This should never happen, for now let's throw
    int actualyQuantity = 0;
    for (auto& player : context.getPlayers()) {
        if (!player.isAlive()) continue;
        
        for (const auto& dice : player.getHand()) {
            if (dice.getFace() == toBeat->getFace() ) {
                actualyQuantity++;
            }
        }
    }
    if (actualyQuantity > toBeat->getQuantity()) {
        return 1;
    } 

    if (actualyQuantity == toBeat->getQuantity()) {
        return 0;
    }

    return -1;
}
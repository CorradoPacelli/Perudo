#include <stdexcept>

#include "ResolutionState.hpp"
#include "RollingState.hpp"
#include "EndGameState.hpp"
#include "GameModel.hpp"
#include "DudoAction.hpp"
#include "ExactlyAction.hpp"
#include "BidAction.hpp"
#include "Player.hpp"
#include "Die.hpp"
#include "IGameView.hpp"

ResolutionState::ResolutionState(ActionType triggerAction) : triggerAction(triggerAction) {}

void ResolutionState::onEnter(GameModel& context) {
    std::optional<Bid> lastBid = context.getLastBid();
    if (!lastBid) { throw std::logic_error("No previous bid to resolve"); }
    resolvedBid = lastBid;

    actualDiceQuantity = countDiceOfFace(context, resolvedBid->getFace());

    switch (triggerAction)
    {
    case ActionType::DUDO:
        // The caller (current player) wins if the actual quantity is less than the bid quantity.
        callerWon = (actualDiceQuantity < resolvedBid->getQuantity());
        break;

    case ActionType::EXACTLY:
        // The caller wins if the actual quantity is exactly the bid quantity.
        callerWon = (actualDiceQuantity == resolvedBid->getQuantity());
        break;
    
    default:
        break;
    }
}

void ResolutionState::step(GameModel& context) {
    // 1. Apply penalties before deciding the next state
    if (triggerAction == ActionType::DUDO) {
        if (callerWon) {
            context.getPreviousAlivePlayer().loseDie();
        } else {
            context.getCurrentPlayer().loseDie();
        }
    } else if (triggerAction == ActionType::EXACTLY) {
        if (callerWon) {
            for (auto& p : context.getPlayers()) {
                if (p != context.getCurrentPlayer()) {
                    p.loseDie();
                }
            }
        } else {
            context.getCurrentPlayer().loseDice(2);
        }
    }

    // 2. Decide where to transit
    if (context.isOnlyOnePlayerAlive()){
        requestStateChange(context, std::make_unique<EndGameState>());
    } else {
        requestStateChange(context, std::make_unique<RollingState>());
    }
}

void ResolutionState::render(const GameModel& context, IGameView& view) const {
    std::string msg = "\n--- RESOLUTION PHASE---\n";

    if (!resolvedBid) {
        // This should not happen if onEnter was called correctly.
        msg += "Error: No bid was resolved.";
        view.displayBroadcastMessage(msg);
        return;
    }

    const auto& bidder = context.getPreviousAlivePlayer();
    const auto& caller = context.getCurrentPlayer();

    msg += "The last bid was: 'There are at least " + std::to_string(resolvedBid->getQuantity()) + " dice with face " + std::to_string(resolvedBid->getFace()) + "'\n";
    msg += "Made by: " + bidder.getName() + "\n";
    msg += caller.getName() + " called " + (triggerAction == ActionType::DUDO ? "DUDO" : "EXACTLY") + "!\n\n";

    msg += "All dice are revealed:\n";
    for (const auto& player : context.getPlayers()) {
        if (!player.isAlive()) continue;
        msg += player.getName() + ": ";
        for (const auto& die : player.getHand()) {
            msg += std::to_string(die.getFace()) + " ";
        }
        msg += "\n";
    }

    msg += "\nThere are actually " + std::to_string(actualDiceQuantity) + " dice with face " + std::to_string(resolvedBid->getFace()) + ".\n\n";

    // Build the final outcome message
    if (triggerAction == ActionType::DUDO) {
        if (callerWon) {
            msg += bidder.getName() + " was caught lying and loses a die!";
        } else {
            msg += caller.getName() + " was wrong and loses a die!";
        }
    } else if (triggerAction == ActionType::EXACTLY) {
        if (callerWon) {
            msg += caller.getName() + " guessed exactly! Everyone else loses a die!";
        } else {
            msg += caller.getName() + " was wrong and loses 2 dice!";
        }
    }

    view.displayBroadcastMessage(msg);
}

int ResolutionState::countDiceOfFace(const GameModel& context, short int face) const {
    int quantity = 0;
    for (const auto& player : context.getPlayers()) {
        if (!player.isAlive()) continue;
        for (const auto& die : player.getHand()) {
            if (die.getFace() == face) quantity++;
        }
    }
    return quantity;
}
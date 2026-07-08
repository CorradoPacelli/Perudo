#include <stdexcept>

#include "BiddingState.hpp"
#include "ResolutionState.hpp"
#include "EndGameState.hpp"
#include "BidAction.hpp"
#include "GameModel.hpp"
#include "IGameView.hpp"
#include "PlayerMessage.hpp"
#include "Die.hpp"

void BiddingState::onEnter(GameModel& context) {
    // Nothing to do here
}

bool BiddingState::requiresAction() const {
    return true;
}

void BiddingState::handleAction(GameModel& context, const IAction& action) {
    switch (action.getType()) {
        case ActionType::DUDO :
        case ActionType::EXACTLY : {
            if (!context.getLastBid().has_value()) {
                throw std::invalid_argument("Cannot call Dudo or Exactly when no bid has been made.");
            }
            requestStateChange(context, std::make_unique<ResolutionState>(action.getType()));
            break; }
        case ActionType::BID : {
            const auto& bidAction = static_cast<const BidAction&>(action);
            std::optional<Bid> lastBid = context.getLastBid();
            if (!lastBid.has_value() || bidAction.getBid() > lastBid.value()) {
                context.getCurrentPlayer().addBid(bidAction.getBid());
                context.nextPlayer();
            } else {
                // Invalid bid (not higher than the last one).
                throw std::invalid_argument("Current bid is lower or equal to the one done previously");
            }
            break; }
        case ActionType::EXIT :
            context.stopGame();
            requestStateChange(context, std::make_unique<EndGameState>());
            break;
        default:
            // Uknown action for this BiddingState??
            throw std::invalid_argument("Unknown action for BiddingState");
            break;
    }
}

void BiddingState::render(const GameModel& context, IGameView& view) const {
    const Player& currentPlayer = context.getCurrentPlayer();
    auto lastBid = context.getLastBid();

    for (const auto& player : context.getPlayers()) {
        if (!player.isAlive()) continue;

        int playerId = &player - &context.getPlayers()[0];
        std::string msg = "\n--- DECLARATION PHASE ---\n";

        if (player == currentPlayer) {
            msg += "It's your turn, " + player.getName() + "!\n";
            msg += "Your hand: ";
            for (const auto& die : player.getHand()) {
                msg += std::to_string(die.getFace()) + " ";
            }
            msg += "\n";

            if (lastBid) {
                msg += "Last bid from " + context.getPreviousAlivePlayer().getName() + ": " + std::to_string(lastBid->getQuantity()) + "x " + std::to_string(lastBid->getFace()) + "\n";
            } else {
                msg += "You are the first player to bid!\n";
            }
            msg += "Enter your action (e.g., 'bid 2 3', 'dudo', 'exactly'):";
        } else {
            msg += "Waiting for " + currentPlayer.getName() + " to make a bid...";
        }

        view.displayMessageToPlayer({playerId, msg});
    }
}

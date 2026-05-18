#include "BiddingState.hpp"
#include "ResolutionState.hpp"
#include "BidAction.hpp"
#include "GameModel.hpp"
#include "IGameView.hpp"
#include "Die.hpp"

void BiddingState::onEnter(GameModel& context) {
    // Nothing to do here
}

void BiddingState::handleAction(GameModel& context, const IAction& action) {
    switch (action.getType()) {
        case ActionType::DUDO :
        case ActionType::EXACTLY :
            requestStateChange(context, std::make_unique<ResolutionState>(action.getType()));
            break;
        
        case ActionType::BID : {
            const auto& bidAction = static_cast<const BidAction&>(action);
            std::optional<Bid> lastBid = context.getLastBid();
            if (lastBid && bidAction.getBid() > *lastBid) {
                context.nextPlayer();
            } else {
                //make another Bid!
            }
            break; }
        default:
            // Uknown action for this BiddingState??
            break;
    }
}

void BiddingState::step(GameModel& context) {
    // nothing to do here
}

void BiddingState::render(const GameModel& context, IGameView& view) const {
    std::string msg = "\n--- DECLARATION PHASE ---\n";
    const Player& current = context.getCurrentPlayer();
    msg += "Player: " + current.getName() + "\n";
    
    msg += "Hand: ";
    for (const auto& die : current.getHand()) {
        msg += std::to_string(die.getFace()) + " ";
    }
    msg += "\n";
    
    auto lastBid = context.getLastBid();
    if (lastBid) {
        msg += "Last bid: 'There are at least " + std::to_string(lastBid->getQuantity()) + " dice with face " + std::to_string(lastBid->getFace()) + "'\n";
        msg += "Made by: " + context.getPreviousAlivePlayer().getName() + "\n";
    } else {
        msg += "You are the first player to bid!";
    }
    
    view.displayMessage(msg);
}

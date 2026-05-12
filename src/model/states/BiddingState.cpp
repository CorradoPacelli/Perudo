#include "BiddingState.hpp"
#include "ResolutionState.hpp"
#include "BidAction.hpp"
#include "GameModel.hpp"

void BiddingState::handleAction(GameModel& context, const IAction& action) {
    switch (action.getType())
    {
    case ActionType::DUDO :
    case ActionType::EXACTLY :
        context.changeState(std::make_unique<ResolutionState>());
        break;
    
    case ActionType::BID :
        const auto& bidAction = static_cast<const BidAction&>(action);
        std::optional<Bid> lastBid = context.getLastBid();
        if (lastBid && bidAction.getBid().isValidAfter(*lastBid)) {
            context.nextPlayer();
        } else {
            //make another Bid!
        }
        break;
    default:
        // Uknown action for this BiddingState??
        break;
    }
}

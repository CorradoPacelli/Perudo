#include <optional>

#include "ResolutionState.hpp"
#include "RollingState.hpp"
#include "EndGameState.hpp"
#include "GameModel.hpp"
#include "DudoAction.hpp"
#include "ExactlyAction.hpp"
#include "BidAction.hpp"

void ResolutionState::handleAction(GameModel& context, const IAction& action) {
    std::optional<Bid> lastBid = context.getLastBid();
    if (!lastBid) throw; // TODO: the first action can not be Dudo! throw for now;

    int result = checkLastBid(context);

    switch (action.getType())
    {
    case ActionType::DUDO :

        if (result >= 0){
            // current player loses
            context.getCurrentPlayer().loseDie();
        } else {
            // past player loses
            context.getPreviousAlivePlayer().loseDie();
        }
        break;

    case ActionType::EXACTLY :

        if (result == 0){
            // all the other players lose 
            for (auto& p : context.getPlayers()) {
                if (p != context.getCurrentPlayer() ){
                    p.loseDie();
                }
            }
        } else {
            // current player lose 2 dice
            context.getCurrentPlayer().loseDice(2);
        }
        break;
    
    default:
        // We should never be here
        break;
    }

    //Decide where to transit
    if (context.isOnlyOnePlayerAlive()){
        requestStateChange(context, std::make_unique<EndGameState>());
    } else {
        requestStateChange(context, std::make_unique<RollingState>());
    }
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
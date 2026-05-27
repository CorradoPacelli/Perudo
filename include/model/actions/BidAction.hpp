#pragma once

#include "IAction.hpp"
#include "Bid.hpp"

class BidAction : public IAction {
private:
    Bid bid;

public:
    BidAction(Bid b, int playerId) : IAction(ActionType::BID, playerId), bid(b) {}

    const Bid getBid() const{ return bid; }
};
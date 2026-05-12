#pragma once

#include "IAction.hpp"
#include "Bid.hpp"

class BidAction : public IAction {
private:
    Bid bid;

public:
    BidAction(Bid b) : IAction(ActionType::BID), bid(b) {}

    const Bid getBid() const{
        return bid;
    }
};
#pragma once

#include "IAction.hpp"
#include "Bid.hpp"

class BidAction : public IAction {
private:
    Bid bid;

public:
    BidAction(Bid b) : bid(b) {
        type = ActionType::BID;
    }

    const Bid getBid() const{
        return bid;
    }
};
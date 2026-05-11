#pragma once

#include "IAction.hpp"
#include "Bid.hpp"

class ExactlyAction : public IAction {
public:
    ExactlyAction() {
        type = ActionType::EXACTLY;
    };
};
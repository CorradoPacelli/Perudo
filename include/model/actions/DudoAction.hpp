#pragma once

#include "IAction.hpp"
#include "Bid.hpp"

class DudoAction : public IAction {
public:
    DudoAction() {
        type = ActionType::DUDO;
    };
};
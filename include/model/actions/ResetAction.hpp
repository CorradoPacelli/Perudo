#pragma once

#include "IAction.hpp"

class ResetAction : public IAction {
public:
    ResetAction(int playerId) : IAction(ActionType::RESET, playerId) {};
};
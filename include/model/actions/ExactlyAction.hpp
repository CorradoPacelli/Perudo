#pragma once

#include "IAction.hpp"

class ExactlyAction : public IAction {
public:
    ExactlyAction(int playerId) : IAction(ActionType::EXACTLY, playerId) {};
};
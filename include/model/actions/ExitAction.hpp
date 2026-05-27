#pragma once

#include "IAction.hpp"

class ExitAction : public IAction {
public:
    ExitAction(int playerId) : IAction(ActionType::EXIT, playerId) {};
};
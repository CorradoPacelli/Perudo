#pragma once

#include "IAction.hpp"

class DudoAction : public IAction {
public:
    DudoAction(int playerId) : IAction(ActionType::DUDO, playerId) {};
};
#pragma once

#include "IAction.hpp"

class ResetAction : public IAction {
public:
    ResetAction() : IAction(ActionType::RESET) {};
};
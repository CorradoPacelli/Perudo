#pragma once

#include "IAction.hpp"

class ExitAction : public IAction {
public:
    ExitAction() : IAction(ActionType::EXIT) {};
};
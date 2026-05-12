#pragma once

#include "IAction.hpp"

class ExactlyAction : public IAction {
public:
    ExactlyAction() : IAction(ActionType::EXACTLY) {};
};
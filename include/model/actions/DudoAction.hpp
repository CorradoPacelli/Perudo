#pragma once

#include "IAction.hpp"

class DudoAction : public IAction {
public:
    DudoAction() : IAction(ActionType::DUDO) {};
};